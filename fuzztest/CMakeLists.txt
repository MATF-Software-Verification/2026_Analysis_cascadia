cmake_minimum_required(VERSION 3.16)

project(fuzz_cascadia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(CASCADIA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cascadia++/cascadia")

add_executable(fuzz_cascadia
    fuzz_cascadia.cpp
    "${CASCADIA_SOURCE_DIR}/game/entities/tile.cpp"
    "${CASCADIA_SOURCE_DIR}/game/entities/tileData.cpp"
    "${CASCADIA_SOURCE_DIR}/game/board/hexagon.cpp"
    "${CASCADIA_SOURCE_DIR}/game/board/hexagonGrid.cpp"
)

target_include_directories(fuzz_cascadia PRIVATE
    "${CASCADIA_SOURCE_DIR}"
    "${CASCADIA_SOURCE_DIR}/common"
    "${CASCADIA_SOURCE_DIR}/game/board"
    "${CASCADIA_SOURCE_DIR}/game/entities"
)

target_link_libraries(fuzz_cascadia PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(fuzz_cascadia PRIVATE
        -g
        -O1
        -fno-omit-frame-pointer
        -fsanitize=address,undefined
    )
    target_link_options(fuzz_cascadia PRIVATE
        -fsanitize=address,undefined
    )
endif()