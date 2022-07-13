//
//  Tokenizer.hpp
//  Database
//
//  Created by rick gessner.
//
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <vector>
#include <cstring>
#include "keywords.hpp"
#include "Scanner.hpp"

namespace MyDB {

	using parseCallback = bool(char aChar);

	enum class TokenType {
		function, identifier, keyword, number, operators, timedate, punctuation, string, unknown
	};

	//-----------------

	struct Token {

		Token& operator=(const Token& aCopy) {
			type = aCopy.type;
			keyword = aCopy.keyword;
			data = aCopy.data;
			return *this;
		}

		TokenType   type;
		Keywords    keyword;
		Operators   op;
		std::string data;
	};

	//-----------------

	class Tokenizer : public Scanner {
	public:
		Tokenizer(std::istream& anInputStream);
        Tokenizer(const Tokenizer& aTokenizer);
        Tokenizer operator =(const Tokenizer& aTokenizer){
            Tokenizer theNewToken(aTokenizer);
            return theNewToken;
        };
		StatusResult  tokenize();
		Token& tokenAt(size_t anOffset);

		Token& current();
		bool          more() { return index < size(); }
		bool          next(int anOffset = 1);
		Token& peek(int anOffset = 1);
        void          restart(size_t anOffset = 0) { index = anOffset;state=true; }
        size_t        curPos()  {return index;}
		size_t        size() { return tokens.size(); }
		size_t        remaining() { return index < size() ? size() - index : 0; }
        bool          checkState(){return state;}

		//these might consume a token...    
		bool          skipTo(Keywords aKeyword);
		bool          skipTo(TokenType aTokenType);
        bool          skipTo(std::string aTokenName);
		bool          skipIf(Keywords aKeyword);
		bool          skipIf(Operators anOperator);
		bool          skipIf(TokenType aTokenType);
		bool          skipIf(char aChar);

		void          dump(); //utility
        
        //query check
        Tokenizer&   start(Keywords aKeyWord);
        Tokenizer&   next(Keywords aKeyWord);
        Tokenizer&   next(TokenType aTokenType);
        Tokenizer&   next(std::string aTokenStr);
        Tokenizer&   endsPunc();

	protected:
		std::vector<Token>    tokens;
		size_t                index;
        bool                  state = true;
	};

}

#endif /* Tokenizer_hpp */

