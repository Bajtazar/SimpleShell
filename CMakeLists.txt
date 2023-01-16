cmake_minimum_required(VERSION 3.17)

project(SimpleShell VERSION 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} main.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/IO/Console.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Util/Strings.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Parser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/IoctlHelper.hpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/ExpressionParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/IoctlHelper.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/VariableParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/PipeParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/ProgramParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/StatementParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/CommandParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/StringParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Parser/Entities/IoctlParser.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/System/LocalVariables.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/System/ChildProcess.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/System/Command.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/System/Util.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/System/BuiltInCommands.cpp
    ${PROJECT_SOURCE_DIR}/src/SimpleShell/Shell/Shell.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src)
