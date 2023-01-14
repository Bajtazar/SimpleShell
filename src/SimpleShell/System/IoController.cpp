#include <SimpleShell/System/IoController.hpp>

#include <cstdlib>

extern "C" {
    #include <unistd.h>
}

namespace shell {

    void IoController::setStdin(descriptor_t newStdin) {
        in = dup(STDIN_FILENO);
        dup2(newStdin, STDIN_FILENO);
    }

    void IoController::setStdout(descriptor_t newStdout) {
        out = dup(STDOUT_FILENO);
        dup2(newStdout, STDOUT_FILENO);
    }

    void IoController::setStderr(descriptor_t newStderr) {
        error = dup(STDERR_FILENO);
        dup2(newStderr, STDERR_FILENO);
    }

    void IoController::restore(void) {
        restoreDescriptor(in, STDIN_FILENO);
        restoreDescriptor(out, STDOUT_FILENO);
        restoreDescriptor(error, STDERR_FILENO);
    }

    void IoController::restoreDescriptor(std::optional<descriptor_t>& handler,
                                         descriptor_t target) const
    {
        if (not handler) return;
        dup2(*handler, target);
        close(*handler);
        handler = std::nullopt;
    }

    IoGuardian::IoGuardian(IoController& controller) :
        controller{controller} {}

    IoGuardian::~IoGuardian(void) {
        controller.restore();
    }

}
