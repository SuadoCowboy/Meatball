#include "ScriptExecutor.h"

bool Meatball::loadFile(const std::filesystem::path& path, lua_State* L) {
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