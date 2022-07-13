#ifndef VERSIONAPPLICATIONSTATEMENT_HPP
#define VERSIONAPPLICATIONSTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class VersionApplicationStatement : public Statement {
	public:
		VersionApplicationStatement() : Statement(StatementType::version) { }
	};
}
#endif // !VersionApplicationStatement
