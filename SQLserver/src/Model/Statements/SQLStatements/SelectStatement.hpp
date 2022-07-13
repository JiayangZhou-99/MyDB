//
//  SelectStatement.hpp
//

#ifndef SelectStatement_hpp
#define SelectStatement_hpp

#include "Statement.hpp"
#include "SQLQuery.hpp"

namespace MyDB{

    class SelectStatement: public Statement{
    public:
        
        SelectStatement(SQLQueryPtr& aSQLQueryPtr):Statement(StatementType::selectTable),selectQueryPtr(std::move(aSQLQueryPtr)) {}

        SQLQueryPtr selectQueryPtr;
        
    };
}
#endif /* SelectStatement_hpp */
