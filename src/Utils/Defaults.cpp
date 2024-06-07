#include "Defaults.h"

#include <filesystem>
#include <cstring>
#include <memory>

#include <HayBCMD.h>

#include "Console.h"

#include "Config/Config.h"
#include "Config/Interface.h"
#include "FontsHandler.h"

void Meatball::init() {
    //auto initData = Config::loadData(meatdataPath);
    
    // WARNING: if the developer does not use this function, he might need to define a default font
    FontsHandler::add(GetFontDefault(), "default");
}

Meatball::ConsoleUIScene Meatball::initLocalConsole(const Rectangle& rect, const std::string &meatdataPath) {
    std::vector<std::string> messages;
    HayBCMD::Output::setPrintFunction([&](const std::string &message) {
        messages.push_back(message);
    });
    
    auto consoleData = Config::loadData(meatdataPath);
    
    Config::ConfigData *data = Config::ifContainsGet(consoleData, "margin");
    Meatball::ConsoleUIScene::margin = data?
        data->unsignedCharV : Meatball::ConsoleUIScene::margin;
    
    auto consoleConfig = std::make_shared<Config::Console>();
    auto closeButtonConfig = std::make_shared<Config::Button>();
    auto mainPanelConfig = std::make_shared<Config::DynamicPanel>();
    auto inputBoxConfig = std::make_shared<Config::InputTextBox>();

    Config::ConfigData *fontSizeData = Config::ifContainsGet(consoleData, "fontSize");
    data = Config::ifContainsGet(consoleData, "font");
    if (data) {
        std::filesystem::path fontPath = data->stringV;
        std::string fontName = fontPath.filename().string();
        if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : 16, nullptr, 0))
            consoleConfig->mainFont = FontsHandler::get(fontName);
        else
            consoleConfig->mainFont = FontsHandler::get("default");

        if (FontsHandler::loadEx(fontPath, fontName+"Half", fontSizeData? fontSizeData->unsignedCharV*0.5 : 8, nullptr, 0))
            consoleConfig->labelFont = FontsHandler::get(fontName+"Half");
        else
            consoleConfig->labelFont = FontsHandler::get("default");
    } else
        consoleConfig->labelFont = consoleConfig->mainFont = FontsHandler::get("default");

    data = Config::ifContainsGet(consoleData, "autoCompleteColor");
    if (data) consoleConfig->autoCompleteColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteTextColor");
    if (data) consoleConfig->autoCompleteTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteHightlightedTextColor");
    if (data) consoleConfig->autoCompleteHighlightedTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteSelectedTextColor");
    if (data) consoleConfig->autoCompleteSelectedTextColor = data->colorV;
    
    data = Config::ifContainsGet(consoleData, "mainPanelColor");
    if (data) mainPanelConfig->color = data->colorV;

    data = Config::ifContainsGet(consoleData, "closeButtonColor");
    if (data) closeButtonConfig->color = data->colorV;

    data = Config::ifContainsGet(consoleData, "closeButtonHoveredColor");
    if (data) closeButtonConfig->hoveredColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "inputBoxColor");
    if (data) inputBoxConfig->color = data->colorV;

    data = Config::ifContainsGet(consoleData, "inputBoxTextColor");
    if (data) inputBoxConfig->textColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "inputBoxCursorColor");
    if (data) inputBoxConfig->cursorColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "labelColor");
    if (data) consoleConfig->labelTextColor = data->colorV;

    strcpy(consoleConfig->labelText, "Local Console");

    auto consoleUI = Meatball::ConsoleUIScene({rect.x, rect.y, rect.width, rect.height}, std::move(consoleConfig), std::move(mainPanelConfig), std::move(closeButtonConfig), std::move(inputBoxConfig));

    data = Config::ifContainsGet(consoleData, "outputBoxColor");
    if (data) consoleUI.outputBox.color = data->colorV;

    data = Config::ifContainsGet(consoleData, "outputBoxTextColor");
    if (data) consoleUI.outputBox.textColor = data->colorV;

    Console::init([&](const std::string &message) {
        size_t spaceIdxBefore = 0;
        size_t currentSpaceIdx = message.find('\n');
        
        while (currentSpaceIdx != std::string::npos) {
            consoleUI.print(message.substr(spaceIdxBefore, currentSpaceIdx-spaceIdxBefore));
            spaceIdxBefore = currentSpaceIdx+1;
            currentSpaceIdx = message.find('\n', spaceIdxBefore);
        }

        consoleUI.print(message.substr(spaceIdxBefore));
    });

    Console::print("Console initialized");

    for (auto &message : messages)
        Console::print(message);

    HayBCMD::Command("clear", 0, 0,
        [&](HayBCMD::Command*, const std::vector<std::string>&) {
            consoleUI.outputBox.clearText();}, "- clears the console");

    return consoleUI;
}