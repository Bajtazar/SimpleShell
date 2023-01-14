#pragma once

#include <vector>
#include <string>
#include <variant>
#include <functional>

namespace shell {

    class Command {
    public:
        using Args = std::vector<std::string>;
        using Invocable = std::function<void(Args const&)>;

        explicit Command(Invocable invocable);

        explicit Command(std::string const& path);

        void setArgs(Args&& args);

        void operator() (void);

        [[nodiscard]] bool isExternalProgram(void) const noexcept;
    private:
        using State = std::variant<Invocable, std::string>;

        Args args;
        State state;

        char** prepareArgTable(void);

        std::string getPureCommand(void);
    };

}
