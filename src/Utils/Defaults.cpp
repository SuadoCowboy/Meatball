#include "Defaults.h"

#include <iostream>

#include <SweatCI.h>

#include "Utils/Json.h"

#include "Console.h"

#include "Interface/DynamicPanel.h"
#include "Interface/Button.h"
#include "Interface/TextButton.h"
#include "Interface/InputTextBox.h"
#include "Interface/ScrollBar.h"
#include "Interface/ScrollTextBox.h"
#include "OutputColors.h"
#include "Utils/Utils.h"

static void defaultConsoleUiPrint(void *pData, const SweatCI::OutputLevel &level, const std::string &text) {
    size_t spaceIdxBefore = 0;
    size_t currentSpaceIdx = text.find('\n');
    
    while (currentSpaceIdx != std::string::npos) {
        ((Meatball::ConsoleUI*)pData)->print(level, text.substr(spaceIdxBefore, currentSpaceIdx-spaceIdxBefore));
        spaceIdxBefore = currentSpaceIdx+1;
        currentSpaceIdx = text.find('\n', spaceIdxBefore);
    }

    ((Meatball::ConsoleUI*)pData)->print(level, text.substr(spaceIdxBefore));
}

static void clearOutputBoxCommand(void *pData, SweatCI::Command&, const std::vector<std::string>&) {
        ((Meatball::ConsoleUI*)pData)->outputBox.clearText();
}

static void printToVector(void *pData, const SweatCI::OutputLevel &level, const std::string &text) {
    ((std::vector<std::pair<std::string, SweatCI::OutputLevel>>*)pData)->push_back({text, level});
}

static void printToStdOut(void*, const SweatCI::OutputLevel &level, const std::string& text) {
    std::cout << "(" << level << ") " << text;
}

void Meatball::Defaults::init(const std::string& jsonPath, Font& defaultFont) {
    SweatCI::Output::setPrintFunction(nullptr, printToStdOut);

    json initData;
    if (!readJSONFile(jsonPath, initData))
        SweatCI::Output::printf(SweatCI::ERROR, "could not read \"{}\" file\n", jsonPath);

    std::string defaultFontPath = "";
    GET_STRING_FROM_JSON(initData, "defaultFont", defaultFontPath, jsonPath);
    if (initData.count("defaultFontSize") == 0)
        SweatCI::Output::printf(SweatCI::WARNING, "missing \"defaultFontSize\" on \"{}\" file\n", jsonPath);
    else if (!initData["defaultFontSize"].is_number())
        SweatCI::Output::print(SweatCI::WARNING, "invalid \"defaultFontSize\" format. Expected a number\n");
    else if (defaultFontPath != "" && !loadFont(defaultFontPath, initData["defaultFontSize"], nullptr, 0, defaultFont))
        SweatCI::Output::printf(SweatCI::WARNING, "\"defaultFont\" gave a unvalid path: {}\n", defaultFontPath);

    dynamicPanelConfig = Config::DynamicPanel();
    GET_COLOR_FROM_JSON(initData, "dynamicPanelColor", dynamicPanelConfig.color, jsonPath);

    buttonConfig = Config::Button();
    {
        bool hasKey = true;
        if (initData.count("button") == 0) hasKey = false; 
        
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["button"], "button", "color", buttonConfig.color, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["button"], "button", "hoveredColor", buttonConfig.color, jsonPath);
    }

    textButtonConfig = Config::TextButton();
    {
        bool hasKey = true;
        if (initData.count("textButton") == 0) hasKey = false; 
        
        textButtonConfig.font = &defaultFont;

        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["textButton"], "textButton", "color", textButtonConfig.color, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["textButton"], "textButton", "textColor", textButtonConfig.textColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["textButton"], "textButton", "hoveredColor", textButtonConfig.hoveredColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["textButton"], "textButton", "hoveredTextColor", textButtonConfig.hoveredTextColor, jsonPath);
    }

    inputTextBoxConfig = Config::InputTextBox();
    {
        bool hasKey = true;
        if (initData.count("inputTextBox") == 0) hasKey = false; 

        inputTextBoxConfig.font = &defaultFont;

        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["inputTextBox"], "inputTextBox", "color", inputTextBoxConfig.color, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["inputTextBox"], "inputTextBox", "textColor", inputTextBoxConfig.textColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["inputTextBox"], "inputTextBox", "selectionColor", inputTextBoxConfig.selectionColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["inputTextBox"], "inputTextBox", "cursorColor", inputTextBoxConfig.cursorColor, jsonPath);
    }

    scrollBarConfig = Config::ScrollBar();
    {
        bool hasKey = true;
        if (initData.count("scrollBar") == 0) hasKey = false; 

        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["scrollBar"], "scrollBar", "barColor", scrollBarConfig.barColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["scrollBar"], "scrollBar", "barHoveredColor", scrollBarConfig.barHoveredColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["scrollBar"], "scrollBar", "thumbColor", scrollBarConfig.thumbColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["scrollBar"], "scrollBar", "thumbHoveredColor1", scrollBarConfig.thumbHoveredColor1, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["scrollBar"], "scrollBar", "thumbHoveredColor2", scrollBarConfig.thumbHoveredColor2, jsonPath);
    }

    scrollTextBoxConfig = Config::ScrollTextBox();
    {
        bool hasKey = true;
        if (initData.count("scrollTextBox") == 0) hasKey = false;

        scrollTextBoxConfig.font = &defaultFont;
        
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, initData["scrollTextBox"], "scrollTextBox", "color", scrollTextBoxConfig.color, jsonPath);
    }
}

bool Meatball::Defaults::loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outGeneralFont, Font& outLabelFont) {
    int size = (int)consoleUI.inputBox.rect.height - 2 + (int)consoleUI.inputBox.rect.height % 2;
    bool success = false;
    if (Meatball::loadFont(fontPath, size, nullptr, 0, outGeneralFont)) {
        consoleUI.inputBox.config->font = consoleUI.outputBox.config->font = &outGeneralFont;
        consoleUI.inputBox.fontSize = consoleUI.inputBox.config->font->baseSize;
        consoleUI.outputBox.fontSize = consoleUI.outputBox.config->font->baseSize;
        success = true;
    }

    size = consoleUI.inputBox.rect.height*0.5;
    if (Meatball::loadFont(fontPath, size, nullptr, 0, outLabelFont)) {
        consoleUI.config.labelFont = &outLabelFont;
        if (!success)
            success = true;
    }
    
    consoleUI.onResize(1, 1);
    return success;
}

Meatball::ConsoleUI Meatball::Defaults::initLocalConsole(const Rectangle& rect, const std::string &jsonPath, Font& outGeneralFont, Font& outLabelFont) {    
    std::vector<std::pair<std::string, SweatCI::OutputLevel>> texts;
    SweatCI::Output::setPrintFunction(&texts, printToVector);

    json consoleData;
    readJSONFile(jsonPath, consoleData);
    
    Config::ConsoleUI consoleConfig = {};

    if (consoleData.count("margin") != 0 && consoleData["margin"].is_number())
        Meatball::ConsoleUI::margin = (int)consoleData["margin"];
    
    {
        bool hasKey = true;
        if (consoleData.count("outputColors") == 0) hasKey = false;
        
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["outputColors"], "outputColors", "default",
            Config::OutputColors::defaultColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["outputColors"], "outputColors", "echo",
            Config::OutputColors::echoColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["outputColors"], "outputColors", "warning",
            Config::OutputColors::warningColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["outputColors"], "outputColors", "error",
            Config::OutputColors::errorColor, jsonPath);
    }

    {
        bool hasKey = true;
        if (consoleData.count("autoComplete") == 0) hasKey = false;

        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["autoComplete"], "autoComplete", "color",
            consoleConfig.autoCompleteColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["autoComplete"], "autoComplete", "textColor",
            consoleConfig.autoCompleteTextColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["autoComplete"], "autoComplete", "highlightedTextColor",
            consoleConfig.autoCompleteHighlightedTextColor, jsonPath);
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["autoComplete"], "autoComplete", "selectedTextColor",
            consoleConfig.autoCompleteSelectedTextColor, jsonPath);
    }

    GET_COLOR_FROM_JSON(consoleData, "labelColor", consoleConfig.labelTextColor, jsonPath);
    
    consoleConfig.labelFont = &outLabelFont;
    consoleConfig.labelText = "Local Console";

    auto consoleUI = Meatball::ConsoleUI({rect.x, rect.y, rect.width, rect.height}, consoleConfig);

    {
        bool hasKey = true;
        if (consoleData.count("closeButton") == 0) hasKey = false;

        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["closeButton"], "closeButton", "color",
            consoleUI.closeButton.config->color, jsonPath);
        
        GET_COLOR_FROM_JSON_INSIDE_JOBJECT(hasKey, consoleData["closeButton"], "closeButton", "hoveredColor",
            consoleUI.closeButton.config->hoveredColor, jsonPath);
    }

    std::string fontPath = "";
    GET_STRING_FROM_JSON(consoleData, "font", fontPath, jsonPath);
    
    if (fontPath == "")
        outGeneralFont = outLabelFont = GetFontDefault();
    else
        loadConsoleFonts(consoleUI, {fontPath}, outGeneralFont, outLabelFont);

    Console::init(&consoleUI, defaultConsoleUiPrint);

    Console::print(SweatCI::OutputLevel::ECHO, "Console initialized");

    for (auto &text : texts)
        Console::print(text.second, text.first);

    SweatCI::Command("clear", 0, 0, clearOutputBoxCommand, "- clears the console", &consoleUI);

    return consoleUI;
}