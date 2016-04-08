onward
======
a game about stuff and things



changelog
---------

###2016-04
- 02: added HelloWorld log category for use when spawning new entities in.
- 03: upped framerate cap to 150. added GameInstance to hold things that should be both globally accessable and persistant. added a temporary WorldTime float to GameInstance. added GameInstance entry to DefaultEngine.ini as per https://wiki.unrealengine.com/Game_Instance,_Custom_Game_Instance_For_Inter-Level_Persistent_Data_Storage .
- 04: tried intellisense speedup via https://forums.unrealengine.com/showthread.php?93546-For-those-who-suffer-from-Visual-Studio-IntelliSense-slowness . added onwardWorldSettings class and plugged it in in editor. added WorldTime debug message category.
- 05: worked on timekeeper math.
- 06: did a bit more to the timekeeper.
- 07: added skeleton of FTimestamp class to store time-related data. changed a few variable and log category names for clarity. moved time period #defines to onward.h so they're available everywhere.
