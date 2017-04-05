#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#include "svVectorField.h"
#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svSummaryGlyph.h"
//#include "svGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include "svOutline.h"
#include "svMesh.h"
#include <string.h>

#define CLUSTER_DIMENSION 7

using namespace __svl_lib;

void reshape(int w, int h);
void display(void);
void key(unsigned char key, int x, int y);

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

view3d view_info;

svQDOT *flow_field;
svDirectArrow *directglyph;
svSummaryGlyph *summaryglyph;
svOutline *outline;
svMesh *mesh;

char *configFile;

char * outputdir;
char *sortfile;
char *formatfile;
char *regionfile;
char *densityfile;

bool summaryVisible;
svScalar scale;
svScalar directradius;
svScalar summaryradius;
vector<int> unique_region;
int regioncount = 0;
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

        vector<string> contourname;	
	ContourProperty contourproperty;
	
	svScalarArray *magrange;
	
} configproperty;

int contourindex = 0;
void Config(char *configfname, ConfigProperty &property);
void Update();

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLint nx, ny, nz;
Trackball trackball;

double zmin;
double zmax;
double zshuffle;

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
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

        image_width  = w;
        image_height = h;

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
                                           view_info.hither,
                                           view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

        

        InitLight();

}

void display(void)
{
        // set new model view and projection matrix
        glClearColor(0.3, 0.3, 0.3, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);

        if (bBoundingBox)
                directglyph->DrawOutline();
        //if (bAxis)
        //	draw_axis();

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);
         outline->DrawXYZ(directglyph->GetLb(), directglyph->GetRb());

        glColor3f(1,0,0);
        mesh->Render();
        directglyph->Render();
        if(summaryVisible)
              summaryglyph->Render();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix();

	glPushMatrix();
//	glScalef(50,100,100);
        if(summaryVisible)
		summaryglyph->RenderColor();
	glPopMatrix();

        glFlush();
        glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{
        switch (key) {
	case 'H':
        case 'h':
              trackball.reset();
              break;
        case 'm':
              {if(regioncount > unique_region.size()-1) regioncount = 0;
              else regioncount++;  
              if(regioncount == unique_region.size())
              {  
                 char *str = new char[200];
                 sprintf(str, "%s/%s/mesh%d.txt", configproperty.storeDir, configproperty.rawFile, 100);
                 mesh->GenerateWireframe(str);
                 delete [] str;

                  cout<<"no region is displayed"<<endl;}
              else
              {
                 char *str = new char[200];
                 sprintf(str, "%s/%s/mesh%d.txt", configproperty.storeDir, configproperty.rawFile, unique_region[regioncount]);
                 mesh->GenerateWireframe(str);
                 cout<<"region "<<unique_region[regioncount]<<endl;
                 delete [] str;
              }
              break;}
        case 'S':
        case 's':
              summaryVisible = 1-summaryVisible;
	      break;
        case 'x':
                scale = scale*1.5;	
		directradius = directradius * 1.5;
		summaryradius = summaryradius * 1.5;
                directglyph->SetScale(scale);
		directglyph->SetRadius(directradius);
                directglyph->Generate();
		summaryglyph->SetRadius(summaryradius);
                summaryglyph->SetScale(scale);
                summaryglyph->Generate(1);
		break;
	case 'X':
                scale = scale/1.5;
                directradius = directradius / 1.5;
                summaryradius = summaryradius / 1.5;
		directglyph->SetScale(scale);
                directglyph->SetRadius(directradius);
               directglyph->Generate();
                summaryglyph->SetRadius(summaryradius);
                summaryglyph->SetScale(scale);
                summaryglyph->Generate(1);
		break;
        case 'u':
        case 'U':
               Update();
               break;
        case 'c':
        case 'C':
              if(contourindex<directglyph->GetContourListSize()-1)contourindex++;
              else contourindex = 0;
              cerr<<"The current contour: "<<configproperty.contourname[contourindex]<<endl;
               directglyph->ResetVisible();
              directglyph->SetVisible(contourindex);
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               break;
        case  'l':
               {cout<<"Please input the index of the min layer"<<endl;
               cin>>zmin;
               cout<<"Please input the index of the  max layer"<<endl;
               cin>>zmax;
               directglyph->ResetVisible();
               directglyph->SetVisible(contourindex);
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
              // cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               }break;   
        /*case 'z':
	      if(zmin>0)			zmin--;
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               break;
        case 'Z':
               if(zmin<zmax) zmin++;
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               break;
               
        case 'x': if(zmax>zmin)zmax--;
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
                cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
                break;
        case 'X':
                if(zmax<flow_field->GetPlaneNum()-1)zmax++;
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
                cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
                break;
               */ 
        case 't':if(zmin>0)
                {zmin = zmin - 1;
                zmax = zmax - 1;
               directglyph->ResetVisible();
               directglyph->SetVisible(contourindex);
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               // cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
                }break; 
        case 'T':
              if(zmax <flow_field->GetPlaneNum()-1)
              {
                zmin = zmin + 1;
                zmax = zmax + 1;
               directglyph->ResetVisible();
               directglyph->SetVisible(contourindex);
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               // cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               } break;
           
                
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
        trackball.mouseUp(s, x, y);
        }
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
//	cerr<<"config"<<endl;
	contourindex = 0;

	ifstream infile(configfname);
	
	property.rawDir = new char[200];
	property.rawFile = new char[100];
	property.storeDir = new char[200];
	
	sortfile = new char[50];
	formatfile = new char[50];
	regionfile = new char[50];
	densityfile = new char [50];
	
	string tmp;
//cerr<<sortfile<<endl;	
	/*-------------file names-----------------*/
	infile>>tmp;
	infile>>property.rawDir;
	infile>>tmp;
	infile>>property.rawFile;
	infile>>tmp;
	infile>>property.storeDir;
//cerr<<tmp<<endl;
	mkdir(property.storeDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

    infile>>tmp;
	infile>>property.plane_center[0]>>property.plane_center[1]>>property.plane_center[2];
	infile>>tmp;
	infile>>property.plane_vector[0]>>property.plane_vector[1]>>property.plane_vector[2];
	infile>>tmp;
	infile>>property.plane_distance;
//	cerr<<tmp<<endl;
	/*--------------initialization of QDOT field------------*/
	char *qdot_format = new char[400];
	sprintf(qdot_format,"%s/%s/format.txt", property.storeDir, 
	            property.rawFile);
	//flow_field = new svQDOT();
	//cerr<<qdot_format<<endl;
    flow_field->SetVTK(property.rawDir, property.rawFile,
	                   property.storeDir,
					   "sort.txt", "format.txt", "density.txt",
					   property.plane_center,
					   property.plane_vector,
					   property.plane_distance);
//cerr<<"done"<<endl;
        flow_field->New(qdot_format);	
       char *str = new char[200];

sprintf(str, "%s/%s/", property.storeDir,  property.rawFile);				
        flow_field->NewMesh(str);
	delete [] qdot_format;
	
	/*------------------contour----------------------*/
	infile>>tmp; //cerr<<tmp<<endl;
	double store;
	infile>>property.isContour;
	infile>>tmp; //cerr<<tmp<<endl;
	int num;
	infile>>num;
        //cerr<<num<<endl;cerr<<flow_field->GetPlaneNum()<<endl;
        property.contourproperty.seed_num = flow_field->GetPlaneNum();
        property.contourproperty.contourValues = new svScalarArray[flow_field->GetPlaneNum()];
        property.contourproperty.isUpdate.free();
        for(int i=0;i<flow_field->GetPlaneNum();i++)
           property.contourproperty.isUpdate.add(0);
        //cerr<<flow_field->GetPlaneNum()<<endl;
	char linestr[5];

       property.contourname.clear();

//       char *str = new char[200];
       sprintf(str, "%s/%s/density.txt", property.storeDir, property.rawFile);
  //   cerr<<num<<" "<<str<<endl; 
	for(int i=0;i<num;i++)
	{
               std::string cn("");

		infile>>linestr;//cerr<<linestr<<endl;
		if(!strcmp(linestr,"G"))
		{
			//cerr<<linestr<<endl;
			char linestr2[10];
			infile>>linestr2;

			if(!strcmp(linestr2,"A"))
			{
                        cn += "Absolute value ";

				infile>>store;
                                       std::ostringstream ss;
                                        ss<<store;
                                        cn += ss.str();

				for(int j=0;j<flow_field->GetPlaneNum();j++)
				{
					property.contourproperty.contourValues[j].add(store);
				       property.contourproperty.isUpdate[j] = 1;
                                 }
			}
			else
			{
                        cn += "Ratio ";

				infile>>store;
                                       std::ostringstream ss;
                                        ss<<store;
                                        cn += ss.str();

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
              property.contourname.push_back(cn);//cerr<<property.contourname[i]<<endl;
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
     property.step1_kmeansproperty.clusterLayer.free();
    property.step1_kmeansproperty.clusterWeight.free();
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step1_kmeansproperty.clusterLayer.add(-1);
    }   
  	infile>>tmp;// cerr<<tmp<<endl;
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
	infile>>tmp; //cerr<<tmp<<endl;
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
property.step2_kmeansproperty.clusterLayer.free();
property.step2_kmeansproperty.clusterWeight.free();
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

   // zmin=0;
   // zmax = flow_field->GetPlaneNum()-1;

    delete [] str;	
	infile.close();

	/*-------------------------end----------------------------------------*/
	//cerr<<"=>config"<<endl;
	
	/*for(int i=0;i<flow_field->GetPlaneNum();i++)
	{
		for(int j=0;j<contourValues[i].size();j++)
		{
			cerr<<i<<" "<<contourValues[i][j]<<" ";
		}cerr<<endl;
	}*/
} 

void Update()
{
    Config(configFile, configproperty);
     unique_region.clear();
  unique_region = flow_field->GetUniqueRegion(); 
      directglyph->New(flow_field, flow_field->GetPlaneNum());
  summaryglyph->New(flow_field, flow_field->GetPlaneNum());
                 //cerr<<"done"<<flow_field->GetPlaneNum()<<endl;
  char *str = new char[400];
  if(configproperty.isContour)
  { //cerr<<"isContour"<<endl;
       directglyph->GenerateContours(configproperty.contourproperty);//cerr<<"done"<<endl;
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
        directglyph->SetContourLabel();
        summaryglyph->SetContourLabel();
   }           
  delete [] str;  
cerr<<zmin<<" "<<zmax<<endl;
  directglyph->ResetCluster();
  summaryglyph->ResetCluster();
   directglyph->ResetVisible();
   directglyph->SetVisible(contourindex);
  directglyph->SetVisible(zmin, zmax);
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
  summaryglyph->ResetVisible();
  summaryglyph->SetVisible(contourindex);
  summaryglyph->SetVisible(zmin, zmax);
  directglyph->Generate();
  summaryglyph->SetRadius(0.27);
  svInt list =10;
  summaryglyph->SetDisplayList(list);
  summaryglyph->Generate(1);

}

void init(char *configfname)//rbfname, char *cpname)
{
  mesh = new svMesh();
  mesh->SetDisplayList(30);
  outline = new svOutline();
  flow_field = new svQDOT();
  configFile = strdup(configfname);
//cerr<<"done"<<endl;
  Config(configfname, configproperty);
   zmin=0;
    zmax = flow_field->GetPlaneNum()-1;

 //cerr<<configproperty.rawDir<<endl; 
  char *str = new char[200];
  unique_region.clear();
  unique_region = flow_field->GetUniqueRegion();

  directglyph = new svDirectArrow(flow_field);//cerr<<"done"<<endl;
  directglyph->New(flow_field, flow_field->GetPlaneNum());
//		 cerr<<"done"<<flow_field->GetPlaneNum()<<endl; 
  summaryglyph = new svSummaryGlyph();
  summaryglyph->New(flow_field, flow_field->GetPlaneNum());
  if(configproperty.isContour)
  {// cerr<<"isContour"<<endl;
       directglyph->GenerateContours(configproperty.contourproperty);//cerr<<"done"<<endl;
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
        directglyph->SetContourLabel();
        summaryglyph->SetContourLabel();
   }
  //delete [] str;  

  directglyph->ResetCluster();
  summaryglyph->ResetCluster();
   directglyph->SetVisible(contourindex);
  directglyph->SetVisible(zmin, zmax);
  directglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
  directglyph->GenerateClusters(configproperty.step1_kmeansproperty);
  directglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
  directglyph->GenerateClusters(configproperty.step2_kmeansproperty);
//cerr<<"done"<<endl;
  summaryglyph->GenerateClusters(directglyph->GetClusterLabels());
  summaryglyph->SetVisible(contourindex);
  summaryglyph->SetVisible(zmin, zmax);

//  cerr<<"done"<<endl;
  summaryglyph->SetNumPower(flow_field->GetNumPower());
  summaryglyph->SetScaling(flow_field->GetScaling());
//cerr<<"done"<<endl;
  directglyph->SetColorByCluster();
  //summaryglyph->SetColors(); 
// cerr<<"done"<<endl;
  directglyph->Generate();
//cerr<<"done"<<endl;
  summaryglyph->SetRadius(0.27);
  //summaryglyph->SetDisplayList(10);
  svInt list = 10;
  summaryglyph->SetDisplayList(list);
  summaryglyph->Generate(1);

//  char *str = new char[200];
  sprintf(str, "%s/%s/mesh%d.txt", configproperty.storeDir, configproperty.rawFile, unique_region[0]);
//cerr<<str<<endl;
  mesh->GenerateWireframe(str);
  delete [] str;
//cerr<<"done"<<endl;

  summaryVisible = 1;
  scale = directglyph->GetScale();
  directradius = directglyph->GetRadius();
  summaryradius = 0.27;

  svVector3 center = flow_field->GetCenter();//cerr<<"done"<<endl;
  center.getValue(view_info.coi);

  GLfloat x, y, z;
  flow_field->GetPhysicalDimension(&x,&y, &z);

  view_info.eye[0] = 0;//x/2.0;
  view_info.eye[1] =0;// y/2.0;
  view_info.eye[2] = z*2.0;

  trackball.setEye(view_info.eye);
  trackball.setFocus(center);
  trackball.setWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);
}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
  srand(12345);
  
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

        image_width  = IMAGE_WIDTH;
        image_height = IMAGE_HEIGHT;

        glutInitWindowSize(image_width, image_height);
        glutCreateWindow("QDOT");
        glutInitWindowPosition(200, 200);

        if(argc == 2)
        {
            init(argv[1]);
        }
        else
        {
            exit(0);
        }
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(key);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);

        glutMainLoop();
        return 0;
}
