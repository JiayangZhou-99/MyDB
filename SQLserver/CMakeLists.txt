cmake_minimum_required(VERSION 3.16)
project(ECE141BDatabase)


############################
### CHANGE THE NAME HERE ###
############################
set(ECE_EXECUTABLE_NAME "SQLserver")



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pedantic")

# Get GTest dependencies
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


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

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)

include_directories(${header_dir_list})

add_subdirectory(src)
add_subdirectory(tests)
