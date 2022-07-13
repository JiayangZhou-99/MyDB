#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream>
#include "Statement.hpp"

namespace MyDB {

	class Manager;
	using ManagerPtr = Manager*;

	class Manager {
	public:

		virtual ~Manager() = default;

		virtual void run(StatementPtr& aStatement) = 0;

		virtual ManagerPtr handles(StatementPtr& aStatement) = 0;

	protected:
		Manager(std::ostream& anOutput) : output(anOutput) {}

		std::ostream& output;
	};

}

#endif // !MANAGER_HPP
