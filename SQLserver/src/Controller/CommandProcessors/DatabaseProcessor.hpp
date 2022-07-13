//
//  DBProcessor.hpp
//
//

#ifndef DBProcessor_hpp
#define DBProcessor_hpp

#include <vector>
#include <iostream>
#include "CommandProcessor.hpp"
#include "Database.hpp"

namespace MyDB {
	class DBProcessor : public CommandProcessor {
		using DatabaseStatementFactory = StatementPtr(DBProcessor::*)(Tokenizer&);

	public:
		DBProcessor(std::ostream& anOutput) : CommandProcessor(anOutput) {}

		virtual ~DBProcessor() = default;

		StatementPtr makeStatement(Tokenizer& aTokenizer) override;

	protected:
		const std::vector<DatabaseStatementFactory> factories = {
			&DBProcessor::createDatabaseStatementFactory,
			&DBProcessor::dumpDatabaseStatementFactory,
			&DBProcessor::dropDatabaseStatementFactory,
			&DBProcessor::showDatabasesStatementFactory,
			&DBProcessor::useDatabaseStatementFactory,
			&DBProcessor::logDatabaseStatementFactory
		};

		StatementPtr createDatabaseStatementFactory(Tokenizer& aTokenizer);
		StatementPtr dumpDatabaseStatementFactory(Tokenizer& aTokenizer);
		StatementPtr dropDatabaseStatementFactory(Tokenizer& aTokenizer);
		StatementPtr showDatabasesStatementFactory(Tokenizer& aTokenizer);
		StatementPtr useDatabaseStatementFactory(Tokenizer& aTokenizer);
		StatementPtr logDatabaseStatementFactory(Tokenizer& aTokenizer);

	};

}
#endif /* DBProcessor_hpp */
