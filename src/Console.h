#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include <HayBCMD.h>

#ifndef MEATBALL_CONSOLE_LOG
#define MEATBALL_CONSOLE_LOG 1
#endif

namespace Meatball {
    class Console {
    public:
        /// @brief initializes static functionalities of HayBCMD
        static void init(std::function<void(const std::string&)> printFunction) {
            HayBCMD::Output::setPrintFunction(printFunction);
            HayBCMD::BaseCommands::init(&variables);
        #if MEATBALL_CONSOLE_LOG 1
            print("initialized\n");
        #endif
        }

        /// @brief parses a string into HayBCMD
        static void run(const std::string& input);

        static constexpr auto print = HayBCMD::Output::print;

        template<typename ...Args>
        static void printf(const std::string &format, Args ...args);

    private:
        static std::unordered_map<std::string, std::string> variables; // HayBCMD aliases are stored here
    };
}