#pragma once

#include <filesystem>

#include <lua.hpp>

#include <HayBCMD.h>

namespace Meatball {
    bool loadFile(const std::filesystem::path& path, lua_State* L);
}