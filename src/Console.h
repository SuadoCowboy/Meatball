#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include <HayBCMD.h>

namespace Meatball {
    class Console {
    public:
        /// @brief initializes static functionalities of HayBCMD
        static void init(void *printFuncData, const HayBCMD::PrintFunction& printFunction);

        /// @brief parses a string into HayBCMD
        static void run(const std::string& input);

        static constexpr auto print = HayBCMD::Output::print;

        template<typename ...Args>
        static void printf(const HayBCMD::OutputLevel& level, const std::string& format, Args ...args) {
            HayBCMD::Output::printf(level, format, args...);
        }
    
        static std::unordered_map<std::string, std::string> variables; // HayBCMD aliases are stored here
    };
}