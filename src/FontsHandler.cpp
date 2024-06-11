#include "FontsHandler.h"

std::unordered_map<unsigned short, std::vector<std::shared_ptr<Font>>> Meatball::FontsHandler::fonts = {};

void Meatball::FontsHandler::add(unsigned short id, const Font &font) {
    if (fonts.count(id) == 0)
        fonts.emplace(id, std::vector<std::shared_ptr<Font>>());

    if (id != 0 || fonts[id].size() == 0)
        fonts[id].push_back(std::make_shared<Font>(font));
}

void Meatball::FontsHandler::clear() {
    fonts.erase(0);

    for (auto& enumeratedFonts : fonts) {
        for (auto& font : enumeratedFonts.second)
            UnloadFont(*font);
    }
    
    fonts.clear();
}

std::shared_ptr<Font> Meatball::FontsHandler::get(unsigned short id, int size) {
    if (size != 0)
        for (auto font : fonts[id])
            if (font->baseSize == size) {
                return font;
            }
    
    return fonts[0][0];
}