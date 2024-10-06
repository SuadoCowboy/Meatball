#include "Defaults.h"

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
    ((std::vector<std::pair<std::string, SweatCI::OutputLevel>>*)pData)->emplace_back(text, level);
}

bool Meatball::Defaults::loadConsoleFonts(ConsoleUI& consoleUI, const std::filesystem::path& fontPath, Font& outInputFont, Font& outOutputFont, Font& outLabelFont) {
    int size = (int)consoleUI.inputBox.rect.height - 2 + (int)consoleUI.inputBox.rect.height % 2;
    bool success = true;
    
    if (Meatball::loadFont(fontPath, size, nullptr, 0, outInputFont)) {
        consoleUI.inputFont = outInputFont;
        consoleUI.inputBox.fontSize = consoleUI.inputFont.baseSize;
    } else
        success = false;

    if (Meatball::loadFont(fontPath, size, nullptr, 0, outOutputFont)) {
        consoleUI.outputFont = outOutputFont;
        consoleUI.outputBox.fontSize = consoleUI.outputFont.baseSize;
    } else
        success = false;

    size = consoleUI.inputBox.rect.height*0.5f;
    if (Meatball::loadFont(fontPath, size, nullptr, 0, outLabelFont))
        consoleUI.labelFont = outLabelFont;
    else
        success = false;
    
    consoleUI.onResize(1, 1);
    return success;
}

Meatball::ConsoleUI Meatball::Defaults::initLocalConsole(const Rectangle& rect, const std::string& jsonPath, Font& outInputFont, Font& outOutputFont, Font& outLabelFont) {    
    std::vector<std::pair<std::string, SweatCI::OutputLevel>> texts;
    SweatCI::Output::setPrintFunction(&texts, printToVector);

    Json consoleData{jsonPath};

    {
        Json data{consoleData, "outputColors"};
        
        data.getColor("default", OutputColors::defaultColor);
        data.getColor("echo", OutputColors::echoColor);
        data.getColor("warning", OutputColors::warningColor);
        data.getColor("error", OutputColors::errorColor);
    }

    auto consoleUI = Meatball::ConsoleUI({rect.x, rect.y, rect.width, rect.height});

    {
        int temp = 0;
        consoleData.getInt("margin", temp);
        consoleUI.margin = temp;
    }

    {
        Json data{consoleData, "autoComplete"};

        data.getColor("color", consoleUI.autoCompleteColor);
        data.getColor("textColor", consoleUI.autoCompleteTextColor);
        data.getColor("highlightedTextColor", consoleUI.autoCompleteHighlightedTextColor);
        data.getColor("selectedTextColor", consoleUI.autoCompleteSelectedTextColor);
    }

    consoleData.getColor("labelColor", consoleUI.labelTextColor);
    consoleUI.labelFont = outLabelFont;
    consoleUI.labelText = "Local Console";

    {
        Json data{consoleData, "closeButton"};

        data.getColor("color", consoleUI.closeButtonTheme.color);
        data.getColor("hoveredColor", consoleUI.closeButtonTheme.hoveredColor);
    }

    std::string fontPath = "";
    if (!consoleData.getString("font", fontPath)
     || !loadConsoleFonts(consoleUI, fontPath, outInputFont, outOutputFont, outLabelFont)) {
        SweatCI::Output::printf(SweatCI::WARNING, "could not load console fonts.\n");
        outInputFont = outOutputFont = outLabelFont = GetFontDefault();
     }

    Console::init(&consoleUI, defaultConsoleUiPrint);

    Console::print(SweatCI::OutputLevel::ECHO, "Console initialized");

    for (auto& text : texts)
        Console::print(text.second, text.first);

    SweatCI::Command("clear", 0, 0, clearOutputBoxCommand, "- clears the console", &consoleUI);

    return consoleUI;
}