#include <unordered_map>
#include <string>
#include <vector>
#include <thread>

#include <HayBCMD.h>

#include <Console.h>
#include <Config.h>

#include "Game.h"

#define SETTINGS_PATH "data/meatdata/Settings.meatdata"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

void loadSettingsAndInit() {
    init(
        DEFAULT_WINDOW_WIDTH,
        DEFAULT_WINDOW_HEIGHT);
    
    auto dataMap = Meatball::Config::loadData(SETTINGS_PATH);

    bool shouldRecreateFile = dataMap.size() == 0;
    if (!shouldRecreateFile) {
        std::vector<std::pair<std::string, Meatball::Config::ConfigType>> defaultDataVector = {
            {"windowWidth", Meatball::Config::ConfigType::INT},
            {"windowHeight", Meatball::Config::ConfigType::INT}
        };

        for (auto& data : defaultDataVector)
            if (dataMap.count(data.first) == 0 || dataMap[data.first]->type != data.second) {
                shouldRecreateFile = true;
                break;
            }
    }

    if (shouldRecreateFile) {
        Meatball::Console::printf(HayBCMD::WARNING, "\"{}\" not found or corrupt. Creating one\n", SETTINGS_PATH);
        
        dataMap["windowWidth"] = new Meatball::Config::ConfigTypeData(DEFAULT_WINDOW_WIDTH);
        dataMap["windowWidth"]->type = Meatball::Config::ConfigType::INT;

        dataMap["windowHeight"] = new Meatball::Config::ConfigTypeData(DEFAULT_WINDOW_HEIGHT);
        dataMap["windowHeight"]->type = Meatball::Config::ConfigType::INT;
        
        Meatball::Config::saveData(SETTINGS_PATH, dataMap);
    }

    int windowWidth = Meatball::Config::getConfig<int>(dataMap["windowWidth"])->value;
    int windowHeight = Meatball::Config::getConfig<int>(dataMap["windowHeight"])->value;

    if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
        SetWindowSize(windowWidth, windowHeight);
    
        resize();
        reloadFonts();
    }
    

    Meatball::Config::clearData(dataMap);
}

int main(int, char**) {
    loadSettingsAndInit();

    std::thread updateThread(update);
    while (!(conditionFlags & 1)) {
        render();
    }

    save(SETTINGS_PATH);
    cleanup();

    delete consoleUI;
}