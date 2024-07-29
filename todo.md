# WARNING
- because sizeof(Color) is below 16(which is shared_ptr size), it's better cloning it or using a reference than actually using a pointer or shared_ptr.

# TODOS
- Test multiple dynamic panels together

- Create another library for network stuff. Meatball can be used to test with it but it's not built-in in meatball unless necessary

- Create tasks system, the console commands will run through tasks and then the players will be able to use the wait command(also add wait_ticks which waits ticks instead of milliseconds)

- Implement UI commands
- FontsHandler still a piece of shit. Fix it.

- ConsoleUI: Try implementing a optional draw lua function so that the creator of the UI can draw the way he wants to. (this way they could also make stupid things, which is fun). If this is implemented, it would be nice to give lua variables the delta time and stuff related to game.

- Example: for Console, the user have some options: mainPanel, closeButton, sendButton(optional), inputBox and OutputTextBox. They also can modify events that the Console allowed, like: mainPanel::onMove, mainPanel::onResize and mainPanel::onResizeStop. The rest of the events are handled by the ConsoleUI code. closeButton::onRelease can not be changed because by default it should set ConsoleUI.visible to false. sendButton is the same thing, it should always send to the output and clear the input text. It would be something like this in lua:
```c++
// THIS IS STILL ON THINKING, THINGS MIGHT NOT BE LIKE THIS ON RELEASE OF THE FIRST VERSION
// Also add something related to resizing. Something like a anchor (see how game engines work).

// BETTER IDEA: It works like Lua's stack. It adds all the data needed and then we call ui_create_button <name> and the data is popped. Any data that is not needed or not recognized, a error is printed but it will still be popped to not ruin any other possible ui object.
// params: name x y width height color borderRadius
ui_color 10 10 10 255
ui_border_radius 0
ui_rect_min 0 0 10 10 // for mainPanel, (x,y) are used to determine where mainPanel can be dragged or resized to
ui_rect_max_percent width height w h // [w,width] = renderWidth; [h,height] = renderHeight
ui_create_dynamic_panel "mainPanel"

// params: name x y width height color hoveredColor BUTTON_STYLE(number)
//ui_create_button "closeButton" x y width height 235 235 235 255 255 255 255 255 1
ui_color 235 235 235 255
ui_hovered_color 255 255 255 255
// x% y% width% height%
ui_anchor "mainPanel" // mainPanel should be defined before closeButton
ui_rect_min_percent 0 0 1 1
ui_rect_max_percent 0 0 3 3 
ui_draw x // it means that we will use drawX function
ui_create_button "closeButton"

ui_set_event "closeButton" "onRelease" toggle_local_console

// params: name x y width height color textColor hoveredColor hoveredTextColor text
//ui_create_text_button "sendButton" x y width height r g b a (x4) "Send"

ui_create_input_text_box "inputBox" x y width height 20 20 20 255 "default text"

ui_create_scroll_text_box name x y width height 20 20 20 255
// OR
// OBS: This method might need to create a math interpreter but it might not bee too hard to do and maybe even the HayBCMD could have already implemented
// where inputBox is specified, it means that it gets which parameter it is(ex: xPosition) and get the member of that object that matches the parameter
ui_create_scroll_text_box name "inputBox" "inputBox - 30" "inputBox" "inputBox"

// and then if someone wants to add a random button non-related to ConsoleUI:
ui_create_text_button "smiteAllButton" x y width height r g b a

ui_finish "ConsoleUI"
// this line gather all objects created by the ui_create commands and create a complete interface with a name that the game might look for and pop from the ui storage and set a defined object using that. Makes sense? I think it's pretty simple to do this. By the way, known objects have to use a specific name so that the game that uses that interface knows who is who

/*TODOS: (those are old todos that was made for the lua project. Don't know if it's still useful now)
if game does not recognize objectName: tell user
if type is not allowed for objectName: tell user and stop running
if a required objectName is not defined in the end: tell user and stop running
*/
```

# FUTURE TODOS:
- use threads with locks, atomic and mutex; (maybe not needed or worse)
- optimize code and use the right data structures;

# THINKING:
- The developers could make their draw functions instead of the classes drawing, because then they could use the same classes but with different styles whether they want. All that the classes could make is giving specific drawing functions for stuff that requires a complexity level or private/protected-access.

- everything related to drawing is handled by the dev, so everyone could make the style they want their game to be!!

# IMPORTANT:
- make things minimalist, if code is too complex, find a better(and if possible optimized) way

# IMPROVE .meatdata TYPE:
- let it have variables to identify colors and transparency stuff in the code or just variables, and also let have comments with # or something idk;
- create documentation for everything that can be modified and which name should they have in .meatdata
- maybe permit a console command to load .meatdata files and even examine them

# FUTURE NEXT-STEPS:
- Input handling with binds by HayBCMD
- multiplayer Stuff
- make 3D easier in raylib
- 3D map maker and loader
- take source engine movement(Bhop, ABH, Strafe, etc)
- good Collision Detection(AABB? Search more about it)
- NPC base class