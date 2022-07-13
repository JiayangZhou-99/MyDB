//
//  DBProcessor.cpp
//
//

#include "DatabaseProcessor.hpp"
#include "DatabaseStatements.hpp"
#include "Helpers.hpp"

namespace MyDB {
	StatementPtr DBProcessor::createDatabaseStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::create_kw).next(Keywords::database_kw).next(TokenType::identifier).endsPunc().checkState())
		{
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			auto theDatabaseName = aTokenizer.current().data;
			aTokenizer.skipTo(";");
			return std::make_unique<CreateDatabaseStatement>(theDatabaseName);
		}
		return nullptr;
	}

	StatementPtr DBProcessor::dumpDatabaseStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::dump_kw).next(Keywords::database_kw).next(TokenType::identifier).endsPunc().checkState())
		{
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			auto theDatabaseName = aTokenizer.current().data;
			aTokenizer.skipTo(";");
			return std::make_unique<DumpDatabaseStatement>(theDatabaseName);
		}
		return nullptr;
	}

	StatementPtr DBProcessor::dropDatabaseStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::drop_kw).next(Keywords::database_kw).next(TokenType::identifier).endsPunc().checkState())
		{
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			auto theDatabaseName = aTokenizer.current().data;
			aTokenizer.skipTo(";");
			return std::make_unique<DropDatabaseStatement>(theDatabaseName);
		}
		return nullptr;
	}

	StatementPtr DBProcessor::showDatabasesStatementFactory(Tokenizer& aTokenizer)
	{
		if (aTokenizer.start(Keywords::show_kw).next(Keywords::databases_kw).endsPunc().checkState())
		{
			return std::make_unique<ShowDatabasesStatement>();
		}
		return nullptr;
	}

	StatementPtr DBProcessor::useDatabaseStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::use_kw).next(TokenType::identifier).endsPunc().checkState())
		{
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			auto theDatabaseName = aTokenizer.current().data;
			aTokenizer.skipTo(";");
			return std::make_unique<UseDatabaseStatement>(theDatabaseName);
		}
		return nullptr;
	}

	StatementPtr DBProcessor::logDatabaseStatementFactory(Tokenizer& aTokenizer){
		size_t theStartPos = aTokenizer.curPos();
		if(aTokenizer.start(Keywords::redo_kw).endsPunc().checkState()){
			return std::make_unique<LogRedoDataBaseStatement>();
		}
		aTokenizer.restart(theStartPos);
		if(aTokenizer.start(Keywords::undo_kw).endsPunc().checkState()){
			return std::make_unique<LogUndoDataBaseStatement>();
		}
		return nullptr;
	}

	StatementPtr DBProcessor::makeStatement(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		for (auto& theFactory : factories) {
			auto theStatement = (this->*theFactory)(aTokenizer);
			if (theStatement) return theStatement;
			aTokenizer.restart(theStartPos);
		}
		return nullptr;
	}
}
