#pragma once

#include <raylib.h>

// Color *color, *hoveredColor, *textColor, *hoveredTextColor; // colors that are equal in other interfaces

namespace Meatball { namespace Config {
    struct Button {
        Button();

        Color color;
        Color hoveredColor;
    };

    struct TextButton {
        TextButton();

        Color color;
        Color textColor;

        Color hoveredColor;
        Color hoveredTextColor;

        Font* font;
    };

    struct DynamicPanel {
        DynamicPanel();

        Color color;
        Vector2 minSize;
        float grabHeight;
    };

    struct InputTextBox {
        InputTextBox();

        Color color, textColor, cursorColor;
        Font *font;
    };
    
    struct Console {
        Console();

        Color autoCompleteColor; // color of the rect
        Color autoCompleteTextColor;
        Color autoCompleteHighlightedTextColor;
        Color autoCompleteSelectedTextColor;

        Color labelTextColor;

        Font *mainFont; // used for input/output and (if exists)send button
        Font *labelFont; // used only for label

        char labelText[20];
    };
}}