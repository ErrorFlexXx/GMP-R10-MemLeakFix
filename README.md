# GMP R10 MemLeakFix



This project is about fixing memory leaks inside the GMP R-10 Client Software.



## Installation (Manual approach)


Simply download this project and copy the file **Release/dinput.dll** into the directory: **Gothic II/System**.


The modification will not break the singleplayer, but might require to have the *Microsoft Visual C++ 2010 Redistributable Package (x86)* installed (https://www.microsoft.com/de-de/download/details.aspx?id=5555).



## Installation (Spine)


Hopefully coming soon. If it's distributed with spine:


- Just tick the box GMP-R10-MemLeakFix box on the page of the started GMP-R10 Modification.



## Known issues


Currently none. If you experience some problems, please create an issue in this project.

## (Some) Technical background

The GMP-R10 multiplayer is using codeof Gothic to create zString objects and feed them into the Gothic Engine.

Sadly the interface inside the GMP to call constructors and functions has no destructor defined for a zString.

Therefore resources are allocated and pointers to this resources are placed from the original constructors of gothic, but if those zStrings get destroyed, when they went out of scope, no one will free the allocated resources.

This project hooks the function thats calling the zString constructor and does the creation itself. Additionally the whole zString object is saved and registered in an own garbage collector, that calls the zString destructor for added objects periodically.

This method works, because there are no heap objects created from the gmp. Otherwise this fix would free resources, that are used later.