//
//  UpdateStatement.hpp
//

#ifndef UpdateStatement_hpp
#define UpdateStatement_hpp

#include "Statement.hpp"
#include "SQLQuery.hpp"

namespace MyDB{

    class UpdateStatement: public Statement{
    public:
        
        UpdateStatement(SQLQueryPtr& aSQLQueryPtr):Statement(StatementType::updateTable),updateQueryPtr(std::move(aSQLQueryPtr)) {}

        SQLQueryPtr updateQueryPtr;
        
    };
}

#endif /* UpdateStatement_hpp */
