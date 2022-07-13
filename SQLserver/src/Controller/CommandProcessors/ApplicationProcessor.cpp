#include "ApplicationProcessor.hpp"
#include "ApplicationStatements.hpp"

namespace MyDB {
	StatementPtr ApplicationProcessor::helpStatementFactory(Tokenizer& aTokenizer)
	{
        size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::help_kw).skipIf(TokenType::punctuation)) {
			return std::make_unique<HelpApplicationStatement>();
		}
        aTokenizer.restart(theStartPos);
		return nullptr;
	}

	StatementPtr ApplicationProcessor::versionStatementFactory(Tokenizer& aTokenizer)
	{
        size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::version_kw).skipIf(TokenType::punctuation)) {
			return std::make_unique<VersionApplicationStatement>();
		}
        aTokenizer.restart(theStartPos);
		return nullptr;
	}

	StatementPtr ApplicationProcessor::quitStatementFactory(Tokenizer& aTokenizer)
	{
        size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::quit_kw).skipIf(TokenType::punctuation)) {
			return std::make_unique<QuitApplicationStatement>();
		}
        aTokenizer.restart(theStartPos);
		return nullptr;
	}

    StatementPtr ApplicationProcessor::runScriptStatementFactory(Tokenizer& aTokenizer){
        size_t theStartPos = aTokenizer.curPos();
        if(aTokenizer.start(Keywords::run_kw).next(TokenType::identifier).endsPunc().checkState()){
            aTokenizer.restart(theStartPos);
            aTokenizer.skipTo(TokenType::identifier);
            std::string theScriptPath = aTokenizer.current().data;
            return std::make_unique<RunScriptStatement>(theScriptPath);
        }
        return nullptr;
    }

	StatementPtr ApplicationProcessor::makeStatement(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		for (auto& theFactory : factories) {
			auto theStatement = (this->*theFactory)(aTokenizer);
			if (theStatement) return theStatement; //Got a valid statement
			aTokenizer.restart(theStartPos);
		}
		return nullptr;
	}
}
