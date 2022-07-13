//
//  Alter.hpp
//
//  Created by JiayangZhou on 6/6/22.
//

#ifndef Alter_hpp
#define Alter_hpp

#include <stdio.h>
#include <string>
#include "Attribute.hpp"

namespace MyDB {


class Alter{
public:
    Alter(std::string theTableName,std::string theAttName, std::string theOperation){
        tableName = theTableName;
        operation = theOperation;
        attriName = theAttName;
    }
    
    Alter(std::string theTableName, std::string theAttName,std::string theOperation, Attribute theAttribute){
        tableName = theTableName;
        operation = theOperation;
        attribute = theAttribute;
        attriName = theAttName;
    }
    
    std::string tableName;
    std::string attriName;
    std::string operation;
    Attribute   attribute;
    
    
};

using AlterPtr = std::unique_ptr<Alter>;
}
#endif /* Alter_hpp */
