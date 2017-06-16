
MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -I/user/X11R6/include 
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa  
	LLDLIBS = -lm
else
	OPENGL_INC= -I/usr/X11R6/include -I/usr/include -I./Ric
	OPENGL_LIB= -L/usr/lib64 
	LLDLIBS =  -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm
endif

CXX=g++

COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS)

TARGETS = ContourTree 

SRCS=$(wildcard *.cpp) 
OBJECTS= $(addprefix ./,$(notdir $(SRCS:.cpp=.o)))  

default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c -Wno-deprecated  $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

ContourTree: ${OBJECTS}
	$(CXX) $(COMPILER_FLAGS) $(LIBS) ${OBJECTS} -o ContourTree  -lm

clean:
	rm -f $(OBJS) $(TARGETS) *~ \.*.swo \.*.swp

#rm ./exe/qdot*

