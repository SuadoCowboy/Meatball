# WARNING: Alot of objects might create alot of new colors in memory.
maybe colors should use references, pointers OR static variables in another class.

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

## asynchronous tasks or threaded things or both?

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