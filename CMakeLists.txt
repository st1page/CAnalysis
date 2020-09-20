cmake_minimum_required(VERSION 3.10)
project(CAnalysis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    ${PROJECT_SOURCE_DIR}/grammers/
    ${PROJECT_SOURCE_DIR}/third/
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/
    ${PROJECT_SOURCE_DIR}/src/
)

file(GLOB antlr4-cpp-src
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/atn/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/dfa/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/misc/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/support/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/tree/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/tree/pattern/*.cpp
    ${PROJECT_SOURCE_DIR}/third/antlr4-runtime/tree/xpath/*.cpp
)
add_library(antlr4-cpp-runtime ${antlr4-cpp-src})

set(grammers_path "${PROJECT_SOURCE_DIR}/grammers")
file(GLOB g4_files "${grammers_path}/*.g4")
execute_process(
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}/grammers/
    # Note that this command runs in pure shell
    COMMAND
        java -jar ${PROJECT_SOURCE_DIR}/third/antlr-4.8-complete.jar -Dlanguage=Cpp -no-listener -visitor ${g4_files}
)

file(GLOB src_dir
    ${PROJECT_SOURCE_DIR}/grammers/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(CAnalysis ${src_dir})
target_link_libraries(CAnalysis antlr4-cpp-runtime)

add_custom_target(update_antlr
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}/grammers/
    COMMAND 
        java -jar ${PROJECT_SOURCE_DIR}/third/antlr-4.8-complete.jar -Dlanguage=Cpp -no-listener -visitor ${g4_files}
    COMMENT 
        "Auto updates for developing antlr grammar"
)
