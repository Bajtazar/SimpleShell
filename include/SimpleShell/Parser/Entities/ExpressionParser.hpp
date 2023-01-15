#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    class ExpressionParser : public ParsingEntity {
    public:
        explicit ExpressionParser(void) noexcept = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

    private:
        using Args = std::vector<std::string>;

        ParsingEntity* statementParser = nullptr;
        ParsingEntity* programParser = nullptr;
        ParsingEntity* ioctlParser = nullptr;

        Command createSubshellExecution(std::string const& command) const;
    };

}
