#pragma once

#include <unordered_map>
#include <string>

#include <raylib.h>

namespace Meatball {
    class ColorsHandler {
    public:
        static void add(const std::string& name, Color color);
        /// @return nullptr if not found
        static Color *get(const std::string& name);
        static void clear();
    private:
        static std::unordered_map<std::string, Color> colors;
    };
}