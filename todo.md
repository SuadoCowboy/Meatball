# A MAYBE:
- A class that handles shared_ptrs of objects, probably colors(because i'm obsessed with colors). When creating a new color object, it checks if this color is already defined and give a shared_ptr to that color. If the color is new create a new object. Everytime a color changes it should also create a new shared_ptr so that it does not modifies the past one. If this works well and it really helps memory usage and does not affect much on performance, do that for stuff that is repeated too much. 

# TODOS:
- how can i make font resize together with screen? let font be uglier(i believe it's a YES)?

- create a .meatui(maybe delete .meatdata or use for another thing that .cfg can't do) file extension to make it possible to create a user interface. Should also include a meatUILoader.dll and meatUIViewer.exe in bin folder so that users can test their interfaces without joining the game everytime. Example: for Console, .meatui, the user have some options: mainPanel, closeButton, sendButton(optional), inputBox and OutputTextBox.
!!! Also set a macro to define its actual version because it could change alot in the future
It would be something like this:
```py
#      x y width height style   objectName
Button x y width height STYLE_X "closeButton"

#            x y width height borderRadius objectName
DynamicPanel x y width height 0           "mainPanel"

# Optional stuff: use Void objectName
Void "sendButton"

#            x y width height defaultText                        objectName
InputTextBox x y width height "default text written in input box" "inputBox"

#             x y width height objectName
ScrollTextBox x y width height "outputBox"

# TODOS:
# if game does not recognize objectName: tell user
# if objectName is already defined: warn user and use the already defined instead
# if x y width height is wrong: warn user but still let it pass with default values
# if type is not allowed for objectName: tell user and stop running(because it might be dangerous)
# if a objectName is not defined in the end: tell user and stop running(because it might be dangerous)
```

# FUTURE TODOS:
- optimize code and use the right data structures.
- use threads with locks, atomic and mutex
- every data structure that is needed everywhere should be sent as a pointer
- update function descriptions and anything else related to reading
- fontSize of stuff all stuffs is related to their height sizes

# THINKING:
- The developers could make their draw functions instead of the classes drawing, because then they could use the same classes but with different styles whether they want. All the classes could make is give specific drawing functions for stuff that requires a complexity level or private/protected-access. And together with that idea, the classes shall not store colors. Maybe store in a ColorsHandler or something and then the draw functions get the colors that way

- everything related to drawing is handled by the dev, so everyone could make the style they want their game to be!!
example:
```cpp
// look for better name
void borderfy(const Rectangle &rect, const Color &borderColor) {
    DrawLine(rect.x, rect.y, rect.x+rect.width, rect.y, borderColor);
    DrawLine(rect.x, rect.y+rect.height, rect.x+rect.width, rect.y+rect.height, borderColor);
    DrawLine(rect.x, rect.y, rect.x, rect.y+rect.height, borderColor);
    DrawLine(rect.x+rect.width, rect.y, rect.x+rect.width, rect.y+rect.height, borderColor);
}
```
- maybe even do that with the hoveredColors and stuff in update function, so that the dev can choose if he wants that functionality or not. It would be something like:
```cpp
/// @brief can be used for everything that has a rect and uses color
const Color* const updateHoverColor(const Rectangle &rect, const Color* const defaultColor, const Color* const hoverColor) {
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