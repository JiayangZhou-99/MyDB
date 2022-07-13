//
//  IndexStatement.hpp
//

#ifndef IndexStatement_hpp
#define IndexStatement_hpp

#include "Statement.hpp"
#include "SQLQuery.hpp"

namespace MyDB{
    
    using IndexFields    = std::vector<std::string>;
    using IndexFieldsPtr = std::unique_ptr<IndexFields>;

    class IndexStatement: public Statement{
    public:
        
        IndexStatement(IndexFieldsPtr& afieldsPtr, std::string aEntityName)
        :Statement(StatementType::indexing),indexFields(std::move(afieldsPtr)),entityName(aEntityName){}
        
        IndexStatement(bool aShowIndexes):Statement(StatementType::indexing),showIndexes(aShowIndexes){}

        IndexFieldsPtr           indexFields;
        std::string              entityName;
        bool                     showIndexes = false;
        
    };

}

#endif /* IndexStatement_hpp */
