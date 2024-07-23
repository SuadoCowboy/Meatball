#include "ScriptExecutor.h"

#include <sstream>

#include "Input.h"
#include "Console.h"

bool Meatball::Script::isRect(lua_State* L, int index) {
    if (!lua_istable(L, index))
        return false;

    lua_getfield(L, index, "x");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);

    lua_getfield(L, index, "width");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);

    lua_getfield(L, index, "height");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);
    
    return true;
}

bool Meatball::Script::isColor(lua_State* L, int index) {
    if (!lua_istable(L, index))
        return false;

    lua_getfield(L, index, "r");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);

    lua_getfield(L, index, "g");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);

    lua_getfield(L, index, "b");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);

    lua_getfield(L, index, "a");
    if (lua_type(L, -1) != LUA_TNUMBER)
        return false;
    lua_pop(L, 1);
    
    return true;
}

namespace ScriptFunctions {
    int luaPrintFunction(lua_State* L) {
        int n = lua_gettop(L);  // Number of arguments
        std::stringstream out;
        for (int i = 1; i <= n; ++i) {
            if (lua_isstring(L, i)) {
                const char* str = lua_tostring(L, i);
                out << str;
            } else {
                out << lua_typename(L, lua_type(L, i));
            }
            if (i < n) {
                out << " ";  // Add a tab between arguments
            }
        }
        out << "\n";  // End with a newline

        Meatball::Console::printf(HayBCMD::OutputLevel::ECHO, "script: {}", out.str());
        return 0;
    }

#pragma region Input
    int bind(lua_State* L) {
        SCRIPT_CHECK_NARGS(L, 2);

        if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
            SCRIPT_INCORRECT_TYPE(L, string);
        
        const char* key = lua_tostring(L, 1);
        const char* callback = lua_tostring(L, 2);

        Meatball::Input::bind(key, callback);
        return 0;
    }

    int unbind(lua_State* L) {
        SCRIPT_CHECK_NARGS(L, 1);

        if (!lua_isstring(L, 1))
            return SCRIPT_INCORRECT_TYPE(L, string);
        
        const char* key = lua_tostring(L, 1);

        Meatball::Input::unbind(key);
        return 0;
    }
#pragma endregion
    
#pragma region Console
    int run(lua_State* L) {
        SCRIPT_CHECK_NARGS(L, 1);

        if (!lua_isstring(L, 1))
            return SCRIPT_INCORRECT_TYPE(L, string);
        
        const char* input = lua_tostring(L, 1);

        Meatball::Console::run(input);
        return 0;
    }
#pragma endregion

#pragma region UI
    int button(lua_State* L) {
        /*
        Button = {
            rect,
            config = {
                color,
                hoveredColor
            },
            onHover,
            onRelease
            type = TYPE_BUTTON
        }
        */
        lua_newtable(L);
        SCRIPT_CREATE_RECT(L, 0,0, 0,0);
        lua_setfield(L, -2, "rect");

        lua_pushnil(L);
        lua_setfield(L, -2, "config");

        lua_pushnil(L);
        lua_setfield(L, -2, "onHover");

        lua_pushnil(L);
        lua_setfield(L, -2, "onRelease");

        lua_newtable(L); // config
            SCRIPT_CREATE_COLOR(L, 0,0,0,255);
            lua_setfield(L, -2, "color");
            SCRIPT_CREATE_COLOR(L, 0,0,0,255);
            lua_setfield(L, -2, "hoveredColor");
        lua_setfield(L, -2, "config");

        lua_pushinteger(L, SCRIPT_UI_TYPE_BUTTON);
        lua_setfield(L, -2, "type");

        return 1;
    }
#pragma endregion
}

lua_State* Meatball::Script::createLuaState() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, ScriptFunctions::luaPrintFunction);
    lua_setglobal(L, "print");

    lua_newtable(L); // Meatball 1
    
    lua_newtable(L); // Input 2
        lua_pushcfunction(L, ScriptFunctions::bind); // 3
        lua_setfield(L, -2, "bind"); // pop(3)

        lua_pushcfunction(L, ScriptFunctions::unbind);
        lua_setfield(L, -2, "unbind");
    lua_setfield(L, -2, "Input");

    lua_newtable(L); // Console
        lua_pushcfunction(L, ScriptFunctions::run);
        lua_setfield(L, -2, "run");
    lua_setfield(L, -2, "Console");

    lua_newtable(L); // UI
        lua_newtable(L); // LayoutTypes
            // If you're looking to add your own type, just get the Meatball
            // table in your code and set from there
            lua_pushinteger(L, SCRIPT_UI_LAYOUTTYPES_CONSOLE);
            lua_setfield(L, -2, "CONSOLE");
        lua_setfield(L, -2, "LayoutTypes");

        lua_newtable(L); // Types
            lua_pushinteger(L, SCRIPT_UI_TYPE_BUTTON);
            lua_setfield(L, -2, "BUTTON");
        lua_setfield(L, -2, "Types");

        lua_pushcfunction(L, ScriptFunctions::button);
        lua_setfield(L, -2, "button");
    lua_setfield(L, -2, "UI");

    lua_setglobal(L, "Meatball");

    luaL_dostring(L, R"(
    function Meatball.rect(x, y, width, height)
        return {x=x, y=y, width=width, height=height}
    end

    function Meatball.vec2(x, y)
        return {x=x, y=y}
    end

    function Meatball.color(r, g, b, a)
        return {r=r, g=g, b=b, a=a}
    end

    )");

    return L;
}

bool Meatball::Script::loadFile(lua_State* L, const std::filesystem::path& path) {
        if (!std::filesystem::exists(path) || std::filesystem::is_directory(path) || path.extension().string() != ".lua") {
            HayBCMD::Output::printf(HayBCMD::ERROR, "could not run lua script: \"{}\" is not compatible or does not exist\n", path.string());
            return false;
        }

        if (luaL_dofile(L, path.string().c_str()) != LUA_OK) {
            const char* error = lua_tostring(L, -1);
            HayBCMD::Output::printf(HayBCMD::ERROR, "could not run lua script: {}\n", error);
            
            lua_pop(L, 1); // Remove error message from stack
            
            return false;
        }

        return true;
    }