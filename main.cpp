#include <SimpleShell/Parser/Entities/CommandParser.hpp>
#include <SimpleShell/Parser/Entities/ProgramParser.hpp>
#include <SimpleShell/Parser/Entities/StatementParser.hpp>
#include <SimpleShell/Parser/Entities/StringParser.hpp>
#include <SimpleShell/Parser/Entities/VariableParser.hpp>
#include <SimpleShell/Parser/Entities/IoctlParser.hpp>
#include <SimpleShell/Parser/Entities/ExpressionParser.hpp>
#include <SimpleShell/Parser/Entities/PipeParser.hpp>
#include <SimpleShell/Shell/Shell.hpp>
#include <SimpleShell/Parser/Parser.hpp>

static void setParsers(void) {
    shell::registerEntity<shell::StringParser>("string");
    shell::registerEntity<shell::VariableParser>("variable");
    shell::registerEntity<shell::CommandParser>("command");
    shell::registerEntity<shell::StatementParser>("statement");
    shell::registerEntity<shell::IoctlParser>("ioctl");
    shell::registerEntity<shell::ExpressionParser>("expression");
    shell::registerEntity<shell::PipeParser>("pipe");
    shell::registerEntity<shell::ProgramParser>("program");
}

int main(int argc, char** argv) {
    setParsers();
    switch (shell::parseShellArgs(argc, argv)) {
        case shell::ShellMode::Interactive:
            shell::startInteractiveShellSession();
            break;
        case shell::ShellMode::Script:
            shell::parseScript(argv[1]);
            break;
        case shell::ShellMode::Unknown:
            std::printf("Unknown shell parameters");
            return -1;
    }
    return 0;
}
