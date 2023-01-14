#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

#include <vector>
#include <string>
#include <variant>

namespace shell {

    class Command {
    public:
        using Args = std::vector<std::string>;
        using Invocable = std::function<void(Args const&)>;

        explicit Command(Invocable invocable);

        explicit Command(std::string const& path);

        void operator() (Args const& args);
    private:
        using State = std::variant<Invocable, std::string>;

        State state;

        char** prepareArgTable(Args const& args);

        std::string getPureCommand(void);
    };

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
