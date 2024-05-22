#include "Defaults.h"

#include "Config.h"

#include <Console.h>

#include <functional>

Meatball::ConsoleUIScene Meatball::initLocalConsole(Rectangle rect, const std::string& meatdataPath) {
    auto consoleUIData = Config::loadData("data/consoleUI.meatdata");
    
    auto marginData = Config::ifContainsGet(consoleUIData, "margin");
    Meatball::ConsoleUIScene::margin = marginData?
        marginData->unsignedCharV : Meatball::ConsoleUIScene::margin;

    auto consoleUI = Meatball::ConsoleUIScene(rect.x, rect.y, rect.width, rect.height);

    Config::ConfigData* mainPanelColorData = Config::ifContainsGet(consoleUIData, "mainPanelColor");
    if (mainPanelColorData) consoleUI.mainPanel.color = mainPanelColorData->colorV;

    Config::ConfigData* sendButtonTextData = Config::ifContainsGet(consoleUIData, "sendButtonText");
    if (sendButtonTextData) consoleUI.sendButton.setText(sendButtonTextData->stringV);

    Config::ConfigData* sendButtonColorData = Config::ifContainsGet(consoleUIData, "sendButtonColor");
    if (sendButtonColorData) consoleUI.sendButton.color = sendButtonColorData->colorV;

    Config::ConfigData* sendButtonHoveredColorData = Config::ifContainsGet(consoleUIData, "sendButtonHoveredColor");
    if (sendButtonHoveredColorData) consoleUI.sendButton.hoveredColor = sendButtonHoveredColorData->colorV;

    Config::ConfigData* closeButtonColorData = Config::ifContainsGet(consoleUIData, "closeButtonColor");
    if (closeButtonColorData) consoleUI.closeButton.color = closeButtonColorData->colorV;

    Config::ConfigData* closeButtonHoveredColorData = Config::ifContainsGet(consoleUIData, "closeButtonHoveredColor");
    if (closeButtonHoveredColorData) consoleUI.closeButton.hoveredColor = closeButtonHoveredColorData->colorV;

    Console::init([&](const std::string& message) {
        consoleUI.print(message);
    });

    return consoleUI;
}