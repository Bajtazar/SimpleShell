#include <iostream>

#include <SimpleShell/IO/Console.hpp>

#include <SimpleShell/Parser/Entities/VariableParser.hpp>

int main(void) {
    shell::Console console;
    shell::VariableParser parser;
    std::cout << std::any_cast<std::string>(parser(console.getCommand())) << '\n';

    return 0;
}
