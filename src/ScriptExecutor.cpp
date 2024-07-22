#include "ScriptExecutor.h"

#include <sstream>

#include "Input.h"
#include "Console.h"

#pragma region Macros
#define INCORRECT_TYPE(L, x) luaL_error(L, "Incorrect data type. Expected " #x ".")
#define INCORRECT_NARGS(L, expected, received) luaL_error(L, "Incorrect number of arguments. Expected " #expected ", got " #received ".")

#define CHECK_NARGS(L, expected)    \
    int nargs = lua_gettop(L);      \
    if (nargs != expected)          \
        return INCORRECT_NARGS(L, expected, nargs)

#define GET_FIELD(L, idx, val, name, convertFunc) \
    lua_getfield(L, idx, name); \
    val = convertFunc(L, -1); \
    lua_pop(L, idx)

#pragma endregion

namespace ScriptFunctions {
    bool isRect(lua_State* L, int idx) {
        if (!lua_istable(L, idx))
            return false;

        if (lua_getfield(L, idx, "x") != LUA_TNUMBER)
            return false;
        if (lua_getfield(L, idx, "y") != LUA_TNUMBER)
            return false;
        if (lua_getfield(L, idx, "width") != LUA_TNUMBER)
            return false;
        if (lua_getfield(L, idx, "height") != LUA_TNUMBER)
            return false;
        
        return true;
    }

    bool isColor(lua_State* L, int idx) {
        if (!lua_istable(L, idx))
            return false;

        if (lua_getfield(L, idx, "r") != LUA_TNUMBER)
            return false;
        if (lua_getfield(L, idx, "g") != LUA_TNUMBER)
            return false;
        if (lua_getfield(L, idx, "b") != LUA_TNUMBER)
            return false;
        if (lua_getfield(L, idx, "a") != LUA_TNUMBER)
            return false;
        
        return true;
    }

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
        CHECK_NARGS(L, 2);

        if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
            INCORRECT_TYPE(L, string);
        
        const char* key = lua_tostring(L, 1);
        const char* callback = lua_tostring(L, 2);

        Meatball::Input::bind(key, callback);
        return 0;
    }

    int unbind(lua_State* L) {
        CHECK_NARGS(L, 1);

        if (!lua_isstring(L, 1))
            return INCORRECT_TYPE(L, string);
        
        const char* key = lua_tostring(L, 1);

        Meatball::Input::unbind(key);
        return 0;
    }
#pragma endregion
    
#pragma region Console
    int run(lua_State* L) {
        CHECK_NARGS(L, 1);

        if (!lua_isstring(L, 1))
            return INCORRECT_TYPE(L, string);
        
        const char* input = lua_tostring(L, 1);

        Meatball::Console::run(input);
        return 0;
    }
#pragma endregion

#pragma region Utils
    int drawRect(lua_State* L) {
        CHECK_NARGS(L, 2);

        if (!isRect(L, 1))
            return INCORRECT_TYPE(L, rect);
        if (!isColor(L, 2))
            return INCORRECT_TYPE(L, color);

        int x,y,width,height;
        GET_FIELD(L, 1, x, "x", lua_tonumber);
        GET_FIELD(L, 1, y, "y", lua_tonumber);
        GET_FIELD(L, 1, width, "width", lua_tonumber);
        GET_FIELD(L, 1, height, "height", lua_tonumber);

        Color color;

        GET_FIELD(L, 2, color.r, "r", lua_tointeger);
        GET_FIELD(L, 2, color.g, "g", lua_tointeger);
        GET_FIELD(L, 2, color.b, "b", lua_tointeger);
        GET_FIELD(L, 2, color.a, "a", lua_tointeger);
        
        DrawRectangle(x, y, width, height, color);

        return 0;
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
    
    lua_newtable(L);
        lua_pushcfunction(L, ScriptFunctions::drawRect);
        lua_setfield(L, -2, "drawRect");
    lua_setfield(L, -2, "Utils");

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