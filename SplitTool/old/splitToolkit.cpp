#include "dual_depth_peeling.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>

//#endif

//#include "svVectorField.h"
#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svBendBar.h"
#include "svDirectBar.h"
#include "svSummaryGlyph.h"
#include "svSplitVectorsBar.h"
//#include "svGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include <string.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define CLUSTER_DIMENSION 7

using namespace __svl_lib;

#ifndef M_PI 
#define M_PI 3.1415926
#endif



#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

view3d view_info;

svQDOT *flow_field;
svDirectArrow *directglyph;
svSummaryGlyph *summaryglyph;
svBendBar *bendglyph;
svDirectBar *directbar;
svPeeling *peeling;
svSplitVectorsBar *splitbar;
char *configFile;

char * outputdir;
char *sortfile;
char *formatfile;
char *regionfile;
char *densityfile;

bool summaryVisible;

int encode_type;

struct ConfigProperty{
	
	char *rawDir;
	char *rawFile;
	char *storeDir;
	
	svVector3 plane_center;
    svVector3 plane_vector;
    svScalar plane_distance;
	
	svBool isContour;
	
	KmeansProperty step1_kmeansproperty;
	KmeansProperty step2_kmeansproperty;
	
	ContourProperty contourproperty;
        SymmetryProperty symmetryproperty;

        RenderProperty  renderproperty;

        BendProperty bendproperty;
        BarProperty barproperty;        
        DirectProperty directproperty;
        SplitVectorsProperty splitproperty;

        svVector4 barcolor;

        svInt frequency;
	
	svScalarArray *magrange;
	
} configproperty;

ViewProperty viewproperty;

void Config(char *configfname, ConfigProperty &property);
void Update();

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLint nx, ny, nz;
Trackball trackball;

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 2.0 };//20
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 0,0,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
 // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

//***************************
// GLUT callback functions
//****************************

void reshape(int w, int h)
{
    peeling->Reshape(w,h);
   peeling-> DeleteDualPeelingRenderTargets();
    peeling-> InitDualPeelingRenderTargets();

        image_width  = w;
        image_height = h;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport (0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
// perspective
                float aspect = view_info.aspect_ratio * float(w)/float(h);
                float GLfloat_fov;
                if ( aspect < 1 ){
                  // fovy is a misnomer.. we GLfloatly mean the fov applies to the
                  // smaller dimension
                  float fovx, fovy;
                  fovx = fovy = view_info.view_angle;
                  GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
                }
                else{
                        GLfloat_fov = view_info.view_angle;
                }

                gluPerspective(GLfloat_fov,
                                           aspect,
                                           0.1,//view_info.hither,
                                           view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

        
        InitLight();
    
    viewproperty.eye = view_info.eye;

    glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix); 
    glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
    glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );
//for(int ii=0;ii<4;ii++)
//cerr<<viewproperty.viewport[ii]<<" ";
  bendglyph->Generate(configproperty.bendproperty,viewproperty,
                flow_field->GetPlaneDir());
}


void display(void)
{
       glEnable(GL_DEPTH_TEST);
glEnable(GL_MULTISAMPLE);

        GLfloat m[16];

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
    //    if (bBoundingBox)

//                bendglyph->DrawOutline(false);
//do_draw();
//bendglyph->DrawSilkPlane(flow_field->GetPlaneDir());
//       if(encode_type ==0)
  //     directbar->Render();
    //  else if(encode_type == 1)
     //  bendglyph->Render();
     // else
       //  splitbar->Render();

    //   bendglyph->DrawSilkPlane(flow_field->GetPlaneDir());

       if(encode_type ==0)
         peeling->RenderDualPeeling(10,20);
       else if(encode_type == 1)
         peeling->RenderDualPeeling(15,25);
        else
         peeling->RenderDualPeeling(5,30);

//
//        glEnable(GL_COLOR_MATERIAL);
//        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//        glEnable(GL_MULTISAMPLE);
//        glEnable(GL_LIGHTING);
//        glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    //     glCallList(20);
   //    bendglyph->RenderLegend();
       glDisable(GL_LIGHTING);
//glDrawBuffer(GL_FRONT_AND_BACK);
       glColor3f(0,0,0);
       
//bendglyph->DrawSilkPlane(flow_field->GetPlaneDir());

//        glDrawBuffer(GL_FRONT_AND_BACK);
 
//       bendglyph->DrawSilkPlane(flow_field->GetPlaneDir());

        glPopMatrix();

        glutSwapBuffers();

}
/*
void display(void)
{
        // set new model view and projection matrix
        glClearColor(1., 1., 1., 1);
//	glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_MULTISAMPLE); 

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        trackball.getMatrix().getValue(viewproperty.tm);

        glPushMatrix();
        glMultMatrixf(m);

        glColor3f(0,0,0);
        if (bBoundingBox)
                bendglyph->DrawOutline(false);
//        bendglyph->DrawSilkPlane(flow_field->GetPlaneDir());        
//if (bAxis)
        //	draw_axis();

//        glEnable(GL_LIGHTING);
//        glEnable(GL_LIGHT0);
     //   glEnable(GL_TEXTURE_2D);

       // directglyph->Render();
       // if(summaryVisible)
       //       summaryglyph->Render();

        bendglyph->Render();
         bendglyph->DrawSilkPlane(flow_field->GetPlaneDir()); 
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix();
        glFlush();
        glutSwapBuffers();
}

*/
void key(unsigned char key, int x, int y)
{
        switch (key) {

        case 'e':
             encode_type ++;
             encode_type = encode_type%3;
             break;

        case 'S':
        case 's':
              summaryVisible = 1-summaryVisible;
	      break;

        case 'u':
        case 'U':  bendglyph->Generate(configproperty.bendproperty,viewproperty,
                                flow_field->GetPlaneDir());

//               Update();
               break;

        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        }
 glutPostRedisplay();


}

void mouse(int button, int state, int x, int y)
{
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

        if (s & Trackball::BUTTON_DOWN){
        trackball.mouseDown(s, x, y);
        }

        if (s & Trackball::BUTTON_UP){
    //    trackball.getMatrix().getValue(viewproperty.tm);

        trackball.mouseUp(s, x, y);
        }
        glutPostRedisplay();

}

void motion(int x, int y)
{

        trackball.mouseMotion(x, y);

        glutPostRedisplay();
}

//******************************************
// initialization code for GL and NV_EXT
//******************************************
/*
struct ConfigProperty{
	
	char *rawDir;
	char *rawFile;
	char *storeDir;
	
	svVector3 plane_center;
    svVector3 plane_vector;
    svScalar plane_distance;
	
	svBool isContour;
	
	KmeansProperty kmeansproperty;
	ContourProperty contourproperty;
	
	svScalarArray *magrange;
	
} configproperty;
*/

void Config(char *configfname, ConfigProperty &property)
{
	ifstream infile(configfname);
	
	property.rawDir = new char[200];
	property.rawFile = new char[100];
	property.storeDir = new char[200];
	
	sortfile = new char[50];
	formatfile = new char[50];
	regionfile = new char[50];
	densityfile = new char [50];
	
	string tmp;
	
	/*-------------file names-----------------*/
	infile>>tmp;
	infile>>property.rawDir;
	infile>>tmp;
	infile>>property.rawFile;
	infile>>tmp;
	infile>>property.storeDir;

	mkdir(property.storeDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

    infile>>tmp;
	infile>>property.plane_center[0]>>property.plane_center[1]>>property.plane_center[2];
	infile>>tmp;
	infile>>property.plane_vector[0]>>property.plane_vector[1]>>property.plane_vector[2];
	infile>>tmp;
	infile>>property.plane_distance;
	
	/*--------------initialization of QDOT field------------*/
	char *qdot_format = new char[400];
	sprintf(qdot_format,"%s/%s/format.txt", property.storeDir, 
	            property.rawFile);
	flow_field = new svQDOT();
    flow_field->SetVTK(property.rawDir, property.rawFile,
	                   property.storeDir,
					   "sort.txt", "format.txt", "density.txt",
					   property.plane_center,
					   property.plane_vector,
					   property.plane_distance);
    flow_field->New(qdot_format);					
	delete [] qdot_format;
	
	/*------------------contour----------------------*/
	infile>>tmp;// cerr<<tmp<<endl;
	double store;
	infile>>property.isContour;
	infile>>tmp; //cerr<<tmp<<endl;
	int num;
	infile>>num;
        //cerr<<num<<endl;cerr<<flow_field->GetPlaneNum()<<endl;
        property.contourproperty.seed_num = flow_field->GetPlaneNum();
        property.contourproperty.contourValues = new svScalarArray[flow_field->GetPlaneNum()];
        for(int i=0;i<flow_field->GetPlaneNum();i++)
           property.contourproperty.isUpdate.add(0);
        //cerr<<flow_field->GetPlaneNum()<<endl;
	char linestr[5];

       char *str = new char[200];
       sprintf(str, "%s/%s/density.txt", property.storeDir, property.rawFile);
  //   cerr<<num<<" "<<str<<endl; 
	for(int i=0;i<num;i++)
	{
		infile>>linestr;cerr<<linestr<<endl;
		if(!strcmp(linestr,"G"))
		{
			//cerr<<linestr<<endl;
			char linestr2[10];
			infile>>linestr2;

			if(!strcmp(linestr2,"A"))
			{
				infile>>store;
				for(int j=0;j<flow_field->GetPlaneNum();j++)
				{
					property.contourproperty.contourValues[j].add(store);
				       property.contourproperty.isUpdate[j] = 1;
                                 }
			}
			else
			{
				infile>>store;
				//cerr<<store<<endl;
				for(int j=0;j<flow_field->GetPlaneNum();j++)
				{
					property.contourproperty.contourValues[j].add(store*flow_field->GetMaxDensitybyLayer(str, j));
                                       property.contourproperty.isUpdate[j] = 1;
				}
			}
		}
		else
		{
			char linestr2[10];
			infile>>linestr2;
			if(!strcmp(linestr2,"A"))
			{
				infile>>tmp; //[
				vector<double> store3(2);
				infile>>store3[0]>>store3[1]; //index of layer
				infile>>tmp; //]
				infile>>store; //value
				
				int index1 = (int)store3[0];
				int index2 = (int)store3[1];
				for(int j=index1;j<index2;j++)
				{
					property.contourproperty.contourValues[j].add(store);
                                                                               property.contourproperty.isUpdate[j] = 1;

				}
			}
			else
			{
				infile>>tmp;
				vector<double> store3(2);
				infile>>store3[0]>>store3[1];
				infile>>tmp;
				infile>>store;
				
				int index1 = (int)store3[0];
				int index2 = (int)store3[1];			
				for(int j=index1;j<index2;j++)
				{
					property.contourproperty.contourValues[j].add( store*flow_field->GetMaxDensitybyLayer(str, j));
                                       property.contourproperty.isUpdate[j] = 1;

				}			
			}			
		}
//		property.contourproperty.isUpdate.add(1);
	}
	sprintf(str, "%s/%s/contour.txt", property.storeDir, property.rawFile);
	property.contourproperty.outputfile = strdup(str);
	sprintf(str, "%s/%s/", property.storeDir, property.rawFile);
	property.contourproperty.vtkdir = strdup(str);
//    cerr<<"done"<<endl;
	
	/*--------------clustering------------------------------*/
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>tmp;

	property.magrange = new svScalarArray[2];
	
	/*-------------step 1-----------------------------------*/
    property.step1_kmeansproperty.dimension = 7;
    property.step1_kmeansproperty.isNormalize = true;
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step1_kmeansproperty.clusterLayer.add(-1);
    }   
  	infile>>tmp; cerr<<tmp<<endl;
	for(int i=0;i<CLUSTER_DIMENSION;i++)
	{
		infile>>store;
		property.step1_kmeansproperty.clusterWeight.add(store);
	}	
	int index1, index2;
	infile>>tmp;//cerr<<tmp<<endl;
	infile>>index1;
	infile>>index2;
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  
    bool isWhole = store;
    if(isWhole)
    {
                 for(int i=index1; i<=index2;i++)
                {
                   property.step1_kmeansproperty.clusterLayer[i]++;
                }
    }
    else
    {
            int count = 1;
             for(int i=index1; i<=index2;i++)
                {
                   property.step1_kmeansproperty.clusterLayer[i]= property.step1_kmeansproperty.clusterLayer[i]+count;
                   count++; 
                }
    }
	infile>>tmp; cerr<<tmp<<endl;
	infile>>store;  property.magrange[0].add(store);
	infile>>store;  property.magrange[0].add(store);
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;   property.step1_kmeansproperty.numCluster = store;	
	sprintf(str, "%s/%s/input.txt", property.storeDir, property.rawFile);
	property.step1_kmeansproperty.file1 = strdup(str);
	sprintf(str, "%s/%s/output.txt", property.storeDir, property.rawFile);
	property.step1_kmeansproperty.file2 = strdup(str);

    /*--------------------step 2-------------------------------------------*/
    property.step2_kmeansproperty.dimension = 7;
    property.step2_kmeansproperty.isNormalize = true;
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step2_kmeansproperty.clusterLayer.add(-1);
    }  
        infile>>tmp;//cerr<<tmp<<endl; 
  	infile>>tmp; //cerr<<tmp<<endl;
	for(int i=0;i<CLUSTER_DIMENSION;i++)
	{
		infile>>store;
		property.step2_kmeansproperty.clusterWeight.add(store);
	}	
	//int index1, index2;
	infile>>tmp;//cerr<<tmp<<endl;
	infile>>index1;
	infile>>index2;
	for(int i=index1; i<=index2;i++)
	{
		property.step2_kmeansproperty.clusterLayer[i]++;
	}
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  
    isWhole = store;
    if(isWhole)
    {
                 for(int i=index1; i<=index2;i++)
                {
                   property.step2_kmeansproperty.clusterLayer[i]++;
                }
    }
    else
    {
            int count = 1;
             for(int i=index1; i<=index2;i++)
                {
                   property.step2_kmeansproperty.clusterLayer[i]= property.step2_kmeansproperty.clusterLayer[i]+count;
                   count++; 
                }
    }
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>store;  property.magrange[1].add(store);
	infile>>store;  property.magrange[1].add(store);
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>store;   property.step2_kmeansproperty.numCluster = store;	
	sprintf(str, "%s/%s/input.txt", property.storeDir, property.rawFile);
	property.step2_kmeansproperty.file1 = strdup(str);
	sprintf(str, "%s/%s/output.txt", property.storeDir, property.rawFile);
	property.step2_kmeansproperty.file2 = strdup(str);

    //delete [] str;	


        infile>>tmp;cerr<<tmp<<endl;
        infile>>store;
        num = store;
        for(int i=0;i<num;i++)
        {
                            
           infile>>store;cerr<<store<<endl;
           configproperty.bendproperty.bendRange.add(store);
           infile>>store;
           configproperty.bendproperty.bendHeight.add(store);
           infile>>store;
           configproperty.bendproperty.bendWidth.add(store);
        }
        configproperty.bendproperty.groupcolor.add(svVector3(49./255., 54./255., 149./255.));
        configproperty.bendproperty.groupcolor.add(svVector3(69./255., 117./255., 180./255.));
        configproperty.bendproperty.groupcolor.add(svVector3(145./255., 191./255., 219./255.));
        configproperty.bendproperty.groupcolor.add(svVector3(254./255., 224./255., 144./255.));
        configproperty.bendproperty.groupcolor.add(svVector3(252./255., 141./255., 89./255.));
        configproperty.bendproperty.groupcolor.add(svVector3(215./255., 48./255., 39./255.));


        configproperty.directproperty.UnitHeight = 4e+6;
        configproperty.directproperty.width = 5;

        configproperty.splitproperty.shiftexp = 7;
        configproperty.splitproperty.expHeight = 10;
        configproperty.splitproperty.coeHeight = 10;
        configproperty.splitproperty.expWidth = 10;
        configproperty.splitproperty.coeWidth = 1;
        configproperty.splitproperty.coeColor[0] = 0.5;
        configproperty.splitproperty.coeColor[1] = 0.5;
        configproperty.splitproperty.coeColor[2] = 0.5;
        configproperty.splitproperty.coeColor[3] = 0.5;


        configproperty.barproperty.scalex = 0.0125/2.;//0.025;//0.0125;//0.025;
        configproperty.barproperty.scaley = 0.125/2.;//0.1;//0.05;//0.1;
        configproperty.barproperty.halowidth = configproperty.barproperty.scalex*configproperty.bendproperty.bendHeight[0]/2.;
        configproperty.barproperty.legend =1e-5;//1.5e-6 ;
        configproperty.barproperty.legendh = 10 * configproperty.barproperty.scaley;
        configproperty.barproperty.legendw = 2 * configproperty.barproperty.scalex;
        configproperty.barproperty.circleradius = configproperty.barproperty.scalex*configproperty.bendproperty.bendWidth[0]*2.;
        configproperty.barproperty.legendcolor[0]=91/255.;//137./255.;//91/255.;
        configproperty.barproperty.legendcolor[1]=168./255.;//70./255.;//168./255.;
        configproperty.barproperty.legendcolor[2]=126./255.;//64./255.;//126./255.;
        configproperty.barproperty.legendcolor[3]=1;
        configproperty.barproperty.circlecolor[0]=1.;//227./255.;//118./255;//227./255.;
        configproperty.barproperty.circlecolor[1]=0.;//218./255.;//206./255.;//218./255.;
        configproperty.barproperty.circlecolor[2]=0.;//87./255.;//58./255.;//87./255.;
        configproperty.barproperty.circlecolor[3]=1.;
        configproperty.barproperty.dirbarh = 5 * configproperty.barproperty.scaley;

        configproperty.renderproperty.color[0] = 0.5;
        configproperty.renderproperty.color[1] = 0.5;
        configproperty.renderproperty.color[2] = 0.5;
        configproperty.renderproperty.color[3] = 0.5;
        configproperty.barcolor[0] = 76./255.;//248./255.;//76./255.;
        configproperty.barcolor[1] = 28./255.;//170./255.;//28./255.;
        configproperty.barcolor[2] = 95./255.;//161./255.;//95./255.;

        svVector3 pdir1, pdir2, ppos1, ppos2;
        pdir1[0]=1;pdir1[1]=1;pdir1[2]=0;
        pdir2[0]=-1;pdir2[1]=1;pdir2[2]=0;
        ppos1[0]=0;ppos1[1]=0;ppos1[2]=0;
        ppos2[0]=0;ppos2[1]=0;ppos2[2]=0;
        configproperty.symmetryproperty.dir.add(normalize(pdir1));
        configproperty.symmetryproperty.dir.add(normalize(pdir2));
        configproperty.symmetryproperty.pos.add(ppos1);
        configproperty.symmetryproperty.pos.add(ppos2);
        sprintf(str, "%s/%s/", property.storeDir, property.rawFile);
        configproperty.symmetryproperty.datafile = new svChar[400];
        configproperty.symmetryproperty.outputfile = new svChar[400];
        sprintf(configproperty.symmetryproperty.datafile, "%s", str);
 
        configproperty.frequency = 1;
        encode_type = 0;

         infile.close();
	/*-------------------------end----------------------------------------*/
	cerr<<"=>config"<<endl;
	
	/*for(int i=0;i<flow_field->GetPlaneNum();i++)
	{
		for(int j=0;j<contourValues[i].size();j++)
		{
			cerr<<i<<" "<<contourValues[i][j]<<" ";
		}cerr<<endl;
	}*/
     delete []str;
} 

void Update()
{
    Config(configFile, configproperty);
   /* 
      directglyph->New(flow_field, flow_field->GetPlaneNum());
  summaryglyph->New(flow_field, flow_field->GetPlaneNum());
                 //cerr<<"done"<<flow_field->GetPlaneNum()<<endl;
  char *str = new char[400];
  if(configproperty.isContour)
  { //cerr<<"isContour"<<endl;
       directglyph->GenerateContours(configproperty.cooontourpropertt);//cerr<<"done"<<endl;
       summaryglyph->GenerateContours(configproperty.contourproperty);
   }           
  else         
   {           
         for(int i=0;i<flow_field->GetPlaneNum();i++)
         {     
               sprintf(str, "%s/%s/%d.txt", configproperty.storeDir, configproperty.rawFile, i);
                   directglyph->SetData(str, i);
                  summaryglyph->SetData(str, i);
         }
   }           
  delete [] str;  

  directglyph->ResetCluster();
  summaryglyph->ResetCluster();
  directglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
  directglyph->GenerateClusters(configproperty.step1_kmeansproperty);
  directglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
  directglyph->GenerateClusters(configproperty.step2_kmeansproperty);
//  cerr<<"done"<<endl;
  summaryglyph->GenerateClusters(directglyph->GetClusterLabels());

  directglyph->SetColorByCluster();
 // cerr<<"done"<<endl;
  summaryglyph->SetNumPower(flow_field->GetNumPower());
  summaryglyph->SetScaling(flow_field->GetScaling());
  directglyph->Generate();
  summaryglyph->SetRadius(0.27);
  svInt list =10;
  summaryglyph->SetDisplayList(list);
  summaryglyph->Generate(1);
*/
//  bendglyph->Generate(configproperty.bendproperty, viewproperty);

}

void init(char *configfname)//rbfname, char *cpname)
{
  configFile = strdup(configfname);

  Config(configfname, configproperty);
// cerr<<configproperty.rawDir<<endl; 
  char *str = new char[200];

  bendglyph = new svBendBar(flow_field);
  bendglyph ->New(flow_field, flow_field->GetPlaneNum());

  directbar = new svDirectBar(flow_field);
  directbar ->New(flow_field, flow_field->GetPlaneNum());

  splitbar = new svSplitVectorsBar(flow_field);
  splitbar ->New(flow_field, flow_field->GetPlaneNum());
/*
  directglyph = new svDirectArrow(flow_field);//cerr<<"done"<<endl;
  directglyph->New(flow_field, flow_field->GetPlaneNum());
//		 cerr<<"done"<<flow_field->GetPlaneNum()<<endl; 
  summaryglyph = new svSummaryGlyph();
  summaryglyph->New(flow_field, flow_field->GetPlaneNum());
*/
  if(configproperty.isContour)
  { cerr<<"isContour"<<endl;
  //     directglyph->GenerateContours(configproperty.contourproperty);//cerr<<"done"<<endl;
  //     summaryglyph->GenerateContours(configproperty.contourproperty); 
       bendglyph->GenerateContours(configproperty.contourproperty);
       directbar->GenerateContours(configproperty.contourproperty);
       splitbar->GenerateContours(configproperty.contourproperty);
  }
  else
   {
       //  for(int i=0;i<flow_field->GetPlaneNum();i++)
         for(int i=40;i<41;i++)
         {
               sprintf(str, "%s/%s/D2.txt", configproperty.storeDir, configproperty.rawFile);//, i);
//	           directglyph->SetData(str, i);
  //                 summaryglyph->SetData(str, i);
                   bendglyph->SetData(str, i);
                   directbar->SetData(str, i);
                   splitbar->SetData(str, i);
         }
   }
cerr<<"done"<<endl;
  delete [] str;  
/*
  directglyph->ResetCluster();
  summaryglyph->ResetCluster();
  directglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
  directglyph->GenerateClusters(configproperty.step1_kmeansproperty);
  directglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
  directglyph->GenerateClusters(configproperty.step2_kmeansproperty);
cerr<<"done"<<endl;
  summaryglyph->GenerateClusters(directglyph->GetClusterLabels());

  cerr<<"done"<<endl;
  summaryglyph->SetNumPower(flow_field->GetNumPower());
  summaryglyph->SetScaling(flow_field->GetScaling());
cerr<<"done"<<endl;
  directglyph->SetColorByCluster();
  //summaryglyph->SetColors(); 
 cerr<<"done"<<endl;
  directglyph->Generate();
cerr<<"done"<<endl;
  summaryglyph->SetRadius(0.27);
  //summaryglyph->SetDisplayList(10);
  svInt list = 10;
  summaryglyph->SetDisplayList(list);
  summaryglyph->Generate(1);
cerr<<"done"<<endl;
*/
 //-----------------------------------------------------------------------------------

 // bendglyph->SetRadius(1); 
//  bendglyph->Generate(configproperty.bendproperty,viewproperty);

  summaryVisible = 1;


  svVector3 center = flow_field->GetCenter();//cerr<<"done"<<endl;
center[2]=0;
  center.getValue(view_info.coi);

  GLfloat x, y, z;
  flow_field->GetPhysicalDimension(&x,&y, &z);

  view_info.eye[0] = 0;//x/2.0;
  view_info.eye[1] = 0;//y/2.0;
  view_info.eye[2] = z*2.0;

  trackball.setEye(view_info.eye);
  trackball.setFocus(center);
  trackball.setWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);

        trackball.getMatrix().getValue(viewproperty.tm);

  directbar->SetBarProperty(configproperty.barproperty);
  directbar->SetAlpha(0.5);
 // directbar->SetSampling(configproperty.symmetryproperty, configproperty.frequency);
  directbar->SetDisplayList(15);cerr<<"done1"<<endl;
  directbar->SetColor(configproperty.barcolor);cerr<<"done2"<<endl;
  directbar->SetROI((svScalar)1e-7, (svScalar)1);
  directbar->Generate(configproperty.directproperty,
                 flow_field->GetPlaneDir());cerr<<"done3"<<endl;
  directbar->SetRenderProperty(configproperty.renderproperty);
 
  splitbar->SetBarProperty(configproperty.barproperty);
  splitbar->SetAlpha(0.5);
  //splitbar->SetSampling(configproperty.symmetryproperty, configproperty.frequency);
  splitbar->SetDisplayList(5);cerr<<"done1"<<endl;
  splitbar->SetColor(configproperty.barcolor);cerr<<"done2"<<endl;
  splitbar->SetROI((svScalar)1e-7, (svScalar)1);
  splitbar->Generate(configproperty.splitproperty,
                 flow_field->GetPlaneDir());
  splitbar->SetRenderProperty(configproperty.renderproperty);
  
  bendglyph->SetBarProperty(configproperty.barproperty);
  bendglyph->SetAlpha(0.5);
 // bendglyph->SetSampling(configproperty.symmetryproperty, configproperty.frequency);
  bendglyph->SetDisplayList(10);cerr<<"done2"<<endl;
  bendglyph->SetLegendList(20);
  bendglyph->SetColor(configproperty.barcolor);cerr<<"done3"<<endl;
  bendglyph->SetSampling(configproperty.frequency);
  bendglyph->Generate(configproperty.bendproperty,viewproperty,
                 flow_field->GetPlaneDir());cerr<<"done4"<<endl;
        svVector3 startPos = svVector3(10,-9.5,-1.5);
       svVector3 dir1= svVector3(1,0,0);
       svVector3 dir2=svVector3(0,1,0);
       svScalar stepD1 = 0.5;
       svScalar stepD2 = 0.5;
       svInt stepN1 = 17;
       svInt stepN2 = 19;
  bendglyph->GenerateLegend(configproperty.bendproperty,startPos, dir1,  dir2,
                        stepD1,  stepD2,
                        stepN1,  stepN2,flow_field->GetPlaneDir());
  bendglyph->SetRenderProperty(configproperty.renderproperty);cerr<<"done5"<<endl;
//  bendglyph->SaveToOBJ("bars.obj", configproperty.bendproperty, flow_field->GetPlaneDir());

  peeling = new svPeeling();
  peeling->SetDisplayList(100);
  vector<float> opac;
  opac.push_back(0.5);
  opac.push_back(1);
  peeling->SetOpacity(opac);
  float c[3];
  c[0]=1;c[1]=1;c[2]=1;
  //peeling->SetBackgroundColor(c);

  peeling ->InitGL("/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2-master/libs/dual_depth_peeling/shaders/");

}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
  srand(12345);
  
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH| GLUT_MULTISAMPLE);

        image_width  = IMAGE_WIDTH;
        image_height = IMAGE_HEIGHT;

        glutInitWindowSize(image_width, image_height);
//glfwWindowHint(GLFW_SAMPLES, 4);
        glutCreateWindow("QDOT");
        glutInitWindowPosition(200, 200);
        if (glewInit() != GLEW_OK)
        {
                printf("glewInit failed. Exiting...\n");
                exit(1);
        }

        if(argc == 2)
        {
//            init(argv[1]);
        }
        else
        {
            exit(0);
        }
	//        glutDisplayFunc(display)
	//
	//   ;

             	glutReshapeFunc(reshape);
		init(argv[1]);
		glutDisplayFunc(display);

		glutKeyboardFunc(key);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);

		glutMainLoop();
		return 0;
	}

