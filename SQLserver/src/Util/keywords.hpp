

#ifndef keywords_h
#define keywords_h

#include <string>
#include <map>
#include "Errors.hpp"

namespace MyDB {
  

  //This enum defines each of the keywords we need to handle across our multiple languages...
  enum class Keywords {
    add_kw=1, all_kw, alter_kw, and_kw, as_kw, asc_kw, avg_kw,
    auto_increment_kw, between_kw, boolean_kw, by_kw, 
    change_kw, changed_kw, char_kw, column_kw, count_kw, create_kw, cross_kw,
    current_date_kw, current_time_kw, current_timestamp_kw,
    database_kw, databases_kw, datetime_kw, decimal_kw, delete_kw,default_kw,
    desc_kw, describe_kw, distinct_kw, double_kw, drop_kw, dump_kw,
    enum_kw, explain_kw, false_kw,
    float_kw, foreign_kw, from_kw, full_kw, group_kw, help_kw,
    in_kw, index_kw, indexes_kw, inner_kw, insert_kw, integer_kw,
    into_kw, join_kw, key_kw, last_kw, left_kw, like_kw, limit_kw,
    max_kw, min_kw, modify_kw, not_kw,  null_kw,
    on_kw, or_kw, order_kw, outer_kw,
    primary_kw, query_kw, quit_kw, references_kw, right_kw, rows_kw,redo_kw,
    run_kw,select_kw, self_kw, set_kw, show_kw, sum_kw,
    table_kw, tables_kw, true_kw,timestamp_kw,
    unique_kw, unknown_kw, update_kw, use_kw,undo_kw,
    values_kw, varchar_kw, version_kw, where_kw,
  };

  
  //This enum defines operators that will be used in SQL commands...
  enum class Operators {
    equal_op=1, notequal_op, lt_op, lte_op, gt_op, gte_op, between_op,
    or_op, nor_op, and_op, not_op, dot_op,
    add_op, subtract_op, multiply_op, divide_op, power_op, mod_op,
    unknown_op
  };
  
  enum class Logical {
    no_op=1, or_op, and_op, not_op, unknown_op
  };

  using KWList = std::initializer_list<Keywords>;

}

#endif /* keywords_h */
