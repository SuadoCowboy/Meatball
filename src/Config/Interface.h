#pragma once

#include <raylib.h>

// Color *color, *hoveredColor, *textColor, *hoveredTextColor; // colors that are equal in other interfaces

namespace Meatball { namespace Config {
    struct Button {
        Color color;
        Color textColor;
        
        Color hoveredColor;
        Color hoveredTextColor;

        Font* font;
    };

    struct TextButton {
        Color color;
        Color textColor;

        Color hoveredColor;
        Color hoveredTextColor;

        Font* font;
    };

    struct DynamicPanel {
        Color color;
        Vector2 minSize;
        float grabHeight;
    };
    
    struct Console {
        Color autoCompleteColor; // color of the rect
        Color autoCompleteTextColor;
        Color autoCompleteHighlightedTextColor;
        Color autoCompleteSelectedTextColor;

        Color labelColor;

        Font *mainFont; // used for input/output and (if exists)send button
        Font *labelFont; // used only for label

        char labelText[20];
    };
}}