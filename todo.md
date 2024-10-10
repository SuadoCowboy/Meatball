# TODOS
- replace "Defaults" namespace after succesfully creating a working ui system

- sometimes console's outputbox does not appear after running the executable. Find out why and fix it!

- idk what to do with this Theme thing I just implemented. I still need to think about how I can make a 100% user-made UI. This means deleting ConsoleUI class and leaving only the needed functions that user can't define

- after making ui system, console ui will not be needed to be found anymore. For commands that are specific for it, like 'toggle_local_console', will search it by its name in the beginning of the program. It should begin as a unordered_map (std::string, UIObject) and later replaced to a vector (UIObject). THIS VECTOR IS JUST LOADED DATA! IT DOESN'T MEAN IT SHOULD BE UPDATED EVERY FRAME AND IT DOESN'T MEAN THE DATA IS BEING USED! ALSO, DO NOT WASTE MEMORY LOADING UI THAT IS NOT BEING USED, ONLY IF IT'S HEAVY TO PROCESS LATER.

- make custom folder work for every current and future projects so that if a file with a recognized name is placed in a folder called "custom", it will replace the default data with that custom data without modifying the original default file

- create tasks system, the console commands will run through tasks and then the players will be able to use the wait command(also add wait_ticks which waits ticks instead of milliseconds)

# FUTURE TODOS:
- use threads with locks, atomic and mutex (maybe not needed or worse)
- make something like the ddnet engine, but without their mixed naming conventions and unecessary pointers
- optimize code and use the right data structures

- this should become a full ui engine that comes with the console. For now use raylib but when the time comes, IF raylib becomes not so useful or slower for the whole engine(including non-ui stuff), do OpenGL/DirectX/Metal stuff. Then we would make a 2d engine with OpenGL first, we'll see DirectX and Metal in the future if needed. After this, the UI System would use this 2d engine. Then we'd create a network framework or engine or idk, everything is called a engine in this project??? lol... Then we'd create a final 2d game and update everything to work well. After this we'd go to 3d! Which would probably become my main focus! It would be like a **base** 3d engine, that would be used to make other engines for specific genres or even a whole game like ddnet has his own engine.

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