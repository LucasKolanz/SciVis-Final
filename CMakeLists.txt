cmake_minimum_required(VERSION 3.14)
project(SciVis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC")

set(ProjFolder "SciVis/")
if(WIN32)
	set(QT_PATH "C:/Qt/6.6.1/msvc2019_64")#NEED PATH TO THIS QT FOLDER FOR Windows
	set(QT_BIN_DIR "C:/Qt/6.6.1/msvc2019_64/bin")#NEED PATH TO THIS QT FOLDER FOR Windows
	set(Qt6_DIR "${QT_PATH}/lib/cmake/Qt6") # #PATH TO QT6Config.cmake
	set(Qt6CoreTools_DIR "C:/Qt/6.6.1/msvc2019_64/lib/cmake/Qt6CoreTools")# #PATH TO QT6CoreToolsConfig
	set(Qt6GuiTools_DIR "C:/Qt/6.6.1/msvc2019_64/lib/cmake/Qt6GuiTools")# #PATH TO QT6GuiToolsConfig
	
	link_directories("${CMAKE_BINARY_DIR}/../SciVis/libraries/glew-2.1.0/lib/Release/x64")
	link_directories("${CMAKE_BINARY_DIR}/../SciVis/libraries/freeglut-3.0.0/lib/x64")
	message("freeglut library path: ${CMAKE_BINARY_DIR}/../SciVis/libraries/freeglut-3.0.0/lib/Release/x64")

elseif(UNIX AND NOT APPLE)
	set(QT_PATH "/home/lucas/Qt/6.6.0/gcc_64") #NEED PATH TO THIS QT FOLDER FOR LINUX
endif()

# Find the necessary Qt package
find_package(Qt6 COMPONENTS Widgets Gui OpenGLWidgets REQUIRED)

set(SOURCES ${ProjFolder}main.cpp ${ProjFolder}ply.cpp ${ProjFolder}polyhedron.cpp ${ProjFolder}Polyline.cpp ${ProjFolder}Project.cpp ${ProjFolder}SciVis.cpp ${ProjFolder}tmatrix.cpp ${ProjFolder}trackball.cpp ${ProjFolder}GLWidget.cpp)
set(HEADERS ${ProjFolder}SciVis.h ${ProjFolder}GLWidget.h)
set(UI_FILES ${ProjFolder}SciVis.ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)    
	add_executable(scivis ${SOURCES})
	target_include_directories(scivis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${ProjFolder})
    	target_link_libraries(scivis Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets opengl32 glew32) # Add other libraries as needed
	add_custom_command(TARGET scivis POST_BUILD
    		COMMAND "${QT_BIN_DIR}/windeployqt" --dir $<TARGET_FILE_DIR:scivis> $<TARGET_FILE:scivis>
    		COMMAND ${CMAKE_COMMAND} -E copy_if_different
    		"${CMAKE_BINARY_DIR}/../SciVis/glew32.dll"
    		$<TARGET_FILE_DIR:scivis>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
    		"${CMAKE_BINARY_DIR}/../SciVis/freeglut.dll"
    		$<TARGET_FILE_DIR:scivis>)
elseif(UNIX AND NOT APPLE)
	add_executable(scivis.x ${SOURCES})
	target_include_directories(scivis.x PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${ProjFolder})
    	target_link_libraries(scivis.x Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets GL GLU GLEW) # Add other libraries as needed
endif()


