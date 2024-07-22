#include <iostream>

#include <raylib.h>

#include <ScriptExecutor.h>
#include <Console.h>
#include <Input.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << argv[0] << " <pathToScript>\n";
        return 1;
    }
    
    Meatball::Console::init([](const HayBCMD::OutputLevel&, const std::string& msg) {
        std::cout << msg;
    });
    
    Meatball::Input::registerCommands();
    Meatball::Input::mapKeyboardKeys();
    Meatball::Input::mapMouseKeys();

    lua_State* L = Meatball::Script::createLuaState();
    if (!Meatball::Script::loadFile(L, argv[1])) {
        lua_close(L);
        return 1;
    }

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 600, "Meatball's Interface Creator");

    lua_newtable(L); // Game
    
    SCRIPT_CREATE_VEC2(L, GetRenderWidth(), GetRenderHeight());
    lua_setfield(L, -2, "viewport");

    lua_setglobal(L, "Game");

    Vector2 viewport = {(float)GetRenderWidth(), (float)GetRenderHeight()};

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);

        float dt = GetFrameTime();

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
            SCRIPT_CREATE_VEC2(L, ratio.x, ratio.y);
            lua_call(L, 1, 0);

            viewport.x = newScreenWidth;
            viewport.y = newScreenHeight;
        }

        if (dt > 0.016)
            dt = 0.016;

        Meatball::Input::update(false);

        lua_getglobal(L, "Update");
        lua_pushnumber(L, (double)dt);
        lua_call(L, 1, 0);

        BeginDrawing();

        lua_getglobal(L, "Draw");
        lua_call(L, 0, 0);

        EndDrawing();
    }

    lua_close(L);
}