
CXX = g++
CXXFLAGS = -g -fPIC $(shell pkg-config --cflags Qt6Widgets Qt6Gui Qt6OpenGLWidgets)
LIBS = $(shell pkg-config --libs Qt6Widgets Qt6Gui Qt6OpenGLWidgets) -lGL -lGLU -lGLEW

# Source files
SOURCES = main.cpp ply.cpp polyhedron.cpp Polyline.cpp Project.cpp SciVis.cpp tmatrix.cpp trackball.cpp GLWidget.cpp 
#MarchingSquare.cpp

# UI files
UI_FILES = SciVis.ui

# Header files for MOC
MOC_HEADERS = SciVis.h GLWidget.h

# MOC-generated source files
MOC_SOURCES = $(MOC_HEADERS:.h=.moc.cpp)

COMBINED_SOURCES = $(SOURCES) $(MOC_SOURCES)


# Object files for both regular and MOC-generated sources
OBJECTS = $(COMBINED_SOURCES:.cpp=.o) 

# Path to MOC
MOC = /home/lucas/Qt/6.6.0/gcc_64/libexec/moc

# Executable name
EXEC = scivis.x

# Default target
all: $(EXEC)

# Linking the executable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJECTS) $(LIBS)

# Generic rule for compiling
%.o: %.cpp $(UI_HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Rule for generating UI headers
ui_SciVis.h: $(UI_FILES)
	/home/lucas/Qt/6.6.0/gcc_64/libexec/uic $< -o $@

# Rule for running MOC
%.moc.cpp: %.h
	$(MOC) $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(EXEC) $(MOC_SOURCES)


