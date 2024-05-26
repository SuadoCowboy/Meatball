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
    
    Config::ConfigData* marginData = Config::ifContainsGet(consoleData, "margin");
    Meatball::ConsoleUIScene::margin = marginData?
        marginData->unsignedCharV : Meatball::ConsoleUIScene::margin;

    Config::ConfigData* fontSizeData = Config::ifContainsGet(consoleData, "fontSize");
    Config::ConfigData* fontData = Config::ifContainsGet(consoleData, "font");
    
    std::filesystem::path fontPath = fontData->stringV;
    std::string fontName = fontPath.filename().string();

    // WARNING: if the developer does not use this function, he might need to define a default font
    FontsHandler::add(GetFontDefault(), "default");

    Font* font = nullptr;
    if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : 16, nullptr, 0))
        font = FontsHandler::get(fontName);
    else
        font = FontsHandler::get("default");
    
    auto consoleUI = Meatball::ConsoleUIScene(rect.x, rect.y, rect.width, rect.height, font);
    
    Config::ConfigData* mainPanelColorData = Config::ifContainsGet(consoleData, "mainPanelColor");
    if (mainPanelColorData) consoleUI.mainPanel.color = mainPanelColorData->colorV;

    Config::ConfigData* closeButtonColorData = Config::ifContainsGet(consoleData, "closeButtonColor");
    if (closeButtonColorData) consoleUI.closeButton.color = closeButtonColorData->colorV;

    Config::ConfigData* closeButtonHoveredColorData = Config::ifContainsGet(consoleData, "closeButtonHoveredColor");
    if (closeButtonHoveredColorData) consoleUI.closeButton.hoveredColor = closeButtonHoveredColorData->colorV;

    Config::ConfigData* outputBoxColorData = Config::ifContainsGet(consoleData, "outputBoxColor");
    if (outputBoxColorData) consoleUI.outputBox.color = outputBoxColorData->colorV;

    Config::ConfigData* outputBoxTextColorData = Config::ifContainsGet(consoleData, "outputBoxTextColor");
    if (outputBoxTextColorData) consoleUI.outputBox.textColor = outputBoxTextColorData->colorV;

    Config::ConfigData* inputBoxColorData = Config::ifContainsGet(consoleData, "inputBoxColor");
    if (inputBoxColorData) consoleUI.inputBox.color = inputBoxColorData->colorV;

    Config::ConfigData* inputBoxTextColorData = Config::ifContainsGet(consoleData, "inputBoxTextColor");
    if (inputBoxTextColorData) consoleUI.inputBox.textColor = inputBoxTextColorData->colorV;

    Config::ConfigData* inputBoxCursorColorData = Config::ifContainsGet(consoleData, "inputBoxCursorColor");
    if (inputBoxCursorColorData) consoleUI.inputBox.cursorColor = inputBoxCursorColorData->colorV;

    Console::init([&](const std::string& message) {
        consoleUI.print(message);
    });

    Console::print("Console initialized");

    for (auto& message : messages)
        Console::print(message);

    return consoleUI;
}