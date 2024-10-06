#include "Theme.h"

#include "Utils/Json.h"

static void getButtonThemes(Meatball::Json& fileData, Meatball::ButtonTheme& theme) {
    Meatball::Json data;
    if (!fileData.getJson("button", data))
        return;

    data.getColor("color", theme.color);
    data.getColor("hoveredColor", theme.hoveredColor);
}

static void getTextButtonThemes(Meatball::Json& fileData, Meatball::TextButtonTheme& theme) {
    Meatball::Json data;
    if (!fileData.getJson("textbutton", data))
        return;

    data.getColor("textColor", theme.textColor);
    data.getColor("color", theme.color);
    data.getColor("hoveredColor", theme.hoveredColor);
}

static void getDynamicPanelThemes(Meatball::Json& fileData, Meatball::DynamicPanelTheme& theme) {
    Meatball::Json data;
    if (!fileData.getJson("dynamicpanel", data))
        return;
    
    data.getColor("color", theme.color);
}

static void getInputTextBoxThemes(Meatball::Json& fileData, Meatball::InputTextBoxTheme& theme) {
    Meatball::Json data;
    if (!fileData.getJson("inputtextbox", data))
        return;

    data.getColor("color", theme.color);
    data.getColor("color", theme.textColor);
    data.getColor("color", theme.cursorColor);
    data.getColor("color", theme.selectionColor);
}

static void getScrollBarThemes(Meatball::Json& fileData, Meatball::ScrollBarTheme& theme) {
    Meatball::Json data;
    if (!fileData.getJson("inputtextbox", data))
        return;

    data.getColor("barColor", theme.barColor);
    data.getColor("barHoveredColor", theme.barHoveredColor);
    data.getColor("thumbColor", theme.thumbColor);
    data.getColor("thumbHoveredColor1", theme.thumbHoveredColor1);
    data.getColor("thumbHoveredColor2", theme.thumbHoveredColor2);
}

static void getScrollBoxThemes(Meatball::Json& fileData, Meatball::ScrollBoxTheme& theme) {
    Meatball::Json data;
    if (!fileData.getJson("inputtextbox", data))
        return;

    data.getColor("barColor", theme.color);
}

Meatball::Theme Meatball::loadTheme(const std::string& path) {
    Json fileData{path};
    Theme theme;

    getButtonThemes(fileData, theme.button);
    getTextButtonThemes(fileData, theme.textButton);
    getDynamicPanelThemes(fileData, theme.dynamicPanel);
    getInputTextBoxThemes(fileData, theme.inputTextBox);
    getScrollBarThemes(fileData, theme.scrollBar);
    getScrollBoxThemes(fileData, theme.scrollBox);

    return theme;
}