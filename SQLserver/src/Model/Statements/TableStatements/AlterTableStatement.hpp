//
//  AlterTableStatement.hpp
//  final_code
//

#ifndef AlterTableStatement_hpp
#define AlterTableStatement_hpp

#include <stdio.h>
#include "Alter.hpp"
#include "Statement.hpp"

namespace MyDB {
    class AlterTableStatement : public Statement {
    public:
        AlterTableStatement(AlterPtr& anAlterPtr):Statement(StatementType::alterTable){
            alterPtr = std::move(anAlterPtr);
        }
        AlterPtr alterPtr;
    };
}
#endif /* AlterTableStatement_hpp */
