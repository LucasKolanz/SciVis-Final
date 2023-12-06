cmake_minimum_required(VERSION 3.14)
project(SciVis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC")

set(ProjFolder "SciVis/")
if(WIN32)
	set(CMAKE_PREFIX_PATH "C:/Qt/6.6.1/msvc2019_64")
elseif(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/home/lucas/Qt/6.6.0/gcc_64")
endif()

# Find the necessary Qt package
find_package(Qt6 COMPONENTS Widgets Gui OpenGLWidgets REQUIRED)

set(SOURCES ${ProjFolder}main.cpp ${ProjFolder}ply.cpp ${ProjFolder}polyhedron.cpp ${ProjFolder}Polyline.cpp ${ProjFolder}Project.cpp ${ProjFolder}SciVis.cpp ${ProjFolder}tmatrix.cpp ${ProjFolder}trackball.cpp ${ProjFolder}GLWidget.cpp)
set(HEADERS ${ProjFolder}SciVis.h ${ProjFolder}GLWidget.h)
set(UI_FILES ${ProjFolder}SciVis.ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(scivis.x ${SOURCES})

target_include_directories(scivis.x PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${ProjFolder})
target_link_libraries(scivis.x Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets GL GLU GLEW)
