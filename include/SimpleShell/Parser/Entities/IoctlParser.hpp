#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/IoController.hpp>

namespace shell {

    class IoctlParser : public ParsingEntity {
    public:
        explicit IoctlParser(void) noexcept = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

    private:
        using descriptor_t = typename DeferredIoctl::descriptor_t;

        ParsingEntity* stringParser = nullptr;

        bool isFileRedirection(std::string const& command) const;

        DeferredIoctl parseFileRedirection(std::string const& command) const;

        DeferredIoctl getCtl(std::string const& left, std::string const& right) const;

        DeferredIoctl fileToProc(std::string const& fileName) const;

        DeferredIoctl procToFile(std::string const& fileName) const;

        DeferredIoctl streamToStream(descriptor_t left, descriptor_t right) const;
    };

}
