#pragma once

#include <string>

#include <raylib.h>

#include <SweatCI.h>
#include "../Utils/Utils.h"

/// @brief predefined functions for some data types for CVARs are stored here
namespace Meatball::CVARFuncs {
    void setColor(void *pData, const std::string& str);
    std::string getColor(void *pData);
}