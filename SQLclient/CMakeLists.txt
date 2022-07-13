cmake_minimum_required(VERSION 3.16)
project(Client)

set(STL_EXECUTABLE_NAME "client")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pedantic")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list CONFIGURE_DEPENDS "*.h" "*.hpp" "*.hxx")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()




set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.c" "*.cxx" "*.h" "*.hpp" "*.hxx")

file(GLOB_RECURSE REMOVE_CMAKE "out/*" "build/*")
list(REMOVE_ITEM SOURCES ${REMOVE_CMAKE})

message(${SOURCES})
add_executable(${STL_EXECUTABLE_NAME} ${SOURCES})

add_library(${STL_EXECUTABLE_NAME}_lib STATIC ${SOURCES})


HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)

include_directories(
    ${header_dir_list}
)