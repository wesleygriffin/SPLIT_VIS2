
MACHINE= $(shell uname -s)

LUDIR = ../libs/LU
JCUTILDIR = ../libs/jcUtil
GLUIDIR = ../libs/glui-2.35
ANNDIR = ../libs/ann_1.1.1
KMEANDIR = ../libs/kmlocal-1.7.2
QVISLIBDIR = ../libs/quanVisLib
PEELINGDIR = ../libs/dual_depth_peeling/common/

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -I/user/X11R6/include -I$(LUDIR) -I$(JCUTILDIR) -I$(QVISLIBDIR) -I$(GLUIDIR)/src/include -I$(ANNDIR)/include
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa  
	LLDLIBS = -L../libs -lsvl-1.0 -lglui -lLU -ljcUtil-1.0  -lm
else
	OPENGL_INC= -I/usr/X11R6/include -I/usr/include -I$(ANNDIR)/include -I$(JCUTILDIR) -I$(GLUIDIR)/src/include -I$(QVISLIBDIR) -I$(PEELINGDIR)/GLEW/include -I$(PEELINGDIR)/include -I$(PEELINGDIR)/nvModel/include
	OPENGL_LIB= -L/local/HEV/external/cuda/cuda-6.5/samples/common/lib/linux/x86_64/ -L/usr/lib64 -L$(ANNDIR)/lib  -L../libs
	LLDLIBS = -lGLEW   -lsvl-1.0 -lLU -ljcUtil-1.0 -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm
endif

CXX=g++ -O3 -fopenmp

COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS)

TARGETS = splitToolkit
#TARGETS =ccStudy
#SRCS = qdotFile.cpp qdotVis.cpp qdotGUI.cpp qdotFunction.cpp
#OBJECTS = qdotFile.o qdotVis.o qdotGUI.o qdotFunction.o

SRCS=splitToolkit.cpp 
OBJECTS=splitToolkit.o 

#SRCS= ccStudy.cpp
#OBJECTS=ccStudy.o


default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c -Wno-deprecated  $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

splitToolkit: splitToolkit.o
	$(CXX) $(COMPILER_FLAGS) $(LIBS) splitToolkit.o -o splitToolkit -lglui -lsvl-1.0 -lLU -ljcUtil-1.0 -lm
#ccStudy: ccStudy.o
#	 $(CXX) $(COMPILER_FLAGS) $(LIBS) ccStudy.o -o ccStudy -lGLEW  -lsvl-1.0 -lLU -ljcUtil-1.0 -lm
#splitToolkitmono: splitToolkitmono.o
#	$(CXX) $(COMPILER_FLAGS) $(LIBS) splitToolkitmono.o -o splitToolkitmono -lglui -lsvl-1.0 -lLU -ljcUtil-1.0  -lm

clean:
	rm -f $(OBJS) $(TARGETS) *~ \.*.swo \.*.swp

#rm ./exe/qdot*

