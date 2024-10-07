#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

#include <SweatCI.h>

#include <Utils/Json.h>
#include <Console.h>
#include <EventHandler.h>
#include <Input.h>

#include "Game.h"

#define SETTINGS_PATH "data/settings.json"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

void loadSettingsAndInit() {
    init(
        DEFAULT_WINDOW_WIDTH,
        DEFAULT_WINDOW_HEIGHT);

    Meatball::Json settingsData{SETTINGS_PATH};

    bool shouldRecreateFile = settingsData.object.size() == 0;
    
    int windowWidth=DEFAULT_WINDOW_WIDTH, windowHeight=DEFAULT_WINDOW_HEIGHT;

    if (!shouldRecreateFile) {
        Meatball::Json windowData{settingsData, "window"};

        bool result = windowData.getInt("height", windowHeight);
        if (!windowData.getInt("width", windowWidth) || !result)
            shouldRecreateFile = true;
    }

    if (shouldRecreateFile) {
        Meatball::Console::printf(SweatCI::WARNING, "\"{}\" not found or corrupt. Creating a new one\n", SETTINGS_PATH);

        if (!settingsData.object.count("window") != 0)
            settingsData.object["window"] = json::parse("{\"width\": 800, \"height\": 600}");
        
        if (!settingsData.object["window"].count("width") != 0
        || !settingsData.object["window"]["width"].is_number() || settingsData.object["window"]["width"] == 0)
            settingsData.object["window"]["width"] = DEFAULT_WINDOW_WIDTH;
        
        if (!settingsData.object["window"].count("height") != 0
        || !settingsData.object["window"]["height"].is_number() || settingsData.object["window"]["height"] == 0)
            settingsData.object["window"]["height"] = DEFAULT_WINDOW_WIDTH;

        std::ofstream file(SETTINGS_PATH);

        if (file)
            file << settingsData.object.dump(2);
        else
            Meatball::Console::printf(SweatCI::ERROR, "could not open file \"{}\"", SETTINGS_PATH);
    }

    if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
        SetWindowSize(windowWidth, windowHeight);
        resize();
        reloadFonts();
    }
}

int main(int, char**) {
    loadSettingsAndInit();

    Meatball::EventHandler::onCharPress = [](int codepoint) {
        pConsoleUI->onCharPress(codepoint);
    };

    Meatball::EventHandler::onKeyboardPress = [](int key) {
        Meatball::Input::onKeyboardPress(key);
        pConsoleUI->onKeyboardPress(key);
    };

    Meatball::EventHandler::onKeyboardRelease = [](int key) {
        Meatball::Input::onKeyboardRelease(key);
    };

    Meatball::EventHandler::onMousePress = [](int button) {
        Meatball::Input::onMousePress(button);
        pConsoleUI->onMousePress(button);
    };

    Meatball::EventHandler::onMouseRelease = [](int button) {
        Meatball::Input::onMouseRelease(button);
        pConsoleUI->onMouseRelease(button);
    };

    Meatball::EventHandler::onMouseMove = [](const Vector2&, const Vector2& mousePosition) {
        pConsoleUI->onMouseMove(mousePosition);
    };

    Meatball::EventHandler::onMouseWheel = [](const Vector2& dir) {
        Meatball::Input::onMouseWheel(dir);
    };

    while (!shouldQuit) {
        float dt = GetFrameTime();
        
        Meatball::EventHandler::handle();

        update(dt);
        render();
    }

    save(SETTINGS_PATH);
    cleanup();
}