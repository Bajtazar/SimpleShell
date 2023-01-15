#pragma once

namespace shell {

    template <Restorable Tp>
    IoGuardian<Tp>::IoGuardian(IoController& controller) :
        controller{controller} {}

    template <Restorable Tp>
    IoGuardian<Tp>::~IoGuardian(void) {
        controller.restore();
    }

}
