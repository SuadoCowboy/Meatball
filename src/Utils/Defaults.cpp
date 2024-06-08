#include "Defaults.h"

#include <filesystem>
#include <cstring>

#include <HayBCMD.h>

#include "Console.h"

#include "Config.h"
#include "FontsHandler.h"

#include "Interface/DynamicPanel.h"
#include "Interface/Button.h"
#include "Interface/TextButton.h"
#include "Interface/InputTextBox.h"
#include "Interface/ScrollBar.h"
#include "Interface/ScrollTextBox.h"

#define DEFAULT_FONT_SIZE 16

void Meatball::Defaults::init(const std::string& meatdataPath) {
    auto initData = Config::loadData(meatdataPath);
    FontsHandler::add(GetFontDefault(), "default");
    
    Meatball::Config::ConfigData* data;
    dynamicPanelConfig = std::make_shared<Config::DynamicPanel>();
    {
        data = Config::ifContainsGet(initData, "mainPanelColor");
        if (data) dynamicPanelConfig->color = data->colorV;
    }

    buttonConfig = std::make_shared<Config::Button>();
    {
        data = Config::ifContainsGet(initData, "buttonColor");
        if (data) buttonConfig->color = data->colorV;

        data = Config::ifContainsGet(initData, "buttonHoveredColor");
        if (data) buttonConfig->hoveredColor = data->colorV;
    }

    textButtonConfig = std::make_shared<Config::TextButton>();
    {
        data = Config::ifContainsGet(initData, "textButtonFont");
        if (data) {
            auto fontSizeData = Config::ifContainsGet(initData, "textButtonFontSize");
            
            std::filesystem::path fontPath = data->stringV;
            std::string fontName = fontPath.filename().string();

            if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : DEFAULT_FONT_SIZE, nullptr, 0))
                textButtonConfig->font = FontsHandler::get(fontName);
        }

        data = Config::ifContainsGet(initData, "textButtonColor");
        if (data) textButtonConfig->color = data->colorV;

        data = Config::ifContainsGet(initData, "textButtonTextColor");
        if (data) textButtonConfig->textColor = data->colorV;

        data = Config::ifContainsGet(initData, "textButtonHoveredColor");
        if (data) textButtonConfig->hoveredColor = data->colorV;

        data = Config::ifContainsGet(initData, "textButtonHoveredTextColor");
        if (data) textButtonConfig->hoveredTextColor = data->colorV;
    }

    inputTextBoxConfig = std::make_shared<Config::InputTextBox>();
    {
        data = Config::ifContainsGet(initData, "InputTextBoxFont");
        if (data) {
            auto fontSizeData = Config::ifContainsGet(initData, "InputTextBoxFontSize");
            
            std::filesystem::path fontPath = data->stringV;
            std::string fontName = fontPath.filename().string();

            if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : DEFAULT_FONT_SIZE, nullptr, 0))
                inputTextBoxConfig->font = FontsHandler::get(fontName);
        }

        inputTextBoxConfig->font = FontsHandler::get("default");

        data = Config::ifContainsGet(initData, "inputTextBoxColor");
        if (data) inputTextBoxConfig->color = data->colorV;

        data = Config::ifContainsGet(initData, "inputTextBoxTextColor");
        if (data) inputTextBoxConfig->textColor = data->colorV;

        data = Config::ifContainsGet(initData, "inputTextBoxCursorColor");
        if (data) inputTextBoxConfig->cursorColor = data->colorV;
    }

    scrollBarConfig = std::make_shared<Config::ScrollBar>();
    {
        data = Config::ifContainsGet(initData, "scrollBarColor");
        if (data) scrollBarConfig->barColor = data->colorV;

        data = Config::ifContainsGet(initData, "scrollBarThumbColor");
        if (data) scrollBarConfig->thumbColor = data->colorV;

        data = Config::ifContainsGet(initData, "scrollBarHoveredColor");
        if (data) scrollBarConfig->barHoveredColor = data->colorV;

        data = Config::ifContainsGet(initData, "scrollBarThumbHoveredColor1");
        if (data) scrollBarConfig->thumbHoveredColor1 = data->colorV;

        data = Config::ifContainsGet(initData, "scrollBarThumbHoveredColor2");
        if (data) scrollBarConfig->thumbHoveredColor2 = data->colorV;
    }

    scrollTextBoxConfig = std::make_shared<Config::ScrollTextBox>();
    {
        data = Config::ifContainsGet(initData, "ScrollTextBoxFont");
        if (data) {
            auto fontSizeData = Config::ifContainsGet(initData, "ScrollTextBoxFontSize");
            
            std::filesystem::path fontPath = data->stringV;
            std::string fontName = fontPath.filename().string();

            if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : DEFAULT_FONT_SIZE, nullptr, 0))
                scrollTextBoxConfig->font = FontsHandler::get(fontName);
        }

        data = Config::ifContainsGet(initData, "color");
        if (data) scrollTextBoxConfig->color = data->colorV;

        data = Config::ifContainsGet(initData, "textColor");
        if (data) scrollTextBoxConfig->textColor = data->colorV;
    }
}

Meatball::ConsoleUIScene Meatball::Defaults::initLocalConsole(const Rectangle& rect, const std::string &meatdataPath) {
    std::vector<std::string> messages;
    HayBCMD::Output::setPrintFunction([&](const std::string &message) {
        messages.push_back(message);
    });
    
    auto consoleData = Config::loadData(meatdataPath);
    auto consoleConfig = std::make_shared<Config::ConsoleUI>();
    Config::ConfigData *data = Config::ifContainsGet(consoleData, "margin");
    
    Meatball::ConsoleUIScene::margin = data?
        data->unsignedCharV : Meatball::ConsoleUIScene::margin;

    Config::ConfigData *fontSizeData = Config::ifContainsGet(consoleData, "fontSize");
    data = Config::ifContainsGet(consoleData, "font");
    if (data) {
        std::filesystem::path fontPath = data->stringV;
        std::string fontName = fontPath.filename().string();
        if (FontsHandler::loadEx(fontPath, fontName, fontSizeData? fontSizeData->unsignedCharV : DEFAULT_FONT_SIZE, nullptr, 0))
            consoleConfig->mainFont = FontsHandler::get(fontName);

        if (FontsHandler::loadEx(fontPath, fontName+"Half", fontSizeData? fontSizeData->unsignedCharV*0.5 : DEFAULT_FONT_SIZE*0.5, nullptr, 0))
            consoleConfig->labelFont = FontsHandler::get(fontName+"Half");
    }

    data = Config::ifContainsGet(consoleData, "autoCompleteColor");
    if (data) consoleConfig->autoCompleteColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteTextColor");
    if (data) consoleConfig->autoCompleteTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteHightlightedTextColor");
    if (data) consoleConfig->autoCompleteHighlightedTextColor = data->colorV;

    data = Config::ifContainsGet(consoleData, "autoCompleteSelectedTextColor");
    if (data) consoleConfig->autoCompleteSelectedTextColor = data->colorV;
    
    data = Config::ifContainsGet(consoleData, "labelColor");
    if (data) consoleConfig->labelTextColor = data->colorV;

    strcpy(consoleConfig->labelText, "Local Console");

    auto consoleUI = Meatball::ConsoleUIScene({rect.x, rect.y, rect.width, rect.height}, std::move(consoleConfig));

    // TODO: if ConsoleUI data contains changes related to static
    // configs, create a new shared ptr and use std::swap(old, new) and maybe std::move if something goes wrong?
    //data = Config::ifContainsGet(consoleData, "outputBoxColor");
    //if (data) consoleUI.outputBox.config->color = data->colorV;

    //data = Config::ifContainsGet(consoleData, "outputBoxTextColor");
    //if (data) consoleUI.outputBox.config->textColor = data->colorV;

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