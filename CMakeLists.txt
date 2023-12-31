project(turing-project)
cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Build type: Debug")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O0 -fsanitize=address -ggdb)
    add_link_options(-fsanitize=address)
else()
    message("Build type: Release")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)
endif()

include_directories(${PROJECT_SOURCE_DIR}/turing-project/include)
set(MAIN ${PROJECT_SOURCE_DIR}/turing-project/main.cpp)
aux_source_directory(${PROJECT_SOURCE_DIR}/turing-project/src/ CPP_SRC)
add_library(turing_lib STATIC ${CPP_SRC})

add_executable(turing ${MAIN})
target_link_libraries(turing turing_lib)
add_custom_command(
    TARGET turing POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:turing> ${PROJECT_SOURCE_DIR}/bin/${TURING_EXECUTABLE}
    COMMENT "Copying executable to bin directory"
)

set_target_properties(turing PROPERTIES
    DEBUG_POSTFIX "_d"
    RELEASE_POSTFIX ""
)

enable_testing()
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
add_test(NAME test_args COMMAND bash ${TEST_DIR}/args/test.sh)

add_executable(test_util ${TEST_DIR}/util/test.cpp)
target_link_libraries(test_util turing_lib)
add_test(NAME test_util COMMAND test_util)
