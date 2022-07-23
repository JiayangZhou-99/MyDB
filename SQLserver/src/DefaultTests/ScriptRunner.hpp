
#ifndef ScriptRunner_h
#define ScriptRunner_h

#include <fstream>
#include <sstream>
#include "Application.hpp"

namespace MyDB {

  class ScriptRunner {
  public:
    ScriptRunner(Application &anApp) : app(anApp) {}
    
    StatusResult run(std::istream &anInput, std::ostream &anOutput) {
      StatusResult theResult{};
      std::string theCommand;
      while(theResult && anInput) {
        std::getline (anInput, theCommand);
        std::stringstream theStream(theCommand);
        anOutput << theCommand << "\n";
        theResult=app.handleInput(theStream);
        if(!theResult) {
          // showErrors(theResult, anOutput);
        }
      }
      if(userTerminated==theResult.error) {
        theResult=noError;
      }
      theResult = noError;
      return theResult;
    }
    
  protected:
    Application app;
  };

}

#endif /* ScriptRunner_h */
