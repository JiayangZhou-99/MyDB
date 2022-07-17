//
//  SQLQuery.hpp
//  
//
//  Created by Jiayang on 4/28/22.
//

#ifndef SQLQuery_hpp
#define SQLQuery_hpp

#include <stdio.h>
#include <vector>
#include <variant>
#include <memory>
#include <string>
#include <functional>
#include "Errors.hpp"
#include "Tokenizer.hpp"
#include "BasicTypes.hpp"
#include "Row.hpp"
#include "Storage.hpp"


namespace  MyDB {


	struct Operand {
		Operand() {}
		Operand(std::string& aName, TokenType aType, Value& aValue, size_t anId = 0)
			: ttype(aType), dtype(DataTypes::varchar_type), name(aName),
			value(aValue), entityId(anId) {}

		TokenType   ttype; //is it a field, or const (#, string)...
		DataTypes   dtype;
		std::string name;  //attr name
		Value       value;
		size_t      entityId;
	};

	//----------------------------------------------------------------------------//

	struct Expression {
		Operand     lhs;  //id
		Operand     rhs;  //usually a constant; maybe a field...
		Operators   op;   //=     //users.id=books.author_id
		Logical     logic; //and, or, not...

		Expression(Operand& aLHSOperand, Operators anOp,
			Operand& aRHSOperand)
			: lhs(aLHSOperand), rhs(aRHSOperand),
			op(anOp), logic(Logical::no_op) {}

		bool operator()(KeyValues& aList);
        bool operator()(KeyValues& aLeftList,KeyValues& aRightList);
	};

	using Expressions = std::vector<Expression>;

	//----------------------------------------------------------------------------//

	enum class Order { DESC, ASC };

    //----------------------------------------------------------------------------//

    struct Join  {
    public:
      Join(const std::string &aTable, Keywords aType,Expression anExpression)
        : table(aTable), joinType(aType), expr(anExpression) {}
              
      std::string table;
      Keywords    joinType;  //left, right, etc...
      Expression  expr;      //the left operand correspond to the main table.
                             //the right operand correspond to the table here.
    };

    using JoinList = std::vector<Join>;

    //----------------------------------------------------------------------------//

	class SQLQuery {
	public:
		SQLQuery() {}

		void setTableName(const std::string aTableName);
		void addTarget(std::string aTarget) { targets.push_back(aTarget); }

		void setWhereConditions(Expressions anExpressions);
		void setWhereLogic(std::string aLogicStr);
		void setGroupConditions(const std::string    aCondition);
		void setOrderConditions(const std::string    aCondition);
		void setOrderConditions(const Order          aCondition);
		void setLimitConditions(const int aLimitCnt, const int aLimitStart = 0);
		void setUpdateInfo(std::string anField, std::string anVal) {
			updateInfo = std::pair<std::string, std::string>(anField, anVal);
		}
        void setJoinConditions(Join& aJoinCond){
            joinConditions.push_back(aJoinCond);
        }

		std::pair<std::string, std::string> getUpdateInfo() { return updateInfo; }

		std::string              getTableName() { return tableName; }
		// limit conditions
		std::pair<int, int>      getLimit() { return limitCondition; }
		std::vector<std::string> getTargets() { return targets; }

		//order conditions
		Order                    getOrderWay() { return orderConditions.second; }
		std::vector<std::string> getOrderFields() { return orderConditions.first; }

		//group condition
		std::vector<std::string> getGroupConditions() { return groupConditions; }
        JoinList                 getJoinConditions() { return joinConditions; }

		// used for check whether this SQLQuery has these conditions
		size_t        getCount() const { return whereExpressions.size(); }
		bool          matches(KeyValues& aList);

		bool limited  = false;
		bool ordered  = false;
		bool grouped  = false;
		bool hasWhere = false;
        bool hasJoin  = false;

	protected:
		std::string                                  tableName;
		std::vector<std::string>                     targets;
		Expressions                                  whereExpressions;
		Logical                                      logicOp = Logical::no_op;
		std::vector<std::string>                     groupConditions;
		std::pair<std::vector<std::string>, Order>   orderConditions;
		std::pair<int, int>                          limitCondition;
		std::pair<std::string, std::string>          updateInfo;
        JoinList                                     joinConditions;
        

		// first is count, second is start ,default start pos is 0
	};

	//--------------------------------------------------------------------------------------//

	using SQLQueryPtr = std::unique_ptr<SQLQuery>;
}
#endif /* SQLQuery_hpp */
