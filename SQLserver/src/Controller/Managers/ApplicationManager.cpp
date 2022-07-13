#include "ApplicationManager.hpp"
#include <fstream>

namespace MyDB {
	void ApplicationManager::versionApplicationHandler(StatementPtr& aStatement)
	{
		output << "Version " << versionString << "\n";
	}

	void ApplicationManager::helpApplicationHandler(StatementPtr& aStatement)
	{
		output << "Help system available\n";
	}

	void ApplicationManager::quitApplicationHandler(StatementPtr& aStatement)
	{
		output << "DB::141 is shutting down\n";
		throw userTerminated;
	}

    void ApplicationManager::runScriptApplicationHandler(StatementPtr& aStatement){
        output << "Start doing script test\n";
		RunScriptStatement* theScriptStatement  = dynamic_cast<RunScriptStatement*>(aStatement.get());
		ScriptRunner theRunner(app);
		std::fstream theScript(theScriptStatement->scriptPath);
		throw theRunner.run(theScript, output);
    }

	void ApplicationManager::run(StatementPtr& aStatement)
	{
		//Gets the function pointer from the map, dereferences it based
		//on the current object then pass parameters and return what the
		//called function returns
		(this->*dispatcher.at(aStatement->getType()))(aStatement);
	}

	ManagerPtr ApplicationManager::handles(StatementPtr& aStatement)
	{
		if (dispatcher.find(aStatement->getType()) != dispatcher.end())
		{
			return this;
		}
		return nullptr;
	}
}
