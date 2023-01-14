#include <iostream>

#include <SimpleShell/IO/Console.hpp>

int main(void) {
    shell::Console console;
    std::cout << console.getCommand() << '\n';
    return 0;
}
