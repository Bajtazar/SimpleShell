#include <SimpleShell/System/ChildProcess.hpp>

extern "C" {
    #include <sys/wait.h>
}

namespace shell {

    ChildProcess::ChildProcess(ChildProcess&& other)
        : childID{std::exchange(other.childID, 0)}
    {}

    std::optional<int> ChildProcess::waitFor(void) {
        int status;
        if (waitpid(childID, &status, 0) > 0) {
            childID = 0;
            return { status };
        }
        return std::nullopt;
    }

    [[nodiscard]] bool ChildProcess::isAlive(void) const noexcept {
        static constexpr int IS_NOT_DEAD = 0;

        int status;
        if (childID <= 0)
            return false;
        return waitpid(childID, &status, WNOHANG) == IS_NOT_DEAD;
    }

    [[nodiscard]] ChildProcess::operator bool() const noexcept {
        return isAlive();
    }

    bool ChildProcess::terminate(void) noexcept {
        if (not kill(childID, SIGTERM)) {
            childID = 0;
            return true;
        }
        return false;
    }

    bool ChildProcess::abort(void) noexcept {
        if (not kill(childID, SIGABRT)) {
            childID = 0;
            return true;
        }
        return false;
    }

    ChildProcess::~ChildProcess(void) {
        if (childID > 0)
            (void) waitFor();
    }

}
