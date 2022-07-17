//
//  Application.cpp
//
//

#include <iostream>
#include "Application.hpp"
#include "Tokenizer.hpp"
#include "Config.hpp"
#include "CommandProcessors.hpp"
#include "DatabaseInstanceManager.hpp"
#include "Managers.hpp"

namespace MyDB {

	Application::Application(std::ostream& anOutput) : output(anOutput) {
		//Initialize our processors
		processors.push_back(std::make_shared<ApplicationProcessor>(output));
		processors.push_back(std::make_shared<DBProcessor>(output));
		processors.push_back(std::make_shared<SQLProcessor>(output));

		//Initialize our managers
		managers.push_back(std::make_shared<ApplicationManager>(output, *this));
		managers.push_back(std::make_shared<DatabaseManager>(output));
		managers.push_back(std::make_shared<SQLManager>(output));
	}

	void    Application::refresh(){
		output.flush();
	}

	//build a tokenizer, tokenize input, ask processors to handle...
	StatusResult Application::handleInput(std::istream& anInput) {
		try {
			Tokenizer theTokenizer(anInput);
			StatusResult theResult = theTokenizer.tokenize();
			if (!theResult) return theResult;
			// std::cout <<"token size:   " <<theTokenizer.size()<<std::endl;
			while (theResult && theTokenizer.more()) {
				Config::getTimer().reset();

				if (auto theCmd = processorsCreateStatement(theTokenizer))
				{
					if (auto theMgr = getHandlingManager(theCmd)) {
						theMgr->run(theCmd);
						theTokenizer.skipIf(';');
					}
					else {
						throw notImplemented;
					}
				}
				else {
					throw unknownCommand;
				}
			}
			// commit
			// when finished all the statements inside a task, commit ->release all the locks -- strong 2 phase lock
			DatabaseInstanceManager::commit();
			return theResult;
		}
		catch (Errors theError) {
			
			return theError;
		}
		catch(TransactionAbortException theException){
			//aborted here --> need to roll back
			DatabaseInstanceManager::abort();
			output<<theException.GetInfo();
			return Errors::transactionAborted;
		}
		catch (...) {
			return unknownError;
		}
	}

	StatementPtr Application::processorsCreateStatement(Tokenizer& aTokenizer)
	{
		for (auto& theProcessor : processors) {
			if (auto theStatement = theProcessor->makeStatement(aTokenizer)) {
				return theStatement;
			}
		}
		return nullptr;
	}

	ManagerPtr Application::getHandlingManager(StatementPtr& aStatement)
	{
		for (auto& theManager : managers) {
			if (auto theManagerPtr = theManager->handles(aStatement)) {
				return theManagerPtr;
			}
		}
		return nullptr;
	}
}
