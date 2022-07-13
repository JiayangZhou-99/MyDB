

#ifndef TestManually_h
#define TestManually_h

#include "Application.hpp"
#include "Errors.hpp"
#include <sstream>


// void showError(MyDB::StatusResult &aResult, std::ostream &anOutput) {

//   static std::map<MyDB::Errors, std::string> theErrorMessages = {
//     {MyDB::illegalIdentifier, "Illegal identifier"},
//     {MyDB::unknownIdentifier, "Unknown identifier"},
//     {MyDB::databaseExists, "Database exists"},
//     {MyDB::noDatabaseSpecified, "No Database Specified"},
//     {MyDB::tableExists, "Table Exists"},
//     {MyDB::primaryKeyRequired,   "require a primary Key"},
//     {MyDB::tooManyPrimaryKeys,    "too much primary Key"},
//     {MyDB::syntaxError, "Syntax Error"},
//     {MyDB::unknownCommand, "Unknown command"},
//     {MyDB::unknownDatabase,"Unknown database"},
//     {MyDB::unknownTable,   "Unknown table"},
//     {MyDB::unknownError,   "Unknown error"},
//     {MyDB::punctuationExpected,"punctuation expected"}
//   };

//   std::string theMessage="Unknown Error";
//   if(theErrorMessages.count(aResult.error)) {
//     theMessage=theErrorMessages[aResult.error];
//   }
//   anOutput << "Error (" << aResult.error << ") " << theMessage << "\n";
// }


// bool doManualTesting(std::string theUserInput,std::string &theOutput) {
//   MyDB::Application   theApp(std::cout);
//   MyDB::StatusResult  theResult{};

//   bool running=true;
//   do {
//     std::cout << "\n> ";
//     if(theUserInput.length()) {
//       std::stringstream theStream(theUserInput);
//       theResult=theApp.handleInput(theStream);
//       if(theResult==MyDB::userTerminated) {
//         running=false;
//       }
//       else if(!theResult) {
//         showError(theResult, std::cout);
//       }
//     }
//     theOutput = theApp.getOutput();
//   }
//   while (running);
//   return theResult;
// }

#endif /* TestManually_h */
