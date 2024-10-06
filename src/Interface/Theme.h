#pragma once

#include <unordered_map>
#include <string>

#include <raylib.h>

namespace Meatball {
    struct ButtonTheme {
        Color color = BLACK;
        Color hoveredColor = WHITE;
    };

    struct TextButtonTheme {
        Color color = BLACK;
        Color textColor = WHITE;

        Color hoveredColor = WHITE;
        Color hoveredTextColor = BLACK;
    };

    struct DynamicPanelTheme {
        Color color = BLACK;
        // TODO: round border stuff(?)
    };

    struct InputTextBoxTheme {
        Color color = {10,10,10,255};
        Color textColor = WHITE;
        Color cursorColor = WHITE;
        Color selectionColor = {100,100,100,55};
    };

    struct ScrollBarTheme {
        Color barColor = {15,15,15,255};
        Color barHoveredColor = {25,25,25,255};
        
        Color thumbColor = {30,30,30,255};
        Color thumbHoveredColor1 = {60,60,60,255};
        Color thumbHoveredColor2 = {90,90,90,255};
    };

    struct ScrollBoxTheme {
        Color color = BLACK;
    };

    struct Theme {
        ButtonTheme button;
        TextButtonTheme textButton;
        DynamicPanelTheme dynamicPanel;
        InputTextBoxTheme inputTextBox;
        ScrollBarTheme scrollBar;
        ScrollBoxTheme scrollBox;
    };

    /// @param path json file containing theme data
    Theme loadTheme(const std::string& path);
}