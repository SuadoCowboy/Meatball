#pragma once

#include <unordered_map>
#include <string>
#include <sstream>

namespace Meatball {
    class Console {
    public:
        /// @brief initializes static functionalities of HayBCMD
        static void init();

        /// @brief parses a string into HayBCMD
        static void run(const std::string& input);
        
        static void addToOutput(const std::string& message);

        static std::stringstream output;
    private:
        static std::unordered_map<std::string, std::string> variables; // HayBCMD aliases are stored here
    };
}