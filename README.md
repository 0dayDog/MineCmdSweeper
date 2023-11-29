# MineCmdSweeper
:heart_eyes: A Mine Sweeper in cmd
### How to compile a cpp file and add icons
:smile: Find a ico file you like. Now we have the mine.ico.  
:smirk: You need to creat a .rc file.  
:satisfied: Edit your .rc file like this.  
```C++
IDI_ICON1 ICON DISCARDABLE "mine.ico"
```
:stuck_out_tongue_winking_eye: Good then you need to compile the .rc file to .res file.  
```C++
windres resources.rc -O coff -o resources.res
```
:kissing_closed_eyes: Nice then you can compile the cpp file.  
```C++
g++ MineCmdSweeper.cpp ico.res -o MineCmdSweeper.exe
```
:sleeping: Of course we give you the ps1 to run directly.  
:heart_eyes: Kissssssss and love your Star!
