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

#define UI_TYPE_BUTTON 0
#define UI_TYPE_DYNAMIC_PANEL 1

Font consoleGeneralFont, consoleLabelFont, defaultFont;

struct UIObject {
    void* object = nullptr;
    const char* name;
    unsigned char type;
    Meatball::VoidFunc& update;
    std::function<void(void*)> draw;

    UIObject(void* object, const char* name, unsigned char type,
            Meatball::VoidFunc& update, const std::function<void(void*)>& draw)
        : object(object), name(name), type(type), update(update), draw(draw) {}
};

Meatball::ConsoleUIScene* init(int width, int height) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Meatball's Interface Creator");

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

    return consoleUI;
}

struct UIOption {
    const char* text;
    Font& font;

    UIOption(const char* text, Font& font) : text(text), font(font) {}
};

int main(int argc, char** argv) {
    argc = 2;
    if (argc < 2) {
        std::cout << argv[0] << " <pathToScript>\n";
        return 1;
    }
    
    auto consoleUI = init(800, 600);
    consoleUI->visible = false;

    {
        auto c = HayBCMD::Command("toggle_local_console", 0, 1, [&consoleUI](void* pData, const std::vector<std::string>& args){
            if (args.size() == 0) {
                consoleUI->visible = not consoleUI->visible;
                return;
            }

            try {
                bool visible = (bool)std::stoi(args[0]);
                consoleUI->visible = visible;
            } catch (...) {
                HayBCMD::Command::printUsage(*(HayBCMD::Command*)pData);
            }
        }, "- toggles local console");
        c.pData = &c;
    }

    //Vector2 viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};
    
    std::vector<UIObject*> uiObjects;
    
    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    bool drawOptions = false;
    // TODO: fully customizable program in the future :D
    // for now becuse ui stuff is not fully developed yet,
    // we don't need to worry about customizable stuff in this
    // program. But in the end this should be also customizable.
    std::vector<UIOption> options;
    // options background color
    Color optionsColor = {20, 20, 20, 255};
    Color optionsTextColor = WHITE;
    Font optionsFont = GetFontDefault();

    options.push_back({"Create Button", optionsFont});

    float optionsMinWidth = 0.0f;
    for (auto& option : options) {
        float width = Meatball::measureTextWidth(option.font, option.font.baseSize, option.text);
        if (width > optionsMinWidth) optionsMinWidth = width;
    }

    HayBCMD::Command("ui_draw_options", 1, 1, [&drawOptions](void*, const std::vector<std::string>& args) {
        drawOptions = std::stoi(args[0]);
    }, "<draw> - draw options where mouse position is at");

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);

        float dt = GetFrameTime();
        if (dt > 0.016)
            dt = 0.016;

        /*if (IsWindowResized()) {
            float newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            Vector2 ratio = { newScreenWidth / viewport.x, newScreenHeight / viewport.y };

            viewport.x = newScreenWidth;
            viewport.y = newScreenHeight;
        }*/

        Meatball::Input::update(false);

        BeginDrawing();

        for (auto& obj : uiObjects) {
            obj->update();
            obj->draw(obj->object);
        }

        if (drawOptions) {
            Vector2 mousePos = GetMousePosition();
            unsigned char i = 0;
            for (; i < options.size(); ++i) {
                Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, mousePos.x, mousePos.y+i*optionsFont.baseSize, optionsTextColor);
            }
            DrawRectangle(mousePos.x, mousePos.y, optionsMinWidth, i*optionsFont.baseSize, optionsColor);
        }

        consoleUI->update();
        consoleUI->draw();

        EndDrawing();
    }
}