cmake_minimum_required(VERSION 2.8.12)
project(SPLIT_VIS2)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

include(ExternalProject)
set(BUILD_SHARED_LIBS OFF) # force static libraries

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS "-m64 -msse4.2")
#set(CMAKE_C_FLAGS "-m64")
#set(CMAKE_EXE_LINKER_FLAGS "-m64 -msse4.2")
#set(CMAKE_MODULE_LINKER_FLAGS "-m64")
#set(CMAKE_SHARED_LINKER_FLAGS "-m64")
#set(CMAKE_REQUIRED_FLAGS "-msse4.2")
#set(WARNINGS "-Wall -Winvalid-pch")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}  ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#   include_directories(${GLEW_INCLUDE_DIRS})
#   link_libraries(${GLEW_LIBRARIES})
#endif()

#include_directories(${GLEW_INCLUDE_DIR})

set(genVTK_SOURCES
    preprocessing/generateVTK/genVTK.cpp
    preprocessing/generateVTK/genVTKfromOrg.cpp
    preprocessing/generateVTK/genVTKfromOrg.h)

add_executable(genVTK ${genVTK_SOURCES})
install(TARGETS genVTK RUNTIME DESTINATION bin)

set(Contour_SOURCES
    preprocessing/generatecontour/main.cpp
    preprocessing/generatecontour/svContour.cpp
    preprocessing/generatecontour/svContour.h)

add_executable(Contour ${Contour_SOURCES})
target_link_libraries(Contour ${VTK_LIBRARIES})
install(TARGETS Contour RUNTIME DESTINATION bin)

ExternalProject_Add(contourtree
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/ContourTree
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/ContourTree ${CMAKE_CURRENT_BINARY_DIR}/../bin/)

ExternalProject_Add(glui
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glui-2.35
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cd src && make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(glui SOURCE_DIR)
set(GLUI_LIBRARIES ${SOURCE_DIR}/src/lib/libglui.a CACHE STRING "" FORCE)
set(GLUI_INCLUDE_DIR ${SOURCE_DIR}/src/include CACHE STRING "" FORCE)

ExternalProject_Add(soil
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/soil
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cd projects/makefile && make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(soil SOURCE_DIR)
set(SOIL_LIBRARIES ${SOURCE_DIR}/lib/libSOIL.a  CACHE STRING "" FORCE)
set(SOIL_INCLUDE_DIR ${SOURCE_DIR}/src  CACHE STRING "" FORCE)

ExternalProject_Add(glew
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.0.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND  make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(glew SOURCE_DIR)
set(GLEW_LIBRARIES ${SOURCE_DIR}/lib/libGLEW.a  CACHE STRING "" FORCE)
set(GLEW_INCLUDE_DIR ${SOURCE_DIR}/include  CACHE STRING "" FORCE)

#set(GLEW_LIBRARIES /usr/local/HEV/external/cuda/cuda-6.5/samples/common/lib/linux/x86_64/libGLEW.a CACHE string "" FORCE)

ExternalProject_Add(jcUtil
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/jcUtil
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(jcUtil SOURCE_DIR)
set(JCUTIL_LIBRARIES ${SOURCE_DIR}/libjcUtil-1.0.a CACHE STRING "" FORCE)
set(JCUTIL_INCLUDE_DIR ${SOURCE_DIR} CACHE STRING "" FORCE)

ExternalProject_Add(LU
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/LU
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND "")
ExternalProject_Get_Property(LU SOURCE_DIR)
set(LU_LIBRARIES ${SOURCE_DIR}/libLU.a CACHE STRING "" FORCE)

set(PEELING_COMMON_INCLUDE_DIR
${CMAKE_CURRENT_SOURCE_DIR}/libs/dual_depth_peeling/common/include CACHE STRING "" FORCE)

set(PEELING_NVMODEL_INCLUDE_DIR
${CMAKE_CURRENT_SOURCE_DIR}/libs/dual_depth_peeling/common/nvModel/include CACHE STRING "" FORCE)

#set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/dual_depth_peeling/common/GLEW/include CACHE STRING "" FORCE)


#include_directories(${PEELING_COMMON_INCLUDE_DIR})
#include_directories(${PEELING_NVMODEL_INCLUDE_DIR})
#include_directories(${GLEW_INCLUDE_DIR})

ExternalProject_Add(kmlocal
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/kmlocal-1.7.2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/bin/kmlsample
        ${CMAKE_CURRENT_BINARY_DIR}/../bin/)

ExternalProject_Add(ex_alpha_shape_3
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/Alpha_shapes_3/
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cmake . && make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>ex_alpha_shapes_3
${CMAKE_CURRENT_BINARY_DIR}/../bin/)
ExternalProject_Get_Property(ex_alpha_shape_3  SOURCE_DIR)


set(svl_SOURCES
    libs/quanVisLib/svUtil.cpp
    libs/quanVisLib/svLut.cpp
    libs/quanVisLib/svParticle.cpp
    libs/quanVisLib/ivview.cpp
    libs/quanVisLib/ivTrackball.cpp
    libs/quanVisLib/svOutline.cpp
    libs/quanVisLib/svDistance.cpp
    libs/quanVisLib/svArrowGlyph.cpp
    libs/quanVisLib/svSlice.cpp
    libs/quanVisLib/svQDOTdistancemap.cpp
    libs/quanVisLib/svQDOTmagnitudemap.cpp
    libs/quanVisLib/CIEColor.cpp
    libs/quanVisLib/svColors.cpp
    libs/quanVisLib/svMap.cpp
    libs/quanVisLib/svDirectArrow.cpp
    libs/quanVisLib/svMesh.cpp
    libs/quanVisLib/svSplitArrow.cpp
    libs/quanVisLib/svVectorField.cpp
    libs/quanVisLib/svQDOT.cpp
    libs/quanVisLib/svPickingray.cpp
    libs/quanVisLib/svDisc.cpp
    libs/quanVisLib/svAnalyticField.cpp
    libs/quanVisLib/svPrimitive.cpp
    libs/quanVisLib/svGlyph.cpp
    libs/quanVisLib/svSummaryGlyph.cpp
    libs/quanVisLib/svBarGlyph.cpp
    libs/quanVisLib/svBendBar.cpp
    libs/quanVisLib/svSplitVectorsBar.cpp
    libs/quanVisLib/svDirectBar.cpp
    libs/quanVisLib/GLSLProgramObject.cpp
    libs/quanVisLib/dual_depth_peeling.cpp
    libs/quanVisLib/svList.cpp
    libs/quanVisLib/svBST.cpp
    libs/quanVisLib/svCCL.cpp
    libs/quanVisLib/svMagnitude.cpp
    libs/quanVisLib/svWidget.cpp
    libs/quanVisLib/svIcons.cpp
    libs/quanVisLib/svUtil.h
    libs/quanVisLib/svLut.h
    libs/quanVisLib/svParticle.h
    libs/quanVisLib/ivview.h
    libs/quanVisLib/ivTrackball.h
    libs/quanVisLib/svOutline.h
    libs/quanVisLib/svDistance.h
    libs/quanVisLib/svArrowGlyph.h
    libs/quanVisLib/svSlice.h
    libs/quanVisLib/svQDOTdistancemap.h
    libs/quanVisLib/svQDOTmagnitudemap.h
    libs/quanVisLib/CIEColor.h
    libs/quanVisLib/svColors.h
    libs/quanVisLib/svMap.h
    libs/quanVisLib/svDirectArrow.h
    libs/quanVisLib/svMesh.h
    libs/quanVisLib/svSplitArrow.h
    libs/quanVisLib/svVectorField.h
    libs/quanVisLib/svQDOT.h
    libs/quanVisLib/svPickingray.h
    libs/quanVisLib/svDisc.h
    libs/quanVisLib/svAnalyticField.h
    libs/quanVisLib/svPrimitive.h
    libs/quanVisLib/svGlyph.h
    libs/quanVisLib/svSummaryGlyph.h
    libs/quanVisLib/svBarGlyph.h
    libs/quanVisLib/svBendBar.h
    libs/quanVisLib/svSplitVectorsBar.h
    libs/quanVisLib/svDirectBar.h
    libs/quanVisLib/GLSLProgramObject.h
    libs/quanVisLib/dual_depth_peeling.h
    libs/quanVisLib/svList.h
    libs/quanVisLib/svBST.h
    libs/quanVisLib/svCCL.h
    libs/quanVisLib/svMagnitude.h
    libs/quanVisLib/svWidget.h
    libs/quanVisLib/svIcons.h)

configure_file(libs/quanVisLib/svConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/svConfig.h)

add_library(svl ${svl_SOURCES}  ${CMAKE_CURRENT_BINARY_DIR}/svConfig.h)
target_include_directories(svl PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${SOIL_INCLUDE_DIR} 
${PEELING_COMMON_INCLUDE_DIR} ${PEELING_NVMODEL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
target_link_libraries(svl PUBLIC ${SOIL_LIBRARIES} ${GLEW_LIBRARIES})
set(SVL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/quanVisLib CACHE STRING "" FORCE)

add_executable(splitToolkit SplitTool/splitToolkit.cpp)
add_dependencies(splitToolkit glui)
target_link_libraries(splitToolkit svl ${SOIL_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUI_LIBRARIES} ${GLEW_LIBRARIES}
)
target_include_directories(splitToolkit PRIVATE ${SVL_INCLUDE_DIR}
${SOIL_INCLUDE_DIR}  } ${JCUTIL_INCLUDE_DIR} ${GLUI_INCLUDE_DIR}
${PEELING_COMMON_INCLUDE_DIR}${PEELING_NVMODEL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
install(TARGETS splitToolkit RUNTIME DESTINATION bin)

#add_executable(ccStudy SplitTool/ccStudy.cpp)
#target_link_libraries(ccStudy svl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
#target_include_directories(ccStudy PRIVATE ${SVL_INCLUDE_DIR}
#${JCUTIL_INCLUDE_DIR} ${PEELLING_COMMON_INCLUDE_DIR}
#${PEELING_NVMODEL_INCLUDE_DIR} ${PEELING_GLEW_INCLUDE_DIR})
#install(TARGETS ccStudy RUNTIME DESTINATION bin)

