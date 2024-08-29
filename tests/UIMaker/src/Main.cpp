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
#include <Utils/CvarFuncs.h>
#include <Scene.h>

#include "GameInterface.h"

Font consoleGeneralFont, consoleLabelFont, defaultFont;

std::vector<UIObject*> uiObjects;

Vector2 viewport;

// CVARS
bool uiEditorMode = true;

Color uiColor = BLACK, uiHoveredColor = BLACK;
std::string uiAnchor = "";

static void resetUiCvars() {
    uiColor = BLACK;
    uiHoveredColor = BLACK;
    uiAnchor = "";
}

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
Font optionsFont;
Vector2 optionsPosition = {0,0};

Meatball::ConsoleUI* pConsoleUI;

#pragma region commands

static void uiCreateButtonCommand(void*, HayBCMD::Command&, const std::vector<std::string>& args) {
    UIObject* buttonObj = createUIObject(args[0].c_str(), uiObjects, {0.0f,0.0f}, UI_TYPE_BUTTON, viewport);
    auto button = (Meatball::Button*)buttonObj->object;
    button->config->color = uiColor;
    button->config->hoveredColor = uiHoveredColor;

    resetUiCvars();
}

static void showUiOptionsCommand(void*, HayBCMD::Command&, const std::vector<std::string>& args) {
    if (!uiEditorMode) return;

    drawOptions = std::stoi(args[0]);
    optionsPosition = GetMousePosition();
}
#pragma endregion

bool stringToInt(const std::string& str, int& buffer) {
    try {
        buffer = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

static void initCommands() {
    HayBCMD::Command("ui_create_button", 1, 1, uiCreateButtonCommand, "<name> - creates a button using the defined data by other ui commands");
    HayBCMD::Command("show_ui_options", 1, 1, showUiOptionsCommand, "<draw> - draw options where mouse position is at");

    HayBCMD::CVARStorage::setCvar("ui_editor_mode",
        &uiEditorMode,
        HayBCMD::CVARUtils::setBoolean,
        HayBCMD::CVARUtils::getBoolean,
        "1/0 - whether should run in editor mode or ui logic"
    );
    HayBCMD::CVARStorage::setCvar("draw_local_console",
        &pConsoleUI->visible,
        HayBCMD::CVARUtils::setBoolean,
        HayBCMD::CVARUtils::getBoolean,
        "1/0 - draws local console"
    );

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
        &uiColor,
        Meatball::CVARFuncs::setColor,
        Meatball::CVARFuncs::getColor,
        "");
    HayBCMD::CVARStorage::setCvar(
        "ui_hovered_color",
        &uiHoveredColor,
        Meatball::CVARFuncs::setColor,
        Meatball::CVARFuncs::getColor,
        "");
    HayBCMD::CVARStorage::setCvar(
        "ui_anchor",
        &uiAnchor,
        HayBCMD::CVARUtils::setString,
        HayBCMD::CVARUtils::getString,
        "");

    Meatball::Input::allowedUiCommands.push_back("draw_local_console");
    //Meatball::Input::allowedUiCommands.push_back("quit");
}

static void init(int width, int height) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Meatball's Interface Creator");
    viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};

    defaultFont = GetFontDefault();

    Meatball::Defaults::init("data/meatdata/Init.meatdata", defaultFont);
    pConsoleUI = new Meatball::ConsoleUI(Meatball::Defaults::initLocalConsole(
        {0, 0, 800 * 0.5f, 600 * 0.75f},
        "data/meatdata/Console.meatdata",
        consoleGeneralFont,
        consoleLabelFont
    ));

    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    initCommands();

    optionsFont = GetFontDefault();
}

int main() {
    init(800, 600);
    SetExitKey(KEY_NULL);
    pConsoleUI->visible = false;

    options.push_back({"Create Button", optionsFont, UI_TYPE_BUTTON});

    float optionsMinWidth = 0.0f;
    for (auto& option : options) {
        float width = Meatball::measureTextWidth(option.font, option.font.baseSize, option.text);
        if (width > optionsMinWidth) optionsMinWidth = width+4;
    }
    
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

        if (uiEditorMode && drawOptions) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                drawOptions = false;
            
            DrawRectangle(optionsPosition.x, optionsPosition.y, optionsMinWidth, options.size()*optionsFont.baseSize, optionsColor);
            for (unsigned char i = 0; i < options.size(); ++i) {
                if (CheckCollisionPointRec(GetMousePosition(), {optionsPosition.x, optionsPosition.y, optionsMinWidth, (float)optionsFont.baseSize})) {
                    Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, optionsPosition.x+2, optionsPosition.y+i*optionsFont.baseSize, optionsHoveredTextColor);
                
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        createUIObject(HayBCMD::formatString("Item{}", uiObjects.size()).c_str(), uiObjects, optionsPosition, options[i].type, viewport);
                } else
                    Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, optionsPosition.x+2, optionsPosition.y+i*optionsFont.baseSize, optionsTextColor);
            }
        }

        pConsoleUI->update();
        pConsoleUI->draw();

        EndDrawing();
    }

    for (auto& obj : uiObjects) {
        delete obj;
    }
}