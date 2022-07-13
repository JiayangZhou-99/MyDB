#ifndef APPLICATIONMANAGER_HPP
#define APPLICATIONMANAGER_HPP

#include <map>
#include <string>
#include <iostream>
#include "Statements.hpp"
#include "Errors.hpp"
#include "Manager.hpp"
#include "ScriptRunner.hpp"

namespace MyDB {
	class ApplicationManager : public Manager {
		using ApplicationManagerHandler = void(ApplicationManager::*)(StatementPtr&);

	public:
		ApplicationManager(std::ostream& anOutput, Application& anApp) : Manager(anOutput), app(anApp) {}

		void run(StatementPtr& aStatement) override;

		ManagerPtr handles(StatementPtr& aStatement) override;

	protected:
		std::map<StatementType, ApplicationManagerHandler> dispatcher = {
			{StatementType::version, &ApplicationManager::versionApplicationHandler},
			{StatementType::help, &ApplicationManager::helpApplicationHandler},
			{StatementType::quit, &ApplicationManager::quitApplicationHandler},
            {StatementType::runScript,&ApplicationManager::runScriptApplicationHandler}
		};

		void versionApplicationHandler(StatementPtr& aStatement);
		void helpApplicationHandler(StatementPtr& aStatement);
		void quitApplicationHandler(StatementPtr& aStatement);
        void runScriptApplicationHandler(StatementPtr& aStatement);

		static const inline std::string versionString = "2.0";

		Application app;
	};
}

#endif // !APPLICATIONMANAGER_HPP
