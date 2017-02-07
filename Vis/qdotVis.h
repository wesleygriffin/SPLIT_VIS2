#include "qdotFile.h"

//-----------
  GLuint window, right_window, left_window, Legend_left_window, Legend_right_window, Legend;

  int image_width, image_height;
  GLuint sub_width,sub_height;
  int legend_image_width, legend_image_height;

  GLdouble vmat[16], pmat[16];
  GLint    viewport[4];

  GLint mouse_state;
  GLint mouse_button;
  int mouse_x;
  int mouse_y;

  view3d view_info;

//------------------------------------

  svQDOT *qdotField[2];
  svArrowSelection *qdotArrowSelection[2]; 
  svColor *qdotColor;
  svCut *qdotCut[2];
  svOutline *qdotOutline[2];
  svQDOTdistancemap *qdotMap[2];
  svBubbleSet *qdotBubble[2];
  svSlice *qdotCutSlice[2];
  svSlice *qdotSymmetrySlice[2];
  svSpaceSlider *qdotSSlider[2];

//--------------------------------------------------------------------
  GLUI *glui;
  GLUI *glui_data_pop1;
  GLUI *glui_data_pop2;
  GLUI *glui_plane_pop;//symmetry
  GLUI *glui_distance_pop;
  GLUI *glui_contour_pop;
  GLUI *glui_cluster_pop;
  GLUI *glui_cutting_pop;

  char LogFile[50];
  char tmpfiledir[50];
  char picturedir[50];

//---------------------------------------------------------------------------------
//data
  GLUI_Button *glui_loaddata[2];
  GLUI_Button *glui_dataok[2];
  GLUI_Button *glui_datacancel[2];
  GLUI_List *glui_datalist[2];
  vector<string> datastring;//need to fix
  vector<string> folderstring; //not used
  char datadir[2][100];
  char datafile[2][100];
  bool qdotNewdata[2];
//show some data information.
//a static box
  GLUI_TextBox *glui_information; //name, x,y,z,mag
//contour, raw, 2d 
  GLUI_Listbox *glui_display[2];
  int qdotDisplay[2];
//contour
  GLUI_Button *glui_contourpop;
  GLUI_Checkbox *glui_contoursync;
  GLUI_TextBox *glui_contouredit[2];
  GLUI_Button *glui_contourok;
  GLUI_Button *glui_contourcancel;
  int qdotContourSync;
  vector< vector<double> > qdotContourLAP[2];
  vector< vector<double> > qdotContourLRP[2];
  vector<double> qdotContourLR[2];
  vector<double> qdotContourLA[2];
  vector<double> qdotContourGR[2];
  vector<double> qdotContourGA[2];
  bool qdotNewcontour[2];
//roi
  GLUI_Checkbox *glui_roisync;
  GLUI_Checkbox *glui_roirefer[2];
  GLUI_Checkbox *glui_roiedit[2];
  GLUI_Listbox *glui_roidisplay[2];
  int qdotROISync;
  int qdotROIRefer[2];
  int qdotROIEdit[2];
  int qdotROIdisplay[2];
  bool qdotNewROI[2];
//symmetry
  GLUI_Button *glui_symmetrypop;
  GLUI_Checkbox *glui_symmetryvisible;
  GLUI_Checkbox *glui_symmetrysync;
  GLUI_TextBox *glui_symmetryedit[2];
  GLUI_Listbox *glui_symmetrytype[2];
  GLUI_Button *glui_symmetryok;
  GLUI_Button *glui_symmetrycancel;
  int qdotSymmetryVisible;
  int qdotSymmetrySync;
  vector<svVector3> qdotSymmetryV[2];
  vector<svVector3> qdotSymmetryN[2];
  int qdotSymmetryType[2];
  bool qdotNewSymmetry[2];
//clustering
  GLUI_Button *glui_clusterpop;
  GLUI_Checkbox *glui_clustersync;
  GLUI_Checkbox *glui_clustertogether;
  GLUI_Listbox *glui_clustertype[2];
  GLUI_Checkbox *glui_clusterROI[2];
  GLUI_EditText *glui_clusternum[2];
  GLUI_Button *glui_clusterok;
  GLUI_Button *glui_clustercancel;
  int qdotClusterSync;
  int qdotClusterTogether;
  int qdotClusterType[2];
  int qdotClusterROI[2];
  int qdotClusterNum[2];
  int qdotCluster[2][2][2];
  bool qdotNewCluster[2];
//distancemap
  GLUI_Button *glui_distancepop;
  GLUI_Checkbox *glui_distancesync;
  GLUI_EditText *glui_distancesample;
  GLUI_Checkbox *glui_distancepoint;
  GLUI_TextBox *glui_distancedit[2];
  GLUI_StaticText *glui_distancezshow[2];
  GLUI_Checkbox *glui_distancez[2];
  GLUI_Button *glui_distanceok;
  GLUI_Button *glui_distancecancel;
  int qdotDistanceSync;
  int qdotDistanceSample;
  int qdotDistancePoint;
  vector<svVector3> qdotDistanceV[2];
  float qdotDistanceZ[2];
  int qdotDistanceZshow[2];
  bool qdotNewDistance[2];
//cuttingplane
  GLUI_Button *glui_cuttingpop;
  GLUI_Checkbox *glui_cuttingsync;
  GLUI_Checkbox *glui_cuttingvisible;
  GLUI_EditText *glui_cuttingv;
  GLUI_EditText *glui_cuttingn;
  int qdotCuttingSync;
  int qdotCuttingVisible;
  svVector3 qdotCuttingV[2];
  svVector3 qdotCuttingN[2];
  bool qdotNewCutting[2];
//projection
  GLUI_Checkbox *glui_projectionvisible;
  GLUI_Checkbox *glui_projectionsync;
  GLUI_Listbox *glui_projectiontype[2];
  int qdotProjectionVisible;
  int qdotProjectionSync;
  int qdotProjectionType[2];
//others
  GLUI_Listbox *glui_encode[2];
  GLUI_StaticText *glui_background;
  GLUI_StaticText *glui_opacity;
  GLUI_StaticText *glui_frequency;
  int qdotEncode[2];
  svVector3 qdotBack;
  float qdotOpacity;
  int qdotFrequency;
  int qdotBlind;
  int qdotBoxVisible;
//----------------------------------------------------------------------------------------------------

  Trackball trackball[2];
  Trackball trackball_twod[2];
  Trackball trackball_legend[2];


  float twodzoom[2];
  float twotranx[2];//= 0.;
  float twotrany[2];// = 0.;

  int twod_mouse_state;
  int twod_mouse_y;

  GLfloat m[2][16];
  GLfloat m1[2][16];
  GLfloat mleft[2][16];
  __svl_lib::Matrix4f zoomintwod[2];
  __svl_lib::Matrix4f panintwod[2];
  float transformation_mat[2][16];

  int s_index;//selected item in plane panel
  int d_index;

