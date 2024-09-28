#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include <SweatCI.h>

namespace Meatball {
    class Console {
    public:
        /// @brief initializes static functionalities of SweatCI
        static void init(void *printFuncData, const SweatCI::PrintFunction& printFunction);

        /// @brief parses a string into SweatCI
        static void run(const std::string& input);

        static constexpr auto print = SweatCI::Output::print;

        template<typename ...Args>
        static void printf(const SweatCI::OutputLevel& level, const std::string& format, Args ...args) {
            SweatCI::Output::printf(level, format, args...);
        }
    
        static std::unordered_map<std::string, std::string> variables; // SweatCI aliases are stored here
    };
}