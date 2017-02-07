#!make
# -----------------------------------------------------------------------
#  makefile for vector / matrix lib 
#  Jian Chen (jichen@umbc.edu)
#
#  Tested on mac (Nov 27 2013)
# ----------------------------------------------------------------------
#  compiler options
#  ---------------------------------------------------------------------
#

MACHINE=$(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC=-FOpenGL
	OPENGL_LIB=-framework OpenGL -framework GLUT -framework Cocoa
else
	OPENGL_INC=-I/use/include -I/usr/X11R6/include -I/usr/local/include 
	OPENGL_LIB=-L/usr/lib64 -L/usr/local/lib 
endif

LLDLIBS= -lGL -lGLU -lglut -lXmu -lXt -lXext -lX11 -lm -lstdc++

CC = g++ -DOPENGL -Wno-deprecated -g 

INCLUDE=$(OPENGL_INC)
LIBS=$(OPENGL_LIB)  $(LLDLIBS)

# ----------------------------------------------------------------------
# the files for cjSveWidget library 
#  ---------------------------------------------------------------------
SOURCES = lubksb.cpp ludcmp.cpp nrutil.cpp
TARGET = ./libLU.a 

# ----------------------------------------------------------------------
# Put all the source together 
#  ---------------------------------------------------------------------
OBJECTS	= $(SOURCES:.cpp=.o)

# ----------------------------------------------------------------------
#  now build the  library
#  ---------------------------------------------------------------------
all: $(TARGET)

lubksb.o: lubksb.cpp
	$(CC) $(INCLUDE) -c lubksb.cpp
ludcmp.o: ludcmp.cpp
	$(CC) $(INCLUDE) -c ludcmp.cpp
nrutil.o: nrutil.cpp
	$(CC) $(INCLUDE) -c nrutil.cpp

$(TARGET): lubksb.o  ludcmp.o nrutil.o
	ar -ruv libLU.a *.o

install: $(TARGET)
	cp ./libLU.a ../

clean:
	rm -f $(TARGET) .*.swp *.o *~ core
