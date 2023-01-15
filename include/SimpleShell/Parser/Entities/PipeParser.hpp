#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    class PipeParser : public ParsingEntity {
    public:
        explicit PipeParser(void) noexcept = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;
    private:
        using PipeCB = typename Command::CBPack;
        using PipeContext = std::pair<PipeCB, PipeCB>;

        ParsingEntity* expressionParser = nullptr;

        std::vector<std::string> splitOnPipe(std::string const& command) const;

        void createPipes(std::vector<Command>& commands) const;

        PipeContext createPipeContext(void) const;
    };

}
