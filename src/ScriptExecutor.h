#pragma once

#include <filesystem>

#include <lua.hpp>

#include <HayBCMD.h>

namespace Meatball::Script {
    /// @brief initializes lua_State while also adding default Meatball variables
    /// @warning don't forget to use lua_close(L)!
    lua_State* createLuaState();

    bool loadFile(lua_State* L, const std::filesystem::path& path);
}