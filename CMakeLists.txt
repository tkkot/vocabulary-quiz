cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if (CMAKE_BUILD_TYPE STREQUAL "")
    # Build type is not set eg. command was "cmake .."
    message(STATUS "  Diag: Build type was unspecified, set to Release")
    set(CMAKE_BUILD_TYPE Release)
else ()
    message(STATUS "  Diag: Build type specified as '${CMAKE_BUILD_TYPE}'")
endif ()

set(OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "_DEBUG")
    set(OUTPUT_PATH ${OUTPUT_PATH}/dbg)
else ()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
    set(OUTPUT_PATH ${OUTPUT_PATH}/rel)
endif ()

if(WIN32)
    set(OUTPUT_PATH ${OUTPUT_PATH}/win)
ELSE()
    set(OUTPUT_PATH ${OUTPUT_PATH}/linux)
ENDIF()

project(vocabularyQuiz)

file (GLOB_RECURSE VOCAB_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")

add_executable(vocabularyQuiz ${VOCAB_SOURCES})

target_include_directories(vocabularyQuiz PRIVATE ${CMAKE_SOURCE_DIR}/src)

set_target_properties(vocabularyQuiz 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
)

target_compile_options(vocabularyQuiz PRIVATE "-includestdint.h")
