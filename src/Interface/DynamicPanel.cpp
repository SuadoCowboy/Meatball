#include "DynamicPanel.h"

Meatball::DynamicPanel::DynamicPanel() : color(BLACK), rect((Rectangle){0,0,0,0}) {}

Meatball::DynamicPanel::DynamicPanel(Rectangle rect) : rect(rect) {}

Meatball::DynamicPanel::DynamicPanel(float x, float y, float width, float height)