#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include <raylib.h>

namespace Meatball {
    /// @brief fonts are loaded and unloaded here
    class FontsHandler {
    public:
        /// @brief loads a new font and adds it to the fonts variable
        /// @param path path to the file that contains the font
        /// @param name name of the font, used to identify the font in the fonts variable
        /// @param codePoints is a variable needed for raylib function LoadFontEx
        /// @param codePointsCount is a variable needed for raylib function LoadFontEx
        /// @note see also load function
        /// @return true if loaded or if it was already loaded
        static bool loadEx(std::filesystem::path path, std::string name, unsigned char fontSize, int *codePoints, int codePointsCount); 
        
        /// @brief loads a new font and adds it to the fonts variable
        /// @param path path to the file that contains the font
        /// @param name name of the font, used to identify the font in the fonts variable
        /// @note see also loadEx function
        /// @return true if loaded or if it was already loaded
        static bool load(std::filesystem::path path, std::string name); 
        
        static void unload(const std::string &name);

        static bool add(Font font, const std::string &name);

        static Vector2 MeasureText(Font *font, const char *text, float spacing = 1);
        static float MeasureTextWidth(Font *font, const char *text, float spacing = 1);
        static float MeasureTextHeight(Font *font, const char *text, float spacing = 1);

        /// @brief unloads all fonts
        static void clear();
        
        /// @return font pointer or nullptr if does not exist 
        static Font *get(const std::string &name);
    
    private:
        static std::unordered_map<std::string, Font> fonts;
    };
}
