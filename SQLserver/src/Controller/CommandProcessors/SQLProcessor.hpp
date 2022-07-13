//
//  SQLProcessor.hpp
//
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <vector>
#include <iostream>
#include "CommandProcessor.hpp"
#include "Entity.hpp"
#include "Row.hpp"
#include "SQLQuery.hpp"
#include "Alter.hpp"
namespace MyDB {

	class Statement;
	class Entity;

	class SQLProcessor : public CommandProcessor {
		using SQLStatementFactory = StatementPtr(SQLProcessor::*)(Tokenizer&);
	public:

		SQLProcessor(std::ostream& anOutput) : CommandProcessor(anOutput) {}

		virtual ~SQLProcessor() = default;

		StatementPtr makeStatement(Tokenizer& aTokenizer) override;

	protected:

		const std::vector<SQLStatementFactory> factories = {
			&SQLProcessor::createTableStatementFactory,
			&SQLProcessor::dropTableStatementFactory,
			&SQLProcessor::describeTableStatementFactory,
			&SQLProcessor::showTablesStatementFactory,
			&SQLProcessor::insertStatementFactory,
			&SQLProcessor::selectStatementFactory,
			&SQLProcessor::updateStatementFactory,
			&SQLProcessor::deleteStatementFactory,
            &SQLProcessor::indexStatementFactory,
            &SQLProcessor::alterStatementFactory,
		};

		StatementPtr createTableStatementFactory(Tokenizer& aTokenizer);
		StatementPtr dropTableStatementFactory(Tokenizer& aTokenizer);
		StatementPtr describeTableStatementFactory(Tokenizer& aTokenizer);
		StatementPtr showTablesStatementFactory(Tokenizer& aTokenizer);
		StatementPtr insertStatementFactory(Tokenizer& aTokenizer);
		StatementPtr selectStatementFactory(Tokenizer& aTokenizer);
		StatementPtr updateStatementFactory(Tokenizer& aTokenizer);
		StatementPtr deleteStatementFactory(Tokenizer& aTokenizer);
        StatementPtr indexStatementFactory(Tokenizer& aTokenizer);
        StatementPtr alterStatementFactory(Tokenizer& aTokenizer);

		void attributeParser(Tokenizer& aTokenizer, EntityPtr& anEntityPtr);
		void insertValueParser(Tokenizer& aTokenizer, RowCollectionPtr& aRowDataPtr);
		void whereConditionParser(Tokenizer& aTokenizer, SQLQueryPtr& aSQLQueryPtr);
        void joinConditionParser(Tokenizer& aTokenizer, SQLQueryPtr& aSQLQueryPtr) ;
		void parseOperand(Tokenizer& aTokenizer,
			Operand& anOperand);
        void parseTableField(Tokenizer& aTokenizer,Operand& aLHS,Operand& aRHS, std::string aAuxTableName);

	};

}
#endif /* SQLProcessor_hpp */
