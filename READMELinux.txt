Linux Readme

To run this program you first need Qt and cmake installed

Build:
1) Open a terminal and go to the "${path to program}SciVis-Final/build" directory.
	If that isn't a directory make it with command "mkdir ${path to program}SciVis-Final/build", replacing "${path to program}" with whatever folder SciVis-Final is in. 

2) make the makefile with the command "cmake .."
	Now you should have a file called Makefile

3) now run the make file with the command "make"
	Now you should have the file scivis.x in your build directory

4) Make sure you copy the NEON .ply data file into the folder "${path to program}SciVis-Final/SciVis/data/NEON_data"

5) Run this new file with the command "./scivis.x"
