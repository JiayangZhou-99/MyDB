//
//  Scanner.hpp
//
//
//  Created by rick gessner.
//

#ifndef Scanner_h
#define Scanner_h

#include <iostream>

namespace MyDB {

	using parseCallback = bool(char aChar);

	const char left_paren = '(';
	const char right_paren = ')';
	const char left_bracket = '[';
	const char right_bracket = ']';
	const char left_brace = '{';
	const char right_brace = '}';
	const char colon = ':';
	const char comma = ',';
	const char dot = '.';
	const char semicolon = ';';
	const char apostrophe = '\'';
	const char quote = '"';


	class Scanner {
	public:

		//----------------- simple parsing utilities ----------------------

		Scanner(std::istream& anInput) : input(anInput) {}

		bool eof() { return input.eof(); }

		std::string readWhile(parseCallback aCallback) {
			std::string theResult;
			while (!input.eof() && (*aCallback)(input.peek())) {
				char theChar = input.get();
				theResult += theChar;
			}
			return theResult;
		}

		std::string readUntil(parseCallback aCallback, bool addTerminal = true) {
			std::string theResult;
			while (!input.eof() && !(*aCallback)(input.peek())) {
				theResult += input.get();
			}
			if (addTerminal) {
				if (!input.eof()) {
					theResult += input.get();
				}
			}
			return theResult;
		}

		std::string readUntil(char aTerminalChar, bool addTerminal = true) {
			std::string theResult;
			while (!input.eof()) {
				char theChar = input.get();
				if (theChar != aTerminalChar) {
					theResult += theChar;
				}
				else break;
			}
			if (addTerminal) {
				if (!input.eof()) {
					theResult += input.get();
				}
			}
			return theResult;
		}

		//skip chars as long as callback returns true
		bool skipWhile(parseCallback aCallback) {
			while (!input.eof() && (*aCallback)(input.peek())) {
				input.get();
			}
			return true;
		}

		//if the char is next in stream, skip it...
		bool skipIfChar(char aChar) {
			return (aChar == input.peek()) ? aChar == input.get() : false;
		}

	protected:
		std::istream& input;
	};

}


#endif /* Scanner_h */
