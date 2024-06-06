#include "FontsHandler.h"

std::unordered_map<std::string, Font> Meatball::FontsHandler::fonts;

static void use(Font *font) {
    SetTextLineSpacing(font->baseSize);
}

bool Meatball::FontsHandler::load(std::filesystem::path path, std::string name) {
    if (fonts.count(name) != 0
        || !std::filesystem::exists(path)
        || std::filesystem::is_directory(path)) return false;
    
    fonts[name] = LoadFont(path.string().c_str());
    return true;
}

bool Meatball::FontsHandler::loadEx(std::filesystem::path path, std::string name, unsigned char fontSize, int *codePoints, int codePointsCount) {
    if (fonts.count(name) != 0
        || !std::filesystem::exists(path)
        || std::filesystem::is_directory(path)) return false;

    fonts[name] = LoadFontEx(path.string().c_str(), fontSize, codePoints, codePointsCount);
    return true;
}

void Meatball::FontsHandler::unload(const std::string &name) {
    if (fonts.count(name) == 0) return;

    UnloadFont(fonts[name]);
    fonts.erase(name);
}

Font *Meatball::FontsHandler::get(const std::string &name) {
    if (fonts.count(name) == 0) return nullptr;

    return &fonts[name];
}

Vector2 Meatball::FontsHandler::MeasureText(Font *font, const char *text, float spacing) {
    use(font);
    return MeasureTextEx(*font, text, font->baseSize, spacing);
}

float Meatball::FontsHandler::MeasureTextWidth(Font *font, const char *text, float spacing) {
    use(font);
    return MeasureTextEx(*font, text, font->baseSize, spacing).x;
}

float Meatball::FontsHandler::MeasureTextHeight(Font *font, const char *text, float spacing) {
    use(font);
    return MeasureTextEx(*font, text, font->baseSize, spacing).y;
}

bool Meatball::FontsHandler::add(Font font, const std::string &name) {
    if (fonts.count(name) != 0) return false;

    fonts[name] = font;
    return true;
}

void Meatball::FontsHandler::clear() {
    for (auto &font : fonts) {
        UnloadFont(font.second);
    }

    fonts.clear();
}