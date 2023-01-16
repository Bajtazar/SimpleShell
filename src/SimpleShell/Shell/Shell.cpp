#include <SimpleShell/Shell/Shell.hpp>
#include <SimpleShell/Parser/Parser.hpp>
#include <SimpleShell/IO/Console.hpp>

#include <fstream>

namespace shell {

    [[nodiscard]] ShellMode parseShellArgs(std::size_t argc, char** argv) {
        for (size_t i = 0; i < argc; ++i)
            setenv(std::to_string(i).c_str(), argv[i], true);
        if (argc == 1)
            return ShellMode::Interactive;
        else if (argc == 2)
            return ShellMode::Script;
        return ShellMode::Unknown;
    }

    void startInteractiveShellSession(void) {
        Console console;
        Parser parser;

        for (;;) {
            try {
                parser(console.getCommand());
            } catch(std::exception const& exception) {
                std::puts(exception.what());
            }
        }
    }

    void parseScript(const char* scriptName) {
        std::ifstream file{scriptName};
        if (not file.good() || not file.is_open()) {
            std::printf("Cannot oepn a \"%s\" script file\n", scriptName);
            return;
        }
        Console console;
        Parser parser;
        for (std::string command; not std::getline(file, command).eof() || command.size(); ) {
            try {
                parser(command);
            } catch(std::exception const& exception) {
                std::puts(exception.what());
            }
        }
    }

}
