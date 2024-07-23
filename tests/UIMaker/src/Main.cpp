#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include <raylib.h>

#include <HayBCMD.h>

#include <ScriptExecutor.h>
#include <Console.h>
#include <Input.h>

#include <Interface/Button.h>
#include <Interface/DynamicPanel.h>

#include <Utils/Defaults.h>
#include <Utils/DrawFuncs.h>

struct ButtonStruct {
    Meatball::Button button;
    int ref = 0;
    const char* name;

    ButtonStruct(const char* name) : name(name) {}
};

struct DynamicPanelStruct {
    Meatball::DynamicPanel panel;
    int ref = 0;
    const char* name;

    DynamicPanelStruct(const char* name) : name(name) {}
};

Meatball::ConsoleUIScene* init(int width, int height) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Meatball's Interface Creator");

    Meatball::Defaults::init("data/meatdata/Init.meatdata");
    auto consoleUI = new Meatball::ConsoleUIScene(Meatball::Defaults::initLocalConsole(
        {0, 0, 800 * 0.5f, 600 * 0.75f},
        "data/meatdata/Console.meatdata"
    ));

    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    return consoleUI;
}
 
/// @return Meatbll.UI.LayoutTypes.NONE
unsigned char setLuaVariables(lua_State* L) {
    unsigned char noneLayoutType;
    { // Meatball.UI.LayoutTypes.NONE = #Meatball.UI.LayoutTypes
        lua_getglobal(L, "Meatball");
        lua_getfield(L, -1, "UI");
        lua_getfield(L, -1, "LayoutTypes");

        noneLayoutType = luaL_len(L, -1);
        lua_pushinteger(L, noneLayoutType);
        lua_setfield(L, -2, "NONE");

        lua_pop(L, 3);
    }

    { // Game = {viewport = Meatball.vec2(renderWidth, renderHeight)}
        lua_newtable(L); // Game
            lua_newtable(L); // viewport
            SCRIPT_SET_VEC2(L, GetRenderWidth(), GetRenderHeight());
            lua_setfield(L, -2, "viewport");
        lua_setglobal(L, "Game");
    }

    return noneLayoutType;
}

/// @brief assuming a lua event call happened and returned the same table the one given at the start of the program
void updateInterface(lua_State*& L, std::vector<ButtonStruct*>& buttons, std::vector<DynamicPanelStruct*>& dynamicPanels) {
    if (lua_gettop(L) == 0 || lua_type(L, -1) != LUA_TTABLE)
        return;

    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        const char* name = lua_tostring(L, -2);
        for (auto& button : buttons) {
            if (strcmp(button->name, name) == 0) continue;

            lua_rawgeti(L, LUA_REGISTRYINDEX, button->ref);

            lua_getfield(L, -1, "config");
                SCRIPT_GET_COLOR(L, -1, button->button.config->color, "color", name);
                SCRIPT_GET_COLOR(L, -1, button->button.config->hoveredColor, "hoveredColor", name);
            lua_pop(L, 1);

            SCRIPT_GET_RECT(L, -1, button->button.rect, "rect", name);
            lua_pop(L, 1);
        }

        for (auto& panel : dynamicPanels) {
            if (strcmp(panel->name, name) == 0) continue;

            lua_rawgeti(L, LUA_REGISTRYINDEX, panel->ref);

            lua_getfield(L, -1, "config");
                SCRIPT_GET_COLOR(L, -1, panel->panel.config->color, "color", name);
                SCRIPT_GET_FIELD(L, -1, panel->panel.config->grabHeight, "grabHeight", lua_tonumber);
                SCRIPT_GET_VEC2(L, -1, panel->panel.config->minSize, "minSize", name);
            lua_pop(L, 1);

            SCRIPT_GET_RECT(L, -1, panel->panel.rect, "rect", name);
            lua_pop(L, 1);
        }

        lua_pop(L, 1);
    }
}

/// @brief stack idx -1 = table with button content
/// @param name is used to throw errors with more precision
/// @warning lua_State should not end while button's events are used
/// @return new ButtonStruct()
ButtonStruct* createButton(lua_State*& L, const std::string& name, std::vector<ButtonStruct*>& buttons, std::vector<DynamicPanelStruct*>& dynamicPanels) {
    Rectangle rect;
    SCRIPT_GET_RECT(L, -1, rect, "rect", name + ".rect");

    ButtonStruct* pButton = new ButtonStruct(name.c_str());
    pButton->ref = luaL_ref(L, LUA_REGISTRYINDEX);
    pButton->button = {rect};

    pButton->button.config = std::make_shared<Meatball::Config::Button>(Meatball::Defaults::buttonConfig);
    
    lua_rawgeti(L, LUA_REGISTRYINDEX, pButton->ref);

    lua_getfield(L, -1, "config");
        SCRIPT_GET_COLOR(L, -1, pButton->button.config->color, "color", name + ".config.color");
        SCRIPT_GET_COLOR(L, -1, pButton->button.config->hoveredColor, "hoveredColor", name + ".config.hoveredColor");
    lua_pop(L, 1);

    lua_getfield(L, -1, "onHover");
    if (lua_isfunction(L, -1))
        pButton->button.onHover = [&L, pButton, &buttons, &dynamicPanels]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pButton->ref);

            lua_getfield(L, -1, "onHover");
            if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onHover script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 1);
                return;
            }

            updateInterface(L, buttons, dynamicPanels);
            lua_pop(L, 1);
        };
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "onRelease");
    if (lua_isfunction(L, -1))
        pButton->button.onRelease = [&L, pButton, &buttons, &dynamicPanels]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pButton->ref);

            lua_getfield(L, -1, "onRelease");
            if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onRelease script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 1);
                return;
            }

            updateInterface(L, buttons, dynamicPanels);

            lua_pop(L, 1);
        };
    lua_pop(L, 1);

    return pButton;
}

/// @brief stack idx -1 = table with dynamic panel content
/// @param name is used to throw errors with more precision
/// @warning lua_State should not end while button's events are used
/// @return new DynamicPanelStruct()
DynamicPanelStruct* createDynamicPanel(lua_State*& L, const std::string& name,
    std::vector<ButtonStruct*>& buttons, std::vector<DynamicPanelStruct*>& dynamicPanels) {    
    Rectangle rect;
    SCRIPT_GET_RECT(L, -1, rect, "rect", name + ".rect");

    DynamicPanelStruct* pDynamicPanel = new DynamicPanelStruct(name.c_str());

    pDynamicPanel->ref = luaL_ref(L, LUA_REGISTRYINDEX);
    pDynamicPanel->panel = {rect};
    
    pDynamicPanel->panel.config = std::make_shared<Meatball::Config::DynamicPanel>(Meatball::Defaults::dynamicPanelConfig);

    lua_rawgeti(L, LUA_REGISTRYINDEX, pDynamicPanel->ref);

    lua_getfield(L, -1, "config");
        SCRIPT_GET_COLOR(L, -1, pDynamicPanel->panel.config->color, "color", name + ".config.color");

        SCRIPT_GET_FIELD(L, -1, pDynamicPanel->panel.config->grabHeight, "grabHeight", lua_tonumber);
        
        SCRIPT_GET_VEC2(L, -1, pDynamicPanel->panel.config->minSize, "minSize", name + ".config.minSize");
    lua_pop(L, 1);

    lua_getfield(L, -1, "onMove");
    if (lua_isfunction(L, -1))
        pDynamicPanel->panel.onMove = [&L, pDynamicPanel, &buttons, &dynamicPanels]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pDynamicPanel->ref);

            lua_getfield(L, -1, "rect");
            SCRIPT_SET_RECT2(L, pDynamicPanel->panel.rect);
            lua_pop(L, 1);

            lua_getfield(L, -1, "onMove");
            if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onMove script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 2);
                return;
            }

            updateInterface(L, buttons, dynamicPanels);
            lua_pop(L, 1);
        };
    
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "onResize");
    if (lua_isfunction(L, -1))
        pDynamicPanel->panel.onResize = [&L, pDynamicPanel, &buttons, &dynamicPanels]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pDynamicPanel->ref);

            lua_getfield(L, -1, "rect");
            SCRIPT_SET_RECT2(L, pDynamicPanel->panel.rect);
            lua_pop(L, 1);

            lua_getfield(L, -1, "onResize");
            if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onResize script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 2);
                return;
            }

            updateInterface(L, buttons, dynamicPanels);
            lua_pop(L, 1);
        };

    lua_pop(L, 1);
    
    lua_getfield(L, -1, "onResizeStop");
    if (lua_isfunction(L, -1))
        pDynamicPanel->panel.onResizeStop = [&L, pDynamicPanel, &buttons, &dynamicPanels]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pDynamicPanel->ref);

            lua_getfield(L, -1, "rect");
            SCRIPT_SET_RECT2(L, pDynamicPanel->panel.rect);
            lua_pop(L, 1);

            lua_getfield(L, -1, "onResizeStop");
            if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onResizeStop script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 2);
                return;
            }

            updateInterface(L, buttons, dynamicPanels);

            lua_pop(L, 1);
        };
    else
        lua_pop(L, 1);

    return pDynamicPanel;
}

void handleNoneLayoutType(lua_State*& L, int tableIndex, std::vector<ButtonStruct*>& buttons, std::vector<DynamicPanelStruct*>& dynamicPanels) {
    lua_pushnil(L); // push the first key
    while (lua_next(L, tableIndex) != 0) {
        // FUTURE TODO: name could be used to see what is what in the window
        std::string name = std::string(lua_tostring(L, -2));

        int objectType;
        SCRIPT_GET_FIELD(L, -1, objectType, "type", lua_tointeger);

        switch (objectType) {
            case SCRIPT_UI_TYPE_BUTTON: {
                ButtonStruct* pButton = createButton(L, name, buttons, dynamicPanels);
                buttons.emplace_back(pButton);
                break;
            }
            case SCRIPT_UI_TYPE_DYNAMIC_PANEL: {
                DynamicPanelStruct* pDynamicPanel = createDynamicPanel(L, name, buttons, dynamicPanels);
                dynamicPanels.emplace_back(pDynamicPanel);
                break;
            }
        }

        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

int main(int argc, char** argv) {
    argc = 2;
    if (argc < 2) {
        std::cout << argv[0] << " <pathToScript>\n";
        return 1;
    }
    
    auto consoleUI = init(800, 600);
    consoleUI->visible = false;

    lua_State* L = Meatball::Script::createLuaState();
    unsigned char noneLayoutType = setLuaVariables(L);

    if (!Meatball::Script::loadFile(L, "data/scripts/Test.lua")) {
        lua_close(L);
        return 1;
    }

    SCRIPT_CHECK_NARGS(L, 2);

    if (lua_type(L, 1) != LUA_TNUMBER) // UIType
        SCRIPT_INCORRECT_TYPE(L, number);
    if (lua_type(L, 2) != LUA_TTABLE) // UIData
        SCRIPT_INCORRECT_TYPE(L, table);

    std::vector<ButtonStruct*> buttons;
    std::vector<DynamicPanelStruct*> dynamicPanels;

    int type = lua_tointeger(L, 1);
    if (type == noneLayoutType)
        handleNoneLayoutType(L, 2, buttons, dynamicPanels);
    
    lua_pop(L, 2); // pop the UIType and UIData

    Vector2 viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};

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

    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);

        float dt = GetFrameTime();
        if (dt > 0.016)
            dt = 0.016;

        if (IsWindowResized()) {
            int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            Vector2 ratio = { (float)newScreenWidth / viewport.x, (float)newScreenHeight / viewport.y };

            lua_pop(L, lua_gettop(L));

            lua_getglobal(L, "Game");
            lua_getfield(L, -1, "viewport");
                lua_pushnumber(L, newScreenWidth);
                lua_setfield(L, -2, "x");

                lua_pushnumber(L, newScreenHeight);
                lua_setfield(L, -2, "y");
            lua_pop(L, 2);

            for (auto& panel : dynamicPanels) {
                panel->panel.rect.x *= ratio.x;
                panel->panel.rect.y *= ratio.y;

                panel->panel.rect.width *= ratio.x;
                panel->panel.rect.height *= ratio.y;

                if (panel->panel.onResize)
                    panel->panel.onResize();
                if (panel->panel.onResizeStop)
                    panel->panel.onResizeStop();
            }
        }

        Meatball::Input::update(false);

        consoleUI->update();

        BeginDrawing();

        for (auto& pPanel : dynamicPanels) {
            pPanel->panel.update();
            Meatball::drawRect(pPanel->panel.rect, pPanel->panel.config->color);
        }
        for (auto& pButton : buttons) {
            pButton->button.update();
            Meatball::drawRect(pButton->button.rect, pButton->button.isHovered()? pButton->button.config->hoveredColor : pButton->button.config->color);
        }

        consoleUI->draw();

        EndDrawing();
    }

    for (auto& obj : buttons) {
        luaL_unref(L, LUA_REGISTRYINDEX, obj->ref);
        delete obj;
    }
    buttons.clear();

    for (auto& obj : dynamicPanels) {
        luaL_unref(L, LUA_REGISTRYINDEX, obj->ref);
        delete obj;
    }
    dynamicPanels.clear();

    lua_close(L);
}