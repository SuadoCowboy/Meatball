# WARNING
- because sizeof(Color) is below 16(which is shared_ptr size), it's better cloning it or using a reference than actually using a pointer or shared_ptr.

# TODOS

- if config is below 16 bytes use copy instead of shared_ptr(by that i mean to check each class that uses a config and modify the ones that need to, and if it's just a single variable, then it doesn't need a config struct)

- create a compiler for meatdata files like .vpk(from source engine) so that it's easier for machine to read. BUT Users might be able to just use their customs without needing to compile(even though it's recommended)

# User Interface with Lua
- Use lua language to create UI. Should also make a way so that users can test their interfaces without joining the game everytime.
- Also set a macro to define its actual version because it could change alot in the future

- It would also require to implement the events related to specific classes for example DynamicPanel events like onMove, onResize, onResizeStop, and other classes that might have events and stuff. My idea is: make it in lua :D, It's just the UI in lua and later will be used in C++ so it will be fine I guess.

- ConsoleUI: Try implementing a optional draw lua function so that the creator of the UI can draw the way he wants to. (this way they could also make stupid things, which is fun). If this is implemented, it would be nice to give lua variables the delta time and stuff related to game.

- Example: for Console, the user have some options: mainPanel, closeButton, sendButton(optional), inputBox and OutputTextBox. They also can modify events that the Console allowed, like: mainPanel::onMove, mainPanel::onResize and mainPanel::onResizeStop. The rest of the events are handled by the ConsoleUI code. closeButton::onRelease can not be changed because by default it should set ConsoleUI.visible to false. sendButton is the same thing, it should always send to the output and clear the input text. It would be something like this in lua:
```lua
-- THIS IS STILL ON THINKING, THINGS MIGHT NOT BE LIKE THIS ON RELEASE OF THE FIRST VERSION

-- params: x y width height style color hoveredColor
closeButton = createButton(x, y, width, height, STYLES.STYLE_X, COLORS.LIGHTGRAY, COLORS.WHITE)

-- params: x y width height borderRadius color
mainPanel = createDynamicPanel(x, y, width, height, 0, {10,10,10,255})

-- Optional stuff: we don't declare
-- params: x, y, width, height, text
--sendButton = createTextButton(x, y, width, height, "Send")

-- params: x y width height defaultText color
inputBox = createInputTextBox(x, y, width, height, "default text written in input box", {20,20,20,255})

-- params: x y width height color
outputBox = createScrollTextBox(x, y, width, height, {20, 20, 20, 255})

--[[ TODOS:
if game does not recognize objectName: tell user
if type is not allowed for objectName: tell user and stop running
if a required objectName is not defined in the end: tell user and stop running
]]--
```

# FUTURE TODOS:
- optimize code and use the right data structures;
- use threads with locks, atomic and mutex;
- every data structure that is needed in alot of places should be shared_ptr;
- update function descriptions and anything else related to reading;

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
- A* path finding for AI