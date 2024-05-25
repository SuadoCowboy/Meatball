# WARNING: Alot of objects might create alot of new colors in memory.
maybe colors should use references, pointers OR static variables in another class.

# Maybe use char buffers instead of std::string because it seems like string is bad? idk, do more research

# IMPORTANT: make things minimalist, things like borders and effects can be added using functions or classes
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
## maybe even do that with the hoveredColors and stuff in update function, so that the dev can choose if he wants that functionality or not. It would be something like:
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

# Use asynchronous tasks/threaded/both?

```cpp
class ConsoleUI /*WORK IN PROGRESS*/ {
    #missing inputbox;
    #missing a little text box that indicates possible words the user might be looking for(maybe do like ddrace one).
}
```

Input handling with binds by HayBCMD;

Multiplayer Stuff;

Make 3D easier in raylib;
3D map maker and loader;

take source engine movement(Bhop, ABH, Strafe, etc);

Good Collision Detection(AABB? Search more about it);

A* path finding for AI;