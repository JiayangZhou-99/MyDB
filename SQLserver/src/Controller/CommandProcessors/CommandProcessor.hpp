//
//  CommandProcessor.hpp
//  
//

#ifndef CommandProcessor_hpp
#define CommandProcessor_hpp

#include "Statement.hpp"
#include "Tokenizer.hpp"

namespace MyDB {

	/// <summary>
	/// Interface for parsing tokens and creating statements from them
	/// </summary>
	class CommandProcessor {
	public:
		virtual ~CommandProcessor() = default;

		virtual StatementPtr makeStatement(Tokenizer& aTokenizer) = 0;

	protected:
		CommandProcessor(std::ostream& anOutput) : output(anOutput) {}

		std::ostream& output;
	};

	using CommandProcessorPtr = CommandProcessor*;

}

#endif /* CommandProcessor */
