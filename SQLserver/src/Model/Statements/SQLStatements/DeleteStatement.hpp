//
//  DeleteStatement.hpp
//

#ifndef DeleteStatement_hpp
#define DeleteStatement_hpp

#include "Statement.hpp"
#include "SQLQuery.hpp"

namespace MyDB{

    class DeleteStatement: public Statement{
    public:
        
        DeleteStatement(SQLQueryPtr& aSQLQueryPtr):Statement(StatementType::deleteTable),deleteQueryPtr(std::move(aSQLQueryPtr)) {}

        SQLQueryPtr deleteQueryPtr;
        
    };
}
#endif /* DeleteStatement_hpp */
