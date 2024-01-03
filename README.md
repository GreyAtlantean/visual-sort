# visual-sort
## Basic Sort Visualiser 


## A basic sorting visualiser written in C with SDL2.

### Controls for the program

P - plays and pauses the program

R - resets array with a new set of random numbers

T - resets the frame counter

S - steps through the algorithm one step at a time

D - toggles having a delay at the end of each frame

[ and ] - decreases/increases the number of steps taken each frame


### Compilation

This project is built using CMake.

Ensure SDL2 and SDL_TTF are both installed on your system.



Whilst in the build folder run cmake ../src

Run with ./visual_sort


To change number of elements in each array, change the value in the defs.h file

### Screenshots

![Unsorted Array](/imgs/unsorted150.png?raw=true "Unsorted 150 elements")

![Partially sorted Array](/imgs/partialsort150.png?raw=true "Partially Sorted 150 elements")

![Sorted Array](/imgs/sorted150.png?raw=true "Sorted 150 elements")


![Unsorted Array](/imgs/Unsorted.png?raw=true "Unsorted")

![Partially Sorted Array](/imgs/partialsort.png?raw=true "Partially sorted")

![Sorted Array](/imgs/sorted.png?raw=true "Sorted")