#include <iostream>

#include <SimpleShell/IO/Console.hpp>

#include <SimpleShell/Parser/Entities/StatementParser.hpp>
#include <SimpleShell/Parser/Parser.hpp>

#include <regex>

void setParsers(void) {
    shell::Parser::registerEntity("string", std::make_unique<shell::StringParser>());
    shell::Parser::registerEntity("variable", std::make_unique<shell::VariableParser>());
    shell::Parser::registerEntity("command", std::make_unique<shell::CommandParser>());

    shell::Parser::registerEntity("program", std::make_unique<shell::StatementParser>());
}


int main(void) {
    setParsers();
    shell::Console console;
    shell::Parser parser;

    for (;;) {
        parser(console.getCommand());
    }

    return 0;
}
