# WARNING
- because sizeof(Color) is below 16(which is shared_ptr size), it's better cloning it or using a reference than actually using a pointer or shared_ptr.

# TODOS
- remove meatdata from project and use json instead

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