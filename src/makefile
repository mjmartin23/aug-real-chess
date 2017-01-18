# OSX compiler
# CC = clang++

# Dwarf compiler
CC = g++ -std=c++11

CXX = $(CC)

# OSX include paths
#CFLAGS = -I/opt/local/include -I../include

# Dwarf include paths
CFLAGS = -I/usr/local/include/opencv -I/usr/local/include -I/usr/include -I../include # opencv includes are in /usr/include
CXXFLAGS = $(CFLAGS)

# OSX Library paths
#LDFLAGS = -L/opt/local/lib

# Dwarf Library paths
LDFLAGS = -L/usr/local/lib  # opencv libraries are here

# OSX libraries
#LDLIBS = -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs

# Dwarf libraries
CVLIBS = -lopencv_calib3d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_imgproc -lopencv_core 
ARUCOLIBS = -laruco
OPENGLLIBS = -lGL -lglut

LDLIBS = $(LDFLAGS) $(CVLIBS) $(ARUCOLIBS) $(OPENGLLIBS)
BINDIR = ../bin

vid: vidDisplay.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

imod: imgMod.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

board: Board.o Video.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

testing: aruco_testing.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

detect: aruco_detect.o board.o square.o objloader.o piece.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

game: game.o board.o square.o objloader.o piece.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

obj: objloader.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

expo: expodetector.o
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o *~ 
