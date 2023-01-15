#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

namespace shell {

    class IoctlParser : public ParsingEntity {
    public:
        using Invocable = std::function<void(void)>;
        using Result = std::pair<Invocable, Invocable>;

        explicit IoctlParser(void);

        IoctlParser(IoctlParser const&) = delete;

        IoctlParser& operator=(IoctlParser const&) = delete;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

        ~IoctlParser(void) override;
    private:
        using descriptor_t = int;

        ParsingEntity* stringParser = nullptr;
        descriptor_t const stdIn;
        descriptor_t const stdOut;
        descriptor_t const stdErr;

        bool isFileRedirection(std::string const& command) const;

        Result parseRedirection(std::string const& command) const;

        Result getCtl(std::string const& left, std::string const& right,
                      bool direction) const;

        Result fileAndProc(std::string const& fileName, bool direction) const;

        // DeferredIoctl fileToProc(std::string const& fileName) const;

        // DeferredIoctl procToFile(std::string const& fileName) const;

        // DeferredIoctl streamToStream(descriptor_t left, descriptor_t right) const;

        static constexpr bool OUT = true;
        static constexpr bool IN = false;
    };

}
