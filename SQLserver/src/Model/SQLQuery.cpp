//
//  SQLQuery.cpp
//
//
//  Created by Jiayang on 4/28/22.
//

#include "SQLQuery.hpp"
#include "Compare.hpp"
#include "Helpers.hpp"

namespace MyDB {
	using Comparator = bool (*)(Value& aLHS, Value& aRHS);

	bool equals(Value& aLHS, Value& aRHS) {
		bool theResult = false;

		std::visit([&](auto const& aLeft) {
			std::visit([&](auto const& aRight) {
				theResult = isEqual(aLeft, aRight);
				}, aRHS);
			}, aLHS);
		return theResult;
	}

	bool less(Value& aLHS, Value& aRHS) {
		if ( aLHS < aRHS) return true;
		return false;
	}

	bool greater(Value& aLHS, Value& aRHS) {
		if (aLHS > aRHS) return true;
		return false;
	}

	static std::map<Operators, Comparator> comparators{
	  {Operators::equal_op, equals},
	  {Operators::lt_op, less},
	  {Operators::gt_op, greater},
	  //may need to add more
	};

	bool Expression::operator()(KeyValues& aList) {
		Value theLHS{ lhs.value };
		Value theRHS{ rhs.value };

		if (TokenType::identifier == lhs.ttype) {
			theLHS = aList[lhs.name]; //get row value
		}

		if (TokenType::identifier == rhs.ttype) {
			theRHS = aList[rhs.name]; //get row value
		}

		return comparators.count(op)
			? comparators[op](theLHS, theRHS) : false;
	}

    bool Expression::operator()(KeyValues& aLeftList,KeyValues& aRightList) {
        Value theLHS{ lhs.value };
        Value theRHS{ rhs.value };

        theLHS = aLeftList[lhs.name];  //get left row value
        theRHS = aRightList[rhs.name]; //get right row value

        return comparators.count(op)
            ? comparators[op](theLHS, theRHS) : false;
    }

	bool validateOperands(Operand& aLHS, Operand& aRHS, Entity& anEntity) {
		if (TokenType::identifier == aLHS.ttype && TokenType::identifier == aRHS.ttype) {
			// Both identifier
			if (anEntity.getAttribute(aLHS.name) && anEntity.getAttribute(aRHS.name)) {
				// Both found in the corresponding Entity
				return true;
			}
		}
		return false;
	}

	//-------------------------------------------------------

	bool SQLQuery::matches(KeyValues& aList) {

		//TODO: still need to implement to support multiple where conditions

		for (auto& theExpr : whereExpressions) {
			if (!theExpr(aList)) {
				return false;
			}
		}
		return true;
	}


	void SQLQuery::setWhereConditions(Expressions anExpressions) {
		whereExpressions = anExpressions;
		hasWhere = true;
	}

	void SQLQuery::setWhereLogic(std::string aLogicStr) {
		if (aLogicStr == "and") {
			logicOp = Logical::and_op;
		}
		if (aLogicStr == "or") {
			logicOp = Logical::or_op;
		}
	}

	void SQLQuery::setTableName(const std::string aTableName) {
		tableName = aTableName;
	}

	void SQLQuery::setGroupConditions(const std::string aCondition) {
		groupConditions.push_back(aCondition);
		grouped = true;
	}

	void SQLQuery::setOrderConditions(const std::string aCondition) {
		orderConditions.first.push_back(aCondition);
		ordered = true;
	}

	void SQLQuery::setOrderConditions(const Order aCondition) {
		orderConditions.second = aCondition;
		ordered = true;
	}

	void SQLQuery::setLimitConditions(const int aLimitCnt, const int aLimitStart) {
		limitCondition = std::make_pair(aLimitCnt, aLimitStart);
		limited = true;
	}

}
