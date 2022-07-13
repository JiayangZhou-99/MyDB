//
//  BasicTypes.hpp
//  RGAssignement1
//
//  Created by rick gessner on 3/9/21.
//

#ifndef BasicTypes_h
#define BasicTypes_h

#include <string>
#include <variant>
#include <map>
#include <optional>
#include <vector>

namespace MyDB {

    enum class DataTypes {
        no_type = 'N', bool_type = 'B', datetime_type = 'D',
        float_type = 'F', int_type = 'I', varchar_type = 'V',
    };

    using StringList = std::vector<std::string>;
    using StringMap = std::map<std::string, std::string>;
    using StringOpt = std::optional<std::string>;
    using IntOpt = std::optional<uint32_t>;
    using Value = std::variant<std::monostate, uint64_t, bool, int, double, std::string>;
    using KeyValues = std::map<const std::string, Value>;

    using SerializedIndexPtrType = uint64_t;
    using SerializedCountType = uint64_t;
    using SerializedBoolType = char;
    using SerializedTypeType = char;
    using SerializedIntType = int;
    using SerializedFloatType = double;
    using SerializedDateTimeType = uint64_t;
    using SerializedBlockTypeType = char;

}
#endif /* BasicTypes_h */
