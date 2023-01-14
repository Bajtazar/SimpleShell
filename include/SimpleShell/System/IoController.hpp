#pragma once

#include <cinttypes>

#include <optional>

namespace shell {

    class IoController {
    public:
        using descriptor_t = int32_t;

        explicit IoController(void) = default;

        void setStdin(descriptor_t newStdin);

        void setStdout(descriptor_t newStdout);

        void setStderr(descriptor_t newStderr);

        void restore(void);
    private:
        std::optional<descriptor_t> in = std::nullopt;
        std::optional<descriptor_t> out = std::nullopt;
        std::optional<descriptor_t> error = std::nullopt;

        void restoreDescriptor(std::optional<descriptor_t>& handler,
                               descriptor_t target) const;
    };

    class IoGuardian {
    public:
        explicit IoGuardian(IoController& controller);

        IoGuardian(IoGuardian const&) = delete;

        IoGuardian& operator=(IoGuardian const&) = delete;

        ~IoGuardian(void);
    private:
        IoController& controller;
    };

}
