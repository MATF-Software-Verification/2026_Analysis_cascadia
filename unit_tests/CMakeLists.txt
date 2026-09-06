cmake_minimum_required(VERSION 3.16)

project(cascadia_unit_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

option(ENABLE_COVERAGE "Build tests with coverage instrumentation" OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

add_executable(cascadia_unit_tests
    test_tile_data.cpp
    ../cascadia++/cascadia/game/entities/tileData.cpp
)

target_include_directories(cascadia_unit_tests PRIVATE
    ../cascadia++/cascadia/common
    ../cascadia++/cascadia/game/entities
)

target_link_libraries(cascadia_unit_tests PRIVATE
    Qt6::Core
    Qt6::Test
)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(cascadia_unit_tests PRIVATE -O0 -g --coverage)
        target_link_options(cascadia_unit_tests PRIVATE --coverage)
    else()
        message(FATAL_ERROR "Coverage is supported only with GCC or Clang")
    endif()
endif()

enable_testing()
add_test(NAME tile_data_tests COMMAND cascadia_unit_tests)
