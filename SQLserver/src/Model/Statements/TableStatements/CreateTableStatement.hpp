#ifndef CREATETABLESTATEMENT_HPP
#define CREATETABLESTATEMENT_HPP

#include "Statement.hpp"
#include "Entity.hpp"

namespace MyDB {
	class CreateTableStatement : public Statement {
	public:
		CreateTableStatement(EntityPtr& anEntityPtr) : Statement(StatementType::createTable), entity(std::move(anEntityPtr)) { }

        EntityPtr entity;
	};
}

#endif // !CREATETABLESTATEMENT_HPP
