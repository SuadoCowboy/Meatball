#include "ScriptExecutor.h"

#include <Input.h>

int scriptBind(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs != 2)
        return luaL_error(L, "Incorrect number of arguments. Expected 2, got %d", nargs);

    if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
        return luaL_error(L, "Incorrect data type. Expected string on both parameters.");
    
    const char* name = lua_tostring(L, 1);
    const char* callback = lua_tostring(L, 2);

    Meatball::Input::bind(name, callback);
    
    return 0;
}

lua_State* Meatball::Script::createLuaState() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_newtable(L); // Meatball 1
    
    lua_pushstring(L, "Input"); // 2
    lua_newtable(L); // Input 3

    lua_pushstring(L, "bind"); // 4
    lua_pushcfunction(L, scriptBind); // 5
    lua_settable(L, 3); // 2[4] = 5; pop(5, 4);

    lua_settable(L, 1); // 1[2] = 3; pop(3, 2);

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