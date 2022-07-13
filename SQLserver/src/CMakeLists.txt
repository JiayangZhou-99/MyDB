file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.c" "*.cxx" "*.h" "*.hpp" "*.hxx")

file(GLOB_RECURSE REMOVE_CMAKE "out/*" "build/*")
list(REMOVE_ITEM SOURCES ${REMOVE_CMAKE})

add_executable(${ECE_EXECUTABLE_NAME} ${SOURCES})

add_library(${ECE_EXECUTABLE_NAME}_lib STATIC ${SOURCES})

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)

target_include_directories(
    ${ECE_EXECUTABLE_NAME}
    PUBLIC
    ${header_dir_list} # Recursive
)
