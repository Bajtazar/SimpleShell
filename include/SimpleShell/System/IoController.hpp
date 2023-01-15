#pragma once

#include <cinttypes>
#include <functional>
#include <map>
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

    template <class Tp>
    concept Restorable = requires(Tp& tp) { tp.restore(); };

    template <Restorable Tp>
    class IoGuardian {
    public:
        explicit IoGuardian(IoController& controller);

        IoGuardian(IoGuardian const&) = delete;

        IoGuardian& operator=(IoGuardian const&) = delete;

        ~IoGuardian(void);
    private:
        IoController& controller;
    };

    class DeferredIoctl {
    public:
        using descriptor_t = typename IoController::descriptor_t;
        using Callback = std::function<void(void)>;
        using ExchangeMap = std::map<uint8_t, descriptor_t>;

        explicit DeferredIoctl(
            ExchangeMap const& exchangeMap,
            Callback&& callback = {});

        void setCallback(Callback&& callback);

        void change(void);

        void restore(void);
    private:
        IoController controller;
        ExchangeMap exchangeMap;
        Callback callback;
    };

}

#include <SimpleShell/System/IoController.tpp>
