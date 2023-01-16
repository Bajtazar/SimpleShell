#include <SimpleShell/System/Command.hpp>

#include <algorithm>
#include <ranges>

extern "C" {
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
}

namespace shell {

    Command::Command(Invocable invocable) :
        state{std::move(invocable)} {}

    Command::Command(std::string const& path) :
        state{path} {}

    std::string Command::getPureCommand(void) {
        auto const& command = std::get<std::string>(state);
        auto iter = std::ranges::find(command, '/');
        if (iter == command.end())
            return command;
        return { iter, command.end() };
    }

    char** Command::prepareArgTable(void) {
        char** table = new char*[args.size() + 2]{};
        auto const command = getPureCommand();
        table[0] = new char[command.size() + 1]{};
        std::ranges::copy(command, table[0]);
        for (size_t i = 1; auto const& arg : args) {
            table[i] = new char[arg.size() + 1]{};
            std::ranges::copy(arg, table[i++]);
        }
        table[args.size() + 1] = nullptr;
        return table;
    }

    void Command::setArgs(Args&& args) {
        this->args = std::move(args);
    }

    void Command::setCallbacks(Callbacks const& callbacks) {
        this->callbacks = callbacks;
    }

    void Command::setPipeCallbacks(Callbacks const& callbacks) {
        this->pipeline = callbacks;
    }

    void Command::operator()(void) {
        try {
            CallbackGuard pipeguard{pipeline};
            CallbackGuard guard{callbacks};
            if (std::holds_alternative<Invocable>(state))
                std::get<Invocable>(state)(args);
            else {
                execvp(std::get<std::string>(state).c_str(), prepareArgTable());
            }
        } catch(std::exception const& exception) {
            if (not isExternalProgram())
                std::rethrow_exception(std::current_exception());
        }
        if (isExternalProgram())
            exit(0);
    }

    void Command::setExternalExecution(void) {
        forcedExternal = true;
    }

    Command::CallbackGuard::CallbackGuard(Callbacks& invocables) :
        invocables{invocables}
    {
        for (auto const& [opener, closer] : invocables)
            opener();
    }

    Command::CallbackGuard::~CallbackGuard(void) {
        for (auto const& [opener, closer] : invocables | std::views::reverse)
            closer();
    }

    void Command::closePipeDesc(void) {
        for (auto const& [opener, closer] : pipeline | std::views::reverse)
            closer();
    }

    [[nodiscard]] bool Command::isExternalProgram(void) const noexcept {
        return std::holds_alternative<std::string>(state) || forcedExternal;
    }

}
