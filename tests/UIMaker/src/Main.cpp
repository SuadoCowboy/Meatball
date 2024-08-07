#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstring>

#include <raylib.h>

#include <HayBCMD.h>

#include <Console.h>
#include <Input.h>

#include <Interface/Button.h>

#include <Utils/Defaults.h>
#include <Utils/DrawFuncs.h>
#include <Utils/Utils.h>

#include "GameInterface.h"

Font consoleGeneralFont, consoleLabelFont, defaultFont;

std::vector<UIObject*> uiObjects;

Vector2 viewport;

// User Interface CVars
Color uiColor = BLACK, uiHoveredColor = BLACK;

bool stringToInt(const std::string& str, int& buffer) {
    try {
        buffer = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

void initCommands() {
    HayBCMD::Command("toggle", 3, 3, [](void*, const std::vector<std::string>& args){
        HayBCMD::CVariable* buffer = nullptr;
        if (!HayBCMD::CVARStorage::getCvar(args[0], buffer)) {
            Meatball::Console::printf(HayBCMD::ERROR, "unknown cvar \"{}\"\n", args[0]);
            return;
        }

        std::string asString = buffer->toString();
        if (asString == args[1])
            buffer->set(args[2]);
        else
            buffer->set(args[1]);
    }, "<cvar> <option1> <option2> - toggles a cvar to option1 or option2");

    HayBCMD::Command("ui_create_button", 1, 1, [](void*, const std::vector<std::string>& args){
        UIObject* buttonObj = createUIObject(args[0].c_str(), uiObjects, {0.0f,0.0f}, UI_TYPE_BUTTON, viewport);
        auto button = (Meatball::Button*)buttonObj->object;
        button->config = std::make_shared<Meatball::Config::Button>(Meatball::Defaults::buttonConfig);
        button->config->color = uiColor;
        button->config->hoveredColor = uiHoveredColor;
    }, "<name> - creates a button using the defined data by other ui commands");
}

Meatball::ConsoleUIScene* init(int width, int height) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Meatball's Interface Creator");
    viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};

    defaultFont = GetFontDefault();

    Meatball::Defaults::init("data/meatdata/Init.meatdata", defaultFont);
    auto consoleUI = new Meatball::ConsoleUIScene(Meatball::Defaults::initLocalConsole(
        {0, 0, 800 * 0.5f, 600 * 0.75f},
        "data/meatdata/Console.meatdata",
        consoleGeneralFont,
        consoleLabelFont
    ));

    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    initCommands();

    return consoleUI;
}

int main() {
    auto consoleUI = init(800, 600);
    SetExitKey(KEY_NULL);
    consoleUI->visible = false;

    /*
    1 = ui_editor_mode
    */
    unsigned char flags = 1;
    CREATE_TOGGLE_CVAR_BITWISE("ui_editor_mode", flags, 1, "1/0 - whether should run in editor mode or ui logic");
    CREATE_TOGGLE_CVAR("draw_local_console", consoleUI->visible, "1/0 - draws local console");
    

    // Unfortunately, we can not create a lambda for cvar, that will be out of scope later. I should fix this soon.
    /*
    // x% y% width% height%
    ui_anchor "mainPanel" // mainPanel should be defined before closeButton
    ui_rect_min_percent 0 0 1 1
    ui_rect_max_percent 0 0 3 3 
    ui_draw x // it means that we will use drawX function
    ui_create_button "closeButton"
    */
    HayBCMD::CVARStorage::setCvar(
        "ui_color",
        [](const std::string& str){
            Meatball::parseStringToColor(str, uiColor);
        },
        [](){
            return HayBCMD::formatString("{}, {}, {}, {}", (short)uiColor.r, (short)uiColor.g, (short)uiColor.b, (short)uiColor.a);
        },
        "");
    HayBCMD::CVARStorage::setCvar(
        "ui_hovered_color",
        [](const std::string& str){
            Meatball::parseStringToColor(str, uiHoveredColor);
        },
        [](){ // TODO: those functions could receive a void pointer so that if we had too much repeating lambdas, it could be a single function that would receive a void*
            return HayBCMD::formatString("{}, {}, {}, {}", (short)uiHoveredColor.r, (short)uiHoveredColor.g, (short)uiHoveredColor.b, (short)uiHoveredColor.a);
        },
        "");

    Meatball::Input::allowedUiCommands.push_back("draw_local_console");
    //Meatball::Input::allowedUiCommands.push_back("quit");

    bool drawOptions = false;
    // TODO: fully customizable program in the future :D
    // for now becuse ui stuff is not fully developed yet,
    // we don't need to worry about customizable stuff in this
    // program. But in the end this should be also customizable.
    std::vector<UIOption> options;
    // options background color
    Color optionsColor = {20, 20, 20, 255};
    Color optionsTextColor = LIGHTGRAY;
    Color optionsHoveredTextColor = WHITE;
    Font optionsFont = GetFontDefault();
    Vector2 optionsPosition = {0,0};

    options.push_back({"Create Button", optionsFont, UI_TYPE_BUTTON});

    float optionsMinWidth = 0.0f;
    for (auto& option : options) {
        float width = Meatball::measureTextWidth(option.font, option.font.baseSize, option.text);
        if (width > optionsMinWidth) optionsMinWidth = width+4;
    }

    HayBCMD::Command("show_ui_options", 1, 1, [&drawOptions, &optionsPosition, &flags](void*, const std::vector<std::string>& args) {
        if (!(flags & 1)) return;

        drawOptions = std::stoi(args[0]);
        optionsPosition = GetMousePosition();
    }, "<draw> - draw options where mouse position is at");

    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    Color backgroundColor = {0,0,0,255};

    while (!WindowShouldClose()) {
        ClearBackground(backgroundColor);

        float dt = GetFrameTime();
        if (dt > 0.016)
            dt = 0.016;

        if (IsWindowResized()) {
            float newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            //Vector2 ratio = { newScreenWidth / viewport.x, newScreenHeight / viewport.y };

            viewport.x = newScreenWidth;
            viewport.y = newScreenHeight;
        }

        Meatball::Input::update(false);

        BeginDrawing();

        for (auto& obj : uiObjects) {
            obj->update(obj->object);
            obj->draw(obj->object);
        }

        if ((flags & 1) && drawOptions) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                drawOptions = false;

            unsigned char i = 0;
            DrawRectangle(optionsPosition.x, optionsPosition.y, optionsMinWidth, options.size()*optionsFont.baseSize, optionsColor);
            for (; i < options.size(); ++i) {
                if (CheckCollisionPointRec(GetMousePosition(), {optionsPosition.x, optionsPosition.y, optionsMinWidth, (float)optionsFont.baseSize})) {
                    Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, optionsPosition.x+2, optionsPosition.y+i*optionsFont.baseSize, optionsHoveredTextColor);
                
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        createUIObject(HayBCMD::formatString("Item{}", uiObjects.size()).c_str(), uiObjects, optionsPosition, options[i].type, viewport);
                } else
                    Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, optionsPosition.x+2, optionsPosition.y+i*optionsFont.baseSize, optionsTextColor);
            }
        }

        consoleUI->update();
        consoleUI->draw();

        EndDrawing();
    }

    for (auto& obj : uiObjects) {
        delete obj;
    }
}