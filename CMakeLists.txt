project(turing-project)
cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)

include_directories(${PROJECT_SOURCE_DIR}/turing-project/include)
set(MAIN ${PROJECT_SOURCE_DIR}/turing-project/main.cpp)
aux_source_directory(${PROJECT_SOURCE_DIR}/turing-project/lib/ LIB)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(turing ${MAIN} ${LIB})
