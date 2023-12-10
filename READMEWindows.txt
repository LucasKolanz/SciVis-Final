Windows Readme

To run this program you first need Qt and cmake installed

Build:
1) Open Powershell and go to the "${path to program}SciVis-Final\build" directory.

2) If you want to run this program with Visual Studio, build with "cmake -G "Visual Studio 17 2022" ..", replacing the Visual Studio version as needed.
	There should now be a SciVis.sln file in the build directory

3) Open the SciVis.sln file with Visual Studio
	Make sure you have the Qt VS Tools extension downloaded on your Visual Studio

4) Make sure the build is set to "Release" and "x64" and hit the "Start Without Debugging" build option.
	If you get the message "Unable to start program . . . The system cannot find the file specified" then just get out
	of Visual Studio and navigate to the folder the executable is in. It should be in "${path to program}SciVis-Final\build\Release

5) Now you should have the scivis.exe file and be able to run it, hopefully.