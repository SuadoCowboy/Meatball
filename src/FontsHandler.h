#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include <raylib.h>

namespace Meatball {
    class FontsHandler {
    public:
        static void add(unsigned short id, const Font &font);

        static void clear();
        
        static std::shared_ptr<Font> get(unsigned short id, int size);

    private:
        //unordered_map<fontIdx, vector<Font->baseSize, Font>>
        //fonts[0][0] should be default raylib font
        static std::unordered_map<unsigned short, std::vector<std::shared_ptr<Font>>> fonts;
    };
}