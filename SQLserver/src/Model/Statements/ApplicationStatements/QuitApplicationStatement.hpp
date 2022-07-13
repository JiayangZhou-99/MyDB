#ifndef QUITAPPLICATIONSTATEMENT_HPP
#define QUITAPPLICATIONSTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class QuitApplicationStatement : public Statement {
	public:
		QuitApplicationStatement() : Statement(StatementType::quit) { }
	};
}
#endif // !QuitApplicationStatement
