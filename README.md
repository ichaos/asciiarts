asciiarts
=========

convert image to ascii arts

using CImg image library

Make
clang++ -o asciiarts asciiarts.cpp -I.. -Wall -W -ansi -pedantic -Dcimg_use_vt100 -I/usr/X11R6/include -lm -L/usr/X11R6/lib -lpthread -lX11

Run
./asciiarts -i ./img/logo.bmp
