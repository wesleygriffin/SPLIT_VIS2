
MACHINE= $(shell uname -s)

LUDIR = ../libs/LU
JCUTILDIR = ../libs/jcUtil
GLUIDIR = ../libs/glui-2.35
ANNDIR = ../libs/ann_1.1.1
KMEANDIR = ../libs/kmlocal-1.7.2
QVISLIBDIR = ../libs/quanVisLib

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -I/user/X11R6/include -I$(LUDIR) -I$(JCUTILDIR) -I$(QVISLIBDIR) -I$(GLUIDIR)/src/include -I$(ANNDIR)/include
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa  
	LLDLIBS = -L../libs -lsvl-1.0 -lglui -lLU -ljcUtil-1.0  -lm
else
	OPENGL_INC= -I/usr/X11R6/include -I/usr/include -I$(ANNDIR)/include -I$(JCUTILDIR) -I$(GLUIDIR)/src/include -I$(QVISLIBDIR)
	OPENGL_LIB= -L/usr/lib64 -L$(ANNDIR)/lib  -L../libs
	LLDLIBS = -lglui  -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lANN -lm
endif

CXX=g++

COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS)

TARGETS = qdotVis

SRCS = qdotFile.cpp qdotVis.cpp qdotGUI.cpp qdotFunction.cpp
OBJECTS = qdotFile.o qdotVis.o qdotGUI.o qdotFunction.o


default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

install: qdotVis
	cp qdotVis ./exe

.cpp.o:
	$(CXX) -c -Wno-deprecated  $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

ifeq ($(MACHINE),Darwin)
qdotVis: qdotFile.o qdotVis.o qdotGUI.o qdotFunction.o
	$(CXX) $(COMPILER_FLAGS) $(LIBS) qdotFile.o qdotVis.o qdotGUI.o qdotFunction.o -o qdotVis -lglui -lsvl-1.0 -lLU -ljcUtil-1.0 -lANN -lm
else
qdotVis: qdotFile.o qdotVis.o qdotGUI.o qdotFunction.o
	$(CXX) $(COMPILER_FLAGS) $(LIBS) qdotFile.o qdotVis.o qdotGUI.o qdotFunction.o -o qdotVis -lglui -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lANN -lm
endif

clean:
	rm -f $(OBJS) $(TARGETS) *~ \.*.swo \.*.swp

#rm ./exe/qdot*
