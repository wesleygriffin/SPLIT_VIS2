.PHONY: all

CXX=g++

all: obj dual_depth_peeling

CFLAGS += -O3 -Wall -DNDEBUG -Winvalid-pch -m64 -fopenmp -msse4.2

CPP_FILES=$(wildcard *.cpp)
OBJ_FILES=$(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))


NVIDIA_COMMON_INC=-I./common/GLEW/include -I./common/include/ -I./common/nvModel/include

OPENGL_LIB= -L/usr/lib64
GLUT_LIB= -L../glew-2.0.0/lib -lGLEW -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm

LIB+=$(OPENGL_LIB) $(GLUT_LIB)
INC+=$(NVIDIA_COMMON_INC)

dual_depth_peeling: $(OBJ_FILES)
	${CXX} $(CFLAGS) -o $@ $(OBJ_FILES) $(LIB)

obj:
	mkdir -p obj

obj/%.o: %.cpp %.h
	${CXX} $(CFLAGS) -o $@ -c $< $(INC) 

obj/%.o: %.cpp
	${CXX} $(CFLAGS) -o $@ -c $< $(INC)

clean:
	rm -f $(OBJ_FILES)
	rm -f dual_depth_peeling
