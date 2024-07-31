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
    std::function<void()> update, draw;

    UIObject(void* object, const char* name, unsigned char type,
            const std::function<void()>& update, const std::function<void()>& draw)
        : object(object), name(name), type(type), update(update), draw(draw) {}
    
    ~UIObject() {
        switch (type) {
            case UI_TYPE_BUTTON:
                delete (Meatball::Button*)object;
                break;
            case UI_TYPE_DYNAMIC_PANEL:
                delete (Meatball::DynamicPanel*)object;
                break;
        }
    }
};

struct UIOption {
    const char* text;
    Font& font;
    unsigned char type;

    UIOption(const char* text, Font& font, unsigned char type) : text(text), font(font), type(type) {}
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

void createUIObject(std::vector<UIObject*>& uiObjects, const Vector2& optionsPosition, unsigned char type, const Vector2& renderSize) {
    switch (type) {
    case UI_TYPE_BUTTON: {
        Meatball::Button* button = new Meatball::Button((Rectangle){optionsPosition.x, optionsPosition.y, renderSize.x*0.1f, renderSize.y*0.06f});
        button->config = std::make_shared<Meatball::Config::Button>(Meatball::Defaults::buttonConfig);

        UIObject* object = new UIObject(
                button,
                HayBCMD::formatString("Item{}", uiObjects.size()).c_str(),
                type,
                [button](){button->update();},
                [button](){
                Meatball::drawRect(button->rect, button->isHovered()? button->config->hoveredColor : button->config->color);
        });

        uiObjects.push_back(object);
        break;
    }
    };

}

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

    Vector2 viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};
    
    std::vector<UIObject*> uiObjects;

    bool drawOptions = false;
    // TODO: fully customizable program in the future :D
    // for now becuse ui stuff is not fully developed yet,
    // we don't need to worry about customizable stuff in this
    // program. But in the end this should be also customizable.
    std::vector<UIOption> options;
    // options background color
    Color optionsColor = {20, 20, 20, 255};
    Color optionsTextColor = WHITE;
    Color optionsHoveredTextColor = LIGHTGRAY;
    Font optionsFont = GetFontDefault();
    Vector2 optionsPosition = {0,0};

    options.push_back({"Create Button", optionsFont, UI_TYPE_BUTTON});

    float optionsMinWidth = 0.0f;
    for (auto& option : options) {
        float width = Meatball::measureTextWidth(option.font, option.font.baseSize, option.text);
        if (width > optionsMinWidth) optionsMinWidth = width+4;
    }

    HayBCMD::Command("ui_draw_options", 1, 1, [&drawOptions, &optionsPosition](void*, const std::vector<std::string>& args) {
        drawOptions = std::stoi(args[0]);
        optionsPosition = GetMousePosition();
    }, "<draw> - draw options where mouse position is at");

    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);

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
            obj->update();
            obj->draw();
        }

        if (drawOptions) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                drawOptions = false;

            unsigned char i = 0;
            DrawRectangle(optionsPosition.x, optionsPosition.y, optionsMinWidth, options.size()*optionsFont.baseSize, optionsColor);
            for (; i < options.size(); ++i) {
                if (CheckCollisionPointRec(GetMousePosition(), {optionsPosition.x, optionsPosition.y, optionsMinWidth, (float)optionsFont.baseSize})) {
                    Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, optionsPosition.x+2, optionsPosition.y+i*optionsFont.baseSize, optionsHoveredTextColor);
                
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        createUIObject(uiObjects, optionsPosition, options[i].type, viewport);
                } else
                    Meatball::drawText(optionsFont, optionsFont.baseSize, options[i].text, optionsPosition.x+2, optionsPosition.y+i*optionsFont.baseSize, optionsTextColor);
            }
        }

        consoleUI->update();
        consoleUI->draw();

        EndDrawing();
    }
}