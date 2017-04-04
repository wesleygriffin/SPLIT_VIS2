//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Depth peeling is traditionally used to perform order independent transparency (OIT)
// with N geometry passes for N transparency layers. Dual depth peeling enables peeling
// N transparency layers in N/2+1 passes, by peeling from the front and the back
// simultaneously using a min-max depth buffer. This sample performs either normal or
// dual depth peeling and blends on the fly.
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma warning( disable : 4996 )

//henan
//#include <nvModel.h>
//#include "nvModel.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "../dual_depth_peeling/common/include/nvShaderUtils.h"
#include "../dual_depth_peeling/common/include/nvSDKPath.h"
#include "GLSLProgramObject.h"
#include "Timer.h"
#include "OSD.h"
#include "nvModel.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <time.h>

#define MAX_DEPTH 1.0


#if 0
#define CHECK_GL_ERRORS  \
{ \
    GLenum err = glGetError(); \
    if (err) \
        printf( "Error %x at line %d\n", err, __LINE__); \
}
#else
#define CHECK_GL_ERRORS {}
#endif
using namespace std;
namespace __svl_lib {

class svPeeling
{
public:

    svPeeling();
    ~svPeeling(){};

    virtual void SetOpacity(vector<float> opacity);//{g_opacity=opacity;}
    virtual void SetOpacity(float opacity){g_opacity.clear();g_opacity.push_back(opacity);}
    void SetBackgroundColor(float *c){g_backgroundColor=c;};
    void Reshape(int w, int h){g_imageWidth=w;g_imageHeight=h;}
    void SetDisplayList(int list){g_quadDisplayList = list;}
    void SetDisplayList(vector<int> list);

    void MakeFullScreenQuad();
    void InitDualPeelingRenderTargets();
    void DeleteDualPeelingRenderTargets();
    void BuildShaders(string SHADER_PATH);
    void DestroyShaders();
    void ReloadShaders();
    void InitGL(string shader);
    void RenderDualPeeling();

     

private:
int g_numPasses;
int g_imageWidth;
int g_imageHeight;

vector<int> display_list;

nv::Model *g_model;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;

bool g_useOQ;
GLuint g_queryId;


static nv::SDKPath sdkPath;

GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;

vector<float> g_opacity;
bool g_showOsd;
bool g_bShowUI;
unsigned g_numGeoPasses;

int g_oldX, g_oldY;
int g_newX, g_newY;

float g_white[3];
float g_black[3];
float *g_backgroundColor;

GLuint g_dualBackBlenderFboId;
GLuint g_dualPeelingSingleFboId;
GLuint g_dualDepthTexId[2];
GLuint g_dualFrontBlenderTexId[2];
GLuint g_dualBackTempTexId[2];
GLuint g_dualBackBlenderTexId;

GLenum g_drawBuffers[7];
};

}
