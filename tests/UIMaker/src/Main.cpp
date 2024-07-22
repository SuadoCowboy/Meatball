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

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);

        Meatball::Input::update(false);

        BeginDrawing();

        EndDrawing();
    }

    lua_close(L);
}