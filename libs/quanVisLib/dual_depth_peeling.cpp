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

#include "dual_depth_peeling.h"
#include <GL/glext.h>
#define GL_RG32F                          0x8230
using namespace std;

namespace __svl_lib {

svPeeling::svPeeling()
{
     g_numPasses = 4;
     g_useOQ = true;

     //g_opacity = 0.6;
     g_showOsd = true;
     g_bShowUI = true;
     g_numGeoPasses = 0;

     g_white[0]=1;g_white[1]=1;g_white[2]=1;
     g_black[0]=0;g_black[1]=0;g_black[2]=1;

     g_drawBuffers[0] = GL_COLOR_ATTACHMENT0_EXT;
     g_drawBuffers[1] = GL_COLOR_ATTACHMENT1_EXT;
     g_drawBuffers[2] = GL_COLOR_ATTACHMENT2_EXT;
     g_drawBuffers[3] = GL_COLOR_ATTACHMENT3_EXT;
     g_drawBuffers[4] = GL_COLOR_ATTACHMENT4_EXT;
     g_drawBuffers[5] = GL_COLOR_ATTACHMENT5_EXT;
     g_drawBuffers[6] = GL_COLOR_ATTACHMENT6_EXT;
g_backgroundColor = g_white; 
}

void svPeeling::SetOpacity(vector<float> opacity)
{
      g_opacity.clear();
      for(int i=0;i<opacity.size();i++)
     {
         g_opacity.push_back(opacity[i]);
     }
}
void svPeeling::SetDisplayList(vector<int> list)
{
      display_list.clear();
      for(int i=0;i<list.size();i++)
     {
         display_list.push_back(list[i]);
     }
}
//--------------------------------------------------------------------------
void svPeeling::InitDualPeelingRenderTargets()
{
	glGenTextures(2, g_dualDepthTexId);
	glGenTextures(2, g_dualFrontBlenderTexId);
	glGenTextures(2, g_dualBackTempTexId);

	glGenFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//henan
		//glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_FLOAT_RG32_NV, g_imageWidth, g_imageHeight,
		 glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RG32F, g_imageWidth, g_imageHeight,
					 0, GL_RGB, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
					 0, GL_RGBA, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
					 0, GL_RGBA, GL_FLOAT, 0);
	}
cout<<"done1"<<endl;
	glGenTextures(1, &g_dualBackBlenderTexId);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, g_imageWidth, g_imageHeight,
				 0, GL_RGB, GL_FLOAT, 0);

	glGenFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);
cout<<"done2"<<endl;
	int j = 0;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	j = 1;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);
cout<<"done3"<<endl;
	CHECK_GL_ERRORS;
}

//--------------------------------------------------------------------------
void svPeeling::DeleteDualPeelingRenderTargets()
{
	glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	glDeleteTextures(2, g_dualDepthTexId);
	glDeleteTextures(2, g_dualFrontBlenderTexId);
	glDeleteTextures(2, g_dualBackTempTexId);
	glDeleteTextures(1, &g_dualBackBlenderTexId);
}

//--------------------------------------------------------------------------
void svPeeling::BuildShaders(string SHADER_PATH)
{
	printf("\nloading shaders...\n");
cout<<SHADER_PATH+"dual_peeling_init_vertex.glsl"<<endl;

	g_shaderDualInit.attachVertexShader(SHADER_PATH+"dual_peeling_init_vertex.glsl");
	g_shaderDualInit.attachFragmentShader(SHADER_PATH+"dual_peeling_init_fragment.glsl");
	g_shaderDualInit.link();

	g_shaderDualPeel.attachVertexShader(SHADER_PATH+"shade_vertex.glsl");
	g_shaderDualPeel.attachVertexShader(SHADER_PATH+"dual_peeling_peel_vertex.glsl");
	g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"shade_fragment.glsl");
	g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"dual_peeling_peel_fragment.glsl");
	g_shaderDualPeel.link();

	g_shaderDualBlend.attachVertexShader(SHADER_PATH+"dual_peeling_blend_vertex.glsl");
	g_shaderDualBlend.attachFragmentShader(SHADER_PATH+"dual_peeling_blend_fragment.glsl");
	g_shaderDualBlend.link();

	g_shaderDualFinal.attachVertexShader(SHADER_PATH+"dual_peeling_final_vertex.glsl");
	g_shaderDualFinal.attachFragmentShader(SHADER_PATH+"dual_peeling_final_fragment.glsl");
	g_shaderDualFinal.link();

}

//--------------------------------------------------------------------------
void svPeeling::DestroyShaders()
{
	g_shaderDualInit.destroy();
	g_shaderDualPeel.destroy();
	g_shaderDualBlend.destroy();
	g_shaderDualFinal.destroy();

}

//--------------------------------------------------------------------------
void svPeeling::ReloadShaders()
{
//	DestroyShaders();
//	BuildShaders();
}

//--------------------------------------------------------------------------
void svPeeling::InitGL(string shader)
{

//cout<<"svPeeling"<<endl; 
	// Allocate render targets first
	InitDualPeelingRenderTargets();
//cout<<"InitDualPeelingRenderTargets"<<endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//cout<<"glBindFramebufferEXT("<<endl;
	BuildShaders(shader);
        MakeFullScreenQuad();

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	glGenQueries(1, &g_queryId);
}

void svPeeling::MakeFullScreenQuad()
{
        g_quadDisplayList = glGenLists(1);
        glNewList(g_quadDisplayList, GL_COMPILE);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
        glBegin(GL_QUADS);
        {
                glVertex2f(0.0, 0.0);
                glVertex2f(1.0, 0.0);
                glVertex2f(1.0, 1.0);
                glVertex2f(0.0, 1.0);
        }
        glEnd();
        glPopMatrix();

        glEndList();
}

//--------------------------------------------------------------------------

void svPeeling::RenderDualPeeling()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// ---------------------------------------------------------------------
	// 1. Initialize Min-Max Depth Buffer
	// ---------------------------------------------------------------------
//cout<<"Initialize Min-Max Depth Buffer"<<endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

	// Render targets 1 and 2 store the front and back colors
;	// Clear to 0.0 and use MAX blending to filter written color
	// At most one front color and one back color can be written every pass
	glDrawBuffers(2, &g_drawBuffers[1]);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render target 0 stores (-minDepth, maxDepth, alphaMultiplier)
	glDrawBuffer(g_drawBuffers[0]);
	glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendEquationEXT(GL_MAX_EXT);

	g_shaderDualInit.bind();
        for(int i=0;i<display_list.size();i++)
	glCallList(display_list[i]);
	g_shaderDualInit.unbind();

	CHECK_GL_ERRORS;

	// ---------------------------------------------------------------------
	// 2. Dual Depth Peeling + Blending
	// ---------------------------------------------------------------------
//cout<<"Initialize Min-Max Depth Buffer"<<endl;
	// Since we cannot blend the back colrs in the geometry passes,
	// we use another render target to do the alpha blending
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glDrawBuffer(g_drawBuffers[6]);
	glClearColor(g_backgroundColor[0], g_backgroundColor[1], g_backgroundColor[2], 0);
	glClear(GL_COLOR_BUFFER_BIT);

	int currId = 0;

	for (int pass = 1; g_useOQ || pass < g_numPasses; pass++) {
		currId = pass % 2;
		int prevId = 1 - currId;
		int bufId = currId * 3;
		
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingFboId[currId]);

		glDrawBuffers(2, &g_drawBuffers[bufId+1]);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(g_drawBuffers[bufId+0]);
		glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render target 0: RG32F MAX blending
		// Render target 1: RGBA MAX blending
		// Render target 2: RGBA MAX blending
		glDrawBuffers(3, &g_drawBuffers[bufId+0]);
		glBlendEquationEXT(GL_MAX_EXT);

		g_shaderDualPeel.bind();
		g_shaderDualPeel.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[prevId], 0);
		g_shaderDualPeel.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[prevId], 1);
	       for(int j=0;j<display_list.size();j++)
               {	
              g_shaderDualPeel.setUniform("Alpha", (float*)&(g_opacity[j]), 1);
		glCallList(display_list[j]);
               }
		g_shaderDualPeel.unbind();

		CHECK_GL_ERRORS;

		// Full screen pass to alpha-blend the back color
		glDrawBuffer(g_drawBuffers[6]);

		glBlendEquationEXT(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (g_useOQ) {
			glBeginQuery(GL_SAMPLES_PASSED_ARB, g_queryId);
		}

		g_shaderDualBlend.bind();
		g_shaderDualBlend.bindTextureRECT("TempTex", g_dualBackTempTexId[currId], 0);
		glCallList(g_quadDisplayList);
		g_shaderDualBlend.unbind();

		CHECK_GL_ERRORS;

		if (g_useOQ) {
			glEndQuery(GL_SAMPLES_PASSED_ARB);
			GLuint sample_count;
			glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);
			if (sample_count == 0) {
				break;
			}
		}
	}

	glDisable(GL_BLEND);

	// ---------------------------------------------------------------------
	// 3. Final Pass
	// ---------------------------------------------------------------------
//cout<<"Final"<<endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
//cout<<"Final"<<endl;
	g_shaderDualFinal.bind();
	g_shaderDualFinal.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[currId], 0);
	g_shaderDualFinal.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[currId], 1);
	g_shaderDualFinal.bindTextureRECT("BackBlenderTex", g_dualBackBlenderTexId, 2);
//cout<<"Final"<<endl;
	glCallList(g_quadDisplayList);
	g_shaderDualFinal.unbind();

	CHECK_GL_ERRORS;
}



}
