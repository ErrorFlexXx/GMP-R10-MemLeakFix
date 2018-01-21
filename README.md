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

The GMP-R10 multiplayer is using code of gothic to create zString objects to call functions of gothic with them.
Sadly the interface inside the GMP, that represents a zString object, has no destructor defined.
Therefore resources are allocated from the zString constructor and pointers to this resources are placed inside the object, but if the objects get destroyed, nobody frees the resources inside the object.

This project hooks the functions, that are calling the zString constructors and does the constructor call itself. Additionally the whole zString object is saved and registered in an own garbage collector, that calls the zString destructor for added objects periodically.
Because of all objects are created on the stack from the gmp, this is a possible solution to destroy the objects. Otherwise it would free resources, that aren't out of scope and are currently used.
