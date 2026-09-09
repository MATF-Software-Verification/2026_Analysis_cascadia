cmake_minimum_required(VERSION 3.16)

project(cascadia_unit_tests LANGUAGES CXX)

set(CMAsKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate Qt meta-object code for test classes
set(CMAKE_AUTOMOC ON)

# Configure instrumentation independently
option(ENABLE_COVERAGE "Build tests with coverage instrumentation" OFF)
option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

# Enable registration of tests with CTest
enable_testing()

# Coverage uses GCC and a matching gcov execsutable
if(ENABLE_COVERAGE AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR
        "Coverage requires GCC. Select g++ or a versioned GCC executable."
    )
endif()

# These sanitizer flags are supported by GCC
if(ENABLE_SANITIZERS AND
   NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(FATAL_ERROR
        "Sanitizers require GCC."
    )
endif()

# Locate the original source tree
set(CASCADIA_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../cascadia++/cascadia"
)

# Configure one test executable with its production dependencies
function(add_data_test target test_file)
    # ARGN contains the additional source arguments passed to this function
    add_executable(${target}
        "${test_file}"
        ${ARGN}
    )

    # Locate the original class headers
    target_include_directories(${target} PRIVATE
        "${CASCADIA_SOURCE_DIR}/common"
        "${CASCADIA_SOURCE_DIR}/game/entities"
    )

    # Link Qt libraries needed by these tests
    target_link_libraries(${target} PRIVATE
        Qt6::Core
        Qt6::Test
    )

    # Instrument the test and production sources for GCC coverage
    if(ENABLE_COVERAGE)
        target_compile_options(${target} PRIVATE
            -O0
            -g
            --coverage
        )

        # Link the coverage runtime
        target_link_options(${target} PRIVATE
            --coverage
        )
    endif()

    # Enable memory-error and undefined-behavior checks
    if(ENABLE_SANITIZERS)
        target_compile_options(${target} PRIVATE
            -g
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )

        # Link the sanitizer runtimes
        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined
        )
    endif()

    # Register the executable as an independent CTest suite
    add_test(NAME ${target} COMMAND ${target})
endfunction()

# Share the original TileData implementation across the suites
set(TILE_SOURCE
    "${CASCADIA_SOURCE_DIR}/game/entities/tileData.cpp"
)

add_data_test(cascadia_unit_tests
    test_tile_data.cpp
    "${TILE_SOURCE}"
)

add_data_test(player_data_tests
    test_player_data.cpp
    "${TILE_SOURCE}"
    "${CASCADIA_SOURCE_DIR}/common/playerData.cpp"
)

add_data_test(turn_tests
    test_turn.cpp
    "${TILE_SOURCE}"
    "${CASCADIA_SOURCE_DIR}/common/turn.cpp"
)