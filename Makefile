CXX = cl
LINKER = link

INCLUDE_PATH = C:/Windows/opencv/build/include
LIB_PATH = .

CXXFLAGS = /c /I $(INCLUDE_PATH) /Zi /EHsc
LDFLAGS = /LIBPATH:$(LIB_PATH) opencv_world4110.lib

SOURCES = main.cpp photo.cpp
OBJECTS = main.obj photo.obj
OUTPUT = program.exe

all: $(OUTPUT)
	del /Q $(OBJECTS)

main.obj: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

photo.obj: photo.cpp
	$(CXX) $(CXXFLAGS) photo.cpp

$(OUTPUT): $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LDFLAGS) /out:$(OUTPUT)

clean:
	del /Q $(OBJECTS) $(OUTPUT)
