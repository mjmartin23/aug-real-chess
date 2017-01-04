# OSX compiler
# CC = clang++

# Dwarf compiler
CC = g++

CXX = $(CC)

# OSX include paths
#CFLAGS = -I/opt/local/include -I../include

# Dwarf include paths
CFLAGS := $(shell pkg-config --cflags opencv) # opencv includes are in /usr/include
CXXFLAGS = $(CFLAGS)

# OSX Library paths
#LDFLAGS = -L/opt/local/lib

# Dwarf Library paths
# LDFLAGS = -L/usr/local/lib # opencv libraries are here

# OSX libraries
#LDLIBS = -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs

# Dwarf libraries
LDLIBS := $(shell pkg-config --libs opencv)

BINDIR = ../bin

vid: vidDisplay.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

imod: imgMod.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o *~ 
