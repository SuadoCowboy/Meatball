#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include <SweatCI.h>

namespace Meatball {
    namespace Console {
        /// @brief initializes static functionalities of SweatCI
        void init(void* printFuncData, const SweatCI::PrintFunction& printFunction);

        /// @brief parses a string into SweatCI
        void run(const std::string& input);

        constexpr auto print = SweatCI::Output::print;

        template<typename ...Args>
        void printf(const SweatCI::OutputLevel& level, const std::string& format, Args ...args) {
            SweatCI::Output::printf(level, format, args...);
        }
    
        extern std::unordered_map<std::string, std::string> variables; // SweatCI aliases are stored here
    };
}