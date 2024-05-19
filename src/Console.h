#pragma once

#include <unordered_map>
#include <string>
#include <sstream>

#include <HayBCMD.h>

namespace Meatball {
    class Console {
    public:
        static void init();
        static void run(const std::string& input);
        static void print(const std::string& message);
        static void printf(const std::string& format, const std::vector<HayBCMD::Data>& args);
        
        static std::stringstream output;

    private:
        static std::unordered_map<std::string, std::string> variables; // HayBCMD aliases are stored here
    };
}