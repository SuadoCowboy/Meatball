#pragma once

#include <filesystem>

#include <lua.hpp>

#include <HayBCMD.h>

#pragma region Macros
#define SCRIPT_INCORRECT_TYPE(L, x) luaL_error(L, "Incorrect data type. Expected " #x ".")
#define SCRIPT_INCORRECT_NARGS(L, expected, received) luaL_error(L, "Incorrect number of arguments. Expected " #expected ", got " #received ".")

#define SCRIPT_CHECK_NARGS(L, expected)    \
    int nargs = lua_gettop(L);      \
    if (nargs != expected)          \
        return SCRIPT_INCORRECT_NARGS(L, expected, nargs)

#define SCRIPT_GET_FIELD(L, idx, val, name, convertFunc) \
    lua_getfield(L, idx, name); \
    val = convertFunc(L, -1); \
    lua_pop(L, 1)

#define SCRIPT_SET_VEC2(L, x, y) \
    lua_pushnumber(L, x); \
    lua_setfield(L, -2, "x"); \
    lua_pushnumber(L, y); \
    lua_setfield(L, -2, "y")

#define SCRIPT_SET_RECT(L, x, y, w, h) \
    lua_pushnumber(L, x); \
    lua_setfield(L, -2, "x"); \
    lua_pushnumber(L, y); \
    lua_setfield(L, -2, "y"); \
    lua_pushnumber(L, w); \
    lua_setfield(L, -2, "width"); \
    lua_pushnumber(L, h); \
    lua_setfield(L, -2, "height")

#define SCRIPT_SET_RECT2(L, rect) SCRIPT_SET_RECT(L, rect.x, rect.y, rect.width, rect.height)

#define SCRIPT_SET_COLOR(L, r, g, b, a) \
    lua_pushnumber(L, r); \
    lua_setfield(L, -2, "r"); \
    lua_pushnumber(L, g); \
    lua_setfield(L, -2, "g"); \
    lua_pushnumber(L, b); \
    lua_setfield(L, -2, "b"); \
    lua_pushnumber(L, a); \
    lua_setfield(L, -2, "a")

#define SCRIPT_GET_RECT(L, idx, rect, name, objectName) \
    lua_getfield(L, idx, name); \
    if (!Meatball::Script::isRect(L, -1)) \
        luaL_error(L, "%s is not a rect.", objectName); \
    SCRIPT_GET_FIELD(L, -1, rect.x, "x", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, rect.y, "y", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, rect.width, "width", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, rect.height, "height", lua_tonumber); \
    lua_pop(L, 1);

#define SCRIPT_GET_VEC2(L, idx, vec2, name, objectName) \
    lua_getfield(L, idx, name); \
    if (!Meatball::Script::isVec2(L, -1)) \
        luaL_error(L, "%s is not a vec2.", objectName); \
    SCRIPT_GET_FIELD(L, -1, vec2.x, "x", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, vec2.y, "y", lua_tonumber); \
    lua_pop(L, 1)

#define SCRIPT_GET_COLOR(L, idx, color, name, objectName) \
    lua_getfield(L, idx, name); \
    if (!Meatball::Script::isColor(L, -1)) \
        luaL_error(L, "%s is not a rect.", objectName); \
    SCRIPT_GET_FIELD(L, -1, color.r, "r", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, color.g, "g", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, color.b, "b", lua_tonumber); \
    SCRIPT_GET_FIELD(L, -1, color.a, "a", lua_tonumber); \
    lua_pop(L, 1)

#pragma endregion

#define SCRIPT_UI_LAYOUT_TYPES_CONSOLE 0

#define SCRIPT_UI_TYPE_BUTTON 0
#define SCRIPT_UI_TYPE_DYNAMIC_PANEL 1

namespace Meatball::Script {
    bool isRect(lua_State* L, int index);
    bool isVec2(lua_State* L, int index);
    bool isColor(lua_State* L, int index);

    /// @brief initializes lua_State while also adding default Meatball variables
    /// @warning don't forget to use lua_close(L)!
    lua_State* createLuaState();

    bool loadFile(lua_State* L, const std::filesystem::path& path);
}