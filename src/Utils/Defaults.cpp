#include "Defaults.h"

#include <memory>

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
#include "OutputColors.h"
#include "Utils/Utils.h"

void Meatball::Defaults::init(const std::string& meatdataPath) {
    auto initData = Config::loadData(meatdataPath);
    
    FontsHandler::add(0, GetFontDefault());

    Meatball::Config::ConfigData* data;
    dynamicPanelConfig = Config::DynamicPanel();
    {
        data = Config::ifContainsGet(initData, "mainPanelColor");
        if (data) dynamicPanelConfig.color = Config::getConfig<Color>(data)->value;
    }

    buttonConfig = Config::Button();
    {
        data = Config::ifContainsGet(initData, "buttonColor");
        if (data) buttonConfig.color = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "buttonHoveredColor");
        if (data) buttonConfig.hoveredColor = Config::getConfig<Color>(data)->value;
    }

    textButtonConfig = Config::TextButton();
    {
        textButtonConfig.font = FontsHandler::get(0,0);

        data = Config::ifContainsGet(initData, "textButtonColor");
        if (data) textButtonConfig.color = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "textButtonTextColor");
        if (data) textButtonConfig.textColor = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "textButtonHoveredColor");
        if (data) textButtonConfig.hoveredColor = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "textButtonHoveredTextColor");
        if (data) textButtonConfig.hoveredTextColor = Config::getConfig<Color>(data)->value;
    }

    inputTextBoxConfig = Config::InputTextBox();
    {
        inputTextBoxConfig.font = FontsHandler::get(0,0);
        inputTextBoxConfig.fontSize = inputTextBoxConfig.font->baseSize;

        data = Config::ifContainsGet(initData, "inputTextBoxColor");
        if (data) inputTextBoxConfig.color = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "inputTextBoxTextColor");
        if (data) inputTextBoxConfig.textColor = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "inputTextBoxCursorColor");
        if (data) inputTextBoxConfig.cursorColor = Config::getConfig<Color>(data)->value;
    }

    scrollBarConfig = Config::ScrollBar();
    {
        data = Config::ifContainsGet(initData, "scrollBarColor");
        if (data) scrollBarConfig.barColor = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "scrollBarThumbColor");
        if (data) scrollBarConfig.thumbColor = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "scrollBarHoveredColor");
        if (data) scrollBarConfig.barHoveredColor = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "scrollBarThumbHoveredColor1");
        if (data) scrollBarConfig.thumbHoveredColor1 = Config::getConfig<Color>(data)->value;

        data = Config::ifContainsGet(initData, "scrollBarThumbHoveredColor2");
        if (data) scrollBarConfig.thumbHoveredColor2 = Config::getConfig<Color>(data)->value;
    }

    scrollTextBoxConfig = Config::ScrollTextBox();
    {
        scrollTextBoxConfig.font = FontsHandler::get(0,0);

        data = Config::ifContainsGet(initData, "color");
        if (data) scrollTextBoxConfig.color = ((Config::ConfigTypeData<Color>*)data)->value;
    }

    Config::clearData(initData);
}

void Meatball::Defaults::loadConsoleFonts(ConsoleUIScene& consoleUI, const std::filesystem::path& fontPath) {
    int size = (int)consoleUI.inputBox.rect.height - 2 + (int)consoleUI.inputBox.rect.height % 2;
    if (Meatball::loadFont(fontPath, 1, size, nullptr, 0))
        consoleUI.inputBox.config->font = consoleUI.outputBox.config->font = FontsHandler::get(1, size);

    size = consoleUI.inputBox.rect.height*0.5;
    if (Meatball::loadFont(fontPath, 1, size, nullptr, 0))
        consoleUI.config->labelFont = FontsHandler::get(1, size);
    
    consoleUI.onResize(1, 1);
}

Meatball::ConsoleUIScene Meatball::Defaults::initLocalConsole(const Rectangle& rect, const std::string &meatdataPath) {
    std::vector<std::pair<std::string, HayBCMD::OutputLevel>> texts;
    HayBCMD::Output::setPrintFunction([&](const HayBCMD::OutputLevel &level, const std::string &text) {
        texts.push_back({text, level});
    });
    
    auto consoleData = Config::loadData(meatdataPath);
    auto consoleConfig = std::make_shared<Config::ConsoleUI>();

    Config::ConfigData *data = Config::ifContainsGet(consoleData, "margin");
    Meatball::ConsoleUIScene::margin = data?
        Config::getConfig<unsigned char>(data)->value : Meatball::ConsoleUIScene::margin;

    data = Config::ifContainsGet(consoleData, "OutputDefaultColor");
    if (data) Config::OutputColors::defaultColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "OutputEchoColor");
    if (data) Config::OutputColors::echoColor = Config::getConfig<Color>(data)->value;
    
    data = Config::ifContainsGet(consoleData, "OutputWarningColor");
    if (data) Config::OutputColors::warningColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "OutputErrorColor");
    if (data) Config::OutputColors::errorColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "autoCompleteColor");
    if (data) consoleConfig->autoCompleteColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "autoCompleteTextColor");
    if (data) consoleConfig->autoCompleteTextColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "autoCompleteHightlightedTextColor");
    if (data) consoleConfig->autoCompleteHighlightedTextColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "autoCompleteSelectedTextColor");
    if (data) consoleConfig->autoCompleteSelectedTextColor = Config::getConfig<Color>(data)->value;

    data = Config::ifContainsGet(consoleData, "labelColor");
    if (data) consoleConfig->labelTextColor = Config::getConfig<Color>(data)->value;

    consoleConfig->labelFont = FontsHandler::get(0,0);
    consoleConfig->labelText = "Local Console";

    auto consoleUI = Meatball::ConsoleUIScene({rect.x, rect.y, rect.width, rect.height}, std::move(consoleConfig));

    data = Config::ifContainsGet(consoleData, "closeButtonColor");
    if (data) consoleUI.closeButton.config->color = Config::getConfig<Color>(data)->value;
    else consoleUI.closeButton.config->color = {100,100,100,255};

    data = Config::ifContainsGet(consoleData, "closeButtonHoveredColor");
    if (data) consoleUI.closeButton.config->hoveredColor = Config::getConfig<Color>(data)->value;
    else consoleUI.closeButton.config->hoveredColor = {255,255,255,255};

    data = Config::ifContainsGet(consoleData, "font");
    if (data) loadConsoleFonts(consoleUI, {Config::getConfig<std::string>(data)->value});

    // TODO: if ConsoleUI data contains changes related to static
    // configs, create a new shared ptr and use std::swap(old, new) and maybe std::move if something goes wrong?
    //data = Config::ifContainsGet(consoleData, "outputBoxColor");
    //if (data) consoleUI.outputBox.config->color = Config::getConfig<Color>(data)->value;

    //data = Config::ifContainsGet(consoleData, "outputBoxTextColor");
    //if (data) consoleUI.outputBox.config->textColor = Config::getConfig<Color>(data)->value;

    Config::clearData(consoleData);

    Console::init([&](const HayBCMD::OutputLevel &level, const std::string &text) {
        size_t spaceIdxBefore = 0;
        size_t currentSpaceIdx = text.find('\n');
        
        while (currentSpaceIdx != std::string::npos) {
            consoleUI.print(level, text.substr(spaceIdxBefore, currentSpaceIdx-spaceIdxBefore));
            spaceIdxBefore = currentSpaceIdx+1;
            currentSpaceIdx = text.find('\n', spaceIdxBefore);
        }

        consoleUI.print(level, text.substr(spaceIdxBefore));
    });

    Console::print(HayBCMD::OutputLevel::ECHO, "Console initialized");

    for (auto &text : texts)
        Console::print(text.second, text.first);

    HayBCMD::Command("clear", 0, 0,
        [&](HayBCMD::Command*, const std::vector<std::string>&) {
            consoleUI.outputBox.clearText();}, "- clears the console");

    return consoleUI;
}