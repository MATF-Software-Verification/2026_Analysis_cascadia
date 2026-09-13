cmake_minimum_required(VERSION 3.22)
project(CascadiaFuzzer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "FuzzTest fuzzing mode requires Clang. Set CXX=clang++ in fuzztest.cfg.")
endif()

set(CASCADIA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cascadia++/cascadia")
if(NOT EXISTS "${CASCADIA_SOURCE_DIR}/game/entities/tileData.cpp")
    message(FATAL_ERROR "Game sources missing. Run git submodule update --init --recursive.")
endif()
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Use the same fixed FuzzTest revision as the reference regex project.
include(FetchContent)
FetchContent_Declare(fuzztest
    GIT_REPOSITORY https://github.com/google/fuzztest.git
    GIT_TAG 1c9fe96505ae027ac0a5a06c2538402f90dc30c3
)
FetchContent_MakeAvailable(fuzztest)

# These flags instrument all production sources listed below as well.
fuzztest_setup_fuzzing_flags()

add_executable(fuzz_cascadia
    fuzz_main.cc
    fuzz_cascadia.cc
    "${CASCADIA_SOURCE_DIR}/game/entities/tile.cpp"
    "${CASCADIA_SOURCE_DIR}/game/entities/tileData.cpp"
    "${CASCADIA_SOURCE_DIR}/game/entities/tileStorage.cpp"
    "${CASCADIA_SOURCE_DIR}/game/entities/token.cpp"
    "${CASCADIA_SOURCE_DIR}/game/entities/token.h"
    "${CASCADIA_SOURCE_DIR}/common/playerData.cpp"
    "${CASCADIA_SOURCE_DIR}/common/turn.cpp"
    "${CASCADIA_SOURCE_DIR}/game/board/hexagon.cpp"
    "${CASCADIA_SOURCE_DIR}/game/board/hexagon.h"
    "${CASCADIA_SOURCE_DIR}/game/board/hexagonGrid.cpp"
    "${CASCADIA_SOURCE_DIR}/game/entities/player.cpp"
    "${CASCADIA_SOURCE_DIR}/game/engine/scoring.cpp"
)

# Generate the meta-object implementation required by Hexagon and Token.
set_target_properties(fuzz_cascadia PROPERTIES AUTOMOC ON)
target_include_directories(fuzz_cascadia PRIVATE
    "${CASCADIA_SOURCE_DIR}/common"
    "${CASCADIA_SOURCE_DIR}/game/entities"
    "${CASCADIA_SOURCE_DIR}/game/board"
    "${CASCADIA_SOURCE_DIR}/game/engine"
)

# A custom main initializes Qt and FuzzTest in the same process.
target_link_libraries(fuzz_cascadia PRIVATE
    Qt6::Core
    Qt6::Widgets
    fuzztest::fuzztest
    fuzztest::init_fuzztest
    GTest::gtest
    absl::failure_signal_handler
    absl::symbolize
)