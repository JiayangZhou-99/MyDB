//
//  Helpers.hpp
//  Database3
//
//  Created by rick gessner on 4/14/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Helpers_h
#define Helpers_h

#include <algorithm>
#include "BasicTypes.hpp"
#include "keywords.hpp"
#include "FormatString.hpp"

namespace MyDB {

	template<typename T, size_t aSize>
	bool in_array(T(&anArray)[aSize], const T& aValue) {
		for (size_t i = 0; i < aSize; i++) {
			if (anArray[i] == aValue) return true;
		}
		return false;
	}

	static std::map<Keywords, DataTypes> gDataType = {
		std::make_pair(Keywords::integer_kw, DataTypes::int_type),
		std::make_pair(Keywords::boolean_kw, DataTypes::bool_type),
		std::make_pair(Keywords::timestamp_kw, DataTypes::datetime_type),
		std::make_pair(Keywords::float_kw, DataTypes::float_type),
		std::make_pair(Keywords::varchar_kw, DataTypes::varchar_type),
	};

	static std::map<std::string, Operators> gExpressionOps = {
	  {"=",   Operators::equal_op},
	  {"<",   Operators::lt_op},
	  {"<=",  Operators::lte_op},
	  {">",   Operators::gt_op},
	  {">=",  Operators::gte_op},
	  {"!=",  Operators::notequal_op},
	  {"not", Operators::notequal_op},
	};

	static std::map<std::string, Operators> gOperators = {
	  std::make_pair(".",   Operators::dot_op),
	  std::make_pair("+",   Operators::add_op),
	  std::make_pair("-",   Operators::subtract_op),
	  std::make_pair("*",   Operators::multiply_op),
	  std::make_pair("/",   Operators::divide_op),
	  std::make_pair("^",   Operators::power_op),
	  std::make_pair("%",   Operators::mod_op),
	  std::make_pair("=",   Operators::equal_op),
	  std::make_pair("!=",  Operators::notequal_op),
	  std::make_pair("not", Operators::notequal_op),
	  std::make_pair("<",   Operators::lt_op),
	  std::make_pair("<=",  Operators::lte_op),
	  std::make_pair(">",   Operators::gt_op),
	  std::make_pair(">=",  Operators::gte_op),
	  std::make_pair("and", Operators::and_op),
	  std::make_pair("or",  Operators::or_op),
	  std::make_pair("nor", Operators::nor_op),
	  std::make_pair("between", Operators::between_op),
	};

	/*static ECE141::Keywords gJoinTypes[] = {
		  ECE141::Keywords::cross_kw,ECE141::Keywords::full_kw, ECE141::Keywords::inner_kw,
		  ECE141::Keywords::left_kw, ECE141::Keywords::right_kw
	};*/

	//a list of known functions...
	static std::map<std::string, int> gFunctions = {
	  std::make_pair("avg", 10),
	  std::make_pair("count", 20),
	  std::make_pair("max", 30),
	  std::make_pair("min", 40)
	};


	//This map binds a keyword string with a Keyword (token)...
	static std::map<std::string, Keywords> gDictionary = {
	  std::make_pair("add",       Keywords::add_kw),
	  std::make_pair("all",       Keywords::all_kw),
	  std::make_pair("alter",     Keywords::alter_kw),
	  std::make_pair("and",       Keywords::and_kw),
	  std::make_pair("as",        Keywords::as_kw),
	  std::make_pair("asc",       Keywords::asc_kw),
	  std::make_pair("avg",       MyDB::Keywords::avg_kw),
	  std::make_pair("auto_increment", Keywords::auto_increment_kw),
	  std::make_pair("between",   MyDB::Keywords::between_kw),
	  std::make_pair("boolean",   MyDB::Keywords::boolean_kw),
	  std::make_pair("by",        MyDB::Keywords::by_kw),
	  std::make_pair("change",    MyDB::Keywords::change_kw),
	  std::make_pair("changed",   MyDB::Keywords::changed_kw),
	  std::make_pair("char",      MyDB::Keywords::char_kw),
	  std::make_pair("column",    MyDB::Keywords::column_kw),
	  std::make_pair("count",     MyDB::Keywords::count_kw),
	  std::make_pair("create",    MyDB::Keywords::create_kw),
	  std::make_pair("cross",     MyDB::Keywords::cross_kw),
	  std::make_pair("current_date",  Keywords::current_date_kw),
	  std::make_pair("current_time",  Keywords::current_time_kw),
	  std::make_pair("current_timestamp", Keywords::current_timestamp_kw),
	  std::make_pair("database",  MyDB::Keywords::database_kw),
	  std::make_pair("databases", MyDB::Keywords::databases_kw),
	  std::make_pair("datetime",  MyDB::Keywords::datetime_kw),
	  std::make_pair("decimal",   MyDB::Keywords::decimal_kw),
	  std::make_pair("desc",      MyDB::Keywords::desc_kw),
	  std::make_pair("delete",    MyDB::Keywords::delete_kw),
	  std::make_pair("default",   MyDB::Keywords::default_kw),
	  std::make_pair("describe",  MyDB::Keywords::describe_kw),
	  std::make_pair("distinct",  MyDB::Keywords::distinct_kw),
	  std::make_pair("double",    MyDB::Keywords::double_kw),
	  std::make_pair("drop",      MyDB::Keywords::drop_kw),
	  std::make_pair("dump",      MyDB::Keywords::dump_kw),
	  std::make_pair("enum",      MyDB::Keywords::enum_kw),
	  std::make_pair("explain",   MyDB::Keywords::explain_kw),
	  std::make_pair("false",     MyDB::Keywords::false_kw),
	  std::make_pair("float",     MyDB::Keywords::float_kw),
	  std::make_pair("foreign",   MyDB::Keywords::foreign_kw),
	  std::make_pair("from",      MyDB::Keywords::from_kw),
	  std::make_pair("full",      MyDB::Keywords::full_kw),
	  std::make_pair("group",     MyDB::Keywords::group_kw),
	  std::make_pair("help",      MyDB::Keywords::help_kw),
	  std::make_pair("in",        MyDB::Keywords::in_kw),
	  std::make_pair("index",     MyDB::Keywords::index_kw),
	  std::make_pair("indexes",   MyDB::Keywords::indexes_kw),
	  std::make_pair("inner",     MyDB::Keywords::inner_kw),
	  std::make_pair("insert",    MyDB::Keywords::insert_kw),
	  std::make_pair("int",       MyDB::Keywords::integer_kw),
	  std::make_pair("integer",   MyDB::Keywords::integer_kw),
	  std::make_pair("into",      MyDB::Keywords::into_kw),
	  std::make_pair("join",      MyDB::Keywords::join_kw),
	  std::make_pair("key",       MyDB::Keywords::key_kw),
	  std::make_pair("last",      MyDB::Keywords::last_kw),
	  std::make_pair("left",      MyDB::Keywords::left_kw),
	  std::make_pair("like",      MyDB::Keywords::like_kw),
	  std::make_pair("limit",     MyDB::Keywords::limit_kw),
	  std::make_pair("max",       MyDB::Keywords::max_kw),
	  std::make_pair("min",       MyDB::Keywords::min_kw),
	  std::make_pair("modify",    MyDB::Keywords::modify_kw),
	  std::make_pair("not",       MyDB::Keywords::not_kw),
	  std::make_pair("null",      MyDB::Keywords::null_kw),
	  std::make_pair("on",        MyDB::Keywords::on_kw),
	  std::make_pair("or",        MyDB::Keywords::or_kw),
	  std::make_pair("order",     MyDB::Keywords::order_kw),
	  std::make_pair("outer",     MyDB::Keywords::outer_kw),
	  std::make_pair("primary",   MyDB::Keywords::primary_kw),
	  std::make_pair("query",     MyDB::Keywords::query_kw),
	  std::make_pair("quit",      MyDB::Keywords::quit_kw),
	  std::make_pair("references",MyDB::Keywords::references_kw),
	  std::make_pair("right",     MyDB::Keywords::right_kw),
	  std::make_pair("rows",      MyDB::Keywords::rows_kw),
	  std::make_pair("run",       MyDB::Keywords::run_kw),
	  std::make_pair("redo",      MyDB::Keywords::redo_kw),
	  std::make_pair("select",    MyDB::Keywords::select_kw),
	  std::make_pair("self",      MyDB::Keywords::self_kw),
	  std::make_pair("set",       MyDB::Keywords::set_kw),
	  std::make_pair("show",      MyDB::Keywords::show_kw),
	  std::make_pair("sum",       MyDB::Keywords::sum_kw),
	  std::make_pair("table",     MyDB::Keywords::table_kw),
	  std::make_pair("tables",    MyDB::Keywords::tables_kw),
	  std::make_pair("true",      MyDB::Keywords::true_kw),
	  std::make_pair("unique",    MyDB::Keywords::unique_kw),
	  std::make_pair("timestamp", MyDB::Keywords::timestamp_kw),
	  std::make_pair("update",    MyDB::Keywords::update_kw),
	  std::make_pair("use",       MyDB::Keywords::use_kw),
	  std::make_pair("undo",      MyDB::Keywords::undo_kw),
	  std::make_pair("values",    MyDB::Keywords::values_kw),
	  std::make_pair("varchar",   MyDB::Keywords::varchar_kw),
	  std::make_pair("version",   MyDB::Keywords::version_kw),
	  std::make_pair("where",     MyDB::Keywords::where_kw)
	};

	class Helpers {
	public:
		static Value StringToValue(Value aValue, std::string aStr) {

			size_t theIndex = aValue.index();
			std::vector<std::string> theVector = { "NULL", "uint_64", "bool", "int", "float", "string" };

			switch (theIndex) {
			case 0:
				return Value(std::monostate());

			case 1:
				//Not sure why but strtoul doesn't work on windows and stoull doesn't work on linux
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
				return Value(std::stoull(aStr));
#else
				return Value(std::strtoul(aStr.c_str(), NULL, 0));
#endif

			case 2:
				if (aStr == "true")  return Value(true);
				else                return Value(false);

			case 3:
				return Value(std::stoi(aStr));

			case 4:
				return Value(std::stof(aStr));

			case 5:
				return Value(aStr);

			default:
				return Value(std::monostate());
			}
		}

		static std::string ValueToString(Value aValue) {
			size_t theIndex = aValue.index();
			std::vector<std::string> theVector = { "NULL", "uint_64", "bool", "int", "float", "string" };

			switch (theIndex) {
			case 0:
				return "NULL";

			case 1:
				return std::to_string(std::get<uint64_t>(aValue));

			case 2:
				if (aValue == Value(true)) return "True";
				else return "False";

			case 3:
				return std::to_string(std::get<int>(aValue));

			case 4:
				return std::to_string(std::get<double>(aValue));

			case 5:
				return std::get<std::string>(aValue);

			default:
				return "NULL";
			}
		}

		//be very careful about collisions!
		static size_t hashString(const char* str) {
			uint32_t h{ 0 };
			unsigned char* p;
			const int gMultiplier = 37;
			for (p = (unsigned char*)str; *p != '\0'; p++)
				h = gMultiplier * h + *p;
			return h;
		}

		static Keywords getKeywordId(const std::string aKeyword) {
			auto theIter = gDictionary.find(aKeyword);
			if (theIter != gDictionary.end()) {
				return theIter->second;
			}
			return Keywords::unknown_kw;
		}

		//convert from char to keyword...
		static Keywords charToKeyword(char aChar) {
			switch (toupper(aChar)) {
			case 'I': return Keywords::integer_kw;
			case 'T': return Keywords::datetime_kw;
			case 'B': return Keywords::boolean_kw;
			case 'F': return Keywords::float_kw;
			case 'V': return Keywords::varchar_kw;
			default:  return Keywords::unknown_kw;
			}
		}

		static std::string dataTypeToString(DataTypes aType, size_t aDefaultSize) {
			switch (aType) {
			case DataTypes::no_type:        return "none";
			case DataTypes::bool_type:      return "bool";
			case DataTypes::datetime_type:  return "datetime";
			case DataTypes::float_type:     return "float";
			case DataTypes::int_type:       return "int";
			case DataTypes::varchar_type:   return formatString("varchar({0})", aDefaultSize);
			default: return "";
			}
		}

		static std::string keywordToString(Keywords aType) {
			switch (aType) {
			case Keywords::boolean_kw:    return "bool";
			case Keywords::create_kw:     return "create";
			case Keywords::database_kw:   return "database";
			case Keywords::databases_kw:  return "databases";
			case Keywords::datetime_kw:   return "datetime";
			case Keywords::describe_kw:   return "describe";
			case Keywords::drop_kw:       return "drop";
			case Keywords::float_kw:      return "float";
			case Keywords::integer_kw:    return "integer";
			case Keywords::show_kw:       return "show";
			case Keywords::table_kw:      return "table";
			case Keywords::tables_kw:     return "tables";
			case Keywords::use_kw:        return "use";
			case Keywords::varchar_kw:    return "varchar";
			default:                      return "unknown";
			}
		}

		static std::string boolToString(bool aBool) {
			if (aBool) return("YES");
			return("NO");
		}

		// USE: ---validate that given keyword is a datatype...
		static bool isDatatype(Keywords aKeyword) {
			switch (aKeyword) {
			case Keywords::char_kw:
			case Keywords::datetime_kw:
			case Keywords::float_kw:
			case Keywords::integer_kw:
			case Keywords::varchar_kw:
				return true;
			default: return false;
			}
		}

		static DataTypes getTypeForKeyword(Keywords aKeyword) {
			return gDataType.count(aKeyword)
				? gDataType[aKeyword] : DataTypes::no_type;
		}

		static Operators toOperator(std::string aString) {
			auto theIter = gOperators.find(aString);
			if (theIter != gOperators.end()) {
				return theIter->second;
			}
			return Operators::unknown_op;
		}

		static int getFunctionId(const std::string anIdentifier) {
			auto theIter = gFunctions.find(anIdentifier);
			if (theIter != gFunctions.end()) {
				return theIter->second;
			}
			return 0;
		}

		static bool isNumericKeyword(Keywords aKeyword) {
			static Keywords theTypes[] = { Keywords::decimal_kw, Keywords::double_kw, Keywords::float_kw, Keywords::integer_kw };
			for (auto theType : theTypes) {
				if (aKeyword == theType) return true;
			}
			return false;
		}

		static std::string QueryOk(size_t aCount, size_t anElapsed) {
			return formatString("Query Ok, {0} rows affected ({1} secs)\n",
				aCount, anElapsed);
		}

	};

}

#endif /* Helpers_h */
