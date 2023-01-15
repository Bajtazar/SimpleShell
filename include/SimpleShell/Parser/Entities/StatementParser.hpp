#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    class StatementParser : public ParsingEntity {
    public:
        using Result = std::function<void(void)>;

        explicit StatementParser(void) noexcept = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

    private:
        using Args = std::vector<std::string>;

        ParsingEntity* commandParser = nullptr;
        ParsingEntity* variableParser = nullptr;
        ParsingEntity* stringParser = nullptr;
        ParsingEntity* ioctlParser = nullptr;

        Args getArgs(Args const& parsed) const;
    };

}
