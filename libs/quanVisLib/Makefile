#!smake
#  @@ Makefile for flow vis lib
#  @@ Jian Chen (jichen@umbc.edu)
#

MACHINE= $(shell uname -s)

VTKPath = /usr/include/vtk

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -I/usr/X11R6/include -I../LU -I../jcUtil
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa 
	LLDLIBS = -L../jcUtil -L../LU -lLU -ljcUtil-1.0  -lm 
else
	OPENGL_INC= -I/usr/include -I../LU -I../jcUtil -I../dual_depth_peeling/common/GLEW/include -I../dual_depth_peeling/common/include -I../dual_depth_peeling/common/nvModel/include
	OPENGL_LIB= -L/usr/lib64 
	LLDLIBS = -L/local/HEV/external/cuda/cuda-6.5/samples/common/lib/linux/x86_64/ -L../jcUtil -L../LU -lLU -ljcUtil-1.0 -lGLEW -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm 
endif

ifeq ($(wildcard $(VTKPath)), $(VTKPath))
	CXX = g++ -D_VTK -O3 -Wall -DNDEBUG -Winvalid-pch -m64 -fopenmp -msse4.2
else
	CXX=g++  -O3 -Wall -DNDEBUG -Winvalid-pch -m64 -fopenmp -msse4.2
endif
#CXX=g++ 

COMPILER_FLAGS= -g 

INCPATH= $(OPENGL_INC)
INCLUDE=$(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS) 

TARGETS = libsvl-1.0.a

#svrbf.cpp 
#svROIbox.cpp\
#svROI.cpp\
#svCut.cpp\
#svQDOTNorm.cpp 

SRCS =svUtil.cpp \
svLut.cpp \
svParticle.cpp \
ivview.cpp \
ivTrackball.cpp \
svOutline.cpp\
svDistance.cpp\
svArrowGlyph.cpp\
svArrowSelection.cpp\
svSlice.cpp\
svQDOTdistancemap.cpp\
svQDOTmagnitudemap.cpp\
CIEColor.cpp\
svColors.cpp\
svMap.cpp\
svDirectArrow.cpp \
svVectorField.cpp \
svQDOT.cpp\
svPickingray.cpp\
svDisc.cpp\
svAnalyticField.cpp \
svPrimitive.cpp\
svGlyph.cpp\
svSummaryGlyph.cpp\
svCylinderGlyph.cpp\
svDirectCylinder.cpp\
svSplitVectorsCylinder.cpp\
svBarGlyph.cpp\
svBendBar.cpp\
svSplitVectorsBar.cpp\
svDirectBar.cpp\
GLSLProgramObject.cpp\
dual_depth_peeling.cpp

#svQDOTField.cpp \
#svMesh.cpp \
#svrbf.o 
#svROIbox.o\
#svROI.o\
#svCut.o\
#svQDOTNorm.o \

OBJECTS = svUtil.o \
svLut.o \
svParticle.o \
ivview.o \
ivTrackball.o \
svOutline.o\
svDistance.o\
svArrowGlyph.o\
svArrowSelection.o\
svSlice.o\
svQDOTdistancemap.o\
svQDOTmagnitudemap.o\
CIEColor.o\
svColors.o\
svMap.o\
svDirectArrow.o \
svVectorField.o \
svQDOT.o\
svPickingray.o\
svDisc.o\
svAnalyticField.o \
svPrimitive.o\
svGlyph.o\
svSummaryGlyph.o\
svCylinderGlyph.o\
svDirectCylinder.o\
svSplitVectorsCylinder.o\
svBarGlyph.o\
svBendBar.o\
svSplitVectorsBar.o\
svDirectBar.o\
GLSLProgramObject.o\
dual_depth_peeling.o

#svQDOTField.o \
#svMesh.o

default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c $(COMPILER_FLAGS) $(INCLUDE) -o $@ $< 

$(TARGETS): $(OBJECTS)
	ar -ruv libsvl-1.0.a *.o

install: $(TARGETS)
	cp ./libsvl-1.0.a ../


svPrimitive.o: svPrimitive.cpp svPrimitive.h \
                svVectorField.h \
                svType.h \
                svVectorMatrix.h \
                svParticle.h \
                svArray.h \
                svLut.h \
                svException.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svPrimitive.o svPrimitive.cpp


svAnalyticField.o: svAnalyticField.cpp svAnalyticField.h \
                svType.h \
                svVectorMatrix.h \
                svParticle.h \
                svVectorField.h \
		svPrimitive.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svAnalyticField.o svAnalyticField.cpp

svVectorField.o: svVectorField.cpp svException.h  svVectorField.h  svType.h svVectorMatrix.h svParticle.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svVectorField.o svVectorField.cpp

svUtil.o: svUtil.cpp svUtil.h svVectorField.h svType.h svVectorMatrix.h svParticle.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svUtil.o svUtil.cpp

svParticle.o: svParticle.cpp svParticle.h svType.h svVectorMatrix.h \
svVectorField.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svParticle.o svParticle.cpp

svLut.o: svLut.cpp svLut.h svType.h svVectorMatrix.h svUtil.h \
svVectorField.h svParticle.h svArray.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svLut.o svLut.cpp

ivview.o: ivview.cpp ivview.h svType.h svVectorMatrix.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ivview.o ivview.cpp

ivTrackball.o: ivTrackball.cpp ivTrackball.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ivTrackball.o ivTrackball.cpp

#svQDOTNorm.o: svQDOTNorm.cpp svQDOTNorm.h svType.h \
svVectorMatrix.h svArray.h svParticle.h svVectorField.h svException.h svUtil.h
	#$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svQDOTNorm.o svQDOTNorm.cpp
svQDOT.o: svQDOT.cpp svQDOT.h svType.h \
svVectorMatrix.h svArray.h svParticle.h svVectorField.h svException.h svUtil.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svQDOT.o svQDOT.cpp
#svCut.o: svCut.cpp svCut.h svType.h svVectorMatrix.h svArray.h svVectorField.h 
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svCut.o svCut.cpp

svOutline.o: svOutline.cpp svOutline.h svType.h svVectorMatrix.h svArray.h svVectorField.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svOutline.o svOutline.cpp
svDistance.o: svDistance.cpp svDistance.h svType.h svVectorMatrix.h svArray.h svVectorField.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svDistance.o svDistance.cpp

svArrowGlyph.o: svArrowGlyph.cpp svArrowGlyph.h svArray.h svVectorMatrix.h \
svType.h svParticle.h svVectorField.h svLut.h svException.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svArrowGlyph.o svArrowGlyph.cpp
svArrowSelection.o: svArrowSelection.cpp svArrowSelection.h svArray.h svVectorMatrix.h \
svType.h svParticle.h svVectorField.h svLut.h svException.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svArrowSelection.o svArrowSelection.cpp 
svSlice.o: svSlice.cpp svSlice.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svSlice.o svSlice.cpp
#svROIbox.o: svROIbox.cpp svROIbox.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svROIbox.o svROIbox.cpp
#svSmallROIbox.o: svSmallROIbox.cpp svSmallROIbox.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
#$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svSmallROIbox.o svSmallROIbox.cpp
#svROI.o: svROI.cpp svROI.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	#$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svROI.o svROI.cpp
svQDOTdistancemap.o: svQDOTdistancemap.cpp svQDOTdistancemap.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svQDOTdistancemap.o svQDOTdistancemap.cpp
svQDOTmagnitudemap.o: svQDOTmagnitudemap.cpp svQDOTmagnitudemap.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svQDOTmagnitudemap.o svQDOTmagnitudemap.cpp
svMap.o: svMap.cpp svMap.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svMap.o svMap.cpp
svGlyph.o: svGlyph.cpp svGlyph.h dual_depth_peeling.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svGlyph.o svGlyph.cpp
svCylinderGlyph.o: svCylinderGlyph.cpp svCylinderGlyph.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svCylinderGlyph.o svCylinderGlyph.cpp
svDirectCylinder.o: svDirectCylinder.cpp svDirectCylinder.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svDirectCylinder.o svDirectCylinder.cpp
svSplitVectorsCylinder.o: svSplitVectorsCylinder.cpp svSplitVectorsCylinder.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svSplitVectorsCylinder.o svSplitVectorsCylinder.cpp
svBarGlyph.o: svBarGlyph.cpp svBarGlyph.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svBarGlyph.o svBarGlyph.cpp
svBendBar.o: svBendBar.cpp svBendBar.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svBendBar.o svBendBar.cpp
svDirectBar.o: svDirectBar.cpp svDirectBar.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svDirectBar.o svDirectBar.cpp
svSplitVectorsBar.o: svSplitVectorsBar.cpp svSplitVectorsBar.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svSplitVectorsBar.o svSplitVectorsBar.cpp
svDirectArrow.o: svDirectArrow.cpp svDirectArrow.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svDirectArrow.o svDirectArrow.cpp
svSummarGlyph.o: svSummarGlyph.cpp svSummarGlyph.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svSummarGlyph.o svSummarGlyph.cpp
svPickingray.o: svPickingray.cpp svPickingray.h svType.h svVectorMatrix.h svArray.h svVectorField.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svPickingray.o svPickingray.cpp
CIEColor.o: CIEColor.cpp CIEColor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CIEColor.o CIEColor.cpp

svColors.o: svColors.cpp svColors.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svColors.o svColors.cpp
svDisc.o: svDisc.cpp svDisc.h svType.h svVectorMatrix.h svArray.h svVectorField.h svUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svDisc.o svDisc.cpp
GLSLProgramObject.o: GLSLProgramObject.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GLSLProgramObject.o GLSLProgramObject.cpp
dual_depth_peeling.o: dual_depth_peeling.cpp nvModel.h GLSLProgramObject.h Timer.h OSD.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dual_depth_peeling.o dual_depth_peeling.cpp
#svMesh.o: svMesh.cpp svMesh.h svType.h svVectorMatrix.h svArray.h svVectorField.h
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svMesh.o svMesh.cpp
clean:
	rm -f $(OBJS) $(TARGETS) *~
