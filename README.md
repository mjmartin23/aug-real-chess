# Augmented Reality Chess
Built with C++, OpenCV and OpenGL.

Advised by Bruce Maxwell.

## Running
You'll need to print out the board on a piece of paper from data/board_small.png, and run calibration on your webcam using aruco's calibration to generate the YML file.
```
$ cd src
$ make game
$ ../bin/game live ../data/config_small.yml ../data/logitech_calibration.yml .0425
```
