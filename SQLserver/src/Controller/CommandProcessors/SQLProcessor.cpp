//
//  SQLProcessor.cpp
//
//

#include "SQLProcessor.hpp"
#include "SQLStatements.hpp"
#include "TableStatements.hpp"
#include "Helpers.hpp"
#include "ParseHelper.hpp"
#include "Row.hpp"
#include "Alter.hpp"
namespace MyDB {
	StatementPtr SQLProcessor::createTableStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::create_kw).next(Keywords::table_kw).next(TokenType::identifier).checkState()) {
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			EntityPtr theEntityPtr = std::make_unique<Entity>(Entity(aTokenizer.current().data));
			attributeParser(aTokenizer, theEntityPtr);
			return std::make_unique<CreateTableStatement>(theEntityPtr);
		}
		return nullptr;
	}

	StatementPtr SQLProcessor::dropTableStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::drop_kw).next(Keywords::table_kw).next(TokenType::identifier).endsPunc().checkState()) {
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			auto theTableName = aTokenizer.current().data;
			aTokenizer.skipTo(";");
			return std::make_unique<DropTableStatement>(theTableName);
		}
		return nullptr;
	}

	StatementPtr SQLProcessor::describeTableStatementFactory(Tokenizer& aTokenizer)
	{
		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::describe_kw).next(TokenType::identifier).endsPunc().checkState()) {
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			auto theTableName = aTokenizer.current().data;
			aTokenizer.skipTo(";");
			return std::make_unique<DescribeTableStatement>(theTableName);
		}
		return nullptr;
	}

	StatementPtr SQLProcessor::showTablesStatementFactory(Tokenizer& aTokenizer)
	{
		if (aTokenizer.start(Keywords::show_kw).next(Keywords::tables_kw).endsPunc().checkState()) {
			return std::make_unique<ShowTablesStatement>();
		}
		return nullptr;
	}

	StatementPtr SQLProcessor::insertStatementFactory(Tokenizer& aTokenizer) {

		size_t theStartPos = aTokenizer.curPos();
		if (aTokenizer.start(Keywords::insert_kw).next(Keywords::into_kw).next(TokenType::identifier).checkState()) {

			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			std::string theTableName = aTokenizer.current().data;
			if (!aTokenizer.next(TokenType::identifier).next("(").checkState()) {
				throw syntaxError;
			}
			RowCollectionPtr theRowDataPtr = std::make_unique<RowCollection>();

			// move the insert info into the theRowDataPtr
			insertValueParser(aTokenizer, theRowDataPtr);

			return std::make_unique<InsertStatement>(theRowDataPtr, theTableName);
		}
		return nullptr;
	}

	StatementPtr SQLProcessor::selectStatementFactory(Tokenizer& aTokenizer) {
		size_t theStartPos = aTokenizer.curPos();

		SQLQueryPtr theSQLqueryPtr = std::make_unique<SQLQuery>();

		if (aTokenizer.start(Keywords::select_kw).next("*").next(Keywords::from_kw).next(TokenType::identifier).checkState()) {
			if (aTokenizer.endsPunc().checkState()) {
				aTokenizer.restart(theStartPos);
				aTokenizer.skipTo(TokenType::identifier);
				theSQLqueryPtr->setTableName(aTokenizer.current().data);
				theSQLqueryPtr->addTarget("*");
				aTokenizer.next();
				return std::make_unique<SelectStatement>(theSQLqueryPtr);

			}
		}
		aTokenizer.restart(theStartPos);

		if (aTokenizer.start(Keywords::select_kw).checkState()) {

			while (aTokenizer.current().type == TokenType::identifier || aTokenizer.current().data == "*") {
				theSQLqueryPtr->addTarget(aTokenizer.current().data);
				aTokenizer.next();
				if (!aTokenizer.skipIf(','))  break;
			}

			if (aTokenizer.skipTo("from")) {

				aTokenizer.next();
				theSQLqueryPtr->setTableName(aTokenizer.current().data);
			}

			aTokenizer.restart(theStartPos);
			if (aTokenizer.skipTo("where")) {
				aTokenizer.next();
				whereConditionParser(aTokenizer, theSQLqueryPtr);
			}
            
            aTokenizer.restart(theStartPos);
            if(aTokenizer.skipTo("join")){
                theSQLqueryPtr->hasJoin = true;
                joinConditionParser(aTokenizer, theSQLqueryPtr);
            }

			//parse the order by condition here
			aTokenizer.restart(theStartPos);
			if (aTokenizer.skipTo("order")) {
				if (aTokenizer.start(Keywords::order_kw).next(Keywords::by_kw).checkState()) {
					while (aTokenizer.current().type == TokenType::identifier || aTokenizer.current().data == "desc" || aTokenizer.current().data == "asc") {
						if (aTokenizer.current().data == "desc") {
							theSQLqueryPtr->setOrderConditions(Order::DESC);
						}
						else if (aTokenizer.current().data == "asc") {
							theSQLqueryPtr->setOrderConditions(Order::ASC);
						}
						else {
							theSQLqueryPtr->setOrderConditions(aTokenizer.current().data);
						}
						aTokenizer.next();
					}
				}
			}

			//parse the group by condition here
			aTokenizer.restart(theStartPos);
			if (aTokenizer.skipTo("group")) {
				if (aTokenizer.start(Keywords::group_kw).next(Keywords::by_kw).checkState()) {
					while (aTokenizer.current().type == TokenType::identifier) {
						theSQLqueryPtr->setGroupConditions(aTokenizer.current().data);
						aTokenizer.next();
						if (!aTokenizer.skipIf(',')) break;
					}
				}
			}

			//parse the limit condition here
			aTokenizer.restart(theStartPos);
			if (aTokenizer.skipTo("limit")) {
				aTokenizer.next();
				std::string theStart;
				std::string theCount;
				if (aTokenizer.current().type == TokenType::number) {
					theCount = aTokenizer.current().data;
				}
				aTokenizer.next();
				if (aTokenizer.skipIf(',')) {
					if (aTokenizer.current().type != TokenType::number) throw syntaxError;
					theStart = aTokenizer.current().data;
					std::swap(theCount, theStart);
					theSQLqueryPtr->setLimitConditions(std::stoi(theCount), std::stoi(theStart));
				}
				else {
					theSQLqueryPtr->setLimitConditions(std::stoi(theCount));
				}
			}
            aTokenizer.restart(theStartPos);
            if (!aTokenizer.skipTo(";")) throw punctuationExpected;
            return std::make_unique<SelectStatement>(theSQLqueryPtr);
		}
		return nullptr;
	}


	StatementPtr SQLProcessor::updateStatementFactory(Tokenizer& aTokenizer) {

		size_t theStartPos = aTokenizer.curPos();
		SQLQueryPtr theSQLqueryPtr = std::make_unique<SQLQuery>();

		if (aTokenizer.start(Keywords::update_kw).next(TokenType::identifier).next(Keywords::set_kw).checkState()) {
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			theSQLqueryPtr->setTableName(aTokenizer.current().data);
			aTokenizer.next(2);

			std::string theFiledName;
			std::string theUpdateValue;

			if (aTokenizer.current().type == TokenType::identifier) {
				theFiledName = aTokenizer.current().data;
				aTokenizer.next();
			}

			if (!aTokenizer.skipIf('=')) throw syntaxError;
			theUpdateValue = aTokenizer.current().data;
			theSQLqueryPtr->setUpdateInfo(theFiledName, theUpdateValue);

			aTokenizer.next();

			if (!aTokenizer.skipIf(Keywords::where_kw)) throw syntaxError;
			whereConditionParser(aTokenizer, theSQLqueryPtr);
			if (aTokenizer.current().data != ";") throw punctuationExpected;
			return std::make_unique<UpdateStatement>(theSQLqueryPtr);
		}

		return nullptr;
	}


	StatementPtr SQLProcessor::deleteStatementFactory(Tokenizer& aTokenizer) {
		size_t theStartPos = aTokenizer.curPos();

		SQLQueryPtr theSQLqueryPtr = std::make_unique<SQLQuery>();
		if (aTokenizer.start(Keywords::delete_kw).next(Keywords::from_kw).next(TokenType::identifier).next(Keywords::where_kw).checkState()) {
			aTokenizer.restart(theStartPos);
			aTokenizer.skipTo(TokenType::identifier);
			theSQLqueryPtr->setTableName(aTokenizer.current().data);
			aTokenizer.next(2);
			whereConditionParser(aTokenizer, theSQLqueryPtr);
			if (aTokenizer.current().data != ";") throw punctuationExpected;
			return std::make_unique<DeleteStatement>(theSQLqueryPtr);
		}
		return nullptr;
	}
    

    StatementPtr SQLProcessor::indexStatementFactory(Tokenizer& aTokenizer){
        size_t theStartPos = aTokenizer.curPos();
        if(aTokenizer.start(Keywords::show_kw).next("indexes").endsPunc().checkState()){
            return std::make_unique<IndexStatement>(true);
        }
        aTokenizer.restart(theStartPos);
        if(aTokenizer.start(Keywords::show_kw).next("index").checkState()){
            IndexFieldsPtr    theFields =   std::make_unique<IndexFields>();
            std::string                     theEntityName;
            while(aTokenizer.more() && aTokenizer.current().type == TokenType::identifier){
                theFields->push_back(aTokenizer.current().data);
                aTokenizer.next();
            }
            if(aTokenizer.start(Keywords::from_kw).checkState()){
                theEntityName = aTokenizer.current().data;
                aTokenizer.next();
            }
            if(! aTokenizer.endsPunc().checkState()){
                throw punctuationExpected;
            }
            return std::make_unique<IndexStatement>(theFields,theEntityName);
        }
        return nullptr;
    }


StatementPtr SQLProcessor::alterStatementFactory(Tokenizer& aTokenizer){
    size_t theStartPos = aTokenizer.curPos();
    
    if(aTokenizer.start(Keywords::alter_kw).next(Keywords::table_kw).checkState()){
        std::string theTableName = aTokenizer.current().data;
        aTokenizer.next();
        
        std::string theOp;
        std::string theAtt;
        if(aTokenizer.next(Keywords::add_kw).checkState()){
            theOp = "add";
        }else{
            theOp = "drop";
            aTokenizer.next();
            theAtt = aTokenizer.current().data;
            AlterPtr  theAlterPtr = std::make_unique<Alter>(theTableName,theAtt,theOp);
            aTokenizer.skipTo(";");
            return std::make_unique<AlterTableStatement>(theAlterPtr);
        }
        
        
        
        aTokenizer.restart(theStartPos);
        aTokenizer.skipTo(TokenType::identifier);
        theTableName = aTokenizer.current().data;
        aTokenizer.next(2);
        theAtt = aTokenizer.current().data;
        
        EntityPtr theEntityPtr = std::make_unique<Entity>(Entity(aTokenizer.current().data));
//        attributeParser(aTokenizer, theEntityPtr);
        
        Attribute theAttribute;
        std::string theAttName = "pub_year";
        theAttribute.setName(theAttName);
        theAttribute.setDataType(DataTypes::int_type);
        AlterPtr  theAlterPtr = std::make_unique<Alter>(theTableName,theAtt,theOp,theAttribute);
        aTokenizer.skipTo(";");
        return std::make_unique<AlterTableStatement>(theAlterPtr);
        
        
        
    }
    return nullptr;
}

//-------------------------------------------------------------------------//



	StatementPtr SQLProcessor::makeStatement(Tokenizer& aTokenizer) {
		size_t theStartPos = aTokenizer.curPos();
		for (auto& theFactory : factories) {
			auto theStatement = (this->*theFactory)(aTokenizer);
			if (theStatement) return theStatement; //Got a valid statement
			aTokenizer.restart(theStartPos);
		}
		return nullptr;
	}

	void  SQLProcessor::parseOperand(Tokenizer& aTokenizer,
		Operand& anOperand) {

		Token& theToken = aTokenizer.current();
		if (TokenType::identifier == theToken.type) {
			anOperand.name = theToken.data;
			anOperand.ttype = theToken.type;
			anOperand.dtype = DataTypes::varchar_type;
			anOperand.value = theToken.data;
		}

		else if (TokenType::number == theToken.type) {
			anOperand.ttype = TokenType::number;
			anOperand.dtype = DataTypes::int_type;
			if (theToken.data.find('.') != std::string::npos) {
				anOperand.dtype = DataTypes::float_type;
				anOperand.value = std::stof(theToken.data);
			}
			else anOperand.value = std::stoi(theToken.data);
		}

		else throw syntaxError;
	}

    void SQLProcessor::parseTableField(Tokenizer& aTokenizer,Operand& aLHS,Operand& aRHS, std::string aAuxTableName){
        
        if(aTokenizer.current().data == aAuxTableName){
            aTokenizer.next(2);
            parseOperand(aTokenizer,aRHS);
        }else{
            aTokenizer.next(2);
            parseOperand(aTokenizer,aLHS);
        }
        
        aTokenizer.next(2);
        
        if(aTokenizer.current().data == aAuxTableName){
            aTokenizer.next(2);
            parseOperand(aTokenizer,aRHS);
        }else{
            aTokenizer.next(2);
            parseOperand(aTokenizer,aLHS);
        }
    }

    void SQLProcessor::joinConditionParser(Tokenizer& aTokenizer, SQLQueryPtr& aSQLQueryPtr) {
        std::string     theTableName;
        Keywords        theJoinType;
        Operand         theLHS, theRHS;
        
        aTokenizer.skipTo(Keywords::join_kw);
        aTokenizer.next(-1);
        theJoinType = aTokenizer.current().keyword;
        
        aTokenizer.next(2);
        theTableName = aTokenizer.current().data;
        
        if(aTokenizer.next(TokenType::identifier).next(Keywords::on_kw).checkState()){
            parseTableField(aTokenizer, theLHS,theRHS,theTableName);
            Expression theExpression(theLHS,Operators::equal_op,theRHS);
            Join       theJoin(theTableName,theJoinType,theExpression);
            aSQLQueryPtr->setJoinConditions(theJoin);
            
            while(aTokenizer.next(Keywords::and_kw).checkState()){
                parseTableField(aTokenizer, theLHS,theRHS,theTableName);
                Expression theExpression(theLHS,Operators::equal_op,theRHS);
                Join       theJoin(theTableName,theJoinType,theExpression);
                aSQLQueryPtr->setJoinConditions(theJoin);
            }
        }
    }

	void SQLProcessor::whereConditionParser(Tokenizer& aTokenizer, SQLQueryPtr& aSQLQueryPtr) {
		// TODO: putting the selecting limitation into the SQLQuertPtr
		Expressions theExpressions;


		while (2 < aTokenizer.remaining()) {
			if (aTokenizer.current().data == "and" || aTokenizer.current().data == "or") {

				aSQLQueryPtr->setWhereLogic(aTokenizer.current().data);
				aTokenizer.next();
				continue;

			}

			Operand theLHS, theRHS;
			Token& theToken = aTokenizer.current();
			if (theToken.type != TokenType::identifier) break;
			parseOperand(aTokenizer, theLHS);

			aTokenizer.next();
			theToken = aTokenizer.current();
			if (theToken.type == TokenType::operators) {
				Operators theOp = Helpers::toOperator(theToken.data);
				aTokenizer.next();
				parseOperand(aTokenizer, theRHS);
				Expression theExpression(theLHS, theOp, theRHS);
				theExpressions.push_back(theExpression);
				if (aTokenizer.skipIf(semicolon)) {
					break;
				}
				aTokenizer.next();
			}
		}
		aSQLQueryPtr->setWhereConditions(theExpressions);

	}

	void SQLProcessor::insertValueParser(Tokenizer& aTokenizer, RowCollectionPtr& aRowDataPtr)
	{

		ParseHelper theParseHelper(aTokenizer);
		StringList  theParsedIdentifier;
		StatusResult theResult = theParseHelper.parseIdentifierList(theParsedIdentifier);

		if (!theResult) throw syntaxError;

		if (aTokenizer.next(Keywords::values_kw).next("(").checkState()) {
			while (aTokenizer.more() && aTokenizer.current().data != ";") {
				Row         theRow;
				StringList  theParsedValue;
				theResult = theParseHelper.parseValueList(theParsedValue);
				aTokenizer.skipIf(',');
				aTokenizer.skipIf('(');
				//check whether the number of value matches the number of identifier
				if (!theResult || theParsedIdentifier.size() != theParsedValue.size()) {
					throw syntaxError;
				}

				//if matched put all of them into the map inside row
				for (size_t theIndex = 0; theIndex < theParsedValue.size(); theIndex++) {
					Value theStr = theParsedValue[theIndex];
					theRow.set(theParsedIdentifier[theIndex], theStr);
				}
				aRowDataPtr->push_back(theRow);
			}
		}
		else {
			throw syntaxError;
		}
	}

	void SQLProcessor::attributeParser(Tokenizer& aTokenizer, EntityPtr& anEntityPtr)
	{
//		Jump to beginning of attributes
		if (!(aTokenizer.next(TokenType::identifier).next("(").checkState())) throw syntaxError;

		//Continue until we hit the end of the statement
		while (aTokenizer.current().data != ";") {
			Attribute theCurrentAttribute;
			aTokenizer.skipTo(TokenType::identifier);

			//Continue the current attribute until we hit either the next attribute or the end
			//of the attribute list
			while (aTokenizer.current().data != "," && aTokenizer.current().data != ")") {
				switch (aTokenizer.current().type)
				{
				case TokenType::identifier:
					theCurrentAttribute.setName(aTokenizer.current().data);
					if (!aTokenizer.more()) throw syntaxError;
					break;

				case TokenType::keyword:
					if (gDataType.find(aTokenizer.current().keyword) != gDataType.end()) {
						theCurrentAttribute.setDataType(gDataType.at(aTokenizer.current().keyword));

						if (aTokenizer.current().keyword == Keywords::varchar_kw) {
							//if varchar set data size...
							aTokenizer.skipTo(TokenType::number);
							theCurrentAttribute.setSize(std::stoi(aTokenizer.current().data));
							aTokenizer.next();
						}
					}
					else if (aTokenizer.current().keyword == Keywords::auto_increment_kw) {
						theCurrentAttribute.setAutoIncrement(true);
					}
					else if (aTokenizer.current().keyword == Keywords::primary_kw) {
						theCurrentAttribute.setPrimaryKey(true);
						theCurrentAttribute.setNullable(false);
					}
					else if (aTokenizer.current().keyword == Keywords::default_kw) {
						aTokenizer.next();
						theCurrentAttribute.setDefault(aTokenizer.current().data);
					}
					else if (aTokenizer.current().keyword == Keywords::not_kw) {
						theCurrentAttribute.setNullable(false);
					}
					break;
				default:
					break;
				}
				if (!aTokenizer.more()) throw unknownEntity;

				aTokenizer.next();
			}
			anEntityPtr->addAttribute(theCurrentAttribute);
			aTokenizer.next();
		}
		size_t theKeyNumber = anEntityPtr->getPrimaryKeyNumber();

		if (theKeyNumber == 0) throw primaryKeyRequired;

		if (theKeyNumber > 1) throw tooManyPrimaryKeys;
	}
}
