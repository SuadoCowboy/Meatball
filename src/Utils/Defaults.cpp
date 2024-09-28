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
#include "Interface/ScrollBox.h"
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

    Json initData{jsonPath};

    std::string defaultFontPath = "";
    int defaultFontSize = 0;

    if (initData.getString("defaultFont", defaultFontPath)
     && initData.getInt("defaultFontSize", defaultFontSize)
     && !loadFont(defaultFontPath, defaultFontSize, nullptr, 0, defaultFont))
        SweatCI::Output::printf(SweatCI::WARNING, "\"defaultFont\" gave a unvalid path: {}\n", defaultFontPath);

    dynamicPanelConfig = Config::DynamicPanel();
    initData.getColor("dynamicPanelColor", dynamicPanelConfig.color);

    buttonConfig = Config::Button();
    {
        Json data{initData, "button"};

        data.getColor("color", buttonConfig.color);
        data.getColor("hoveredColor", buttonConfig.hoveredColor);
    }

    textButtonConfig = Config::TextButton();
    {
        Json data{initData, "textButton"};
        
        textButtonConfig.font = &defaultFont;

        data.getColor("color", textButtonConfig.color);
        data.getColor("textColor", textButtonConfig.textColor);
        data.getColor("hoveredColor", textButtonConfig.hoveredColor);
        data.getColor("hoveredTextColor", textButtonConfig.hoveredTextColor);
    }

    inputTextBoxConfig = Config::InputTextBox();
    {
        Json data{initData, "inputTextBox"};

        inputTextBoxConfig.font = &defaultFont;

        data.getColor("color", inputTextBoxConfig.color);
        data.getColor("textColor", inputTextBoxConfig.textColor);
        data.getColor("selectionColor", inputTextBoxConfig.selectionColor);
        data.getColor("cursorColor", inputTextBoxConfig.cursorColor);
    }

    scrollBarConfig = Config::ScrollBar();
    {
        Json data{initData, "scrollBar"};

        data.getColor("barColor", scrollBarConfig.barColor);
        data.getColor("barHoveredColor", scrollBarConfig.barHoveredColor);
        data.getColor("thumbColor", scrollBarConfig.thumbColor);
        data.getColor("thumbHoveredColor1", scrollBarConfig.thumbHoveredColor1);
        data.getColor("thumbHoveredColor2", scrollBarConfig.thumbHoveredColor2);
    }

    scrollBoxConfig = Config::ScrollBox();
    {
        Json data{initData, "scrollBox"};

        scrollBoxConfig.font = &defaultFont;
        
        data.getColor("color", scrollBoxConfig.color);
    }
}

bool Meatball::Defaults::loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outGeneralFont, Font& outLabelFont) {
    int size = (int)consoleUI.inputBox.rect.height - 2 + (int)consoleUI.inputBox.rect.height % 2;
    bool success = true;
    
    if (Meatball::loadFont(fontPath, size, nullptr, 0, outGeneralFont)) {
        consoleUI.inputBox.config->font = consoleUI.outputBox.config->font = &outGeneralFont;
        consoleUI.inputBox.fontSize = consoleUI.inputBox.config->font->baseSize;
        consoleUI.outputBox.fontSize = consoleUI.outputBox.config->font->baseSize;
    } else
        success = false;

    size = consoleUI.inputBox.rect.height*0.5;
    if (Meatball::loadFont(fontPath, size, nullptr, 0, outLabelFont))
        consoleUI.config.labelFont = &outLabelFont;
    else
        success = false;
    
    consoleUI.onResize(1, 1);
    return success;
}

Meatball::ConsoleUI Meatball::Defaults::initLocalConsole(const Rectangle& rect, const std::string &jsonPath, Font& outGeneralFont, Font& outLabelFont) {    
    std::vector<std::pair<std::string, SweatCI::OutputLevel>> texts;
    SweatCI::Output::setPrintFunction(&texts, printToVector);

    Json consoleData{jsonPath};
    
    Config::ConsoleUI consoleConfig = {};

    {
        int temp = 0;
        consoleData.getInt("margin", temp);
        Meatball::ConsoleUI::margin = temp;
    }
    
    {
        Json data{consoleData, "outputColors"};
        
        data.getColor("default", Config::OutputColors::defaultColor);
        data.getColor("echo", Config::OutputColors::echoColor);
        data.getColor("warning", Config::OutputColors::warningColor);
        data.getColor("error", Config::OutputColors::errorColor);
    }

    {
        Json data{consoleData, "autoComplete"};

        data.getColor("color", consoleConfig.autoCompleteColor);
        data.getColor("textColor", consoleConfig.autoCompleteTextColor);
        data.getColor("highlightedTextColor", consoleConfig.autoCompleteHighlightedTextColor);
        data.getColor("selectedTextColor", consoleConfig.autoCompleteSelectedTextColor);
    }

    consoleData.getColor("labelColor", consoleConfig.labelTextColor);
    
    consoleConfig.labelFont = &outLabelFont;
    consoleConfig.labelText = "Local Console";

    auto consoleUI = Meatball::ConsoleUI({rect.x, rect.y, rect.width, rect.height}, consoleConfig);

    {
        Json data{consoleData, "closeButton"};

        data.getColor("color", consoleUI.closeButton.config->color);
        data.getColor("hoveredColor", consoleUI.closeButton.config->hoveredColor);
    }

    std::string fontPath = "";
    if (consoleData.getString("font", fontPath)) {
        if (!loadConsoleFonts(consoleUI, {fontPath}, outGeneralFont, outLabelFont))
            SweatCI::Output::printf(SweatCI::WARNING, "could not load console fonts.\n");
    } else
        outGeneralFont = outLabelFont = GetFontDefault();

    Console::init(&consoleUI, defaultConsoleUiPrint);

    Console::print(SweatCI::OutputLevel::ECHO, "Console initialized");

    for (auto &text : texts)
        Console::print(text.second, text.first);

    SweatCI::Command("clear", 0, 0, clearOutputBoxCommand, "- clears the console", &consoleUI);

    return consoleUI;
}