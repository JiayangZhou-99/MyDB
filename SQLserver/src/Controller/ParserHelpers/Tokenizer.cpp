//
//  Tokenizer.cpp
//
//  Created by rick gessner.
//
//

#include "Tokenizer.hpp"
#include "Helpers.hpp"
#include <exception>

namespace MyDB {


	bool isWhitespace(char aChar) {
		static const char* theWS = " \t\r\n\b";
		return strchr(theWS, aChar);
	}

	bool isNumber(char aChar) {
		return isdigit(aChar) || '.' == aChar;
	}

	bool isAlphaNum(char aChar) {
		return isalnum(aChar) || '_' == aChar;
	}

	bool isQuote(char aChar) {
		return quote == aChar;
	}

	bool isOperator(char aChar) {
		return strchr("+-/*%=>!<.", aChar);
	}

	bool isSign(char aChar) {
		return strchr("+-", aChar);
	}

	bool isPunctuation(char aChar) {
		return strchr("()[]{}:;,|", aChar);
	}

	//-----------------------------------
    Tokenizer::Tokenizer(const Tokenizer& aTokenizer)
        :Scanner(aTokenizer.input), index(0){
        tokens = aTokenizer.tokens;
        index  = aTokenizer.index;
    }
	Tokenizer::Tokenizer(std::istream& anInput)
		: Scanner(anInput), index(0) {
		skipWhile(isWhitespace);
	}

	Token& Tokenizer::tokenAt(size_t anOffset) {
		if (anOffset >= 0 && anOffset < tokens.size()) {
			return tokens[anOffset];
		}
        return tokens[0];
	}

	bool Tokenizer::next(int anOffset) {
		if (index + anOffset <= size()) {
			index += anOffset;
			return true;
		}
		return false;
	}

	Token& Tokenizer::current() {
		return tokens[index];
	}

	Token& Tokenizer::peek(int anOffset) {
		return tokenAt(index + anOffset);
	}

	//skip any token till you find this target...
	bool Tokenizer::skipTo(TokenType aTarget) {
		while (more()) {
			Token& theToken = current();
			if (theToken.type == aTarget) {
				return true;
			}
			next();
		}
		return false;
	}

	//skip any token till you find this target...
	bool Tokenizer::skipTo(Keywords aTarget) {
		while (more()) {
			Token& theToken = current();
			if (TokenType::keyword == theToken.type && aTarget == theToken.keyword) {
				return true;
			}
			next();
		}
		return false;
	}

    bool Tokenizer::skipTo(std::string aTokenName) {
        while (more()) {
            Token& theToken = current();
            if (theToken.data == aTokenName) {
                return true;
            }
            next();
        }
        return false;
    }

	//move ahead IFF the current keyword matches given...
	bool Tokenizer::skipIf(Keywords aKeyword) {
		if (more() && (aKeyword == current().keyword)) {
			next(); //eat the target...
			return true;
		}
		return false;
	}

	// USE: skip a regular char, usually punctuation....
	bool Tokenizer::skipIf(char aChar) {
		if (more() && (aChar == current().data[0])) {
			next(); //eat the target...
			return true;
		}
		return false;
	}

	//move ahead IFF the current token/op matches given...
	bool Tokenizer::skipIf(Operators anOperator) {
		Token& theToken = current();
		if (more() && (theToken.type == TokenType::operators) && (theToken.op == anOperator)) {
			next(); //eat the target...
			return true;
		}
		return false;
	}
    bool Tokenizer::skipIf(TokenType aTokenType){
        if (more() && (aTokenType == current().type)){
            next(); //eat the target...
            return true;
        }
        return false;
    }

	//run on input provided in constructor; produce vector of tokens...
	StatusResult Tokenizer::tokenize() {

		StatusResult theResult;
		char         theChar;

		while (!input.eof()) {
			theChar = input.peek();
			if (isPunctuation(theChar)) {
				Token theToken{ TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op };
				theToken.data.push_back(input.get());
				tokens.push_back(theToken);
			}
			else if (isOperator(theChar)) {
				std::string theTemp;
				Token theToken{ TokenType::operators };
				theToken.data.push_back(input.get());
				theToken.op = gOperators[theTemp];
				tokens.push_back(theToken);
			}
			else if (isNumber(theChar)) {
				Token theToken{ TokenType::number, Keywords::unknown_kw, Operators::unknown_op };
				theToken.data = readWhile(isNumber);
				tokens.push_back(theToken);
			}
			else if (isQuote(theChar) || (apostrophe == theChar)) {
				input.get(); //skip first quote...
				Token theToken{ TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op };
				theToken.data = readUntil(theChar, false);
				tokens.push_back(theToken);
			}
			else if (isAlphaNum(theChar)) {
				std::string theString = readWhile(isAlphaNum);
				std::string theTemp(theString);
				std::transform(theTemp.begin(), theTemp.end(), theTemp.begin(), ::tolower);

				Token theToken{ TokenType::keyword, Keywords::unknown_kw, Operators::unknown_op, theTemp };
				theToken.keyword = Helpers::getKeywordId(theTemp);
				if (Keywords::unknown_kw != theToken.keyword) {
					tokens.push_back(theToken);
				}
				else {
					theToken.type = TokenType::identifier;
					theToken.data = theString;
					tokens.push_back(Token(theToken));
				}
			}
			skipWhile(isWhitespace);
		}

		return theResult;
	}



	// USE: ----------------------------------------------

	void Tokenizer::dump() {
		for (auto& theToken : tokens) {
			std::cerr << "type ";
			switch (theToken.type) {
			case TokenType::punctuation:
				std::cerr << "punct " << theToken.data << "\n";
				break;

				//case TokenType::operators:
			case TokenType::operators:
				std::cerr << "operator " << theToken.data << "\n";
				break;

			case TokenType::number:
				std::cerr << "number " << theToken.data << "\n";
				break;

			case TokenType::string:
				std::cerr << "string " << theToken.data << "\n";
				break;

			case TokenType::identifier:
				std::cerr << "identifier " << theToken.data << "\n";
				break;

			case TokenType::keyword:
				std::cerr << "keyword " << theToken.data << "\n";
				break;

			default:
				break;
			}
		}
	}
    //query
    Tokenizer&   Tokenizer::start(Keywords aKeyWord){
        if(current().keyword == aKeyWord){
            if(more()){ next();}
            return *this;
        }
        state = false;
        return *this;
    }

    Tokenizer&   Tokenizer::next(Keywords aKeyWord){
        if(current().keyword == aKeyWord){
            if(more()){ next();}
            return *this;
        }
        state = false;
        return *this;
    }
    
    Tokenizer&   Tokenizer::next(TokenType aTokenType){
        if(current().type == aTokenType){
            if(more()){ next();}
            return *this;
        }
        state = false;
        return *this;
    }

    Tokenizer&   Tokenizer::next(std::string aTokenStr){
        if(current().data == aTokenStr){
            if(more()){ next();}
            return *this;
        }
        state = false;
        return *this;
    }

    Tokenizer& Tokenizer::endsPunc(){
        if(current().data == ";"){
            return *this;
        }
        state = false;
        return *this;
    }


}
