#include "ScriptExecutor.h"

#include <sstream>

#include "Input.h"
#include "Console.h"

namespace ScriptFunctions {
#pragma region Input
    int bind(lua_State* L) {
        int nargs = lua_gettop(L);
        if (nargs != 2)
            return luaL_error(L, "Incorrect number of arguments. Expected 2, got %d", nargs);

        if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
            return luaL_error(L, "Incorrect data type. Expected string on both parameters.");
        
        const char* key = lua_tostring(L, 1);
        const char* callback = lua_tostring(L, 2);

        Meatball::Input::bind(key, callback);
        return 0;
    }

    int unbind(lua_State* L) {
        int nargs = lua_gettop(L);
        if (nargs != 1)
            return luaL_error(L, "Incorrect number of arguments. Expected 1, got %d", nargs);

        if (!lua_isstring(L, 1))
            return luaL_error(L, "Incorrect data type. Expected string.");
        
        const char* key = lua_tostring(L, 1);

        Meatball::Input::unbind(key);
        return 0;
    }
#pragma endregion
    
#pragma region Console
    int run(lua_State* L) {
        int nargs = lua_gettop(L);
        if (nargs != 1)
            return luaL_error(L, "Incorrect number of arguments. Expected 1, got %d", nargs);

        if (!lua_isstring(L, 1))
            return luaL_error(L, "Incorrect data type. Expected string.");
        
        const char* input = lua_tostring(L, 1);

        Meatball::Console::run(input);
        return 0;
    }
#pragma endregion

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
    
    lua_setglobal(L, "Meatball");
    
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