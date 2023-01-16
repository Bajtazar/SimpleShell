#pragma once

#include <cinttypes>

namespace shell {

    enum class ShellMode : uint8_t {
        Unknown = 0,
        Interactive = 1,
        Script = 2
    };

    [[nodiscard]] ShellMode parseShellArgs(std::size_t argc, char** argv);

    void startInteractiveShellSession(void);

    void parseScript(const char* scriptName);

}
