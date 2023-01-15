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
        using Callback = std::function<void(void)>;
        using CBPack = std::pair<Callback, Callback>;
        using Callbacks = std::vector<CBPack>;

        explicit Command(Invocable invocable);

        explicit Command(std::string const& path);

        void setArgs(Args&& args);

        void setCallbacks(Callbacks const& callbacks);

        void setPipeCallbacks(Callbacks const& callbacks);

        void operator() (void);

        void setExternalExecution(void);

        [[nodiscard]] bool isExternalProgram(void) const noexcept;
    private:
        using State = std::variant<Invocable, std::string>;

        class CallbackGuard {
        public:
            explicit CallbackGuard(Callbacks&& invocables);

            CallbackGuard(CallbackGuard const&) = delete;

            CallbackGuard& operator=(CallbackGuard const&) = delete;

            ~CallbackGuard(void);
        private:
            std::vector<Callback> invocables;
        };

        Callbacks callbacks;
        Callbacks pipeline;
        Args args;
        State state;
        bool forcedExternal = false;

        char** prepareArgTable(void);

        std::string getPureCommand(void);
    };

}
