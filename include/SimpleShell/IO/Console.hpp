#pragma once

#include <string>
#include <string_view>

namespace shell {

    class Console {
    public:
        explicit Console(void);

        void printPrompt(void);

    private:
        void actualizeUser(void);

        void actualizeDirectory(void);

        void formatDirectoryStr(std::string_view directory);

        std::string_view getHome(void);

        std::string user;
        std::string directory;
    };

}
