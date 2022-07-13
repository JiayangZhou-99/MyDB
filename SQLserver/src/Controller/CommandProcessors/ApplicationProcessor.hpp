#ifndef APPLICATIONPROCESSOR_HPP
#define APPLICATIONPROCESSOR_HPP

#include <vector>
#include <iostream>
#include "CommandProcessor.hpp"

namespace MyDB {
	class ApplicationProcessor : public CommandProcessor {
		using ApplicationStatementFactory = StatementPtr(ApplicationProcessor::*)(Tokenizer&);

	public:
		ApplicationProcessor(std::ostream& anOutput) : CommandProcessor(anOutput) {}

		virtual ~ApplicationProcessor() = default;

		StatementPtr makeStatement(Tokenizer& aTokenizer) override;

	protected:

		const std::vector<ApplicationStatementFactory> factories = {
			 &ApplicationProcessor::helpStatementFactory,
			 &ApplicationProcessor::versionStatementFactory,
			 &ApplicationProcessor::quitStatementFactory,
             &ApplicationProcessor::runScriptStatementFactory,
		};

		StatementPtr helpStatementFactory(Tokenizer& aTokenizer);
		StatementPtr versionStatementFactory(Tokenizer& aTokenizer);
		StatementPtr quitStatementFactory(Tokenizer& aTokenizer);
        StatementPtr runScriptStatementFactory(Tokenizer& aTokenizer);
	};
}
#endif // !APPLICATIONPROCESSOR_HPP
