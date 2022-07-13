#ifndef FORMATSTRING_HPP
#define FORMATSTRING_HPP

#include <iostream>
#include <regex>
#include <sstream>

template <typename ... T>
std::string formatString(const std::string& aFormat, T&& ... aVarArg)
{
	size_t theReplacementIndex = 0;
	std::string theFormattedString = aFormat;
	std::regex theRegEx;
	std::stringstream theStream;
	//Unpack each variadic argument
	([&](auto& arg)
		{
			//Using a stringstream to convert arbitrary types that 
			//implement << to string
			theStream << arg;

			//Generate regex pattern to match {number} where 
			//number is the index of the argument
			theRegEx = "\\{" + std::to_string(theReplacementIndex++) + "\\}";

			//Perform the pattern replacement
			theFormattedString = std::regex_replace(theFormattedString, theRegEx, theStream.str());

			//Clear the stringstream for the next iteration
			theStream.str(std::string());
		} (aVarArg), ...);
	return theFormattedString;
}

#endif // !FORMATSTRING_HPP
