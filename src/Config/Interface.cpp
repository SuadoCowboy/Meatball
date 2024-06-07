#include "Interface.h"

Meatball::Config::Button::Button()
 : color(BLACK), hoveredColor(WHITE) {}

Meatball::Config::TextButton::TextButton()
 : color(BLACK), textColor(WHITE), hoveredColor(WHITE), hoveredTextColor(BLACK), font(nullptr) {}

Meatball::Config::DynamicPanel::DynamicPanel()
 : color(BLACK), minSize({4, 6}), grabHeight(2) {}

 Meatball::Config::InputTextBox::InputTextBox()
  : color(BLACK), textColor(WHITE), cursorColor(WHITE), font(nullptr) {}

Meatball::Config::Console::Console()
 : autoCompleteColor(BLACK), autoCompleteTextColor(WHITE),
 autoCompleteHighlightedTextColor(YELLOW), autoCompleteSelectedTextColor(PURPLE),
 labelTextColor(WHITE), mainFont(nullptr), labelFont(nullptr), labelText("") {}