#include "ColorsHandler.h"

void Meatball::ColorsHandler::add(const std::string& name, Color color) {
    colors[name] = color;
}

Color *Meatball::ColorsHandler::get(const std::string& name) {
    if (colors.count(name) == 0) return nullptr;

    return &colors[name];
}

void Meatball::ColorsHandler::clear() {
    colors.clear();
}