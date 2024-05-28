#include "DynamicPanel.h"

Meatball::DynamicPanel::DynamicPanel() : color(BLACK), rect({0,0,0,0}) {}

Meatball::DynamicPanel::DynamicPanel(Rectangle rect) : rect(rect) {}

Meatball::DynamicPanel::DynamicPanel(float x, float y, float width, float height) {
    rect = {x, y, width, height};
}