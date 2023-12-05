cmake_minimum_required(VERSION 3.14)  # Set the minimum version of CMake

project(SciVis)  # Set the project name

set(CMAKE_CXX_STANDARD 11)  # Define the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC")  # Additional flags

# Find the necessary Qt packages
set(Qt6CoreTools_DIR "/home/lucas/Qt/6.6.0/gcc_64/lib/cmake/Qt6CoreTools")
set(Qt6Widgets_DIR "/home/lucas/Qt/6.6.0/gcc_64/lib/cmake/Qt6Widgets")
set(Qt6_DIR "/home/lucas/Qt/6.6.0/gcc_64/lib/cmake/Qt6")
set(Qt6WidgetsTools_DIR "/home/lucas/Qt/6.6.0/gcc_64/lib/cmake/Qt6WidgetsTools")
set(Qt6GuiTools_DIR "/home/lucas/Qt/6.6.0/gcc_64/lib/cmake/Qt6GuiTools")
# find_package(Qt6CoreTools REQUIRED HINTS "/home/lucas/Qt/6.6.0/gcc_64/lib/cmake/Qt6CoreTools")
find_package(Qt6 COMPONENTS Widgets Gui OpenGLWidgets REQUIRED)

# Set the source and header files
set(SOURCES SciVis/main.cpp SciVis/ply.cpp SciVis/polyhedron.cpp SciVis/Polyline.cpp SciVis/Project.cpp SciVis/SciVis.cpp SciVis/tmatrix.cpp SciVis/trackball.cpp SciVis/GLWidget.cpp)
set(HEADERS SciVis/SciVis.h SciVis/GLWidget.h)
set(UI_FILES SciVis/SciVis.ui)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# Use the `qt5_wrap_cpp` and `qt5_wrap_ui` macros for MOC and UI files
# qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
# qt6_wrap_ui(UI_HEADERS ${UI_FILES})

# Define the executable

add_executable(scivis.x ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS})

target_include_directories(scivis.x PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SciVis)
# Link the Qt libraries
target_link_libraries(scivis.x Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets GL GLU GLEW)

# Set additional properties if necessary
