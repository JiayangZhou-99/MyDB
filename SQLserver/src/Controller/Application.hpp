//
//  Application.hpp
//
//

#ifndef Application_hpp
#define Application_hpp

#include <vector>
#include "CommandProcessor.hpp"
#include "Manager.hpp"
#include <iostream>
#include <sstream>
#include <string>

namespace MyDB {

	class Application {
	public:

		Application(std::ostream& anOutput);
		~Application(){};//release the static resourse in the databaseInstanceManager and applicationInstanceManager;

		StatusResult  handleInput(std::istream& anInput);
		std::string   getOutput(){
			std::stringstream ss;
			ss << output.rdbuf();
			return ss.str();
		}
		void          refresh();

	protected:
		StatementPtr 	processorsCreateStatement(Tokenizer& aTokenizer);
		ManagerPtr 		getHandlingManager(StatementPtr& aStatement);

		std::vector<std::shared_ptr<CommandProcessor>> 	processors;
		std::vector<std::shared_ptr<Manager>> 		   	managers;

		std::ostream& output;
	};
}

#endif /* Application_hpp */
