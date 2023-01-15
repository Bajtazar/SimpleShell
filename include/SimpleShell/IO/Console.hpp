#pragma once

#include <string>
#include <string_view>

namespace shell {

    class Console {
    public:
        explicit Console(void);

        [[nodiscard]] std::string getCommand(void);

        void printPrompt(void);

        [[nodiscard]] std::string getLineFromStdin(void);

    private:

        void actualizeUser(void);

        void actualizeDirectory(void);

        void formatDirectoryStr(std::string_view directory);

        std::string_view getHome(void);

        std::string user;
        std::string directory;
        std::string const host;

        static std::string getHostname(void);
    };

}
