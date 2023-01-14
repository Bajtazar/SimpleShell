#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    class CommandParser : public ParsingEntity {
    public:
        explicit CommandParser(void) = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;
    private:

        bool isSpecialDot(std::string const& command) const;

        bool isUsrBinExec(std::string const& command) const;

        bool isPathBasedExec(std::string const& command) const;

        bool doesExecExist(std::string const& path) const;

        Command generateVariableSetter(std::string const& command) const;

        std::string generateSpecialDotPath(std::string const& command) const;

        std::string generateUsrBinPath(std::string const& command) const;
    };

}
