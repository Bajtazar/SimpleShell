#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

namespace shell {

    class IoctlParser : public ParsingEntity {
    public:
        using Invocable = std::function<void(void)>;
        using Result = std::pair<Invocable, Invocable>;

        explicit IoctlParser(void) = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

    private:
        using descriptor_t = int;

        struct Descrpitors {
            explicit Descrpitors(void);

            descriptor_t const stdIn;
            descriptor_t const stdOut;
            descriptor_t const stdErr;

            ~Descrpitors(void);
        };

        class Stream {
        public:
            explicit Stream(std::string const& stream);

            [[nodiscard]] descriptor_t get(
                Descrpitors const& desc) const noexcept;

            [[nodiscard]] descriptor_t set(
                Descrpitors const& desc) const noexcept;

            [[nodiscard]] static bool isStream(std::string const& stream) noexcept;
        private:
            descriptor_t descriptor;
            bool isReference;
        };

        Descrpitors descriptors = Descrpitors{};
        ParsingEntity* stringParser = nullptr;

        bool isFileRedirection(std::string const& command) const;

        Result parseRedirection(std::string const& command) const;

        Result getCtl(std::string const& left, std::string const& right,
                      bool direction) const;

        Result fileAndProc(std::string const& fileName, bool direction) const;

        Result streamToStream(std::string const& left, std::string const& right,
                             bool direction) const;

        static constexpr bool OUT = true;
        static constexpr bool IN = false;
    };

}
