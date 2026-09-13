cmake_minimum_required(VERSION 3.20)
project(cascadia_tracy LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Multimedia)
find_program(PATCH_EXECUTABLE patch REQUIRED)

# Keep the client and capture tool on the same release.
include(FetchContent)
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy zones" FORCE)
set(TRACY_NO_SYSTEM_TRACING ON CACHE BOOL "Use explicit zones only" FORCE)
set(TRACY_ONLY_LOCALHOST ON CACHE BOOL "Capture on the same computer" FORCE)
set(TRACY_NO_BROADCAST ON CACHE BOOL "Disable network discovery" FORCE)
FetchContent_Declare(tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG v0.11.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tracy)

# Instrument a build copy of the submodule sources.
set(ORIGINAL_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../cascadia++/cascadia")
set(PROFILE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/source")
if(NOT EXISTS "${ORIGINAL_SOURCE}/CMakeLists.txt")
    message(FATAL_ERROR "Missing cascadia++/cascadia. Run git submodule update --init --recursive.")
endif()
file(COPY "${ORIGINAL_SOURCE}/" DESTINATION "${PROFILE_SOURCE}")
set(PROFILE_PATCHES instrumentation.patch)
foreach(PROFILE_PATCH IN LISTS PROFILE_PATCHES)
    execute_process(
        COMMAND "${PATCH_EXECUTABLE}" --batch --forward -p1
            -i "${CMAKE_CURRENT_SOURCE_DIR}/${PROFILE_PATCH}"
        WORKING_DIRECTORY "${PROFILE_SOURCE}"
        RESULT_VARIABLE PATCH_RESULT
    )
    if(NOT PATCH_RESULT EQUAL 0)
        message(FATAL_ERROR "${PROFILE_PATCH} does not match these Cascadia sources. See README.md.")
    endif()
endforeach()

# The original CMake file defines the full game target.
add_subdirectory("${PROFILE_SOURCE}" "${CMAKE_CURRENT_BINARY_DIR}/game" EXCLUDE_FROM_ALL)
# Match the plain target_link_libraries signature used by the original project.
target_link_libraries(cascadia Tracy::TracyClient)
set_target_properties(cascadia PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# Build a small, repeatable workload with the same instrumented production code.
add_executable(cascadia_profile
    scenario.cpp
    "${PROFILE_SOURCE}/game/entities/player.cpp"
    "${PROFILE_SOURCE}/game/entities/tile.cpp"
    "${PROFILE_SOURCE}/game/entities/tileData.cpp"
    "${PROFILE_SOURCE}/game/entities/tileStorage.cpp"
    "${PROFILE_SOURCE}/game/board/hexagon.cpp"
    "${PROFILE_SOURCE}/game/board/hexagon.h"
    "${PROFILE_SOURCE}/game/board/hexagonGrid.cpp"
    "${PROFILE_SOURCE}/game/engine/scoring.cpp"
    "${PROFILE_SOURCE}/resource.qrc"
)
set_target_properties(cascadia_profile PROPERTIES AUTOMOC ON AUTORCC ON)
target_include_directories(cascadia_profile PRIVATE
    "${PROFILE_SOURCE}/common"
    "${PROFILE_SOURCE}/game/entities"
    "${PROFILE_SOURCE}/game/board"
    "${PROFILE_SOURCE}/game/engine"
)
target_link_libraries(cascadia_profile PRIVATE Qt6::Widgets Tracy::TracyClient)
