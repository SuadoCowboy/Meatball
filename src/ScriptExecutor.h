#pragma once

#include <filesystem>

#include <lua.hpp>

#include <HayBCMD.h>

#define SCRIPT_CREATE_VEC2(L, x, y) \
    lua_newtable(L); \
    lua_pushnumber(L, x); \
    lua_setfield(L, -2, "x"); \
    lua_pushnumber(L, y); \
    lua_setfield(L, -2, "y")

namespace Meatball::Script {
    /// @brief initializes lua_State while also adding default Meatball variables
    /// @warning don't forget to use lua_close(L)!
    lua_State* createLuaState();

    bool loadFile(lua_State* L, const std::filesystem::path& path);
}