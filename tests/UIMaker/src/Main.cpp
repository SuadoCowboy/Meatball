#include <iostream>
#include <vector>

#include <raylib.h>

#include <HayBCMD.h>

#include <ScriptExecutor.h>
#include <Console.h>
#include <Input.h>
#include <Interface/Button.h>
#include <Utils/Defaults.h>
#include <Utils/DrawFuncs.h>

struct ButtonStruct {
    Meatball::Button button;
    int onHoverFuncRef = 0, onReleaseFuncRef = 0;
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
            SCRIPT_CREATE_VEC2(L, GetRenderWidth(), GetRenderHeight());
            lua_setfield(L, -2, "viewport");
        lua_setglobal(L, "Game");
    }

    return noneLayoutType;
}

/// @brief stack idx -1 = table with button content
/// @warning lua_State should not end while button's events are used
/// @return new ButtonStruct()
ButtonStruct* createButton(lua_State*& L) {
    lua_getfield(L, -1, "rect");
        if (!Meatball::Script::isRect(L, -1))
            luaL_error(L, "Button.rect is not a rect.");
        
        Rectangle rect;
        SCRIPT_GET_RECT(L, -1, rect);
    lua_pop(L, 1);

    ButtonStruct* pButton = new ButtonStruct();
    pButton->button = {rect};

    pButton->button.config = std::make_shared<Meatball::Config::Button>(Meatball::Defaults::buttonConfig);
    lua_getfield(L, -1, "config");
    lua_getfield(L, -1, "color");
        SCRIPT_GET_COLOR(L, -1, pButton->button.config->color);
    lua_pop(L, 1);
    lua_getfield(L, -1, "hoveredColor");
        SCRIPT_GET_COLOR(L, -1, pButton->button.config->hoveredColor);
    lua_pop(L, 2);

    lua_getfield(L, -1, "onHover");
    if (lua_isfunction(L, -1)) {
        pButton->onHoverFuncRef = luaL_ref(L, LUA_REGISTRYINDEX);
        pButton->button.onHover = [&L, pButton]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pButton->onHoverFuncRef);

            if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onHover script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 1);
                return;
            }
        };
    } else
        lua_pop(L, 1);
    
    lua_getfield(L, -1, "onRelease");
    if (lua_isfunction(L, -1)) {
        pButton->onReleaseFuncRef = luaL_ref(L, LUA_REGISTRYINDEX);
        pButton->button.onRelease = [&L, pButton]() {
            lua_rawgeti(L, LUA_REGISTRYINDEX, pButton->onReleaseFuncRef);

            if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                Meatball::Console::printf(HayBCMD::ERROR, "error calling onRelease script: {}\n", lua_tostring(L, -1));
                lua_pop(L, 1);
                return;
            }
        };
    } else
        lua_pop(L, 1);

    return pButton;
}

void handleNoneLayoutType(lua_State*& L, std::vector<ButtonStruct*>& buttons) {
    lua_pushnil(L); // push the first key
    while (lua_next(L, 2) != 0) {
        // FUTURE TODO: name could be used to see what is what in the window
        //const char* name = lua_tostring(L, -2);

        int objectType;
        SCRIPT_GET_FIELD(L, -1, objectType, "type", lua_tointeger);

        switch (objectType) {
            case SCRIPT_UI_TYPE_BUTTON: {
                ButtonStruct* pButton = createButton(L);
                buttons.emplace_back(pButton);
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

    int type = lua_tointeger(L, 1);
    if (type == noneLayoutType)
        handleNoneLayoutType(L, buttons);
    
    lua_pop(L, 2); // pop the UIType and UIData

    Vector2 viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};

    HayBCMD::Command("toggle_local_console", 0, 0, [&consoleUI](void*, const std::vector<std::string>&){
        consoleUI->visible = not consoleUI->visible;
    }, "- toggles local console");
    HayBCMD::execConfigFile("data/cfg/config.cfg", Meatball::Console::variables);

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);

        float dt = GetFrameTime();
        if (dt > 0.016)
            dt = 0.016;

        if (IsWindowResized()) {
            int newScreenWidth = GetRenderWidth(), newScreenHeight = GetRenderHeight();
            Vector2 ratio = { (float)newScreenWidth / viewport.x, (float)newScreenHeight / viewport.y };

            lua_getglobal(L, "Game");
            lua_getfield(L, -1, "viewport");
            
            lua_pushnumber(L, newScreenWidth);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, newScreenHeight);
            lua_setfield(L, -2, "y");

            lua_pop(L, 2);

            lua_getglobal(L, "OnResize");
            if (!lua_isnil(L, -1)) {
                SCRIPT_CREATE_VEC2(L, ratio.x, ratio.y);
                lua_call(L, 1, 0);

                viewport.x = newScreenWidth;
                viewport.y = newScreenHeight;
            } else
                lua_pop(L, 1);
        }

        Meatball::Input::update(false);

        consoleUI->update();

        BeginDrawing();

        for (auto& pButton : buttons) {
            pButton->button.update();
            Meatball::drawRect(pButton->button.rect, pButton->button.isHovered()? pButton->button.config->hoveredColor : pButton->button.config->color);
        }
        consoleUI->draw();

        EndDrawing();
    }

    for (auto& obj : buttons) {
        if (obj->onHoverFuncRef != 0)
            luaL_unref(L, LUA_REGISTRYINDEX, obj->onHoverFuncRef);
        if (obj->onReleaseFuncRef != 0)
            luaL_unref(L, LUA_REGISTRYINDEX, obj->onReleaseFuncRef);
        delete obj;
    }
    buttons.clear();

    lua_close(L);
}