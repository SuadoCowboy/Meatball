#include "Defaults.h"

#include <sstream>
#include <functional>
#include <filesystem>

#include <HayBCMD.h>

#include "Config.h"
#include "Console.h"
#include "FontsHandler.h"

Meatball::ConsoleUIScene Meatball::initLocalConsole(Rectangle rect, const std::string& meatdataPath) {
    std::vector<std::string> messages;
    HayBCMD::Output::setPrintFunction([&](const std::string& message) {
        messages.push_back(message);
    });
    
    auto consoleData = Config::loadData(meatdataPath);
    
    Config::ConfigData* data = Config::ifContainsGet(consoleData, "margin");
    Meatball::ConsoleUIScene::margin = data?
        data->unsignedCharV : Meatball::ConsoleUIScene::margin;

    Config::ConfigData* fontSizeData = Config::ifContainsGet(consoleData, "fontSize");
    Config::ConfigData* fontData = Config::ifContainsGet(consoleData, "font");

    // WARNING: if the developer does not use this function, he might need to define a default font
    FontsHandler::add(GetFontDefault(), "default");

    Font* halfSizedFont = nullptr;
    Font* font = nullptr;
    if (fontData) {
        std::filesystem::path fontPath = fontData->stringV;
        std::string fontName = fontPath.filename().string();
        if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : 16, nullptr, 0))
            font = FontsHandler::get(fontName);
        else
            font = FontsHandler::get("default");

        if (FontsHandler::loadEx(fontPath, fontName+"Half", fontSizeData? fontSizeData->unsignedCharV*0.5 : 8, nullptr, 0))
            halfSizedFont = FontsHandler::get(fontName+"Half");
        else
            halfSizedFont = FontsHandler::get("default");
    } else {
        halfSizedFont = font = FontsHandler::get("default");
    }
    
    auto consoleUI = Meatball::ConsoleUIScene(rect.x, rect.y, rect.width, rect.height, font, halfSizedFont);
    
    data = Config::ifContainsGet(consoleData, "autoCompleteColor");
    if (data) consoleUI.autoCompleteBox.color = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteTextColor");
    if (data) consoleUI.autoCompleteTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteHightlightedTextColor");
    if (data) consoleUI.autoCompleteHighlightedTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteSelectedTextColor");
    if (data) consoleUI.autoCompleteSelectedTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "mainPanelColor");
    if (data) consoleUI.mainPanel.color = data->colorV;

    data = Config::ifContainsGet(consoleData, "closeButtonColor");
    if (data) consoleUI.closeButton.color = data->colorV;

    data = Config::ifContainsGet(consoleData, "closeButtonHoveredColor");
    if (data) consoleUI.closeButton.hoveredColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "outputBoxColor");
    if (data) consoleUI.outputBox.color = data->colorV;

    data = Config::ifContainsGet(consoleData, "outputBoxTextColor");
    if (data) consoleUI.outputBox.textColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "inputBoxColor");
    if (data) consoleUI.inputBox.color = data->colorV;

    data = Config::ifContainsGet(consoleData, "inputBoxTextColor");
    if (data) consoleUI.inputBox.textColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "inputBoxCursorColor");
    if (data) consoleUI.inputBox.cursorColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "labelColor");
    if (data) consoleUI.labelColor = data->colorV;

    Console::init([&](const std::string& message) {
        consoleUI.print(message);
    });

    Console::print("Console initialized");

    for (auto& message : messages)
        Console::print(message);

    HayBCMD::Command("clear", 0, 0,
        [&](HayBCMD::Command*, const std::vector<std::string>&) {
            consoleUI.outputBox.clearText();}, "- clears the console");

    return consoleUI;
}