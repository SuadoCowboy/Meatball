#pragma once

#include <unordered_map>
#include <string>
#include <sstream>

namespace Meatball {
    class Console {
    public:
        static void init();
        static void run(const std::string& input);
        static void print(const std::string& message);
        static std::stringstream output;

    private:
        static std::unordered_map<std::string, std::string> variables; // HayBCMD aliases are stored here
    };
}