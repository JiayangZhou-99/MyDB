#ifndef HELPAPPLICATIONSTATEMENT_HPP
#define HELPAPPLICATIONSTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class HelpApplicationStatement : public Statement {
	public:
		HelpApplicationStatement() : Statement(StatementType::help) { }
	};
}
#endif // !HelpApplicationStatement
