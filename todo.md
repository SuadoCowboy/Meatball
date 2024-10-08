# TODOS
- Change DynamicPanel onMouseMove to use mouse delta instead of offset variable(if possible and better)

- "Defaults" namespace does not make sense with its current functions. Maybe it should be deleted but because console ui will already change in the (hopefully)near future, there is no need to update this for now

- Idk what to do with this Theme thing I just implemented. I still need to think about how I can make a 100% user-made UI, this means deleting ConsoleUI class and leaving only the needed functions that user can't define

- I could try making ConsoleUI working without it being a pointer but it's going to be deleted anyways in the future so it doesn't need to change at the moment

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