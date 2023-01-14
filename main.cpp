#include <iostream>

#include <SimpleShell/IO/Console.hpp>

int main(void) {
    shell::Console console;
    console.printPrompt();
    return 0;
}
