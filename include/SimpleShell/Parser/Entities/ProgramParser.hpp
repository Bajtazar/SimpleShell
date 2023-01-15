#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    class ProgramParser : public ParsingEntity {
    public:
        explicit ProgramParser(void) = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;
    private:
        ParsingEntity* pipeParser = nullptr;

        std::vector<Command> getCommands(std::string const& command) const;

        void invokeCommand(Command&& command) const;
    };

}
