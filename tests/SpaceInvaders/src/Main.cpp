#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

#include <SweatCI.h>

#include <Utils/Json.h>
#include <Console.h>

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
    
    int windowWidth=1, windowHeight=1;

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

    while (!shouldQuit) {
        float dt = GetFrameTime();
        update(dt);
        render();
    }

    save(SETTINGS_PATH);
    cleanup();
}