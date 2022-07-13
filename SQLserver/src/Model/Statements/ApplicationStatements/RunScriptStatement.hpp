
#ifndef RunScriptStatement_hpp
#define RunScriptStatement_hpp

#include <string>
#include "Statement.hpp"

namespace MyDB {
    class RunScriptStatement : public Statement {
    public:
        RunScriptStatement(std::string aScriptPath) : Statement(StatementType::runScript),scriptPath(aScriptPath) { }
        std::string scriptPath;
    };
}

#endif /* RunScriptStatement_hpp */
