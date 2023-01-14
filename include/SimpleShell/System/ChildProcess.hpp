#pragma once

#include <cinttypes>
#include <type_traits>
#include <concepts>
#include <optional>

extern "C" {
    #include <sys/types.h>
    #include <unistd.h>
    #include <stdlib.h>
}

namespace shell {

    class ChildProcess {
    public:
        template <std::invocable Invocable>
        ChildProcess(Invocable invocable);

        ChildProcess(ChildProcess&& other);

        ChildProcess& operator=(ChildProcess&&) = delete;

        [[nodiscard]] std::optional<int> waitFor(void);

        [[nodiscard]] bool isAlive(void) const noexcept;

        [[nodiscard]] operator bool() const noexcept;

        bool terminate(void) noexcept;

        bool abort(void) noexcept;

        ~ChildProcess(void);
    private:
        pid_t childID;
    };

}

#include <SimpleShell/System/ChildProcess.tpp>
