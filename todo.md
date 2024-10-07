# TODOS
- Add mouse movement event and keep 'eventing' console update function
- How can KeyRepeat still work after making input as a event?
- Change DynamicPanel onMouseMove to use mouse delta instead of offset variable

- Why I created Console class if it's just SweatCI?

- Swap IsKeyPressed and IsKeyReleased around the project to use Meatball::EventHandler

- "Defaults" namespace does not make sense with its current functions. Maybe it should be deleted but because console ui will already change in the (hopefully)near future, there is no need to update this for now.

- Idk what to do with this Theme thing I just implemented. I still need to think about how I can make a 100% user-made UI, this means deleting ConsoleUI class and leaving only the needed functions that user can't define.

- UI Stuff: Instead of them being updated on every frame, it would be easier with functions that handles events like a mousepress function which calls the current scene stuff that requires mousepress, keypress/keyrelease, mousemove, etc

- config stuff can be removed and their members can be passed as parameters in the needed functions. This will make the code clearer to understand(probably). Maybe let default config stuff since it might still be useful for now

- Delete default config and make configs be json file that can be loaded in the code with a console function (or auto detect idk) and they can be set by names. If a default json is not set, then it will create a pretty default one

- because the config of any ui class by default is a pointer to the default config, how can consoleUI have different color schemes and stuff? should I make commands that create new configurations and passes the pointers to the ui object it wants to have the config or is that too much? Personally, I think that's a fun idea

- either make ConsoleUI static or allow a default constructor

- make custom folder work for every current and future projects so that if a file with a recognized name is placed in a folder called "custom", it will replace the default data with that custom data without modifying the original default file

- create another library for network stuff. Meatball can be used to test with it but it's not built-in in meatball unless necessary

- create tasks system, the console commands will run through tasks and then the players will be able to use the wait command(also add wait_ticks which waits ticks instead of milliseconds)

# FUTURE TODOS:
- use threads with locks, atomic and mutex (maybe not needed or worse)
- optimize code and use the right data structures

# IMPORTANT:
- make things minimalist, if code is too complex, find a better(and if possible optimized) way

# JSON FILE TYPE:
- create documentation for everything that can be modified and which name should they have
- maybe allow a console command to load those files and even "debug" them

# FUTURE NEXT-STEPS:
- multiplayer Stuff
- make 3D easier in raylib
- 2D/3D map editor(project) and loader(namespace inside the project)
- take source engine movement(Bhop, ABH, Strafe, etc)
- good Collision Detection(AABB? Search more about it)
- NPC base class(this is way too far right now)