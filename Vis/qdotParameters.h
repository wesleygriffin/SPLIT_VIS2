
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

//#include <GL/glut.h>
#include <assert.h>
#include <ctime>
#include <unistd.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
//#include <X11/Xlib.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <cstdlib>

#include "GL/glui.h"

#include "svQDOT.h"
#include "svColor.h"
#include "ivTrackball.h"
#include "svArrowSelection.h"
//#include "svCut.h"
#include "svOutline.h"
#include "svQDOTdistancemap.h"
#include "svSlice.h"
#include "ivview.h"
//#include "svSpaceSlider.h"
//#include "svSlider.h"
//#include "svBubbleSet.h"
#include "svPickingray.h"
#include "svDisc.h"
#include "svMesh.h"

using namespace __svl_lib;
using namespace std;



extern GLuint window, right_window, left_window, Legend_left_window, Legend_right_window, Legend;

extern int image_width, image_height;
extern GLuint sub_width,sub_height;
extern int legend_image_width, legend_image_height;

extern GLdouble vmat[16], pmat[16];
extern GLint    viewport[4];

extern GLint mouse_state;
extern GLint mouse_button;
extern int mouse_x;
extern int mouse_y;
extern int mouse_left_x;
extern int mouse_left_y;
extern int mouse_right_x;
extern int mouse_right_y;
extern view3d view_info;

//------------parameters--------------

extern svQDOT *qdotField[2];
extern svArrowSelection *qdotArrowSelection[2]; 
extern svColor *qdotColor;
//extern svCut *qdotCut[2];
extern svOutline *qdotOutline[2];
extern svQDOTdistancemap *qdotMap[2];
extern svPickingray *qdotPicking[2];
extern svDisc *qdotDisc[2];
extern svSlice *qdotCutSlice[2];
extern svMesh *qdotMesh[2];

//--------------------windows----------------------------------------
extern GLUI *glui[2];
extern GLUI *glui_visual;
extern GLUI *glui_data_pop1;
extern GLUI *glui_data_pop2;
extern GLUI *glui_cutting[2];

extern char LogFile[50];
extern char tmpfiledir[50];
extern char picturedir[50];

//--------------------ids----------------------------
extern int Loaddata_id[2];
extern int Loaddata_List_id[2];
extern int Loaddata_OK_id[2];
extern int Loaddata_Cancel_id[2];

extern int Display_id[2];
extern int Encode_id[2];
extern int Color_id[2];
extern int Shape_id[2];
extern int Overview_id[2];

extern int Contour_Sync_id[2];
extern int Contour_Edit_id[2];

extern int Cluster_Sync_id[2];
extern int Cluster_Type_id[2];
extern int Cluster_Layer_id[2]; 
extern int Cutting_Edit_id[2];
extern int Cutting_Move_id[2];
extern int Cutting_List_id[2];
extern int Cluster_Num_id0[2];
extern int Cluster_Num_id1[2];
extern int Cutting_Distance_id[2];
extern int Lweight_id0[2];
extern int Vweight_id0[2];
extern int Dweight_id0[2];
extern int MRange_id00[2];
extern int MRange_id01[2];
extern int Lweight_id1[2];
extern int Vweight_id1[2];
extern int Dweight_id1[2];
extern int MRange_id10[2];
extern int MRange_id11[2];

extern int Cuttingok_id[2];
extern int Cuttingbutton_id[2];

extern int Distance_Sync_id[2];
extern int Distance_Sample_id[2];
extern int Distance_Point_id[2];//no function
extern int Distance_List_id[2];
extern int Distance_Z_id[2];

extern int Region_id[2];

extern int Update_id[2];

extern int Background_id;
extern int Opacity_id;
extern int Arrow_id;
extern int Length_id;
extern int Blind_id;
extern int Frequency_id;
extern int Cutvisible_id;

extern int Rerender_id;
extern int Picture_id;
extern int Save_id;
extern int Exit_id;

//--------------------glui---------------------------
extern GLUI_Button *glui_loaddata[2];
extern GLUI_Button *glui_dataok[2];
extern GLUI_Button *glui_datacancel[2];
extern GLUI_List *glui_datalist[2];
extern GLUI_StaticText *text_data[2];

extern GLUI_Listbox *glui_display[2];
extern GLUI_Listbox *glui_encode[2];
extern GLUI_Listbox *glui_color[2];
extern GLUI_Listbox *glui_shape[2];
extern GLUI_Checkbox *glui_overview[2];

extern GLUI_Checkbox *glui_contoursync[2];
extern GLUI_TextBox *glui_contouredit[2];

extern GLUI_Checkbox *glui_clustersync[2];
extern GLUI_Listbox *glui_clustertype[2];
extern GLUI_Checkbox *glui_clusterlayer[2];
extern GLUI_TextBox *glui_cuttingedit[2];
extern GLUI_EditText *glui_cuttingmove[2];
extern GLUI_TextBox *glui_cuttinglist[2];
extern GLUI_TextBox *glui_cuttingscroll[2];
extern GLUI_StaticText *glui_LWtext0[2];
extern GLUI_Scrollbar *sb_location0[2];
extern GLUI_StaticText *glui_VWtext0[2];
extern GLUI_Scrollbar *sb_velocity0[2];
extern GLUI_StaticText *glui_DWtext0[2];
extern GLUI_Scrollbar *sb_density0[2];
extern GLUI_StaticText *glui_Mtext00[2];
extern GLUI_StaticText *glui_Mtext01[2];
extern GLUI_EditText *sb_magrange00[2];
extern GLUI_EditText *sb_magrange01[2];
extern GLUI_EditText *glui_cluster_text0[2];
extern GLUI_StaticText *glui_LWtext1[2];
extern GLUI_Scrollbar *sb_location1[2];
extern GLUI_StaticText *glui_VWtext1[2];
extern GLUI_Scrollbar *sb_velocity1[2];
extern GLUI_StaticText *glui_DWtext1[2];
extern GLUI_Scrollbar *sb_density1[2];
extern GLUI_EditText *sb_magrange10[2];
extern GLUI_EditText *sb_magrange11[2];
extern GLUI_StaticText *glui_Mtext10[2];
extern GLUI_StaticText *glui_Mtext11[2];
extern GLUI_EditText *glui_cluster_text1[2];

extern GLUI_Checkbox *glui_distancesync[2];
extern GLUI_EditText *glui_distancesample[2];
extern GLUI_Checkbox *glui_distancepoint[2];
extern GLUI_TextBox *glui_distanceedit[2];
extern GLUI_Checkbox *glui_distancez[2];

extern GLUI_Listbox *glui_mesh[2];

extern GLUI_StaticText *glui_background;
extern GLUI_StaticText *glui_opacity;
extern GLUI_StaticText *glui_frequency;
extern GLUI_StaticText *glui_base;
extern GLUI_StaticText *glui_length;
//--------------------variables-------------------------
extern vector<string> datastring;//need to fix
extern vector<string> folderstring; //not used
extern char datadir[2][100];
extern char datafile[2][100];
extern bool qdotNewdata[2];
extern bool qdotNewArrowData[2];
extern bool qdotNewArrowContour[2];

extern int qdotDisplay[2];
extern int qdotEncode[2];
extern int qdotColorby[2];
extern int qdotShape[2];
extern int qdotOverview[2];

extern int qdotContourSync;
extern vector< vector<double> > qdotContourLAP[2];
extern vector< vector<double> > qdotContourLRP[2];

extern vector<double> qdotContourLR[2];
extern vector<double> qdotContourLA[2];
extern vector<double> qdotContourGR[2];
extern vector<double> qdotContourGA[2];

extern bool qdotNewContour[2];
extern bool qdotNewLayerContour[2];

extern int qdotClusterSync;
extern int qdotClusterType[2];
extern int qdotClusterlayer[2];

extern svVector3 qdotCuttingV[2];
extern svVector3 qdotCuttingN[2];
extern float qdotCuttingMove[2];

extern vector<int> qdotCuttingSingle[2];
extern vector< vector<int> > qdotCuttingMulti[2];

extern svScalar qdotMDegree[2]; //the minimum degree of movement 
extern float qdotLweight[2][2];
extern float qdotVweight[2][2];
extern float qdotDweight[2][2];
extern float qdotMagRange0[2][2];
extern float qdotMagRange1[2][2];
extern int qdotHCNum[2][2];

extern bool qdotNewCluster[2];
extern bool qdotNewContourCluster[2];

extern int qdotDistanceSync;
extern int qdotDistanceSample[2];
extern int qdotDistancePoint[2];
extern int qdotDistanceZ[2];
extern vector<svVector3> qdotDistanceV[2];
extern bool qdotNewDistance[2];

extern bool qdotNewArrow[2];//????????????????

extern int qdotRegion[2];
extern vector<int> qdotRegionList[2];


extern float qdotBack;
extern float qdotOpacity;
extern float qdotLength;
extern float qdotArrowBase;
extern int qdotBlind;
extern int qdotFrequency;
extern svVector3 qdotFont; 
extern int qdotCutVisible;

extern svScalar qdotScalar;

//checkpoint!!!!!!!!!!!!!!!!!!!!!


//----------------------------------------------------------------------------------------------------

extern Trackball trackball[2];
extern Trackball trackball_twod[2];
extern Trackball trackball_legend[2];


extern float twodzoom[2];
extern float twotranx[2];//= 0.;
extern float twotrany[2];// = 0.;

extern int twod_mouse_state;
extern int twod_mouse_y;

extern GLfloat m[2][16];
extern GLfloat m1[2][16];
extern GLfloat mleft[2][16];
extern __svl_lib::Matrix4f zoomintwod[2];
extern __svl_lib::Matrix4f panintwod[2];
extern float transformation_mat[2][16];

extern int s_index;//selected item in plane panel
extern int d_index;

