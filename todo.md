# WARNINGS:
- Alot of objects might create alot of new colors in memory.


# TODOS (to actually do them right know):
- InputBox CTRL+A/LEFT/RIGHT/ (MAYBE)+SHIFT+LEFT/RIGHT
- InputBox mouse select string
- colors should use references, pointers OR (static) variables in another class.

# FUTURE TODOS:
- optimize code and use the right data structures. Also research const char* vs char[] vs std::string and look which is better and for what
- use asynchronous tasks/threaded/both?
- update function descriptions and anything else related to reading

# THINKING:
- The developers could make their draw functions instead of the classes drawing, because then they could use the same classes but with different styles whether they want. All the classes could make is give specific drawing functions for stuff that requires a complexity level or private/protected-access. And together with that idea, the classes shall not store colors. Maybe store in a ColorsHandler or something and then the draw functions get the colors that way

- everything related to drawing is handled by the dev, so everyone could make the style they want their game to be!!
example:
```cpp
// look for better name
void borderfy(const Rectangle& rect, const Color& borderColor) {
    DrawLine(rect.x, rect.y, rect.x+rect.width, rect.y, borderColor);
    DrawLine(rect.x, rect.y+rect.height, rect.x+rect.width, rect.y+rect.height, borderColor);
    DrawLine(rect.x, rect.y, rect.x, rect.y+rect.height, borderColor);
    DrawLine(rect.x+rect.width, rect.y, rect.x+rect.width, rect.y+rect.height, borderColor);
}
```
- maybe even do that with the hoveredColors and stuff in update function, so that the dev can choose if he wants that functionality or not. It would be something like:
```cpp
/// @brief can be used for everything that has a rect and uses color
const Color* const updateHoverColor(const Rectangle& rect, const Color* const defaultColor, const Color* const hoverColor) {
    return CheckCollisionPointRec(GetMousePosition(), rect)? hoverColor : defaultColor;
}

void update() {
    /*
    TextBox textBox;
    Color textBoxColor, textBoxHoverColor;
    */
    // this means that each class should only need a pointer to a color and then no need to copy colors
    textBox.color = updateHoverColor(textBox.getRect(), textBoxColor, textBoxHoverColor);
}
```

- use char buffers instead of std::string because it seems like string is bad? idk, do more research

# IMPORTANT:
- make things minimalist, things like borders and effects can be added using functions or classes

# IMPROVE .meatdata FILE TYPES:
- let it have variables to identify colors and transparency stuff in the code or just variables, and also let have comments with # or something idk
- create documentation for everything that can be modified and which name should they have in .meatdata
- maybe permit a console command to load .meatdata files and even examine them

```cpp
class ConsoleUI /*WORK IN PROGRESS*/ {
    #missing inputbox;
    #missing a little text box that indicates possible words the user might be looking for(maybe do like ddrace one).
}
```

# FUTURE NEXT-STEPS:
- Input handling with binds by HayBCMD
- multiplayer Stuff
- make 3D easier in raylib
- 3D map maker and loader
- take source engine movement(Bhop, ABH, Strafe, etc)
- good Collision Detection(AABB? Search more about it)
- A* path finding for AI