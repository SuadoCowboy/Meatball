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

    json settingsData;
    Meatball::readJSONFile(SETTINGS_PATH, settingsData);

    bool shouldRecreateFile = settingsData.size() == 0;

    int windowWidth, windowHeight;
    { // Window
        bool hasKey = true;
        if (settingsData.count("window") == 0) hasKey = false;

        if (hasKey && settingsData["window"].count("width") != 0 && settingsData["window"]["width"].is_number())
            windowWidth = settingsData["window"]["width"];
        else
            shouldRecreateFile = true;

        if (hasKey && settingsData["window"].count("height") != 0 && settingsData["window"]["height"].is_number())
            windowHeight = settingsData["window"]["height"];
        else
            shouldRecreateFile = true;
    }

    if (shouldRecreateFile) {
        Meatball::Console::printf(SweatCI::WARNING, "\"{}\" not found or corrupt. Creating a new one\n", SETTINGS_PATH);

        if (!settingsData.count("window") != 0)
            settingsData["window"] = json::parse("{\"width\": 0, \"height\": 0}");
        
        if (!settingsData["window"].count("width") != 0
        || !settingsData["window"]["width"].is_number() || settingsData["window"]["width"] == 0)
            settingsData["window"]["width"] = DEFAULT_WINDOW_WIDTH;
        
        if (!settingsData["window"].count("height") != 0
        || !settingsData["window"]["height"].is_number() || settingsData["window"]["height"] == 0)
            settingsData["window"]["height"] = DEFAULT_WINDOW_WIDTH;

        std::ofstream file(SETTINGS_PATH);

        if (file)
            file << settingsData.dump(2);
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