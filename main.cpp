#include <SimpleShell/IO/Console.hpp>
#include <SimpleShell/Parser/Entities/CommandParser.hpp>
#include <SimpleShell/Parser/Entities/ProgramParser.hpp>
#include <SimpleShell/Parser/Entities/StatementParser.hpp>
#include <SimpleShell/Parser/Entities/StringParser.hpp>
#include <SimpleShell/Parser/Entities/VariableParser.hpp>
#include <SimpleShell/Parser/Entities/IoctlParser.hpp>
#include <SimpleShell/Parser/Entities/ExpressionParser.hpp>
#include <SimpleShell/Parser/Entities/PipeParser.hpp>
#include <SimpleShell/Parser/Parser.hpp>

template <std::derived_from<shell::ParsingEntity> Tp>
static void registerEntity(std::string const& name) {
    shell::Parser::registerEntity(name, std::make_unique<Tp>());
}

static void setParsers(void) {
    registerEntity<shell::StringParser>("string");
    registerEntity<shell::VariableParser>("variable");
    registerEntity<shell::CommandParser>("command");
    registerEntity<shell::StatementParser>("statement");
    registerEntity<shell::IoctlParser>("ioctl");
    registerEntity<shell::ExpressionParser>("expression");
    registerEntity<shell::PipeParser>("pipe");
    registerEntity<shell::ProgramParser>("program");
}

int main(void) {
    setParsers();
    shell::Console console;
    shell::Parser parser;

    for (;;) {
        try {
            parser(console.getCommand());
        } catch(std::exception const& exception) {
            std::puts(exception.what());
        }
    }

    return 0;
}
