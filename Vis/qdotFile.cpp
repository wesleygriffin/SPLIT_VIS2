#include "qdotFile.h"


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
 int mouse_left_x;
 int mouse_left_y;
 int mouse_right_x;
 int mouse_right_y;
 view3d view_info;

//------------parameters--------------

 svQDOT *qdotField[2];
 svArrowSelection *qdotArrowSelection[2]; 
 svColor *qdotColor;
// svCut *qdotCut[2];
 svOutline *qdotOutline[2];
 svQDOTdistancemap *qdotMap[2];
 svPickingray *qdotPicking[2];
 svDisc *qdotDisc[2];
 //svSlider *qdotSlider[2];
//JC17
 //svMesh *qdotMesh[2];
  svSlice *qdotCutSlice[2];

//--------------------windows----------------------------------------
 char LogFile[50];
 char tmpfiledir[50];
 char picturedir[50];

//--------------------ids----------------------------
 int Loaddata_id[2];
 int Loaddata_List_id[2];
 int Loaddata_OK_id[2];
 int Loaddata_Cancel_id[2];

 int Display_id[2];
 int Encode_id[2];
 int Color_id[2];
 int Shape_id[2];
 int Overview_id[2];

 int Contour_Sync_id[2];
 int Contour_Edit_id[2];

 int Cluster_Num_id0[2];
 int Cluster_Num_id1[2];
 int Cluster_Sync_id[2];
 int Cluster_Type_id[2];
 int Cluster_Layer_id[2];
 int Cutting_Edit_id[2];
 int Cutting_List_id[2];
 int Cutting_Move_id[2];
 int Cutting_Distance_id[2];
 int Lweight_id0[2];
 int Vweight_id0[2];
 int Dweight_id0[2];
 int MRange_id00[2];
 int MRange_id01[2];
 int Lweight_id1[2];
 int Vweight_id1[2];
 int Dweight_id1[2];
 int MRange_id10[2];
 int MRange_id11[2];


 int Cuttingok_id[2];
 int Cuttingbutton_id[2];

 int Distance_Sync_id[2];
 int Distance_Sample_id[2];
 int Distance_Point_id[2];
 int Distance_List_id[2];
 int Distance_Z_id[2];

 int Region_id[2];

 int Update_id[2];

 int Background_id;
 int Opacity_id;
 int Arrow_id;
 int Length_id;
 int Blind_id;
 int Frequency_id;
 int Cutvisible_id;

 int Rerender_id;
 int Picture_id;
 int Save_id;
 int Exit_id;

//--------------------variables-------------------------
 vector<string> datastring;//need to fix
 vector<string> folderstring; //not used
 char datadir[2][100];
 char datafile[2][100];
 bool qdotNewdata[2];

 int qdotDisplay[2];
 int qdotEncode[2];
 int qdotColorby[2];
 int qdotShape[2];
 int qdotOverview[2];

 int qdotContourSync;
 vector< vector<double> > qdotContourLAP[2];
 vector< vector<double> > qdotContourLRP[2];
 vector<double> qdotContourLR[2];
 vector<double> qdotContourLA[2];
 vector<double> qdotContourGR[2];
 vector<double> qdotContourGA[2];
 bool qdotNewContour[2];
 bool qdotNewLayerContour[2];

 int qdotClusterSync;
 int qdotClusterType[2];
 int qdotClusterlayer[2];
 svVector3 qdotCuttingV[2];
 svVector3 qdotCuttingN[2];
 float qdotCuttingMove[2];
 vector<int> qdotCuttingSingle[2];
 vector< vector<int> > qdotCuttingMulti[2];
 svScalar qdotMDegree[2]; //the minimum degree of movement 
 float qdotLweight[2][2];
 float qdotVweight[2][2];
 float qdotDweight[2][2];
 float qdotMagRange0[2][2];
 float qdotMagRange1[2][2];
 int qdotHCNum[2][2];
 bool qdotNewCluster[2];
 bool qdotNewContourCluster[2];
 bool qdotNewArrowData[2];
 bool qdotNewArrowContour[2];

 int qdotDistanceSync;
 int qdotDistanceSample[2];
 int qdotDistancePoint[2];
 int qdotDistanceZ[2];
 vector<svVector3> qdotDistanceV[2];
 bool qdotNewDistance[2];

 bool qdotNewArrow[2];//????????????????

 int qdotRegion[2];
 vector<int> qdotRegionList[2];

 float qdotBack;
 float qdotOpacity;
 float qdotLength;
 float qdotArrowBase;
 int qdotBlind;
 int qdotFrequency;
svVector3 qdotFont; 
 int qdotCutVisible;

 //svVector3 qdotFont;
 svScalar qdotScalar;

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



//http://www.cplusplus.com/forum/beginner/9173/
void ListFile(char *dir)
{
	datastring.clear();

      	DIR *pDIR;
        struct dirent *entry;
	int i=0;
        if( pDIR=opendir(dir) )
	{
                while(entry = readdir(pDIR)){
                        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
				datastring.push_back(entry->d_name);
		                //cout << entry->d_name << "\n";
				cout <<datastring[i]<<endl;
				i++;
			}
                }
                closedir(pDIR);
        }	
}

//http://stackoverflow.com/questions/5043403/listing-only-folders-in-directory
/*void ListFolder(char *dir2)
{
    DIR *dir = opendir(dir2);

    folderstring.clear();

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
			folderstring.push_back( entry->d_name);
		   // printf("%s\n", entry->d_name);
	}

        entry = readdir(dir);
    }

    closedir(dir);

   // return 0;
}*/

void InitFile(char *file)
{
 	ifstream inparameters(file);

  	string tmp;

	if(inparameters.fail())
	{
		cerr<<"No input parameter file"<<endl;
		exit(-1);
	}

	float store;
	
	//-----------data name-------------------------
	inparameters>>tmp;//data file name
	inparameters>>tmp;//data file name
	inparameters>>datadir[0];
	inparameters>>datafile[0];
	inparameters>>tmp;//data file name
	cerr<<tmp<<endl;
	inparameters>>datadir[1];
	inparameters>>datafile[1];

	sprintf(LogFile, "../data/Log/");
	sprintf(picturedir, "../data/Pictures/");
	sprintf(tmpfiledir, "../data/tmp/");
	//---------------------------------------------


	//-----------display----------------------------
	qdotDisplay[0] = 0;//D_Raw;//Contour;
	qdotDisplay[1] = 2;//D_Raw;//Contour;
	//---------------------------------------------

	char linestr[20];

	//------------contour-----------------------------
        inparameters>>tmp; // contours
	cerr<<tmp<<endl;
        inparameters>>tmp; // contours
	cerr<<tmp<<endl;
	inparameters>>linestr;
	cerr<<linestr<<endl;
	while(strcmp(linestr,"Data2:"))
	{
		if(!strcmp(linestr,"G"))
		{
			char linestr2[10];
			inparameters>>linestr2;

			if(!strcmp(linestr2,"A"))
			{
				inparameters>>store;
				qdotContourGA[0].push_back(store);
			}
			else
			{
				inparameters>>store;
				qdotContourGR[0].push_back(store);
			}
		}
		else
		{
			char linestr2[10];
			inparameters>>linestr2;
			if(!strcmp(linestr2,"A"))
			{
				inparameters>>tmp;
				vector<double> store3(2);
				inparameters>>store3[0]>>store3[1];

				inparameters>>tmp;

				qdotContourLAP[0].push_back(store3);

				inparameters>>store;
				qdotContourLA[0].push_back(store);
			}
			else
			{
				inparameters>>tmp;
				vector<double> store3(2);
				inparameters>>store3[0]>>store3[1];

				inparameters>>tmp;
				qdotContourLRP[0].push_back(store3);

				inparameters>>store;
				qdotContourLR[0].push_back(store);
			}			
		}
		inparameters>>linestr;
	}

	inparameters>>linestr;
	while(strcmp(linestr,"#Num_of_Clustering"))
	{
		cerr<<linestr<<endl;
		if(!strcmp(linestr,"G"))
		{
			char linestr2[10];
			inparameters>>linestr2;

			if(!strcmp(linestr2,"A"))
			{
				inparameters>>store;
				qdotContourGA[1].push_back(store);
			}
			else
			{
				inparameters>>store;
				qdotContourGR[1].push_back(store);
			}
		}
		else
		{
			char linestr2[10];
			inparameters>>linestr2;
			if(!strcmp(linestr2,"A"))
			{
				inparameters>>tmp;
				vector<double> store3(2);
				inparameters>>store3[0]>>store3[1];

				inparameters>>tmp;
				qdotContourLAP[1].push_back(store3);

				inparameters>>store;
				qdotContourLA[1].push_back(store);
			}
			else
			{
				inparameters>>tmp;
				vector<double> store3(2);
				inparameters>>store3[0]>>store3[1];

				inparameters>>tmp;
				qdotContourLRP[1].push_back(store3);

				inparameters>>store;
				qdotContourLR[1].push_back(store);
			}			
		}
		inparameters>>linestr;
	}
	//---------------------------------------------------------------------------

	//----------------------------cluster-------------------------------------------
	inparameters>>tmp;//cluster num
	cerr<<tmp<<endl;
	//inparameters>>linestr;
	//inparameters>>tmp;
	inparameters>>store;
	qdotHCNum[0][0] = (int)store;
	inparameters>>store;
	qdotHCNum[0][1] = (int)store;
	inparameters>>tmp;
	inparameters>>store;
	qdotHCNum[1][0] = (int)store;
	inparameters>>store;
	qdotHCNum[1][1] = (int)store;
	/*if(strcmp(linestr, "Yes"))
	{
		qdotClusterTogether = 0;
	}
	else
	{
		qdotClusterTogether = 1;
	}*/
	//qdotClusterTogether  = 0;

	qdotClusterType[0] = 0;//one layer
	qdotClusterType[1] = 0;
	qdotClusterSync = 0;
	qdotClusterlayer[0] = 0;
	qdotClusterlayer[1] = 0;
	//qdotClusterROI[0] = 1;
	//qdotClusterROI[1] = 1;

	/*qdotCluster[0][0][0] = 13;
	qdotCluster[0][0][1] = qdotClusterNum[0];
	qdotCluster[0][1][0] = 13;
	qdotCluster[0][1][1] = qdotClusterNum[0];
	qdotCluster[1][0][0] = 13;
	qdotCluster[1][0][1] = qdotClusterNum[1];
	qdotCluster[1][1][0] = 13;
	qdotCluster[1][1][1] = qdotClusterNum[1];*/
	//----------------------------------------------------------------------------------

	//--------------------------distance-----------------------------------
        inparameters>>tmp;//reference layer
	cerr<<tmp<<endl;
        inparameters>>tmp;//reference layer
        inparameters>>store;
	qdotDistanceZ[0]=store;//cerr<<refer_z[0]<<endl;
        inparameters>>tmp;//reference layer
	inparameters>>store;
	qdotDistanceZ[1] = store;

	inparameters>>tmp;//input vector
	cerr<<tmp<<endl;
	inparameters>>tmp;//input vector
	inparameters>>tmp>>store;
	qdotDistanceV[0].resize((int)store);
	for(int i=0;i<(int)store;i++)
        {
		inparameters>>tmp>>qdotDistanceV[0][i][2]>>tmp>>qdotDistanceV[0][i][0]>>tmp>>qdotDistanceV[0][i][1];
	}
	inparameters>>tmp;//input vector
	inparameters>>tmp>>store;
	qdotDistanceV[1].resize((int)store);
	for(int i=0;i<(int)store;i++)
        {
		inparameters>>tmp>>qdotDistanceV[1][i][2]>>tmp>>qdotDistanceV[1][i][0]>>tmp>>qdotDistanceV[1][i][1];
	}
	//--------------------------------------------------------------------------

	//----------------others--------------------
	qdotEncode[0] = 0;// LINEAR;//SPLITVECTORS;
	qdotEncode[1] = 0;//LINEAR;//SPLITVECTORS;
//	qdotFrequency = 8;

	//--------------------cutting plane----------------------------	
	inparameters>>tmp;//cutting plane 
	cerr<<tmp<<endl;
	inparameters>>tmp;//cutting plane 
cerr<<tmp<<endl;
        inparameters>>tmp>>qdotCuttingV[0][0]>>qdotCuttingV[0][1]>>qdotCuttingV[0][2];cerr<<tmp<<endl;
        inparameters>>tmp>>qdotCuttingN[0][0]>>qdotCuttingN[0][1]>>qdotCuttingN[0][2];cerr<<tmp<<endl;
	inparameters>>tmp>>qdotMDegree[0];cerr<<tmp<<endl;
	inparameters>>tmp;//cutting plane 
cerr<<tmp<<endl;
        inparameters>>tmp>>qdotCuttingV[1][0]>>qdotCuttingV[1][1]>>qdotCuttingV[1][2];cerr<<tmp<<endl;
        inparameters>>tmp>>qdotCuttingN[1][0]>>qdotCuttingN[1][1]>>qdotCuttingN[1][2];cerr<<tmp<<endl;
	inparameters>>tmp>>qdotMDegree[1];cerr<<tmp<<endl;

	//qdotMDegree[0] = 0.5* sqrt(2);
	//qdotMDegree[1] = 0.5* sqrt(2);

	//cout<<qdotCuttingN[0][0]<<endl;
	//----------------------------------

	cerr<<"Read Config File...done"<<endl;

	//---------------

	qdotNewdata[0] = false;
	qdotNewdata[1] = false;

	qdotColorby[0] = 0; //cluster
	qdotColorby[1] = 0; //cluster
	qdotShape[0] = 0;
	qdotShape[1] = 0;
	qdotOverview[0] = 0;
	qdotOverview[1] = 0;

	qdotContourSync = 0;
	qdotNewContour[0] = false;
	qdotNewContour[1] = false;
	qdotNewLayerContour[0] = false;
	qdotNewLayerContour[1] = false;

	qdotCuttingMove[0] = 0; //set based on data
	qdotCuttingMove[1] = 0;

	qdotLweight[0][0] = 1;
	qdotVweight[0][0] = 1;
	qdotDweight[0][0] = 1;
	qdotLweight[0][1] = 1;
	qdotVweight[0][1] = 1;
	qdotDweight[0][1] = 1;
	qdotLweight[1][0] = 1;
	qdotVweight[1][0] = 1;
	qdotDweight[1][0] = 1;
	qdotLweight[1][1] = 1;
	qdotVweight[1][1] = 1;
	qdotDweight[1][1] = 1;

	//----------qdotMagRange0 based on data----------------

	qdotNewCluster[0] = false;
	qdotNewCluster[1] = false;
	qdotNewContourCluster[0] = false;
	qdotNewContourCluster[1] = false;
	qdotNewArrowData[0] = false;
	qdotNewArrowData[1] = false;
	qdotNewArrowContour[0] = false;
	qdotNewArrowContour[1] = false;

	qdotDistanceSync = 0;
	qdotDistanceSample[0] = 8;
	qdotDistanceSample[1] = 8;
	qdotDistancePoint[0] = 0;
	qdotDistancePoint[1] = 0;
	qdotNewDistance[0] = false;
	qdotNewDistance[1] = false;

	qdotNewArrow[0] = false;
	qdotNewArrow[1] = false;

	qdotRegion[0] = 0;
	qdotRegion[1] = 0;

	//qdotregionlist from data

	qdotBack = 0.5;
	qdotFont[0] = 1;
	qdotFont[1] = 1;
	qdotFont[2] = 1;
	qdotBlind = 1;
	twodzoom[0]=1;
	twodzoom[1]=1;
	twotranx[0]=0;
	twotranx[1]=0;
	twotrany[0]=0;
	twotrany[1]=0;
	qdotOpacity=1;
	qdotFrequency=1;
	qdotArrowBase = 0.25;
	qdotCutVisible = 0;
	//if(qdotEncode == 1)
	//	qdotLength = 0.25 * 5e-11; //for length
	//else
		qdotLength = 0.25; //for length

	//qdot scalar from data


Loaddata_id[0]=1;
Loaddata_id[1]=2;
Loaddata_List_id[0]=3;
Loaddata_List_id[1]=4;
Loaddata_OK_id[0]=5;
Loaddata_OK_id[1]=6;
Loaddata_Cancel_id[0]=7;
Loaddata_Cancel_id[1]=8;

Display_id[0]=9;
Display_id[1]=10;
Encode_id[0]=11;
Encode_id[1]=12;
Color_id[0]=13;
Color_id[1]=14;
Shape_id[0]=15;
Shape_id[1]=16;
Overview_id[0]=17;
Overview_id[1]=18;

Contour_Sync_id[0]=19;
Contour_Edit_id[0]=20;
Contour_Edit_id[1]=21;

Cluster_Sync_id[0]=22;
Cluster_Type_id[0]=23;
Cluster_Layer_id[0]=24;
Cutting_Edit_id[0]=25;
Cutting_Move_id[0]=26;
Lweight_id0[0]=27;
Vweight_id0[0]=28;
Dweight_id0[0]=29;
MRange_id00[0]=30;
MRange_id01[0]=31;
Lweight_id1[0]=32;
Vweight_id1[0]=33;
Dweight_id1[0]=34;
MRange_id10[0]=35;
MRange_id11[0]=36;
Cluster_Type_id[1]=37;
Cluster_Layer_id[1]=38;
Cutting_Edit_id[1]=39;
Cutting_Move_id[1]=40;
Lweight_id0[1]=41;
Vweight_id0[1]=42;
Dweight_id0[1]=43;
MRange_id00[1]=44;
MRange_id01[1]=45;
Lweight_id1[1]=46;
Vweight_id1[1]=47;
Dweight_id1[1]=48;
MRange_id10[1]=49;
MRange_id11[1]=50;

Distance_Sync_id[0]=51;
Distance_Sample_id[0]=52;
Distance_Point_id[0]=53;
Distance_List_id[0]=54;
Distance_Z_id[0]=55;
Distance_Sample_id[1]=56;
Distance_Point_id[1]=57;
Distance_List_id[1]=58;
Distance_Z_id[1]=59;
Region_id[0]=60;
Update_id[0]=61;
Region_id[1]=62;
Update_id[1]=63;
Background_id=64;
Opacity_id=65;
Arrow_id=66;
Length_id=67;
Blind_id=68;
Frequency_id=69;
Picture_id=70;
Save_id=71;
Exit_id=72;

Cluster_Num_id0[0] = 73;
Cluster_Num_id0[1] = 74;
Contour_Sync_id[1]=75;
Cluster_Sync_id[1]=76;
Distance_Sync_id[1]=77;
Cutting_Distance_id[0]=78;
Cutting_Distance_id[1]=79;
Cluster_Num_id1[0] = 80;
Cluster_Num_id1[1] = 81;

Cutvisible_id = 82;
Rerender_id = 83;

Cuttingok_id[0] = 84;
Cuttingok_id[1] = 85;
Cuttingbutton_id[0] = 86;
Cuttingbutton_id[1] = 87;
Cutting_List_id[0] = 88;
Cutting_List_id[1] = 89;
}


