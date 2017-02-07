#include "qdotGUI.h"

 GLUI *glui[2];
 GLUI *glui_visual;
 GLUI *glui_data_pop1;
 GLUI *glui_data_pop2;
 GLUI *glui_cutting[2];

 GLUI_Button *glui_loaddata[2];
 GLUI_Button *glui_dataok[2];
 GLUI_Button *glui_datacancel[2];
 GLUI_List *glui_datalist[2];
 GLUI_StaticText *text_data[2];

 GLUI_Listbox *glui_display[2];
 GLUI_Listbox *glui_encode[2];
 GLUI_Listbox *glui_color[2];
 GLUI_Listbox *glui_shape[2];
 GLUI_Checkbox *glui_overview[2];

 GLUI_Checkbox *glui_contoursync[2];
 GLUI_TextBox *glui_contouredit[2];

 GLUI_Checkbox *glui_clustersync[2];
 GLUI_Listbox *glui_clustertype[2];
 GLUI_Checkbox *glui_clusterlayer[2];
 GLUI_TextBox *glui_cuttingedit[2];
 GLUI_EditText *glui_cuttingmove[2];
 GLUI_TextBox *glui_cuttinglist[2];
 GLUI_TextBox *glui_cuttingscroll[2];
 GLUI_StaticText *glui_LWtext0[2];
 GLUI_Scrollbar *sb_location0[2];
 GLUI_StaticText *glui_VWtext0[2];
 GLUI_Scrollbar *sb_velocity0[2];
 GLUI_StaticText *glui_DWtext0[2];
 GLUI_Scrollbar *sb_density0[2];
 GLUI_EditText *sb_magrange00[2];
 GLUI_EditText *sb_magrange01[2];
 GLUI_EditText *glui_cluster_text0[2];
 GLUI_StaticText *glui_LWtext1[2];
 GLUI_Scrollbar *sb_location1[2];
 GLUI_StaticText *glui_VWtext1[2];
 GLUI_Scrollbar *sb_velocity1[2];
 GLUI_StaticText *glui_DWtext1[2];
 GLUI_Scrollbar *sb_density1[2];
 GLUI_EditText *sb_magrange10[2];
 GLUI_EditText *sb_magrange11[2];
 GLUI_EditText *glui_cluster_text1[2];
 GLUI_StaticText *glui_Mtext00[2];
 GLUI_StaticText *glui_Mtext01[2];
 GLUI_StaticText *glui_Mtext10[2];
 GLUI_StaticText *glui_Mtext11[2];

 GLUI_Checkbox *glui_distancesync[2];
 GLUI_EditText *glui_distancesample[2];
 GLUI_Checkbox *glui_distancepoint[2];
 GLUI_TextBox *glui_distanceedit[2];
 GLUI_Checkbox *glui_distancez[2];

 GLUI_Listbox *glui_mesh[2];

 GLUI_StaticText *glui_background;
 GLUI_StaticText *glui_opacity;
 GLUI_StaticText *glui_frequency;
 GLUI_StaticText *glui_length;
 GLUI_StaticText *glui_base;
/*

 GLUI *glui[2];
 GLUI *glui_data_pop1;
 GLUI *glui_data_pop2;
 GLUI *glui_plane_pop;//symmetry
 GLUI *glui_distance_pop;
 GLUI *glui_contour_pop;
 GLUI *glui_cluster_pop;
 GLUI *glui_cutting_pop;
 GLUI *glui_symmetry_pop;
 GLUI *glui_information_pop;
 GLUI *glui_roi_pop;

//---------------------------------------------------------------------------------
//layer clustering
GLUI *glui_lcluster;
GLUI *glui_rcluster;
 GLUI_StaticText *glui_LWtext00;
 GLUI_StaticText *glui_VWtext00;
 GLUI_StaticText *glui_DWtext00;
 GLUI_StaticText *glui_LWtext01;
 GLUI_StaticText *glui_VWtext01;
 GLUI_StaticText *glui_DWtext01;
 GLUI_StaticText *glui_LWtext10;
 GLUI_StaticText *glui_VWtext10;
 GLUI_StaticText *glui_DWtext10;
 GLUI_StaticText *glui_LWtext11;
 GLUI_StaticText *glui_VWtext11;
 GLUI_StaticText *glui_DWtext11;
//data
 GLUI_Button *glui_loaddata[2];
 GLUI_Button *glui_dataok[2];
 GLUI_Button *glui_datacancel[2];
 GLUI_List *glui_datalist[2];
//show some data information.
//a static box
 GLUI_TextBox *glui_information; //name, x,y,z,mag
//contour, raw, 2d 
 GLUI_Listbox *glui_display[2];
//contour
 GLUI_Button *glui_contourpop;
 GLUI_Checkbox *glui_contoursync;
 GLUI_TextBox *glui_contouredit[2];
 GLUI_Button *glui_contourok;
 GLUI_Button *glui_contourcancel;
//roi
 GLUI_Checkbox *glui_roisync;
 GLUI_Button *glui_roirefer[2];
 GLUI_Checkbox *glui_roiedit[2];
 GLUI_Button *glui_roiadd[2];
 GLUI_Button *glui_roidelete[2];
 GLUI_Listbox *glui_roidisplay[2];
 GLUI_Button *glui_roiok;
//symmetry
 GLUI_Button *glui_symmetrypop;
 GLUI_Checkbox *glui_symmetryvisible;
 GLUI_Checkbox *glui_symmetrysync;
 GLUI_TextBox *glui_symmetryedit[2];
 GLUI_Checkbox *glui_symmetrytype;
 GLUI_Button *glui_symmetryok;
 GLUI_Button *glui_symmetrycancel;
//clustering
 GLUI_Button *glui_clusterpop;
 GLUI_Checkbox *glui_clustersync;
 GLUI_Checkbox *glui_clusterlayer;
 GLUI_Checkbox *glui_clustertogether;
 GLUI_Listbox *glui_clustertype[2];
 GLUI_Checkbox *glui_clusterROI[2];
 GLUI_EditText *glui_clusternum[2];
 GLUI_Button *glui_clusterok;
 GLUI_Button *glui_clustercancel;
//distancemap
 GLUI_Button *glui_distancepop;
 GLUI_Checkbox *glui_distancesync;
 GLUI_EditText *glui_distancesample;
 GLUI_Checkbox *glui_distancepoint;
 GLUI_TextBox *glui_distanceedit[2];
 GLUI_StaticText *glui_distancezshow[2];
 GLUI_Checkbox *glui_distancez[2];
 GLUI_Button *glui_distanceok;
 GLUI_Button *glui_distancecancel;
//cuttingplane
 GLUI_Button *glui_cuttingpop;
 GLUI_Checkbox *glui_cuttingsync;
 GLUI_Checkbox *glui_cuttingvisible;
 GLUI_EditText *glui_cuttingvx[2];
 GLUI_EditText *glui_cuttingvy[2];
 GLUI_EditText *glui_cuttingvz[2];
 GLUI_EditText *glui_cuttingnx[2];
 GLUI_EditText *glui_cuttingny[2];
 GLUI_EditText *glui_cuttingnz[2];
 GLUI_Checkbox *glui_cuttingedit;
//projection
 GLUI_Checkbox *glui_projectionvisible;
 GLUI_Checkbox *glui_projectionsync;
 GLUI_Listbox *glui_projectiontype[2];
//others
 GLUI_Listbox *glui_encode[2];
 GLUI_StaticText *glui_background;
 GLUI_StaticText *glui_opacity;
 GLUI_StaticText *glui_frequency;*/

/*
void glui_lhc()
{

  	glui_lcluster = GLUI_Master.create_glui_subwindow( left_window, 
					    GLUI_SUBWINDOW_TOP);
//----------------------------------------------------------------
	
	GLUI_Panel *obj_data = glui_lcluster->add_panel("Weighted K-means clustering");

//----------------------------------------------------------------
	GLUI_Panel *obj_1 = glui->add_panel_to_panel(obj_data,"Step 1");

	GLUI_Panel * obj_l = glui->add_panel_to_panel(obj_1,"Location");
	char str[20];
	sprintf(str,"%0.2f",qdotLweight[0][0]);
	glui_LWtext00 = glui_lcluster->add_statictext_to_panel(obj_l,str);
	glui_LWtext00 -> set_w(5);
	glui_lcluster->add_column_to_panel(obj_l, true);
	GLUI_Scrollbar *sb_location = new GLUI_Scrollbar(obj_l, "Location",  GLUI_SCROLL_HORIZONTAL,&qdotLweight[0][0], Lweight_id00, control);
	sb_location->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_1, true);

	GLUI_Panel * obj_v = glui->add_panel_to_panel(obj_1,"Direction");
	sprintf(str,"%0.2f",qdotVweight[0][0]);
	glui_VWtext00 = glui_lcluster->add_statictext_to_panel(obj_v,str);
	glui_VWtext00 -> set_w(5);
	glui_lcluster->add_column_to_panel(obj_v, true);
	GLUI_Scrollbar *sb_velocity = new GLUI_Scrollbar(obj_v, "Direction",  GLUI_SCROLL_HORIZONTAL,&qdotVweight[0][0], Vweight_id00, control);
	sb_velocity->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_1, true);

	GLUI_Panel * obj_d = glui->add_panel_to_panel(obj_1,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[0][0]);
	glui_DWtext00 = glui_lcluster->add_statictext_to_panel(obj_d,str);
	glui_DWtext00->set_w(5);
	glui_lcluster->add_column_to_panel(obj_d, true);
	GLUI_Scrollbar *sb_density = new GLUI_Scrollbar(obj_d, "Magnitude",  GLUI_SCROLL_HORIZONTAL, &qdotDweight[0][0], Dweight_id00, control);
	sb_density->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_1, true);
	GLUI_EditText *glui_text = new GLUI_EditText(obj_1, "# of clusters", &qdotHCNum[0][0]);
	glui_text->set_w(8);

//----------------------------------------------------------------
	GLUI_Panel *obj_2 = glui->add_panel_to_panel(obj_data,"Step 2");


	GLUI_Panel * obj_l2 = glui->add_panel_to_panel(obj_2,"Location");
	//char str[20];
	sprintf(str,"%0.2f",qdotLweight[0][1]);
	glui_LWtext01 = glui_lcluster->add_statictext_to_panel(obj_l2,str);
	glui_LWtext01->set_w(5);
	glui_lcluster->add_column_to_panel(obj_l2, true);
	GLUI_Scrollbar *sb_location1 = new GLUI_Scrollbar(obj_l2, "Location",  GLUI_SCROLL_HORIZONTAL,&qdotLweight[0][1], Lweight_id01, control);
	sb_location->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_2, true);

	GLUI_Panel * obj_v2 = glui->add_panel_to_panel(obj_2,"Direction");
	sprintf(str,"%0.2f",qdotVweight[0][1]);
	glui_VWtext01 = glui_lcluster->add_statictext_to_panel(obj_v2,str);
	glui_VWtext01->set_w(5);
	glui_lcluster->add_column_to_panel(obj_v2, true);
	GLUI_Scrollbar *sb_velocity1 = new GLUI_Scrollbar(obj_v2, "Direction",  GLUI_SCROLL_HORIZONTAL,&qdotVweight[0][1], Vweight_id01, control);
	sb_velocity->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_2, true);

	GLUI_Panel * obj_d2 = glui->add_panel_to_panel(obj_2,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[0][1]);
	glui_DWtext01 = glui_lcluster->add_statictext_to_panel(obj_d2,str);
	glui_DWtext01->set_w(5);
	glui_lcluster->add_column_to_panel(obj_d2, true);
	GLUI_Scrollbar *sb_density1 = new GLUI_Scrollbar(obj_d2, "Magnitude",  GLUI_SCROLL_HORIZONTAL, &qdotDweight[0][1], Dweight_id01, control);
	sb_density->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_2, true);
	GLUI_EditText *glui_text3 = new GLUI_EditText(obj_2, "# of clusters", &qdotHCNum[0][1]);
	glui_text3->set_w(5);

	glui_lcluster->add_column_to_panel(obj_data, true);
//----------------------------------------------------------------
	GLUI_Button *sb_update = new GLUI_Button(obj_data, "Update", Uweight_id0, control);
}
*/
/*void glui_rhc()
{


	glui_rcluster = GLUI_Master.create_glui_subwindow( right_window, 
					    GLUI_SUBWINDOW_TOP);
//----------------------------------------------------------------
	
	GLUI_Panel *obj_data = glui_rcluster->add_panel("Weighted K-means clustering");

//----------------------------------------------------------------
	GLUI_Panel *obj_1 = glui->add_panel_to_panel(obj_data,"Step 1");

	GLUI_Panel * obj_l = glui->add_panel_to_panel(obj_1,"Location");
	char str[20];
	sprintf(str,"%0.2f",qdotLweight[1][0]);
	glui_LWtext10 = glui_rcluster->add_statictext_to_panel(obj_l,str);
	glui_LWtext10->set_w(5);
	glui_rcluster->add_column_to_panel(obj_l, true);
	GLUI_Scrollbar *sb_location = new GLUI_Scrollbar(obj_l, "Location",  GLUI_SCROLL_HORIZONTAL,&qdotLweight[1][0], Lweight_id10, control);
	sb_location->set_float_limits(0,1);
	glui_rcluster->add_column_to_panel(obj_1, true);

	GLUI_Panel * obj_v = glui->add_panel_to_panel(obj_1,"Direction");
	sprintf(str,"%0.2f",qdotVweight[1][0]);
	glui_VWtext10 = glui_rcluster->add_statictext_to_panel(obj_v,str);
	glui_VWtext10->set_w(5);
	glui_rcluster->add_column_to_panel(obj_v, true);
	GLUI_Scrollbar *sb_velocity = new GLUI_Scrollbar(obj_v, "Direction",  GLUI_SCROLL_HORIZONTAL,&qdotVweight[1][0], Vweight_id10, control);
	sb_velocity->set_float_limits(0,1);
	glui_rcluster->add_column_to_panel(obj_1, true);

	GLUI_Panel * obj_d = glui->add_panel_to_panel(obj_1,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[1][0]);
	glui_DWtext10 = glui_rcluster->add_statictext_to_panel(obj_d,str);
	glui_DWtext10->set_w(5);
	glui_rcluster->add_column_to_panel(obj_d, true);
	GLUI_Scrollbar *sb_density = new GLUI_Scrollbar(obj_d, "Magnitude",  GLUI_SCROLL_HORIZONTAL, &qdotDweight[1][0], Dweight_id10, control);
	sb_density->set_float_limits(0,1);
	glui_rcluster->add_column_to_panel(obj_1, true);
	GLUI_EditText *glui_text = new GLUI_EditText(obj_1, "# of clusters", &qdotHCNum[1][0]);

//----------------------------------------------------------------
	GLUI_Panel *obj_2 = glui->add_panel_to_panel(obj_data,"Step 2");


	GLUI_Panel * obj_l2 = glui->add_panel_to_panel(obj_2,"Location");
	//char str[20];
	sprintf(str,"%0.2f",qdotLweight[1][1]);
	glui_LWtext11 = glui_rcluster->add_statictext_to_panel(obj_l2,str);
	glui_LWtext11->set_w(5);
	glui_rcluster->add_column_to_panel(obj_l2, true);
	GLUI_Scrollbar *sb_location1 = new GLUI_Scrollbar(obj_l2, "Location",  GLUI_SCROLL_HORIZONTAL,&qdotLweight[1][1], Lweight_id11, control);
	sb_location->set_float_limits(0,1);
	glui_rcluster->add_column_to_panel(obj_2, true);

	GLUI_Panel * obj_v2 = glui->add_panel_to_panel(obj_2,"Direction");
	sprintf(str,"%0.2f",qdotVweight[1][1]);
	glui_VWtext11 = glui_rcluster->add_statictext_to_panel(obj_v2,str);
	glui_VWtext11->set_w(5);
	glui_rcluster->add_column_to_panel(obj_v2, true);
	GLUI_Scrollbar *sb_velocity1 = new GLUI_Scrollbar(obj_v2, "Direction",  GLUI_SCROLL_HORIZONTAL,&qdotVweight[1][1], Vweight_id11, control);
	sb_velocity->set_float_limits(0,1);
	glui_rcluster->add_column_to_panel(obj_2, true);

	GLUI_Panel * obj_d2 = glui->add_panel_to_panel(obj_2,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[1][1]);
	glui_DWtext11 = glui_rcluster->add_statictext_to_panel(obj_d2,str);
	glui_DWtext11->set_w(5);
	glui_rcluster->add_column_to_panel(obj_d2, true);
	GLUI_Scrollbar *sb_density1 = new GLUI_Scrollbar(obj_d2, "Magnitude",  GLUI_SCROLL_HORIZONTAL, &qdotDweight[1][1], Dweight_id11, control);
	sb_density->set_float_limits(0,1);
	glui_rcluster->add_column_to_panel(obj_2, true);
	GLUI_EditText *glui_text3 = new GLUI_EditText(obj_2, "# of clusters", &qdotHCNum[1][1]);
	glui_text3->set_w(8);

//----------------------------------------------------------------
	glui_rcluster->add_column_to_panel(obj_data, true);
	GLUI_Button *sb_update = new GLUI_Button(obj_data, "Update", Uweight_id1, control);
}
*/



/*
void glui_function()
{
	glui= GLUI_Master.create_glui("Menu");

//data--------------------------------------------------------------------------------------------
	GLUI_Panel *obj_data = glui->add_panel("Data");
	obj_data->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_Panel *obj_data_left = glui->add_panel_to_panel(obj_data, "Left Window");
	obj_data_left->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_StaticText *text_data1 = glui->add_statictext_to_panel(obj_data_left, datafile[0]);
	glui->add_column_to_panel(obj_data_left, true);
	glui_loaddata[0] = glui->add_button_to_panel(obj_data_left,"Edit", Loaddata_id0, control);
	glui_loaddata[0] -> set_w(50);
	GLUI_Panel *obj_data_right = glui->add_panel_to_panel(obj_data, "Right Window");
	obj_data_right->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_StaticText *text_data2 = glui->add_statictext_to_panel(obj_data_right, datafile[1]);
	glui->add_column_to_panel(obj_data_right, true);
	glui_loaddata[1] = glui->add_button_to_panel(obj_data_right,"Edit", Loaddata_id1, control);
	glui_loaddata[1] -> set_w(50);

//information--------------------------------------------------------------------------------------------	
	//GLUI_Panel *obj_information = glui->add_panel("Information");
	//obj_information->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_Button *glui_buttoninformation =glui->add_button_to_panel(obj_data,"Show data information",Information_Pop_id,control);

	GLUI_Panel *group_1 = glui->add_panel("");
	group_1->set_alignment(GLUI_ALIGN_LEFT);
//display--------------------------------------------------------------------------------------------	

	GLUI_Panel *obj_display = glui->add_panel_to_panel(group_1, "Display");
	obj_display->set_alignment(GLUI_ALIGN_LEFT);
	glui_display[0] = glui->add_listbox_to_panel(obj_display, "Left window", &qdotDisplay[0], 
					Display_id0, control);
	glui_display[0]->add_item(0, "3D contour");
	glui_display[0]->add_item(1, "3D original data");
	glui_display[0]->add_item(2, "2D distance map");
	glui_display[0]->add_item(3, "2D projection");
	glui_display[1] = glui->add_listbox_to_panel(obj_display, "Right display", &qdotDisplay[1], 
					Display_id1, control);
	glui_display[1]->add_item(0, "3D contour");
	glui_display[1]->add_item(1, "3D original data");
	glui_display[1]->add_item(2, "2D distance map");
	glui_display[1]->add_item(3, "2D projection");

//contour--------------------------------------------------------------------------------------------	
	glui->add_column_to_panel(group_1, true);
	GLUI_Panel *obj_contour = glui->add_panel_to_panel(group_1, "Contour");
	glui_contourpop = glui->add_button_to_panel(obj_contour,"Edit", Contour_Pop_id, control);

//roi--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_roi = glui->add_panel("Regions of interest");	
	GLUI_Button *glui_roipop = glui->add_button_to_panel(obj_roi, "Edit", ROI_Pop_id, control);
	obj_roi->set_alignment(GLUI_ALIGN_LEFT);
	glui_roirefer = glui->add_checkbox_to_panel(obj_roi, "Enable referred data", &qdotROIRefer, ROI_Refer_id, control);
	glui_roisync = glui->add_checkbox_to_panel(obj_roi, "Sync", &qdotROISync, ROI_Sync_id, control);
	GLUI_Panel *group_2 = glui->add_panel_to_panel(obj_roi,"");
	GLUI_Panel *obj_roi_left = glui->add_panel_to_panel(group_2, "Left window");
	glui_roiedit[0] = glui->add_checkbox_to_panel(obj_roi_left, "Edit ", &qdotROIEdit[0], ROI_Edit_id0, control);
	glui_roidisplay[0] = glui->add_listbox_to_panel(obj_roi_left, "Display Panel", &qdotROIdisplay[0], ROI_Display_id0, control);
	glui_roidisplay[0]->add_item(0, "None");
	glui->add_column_to_panel(group_2);
	GLUI_Panel *obj_roi_right = glui->add_panel_to_panel(group_2, "Right window");
	glui_roiedit[1] = glui->add_checkbox_to_panel(obj_roi_right, "Edit", &qdotROIEdit[1], ROI_Edit_id1, control);
	glui_roidisplay[1] = glui->add_listbox_to_panel(obj_roi_right, "Display Panel", &qdotROIdisplay[1], ROI_Display_id1, control);
	glui_roidisplay[1]->add_item(0, "None");
	
	GLUI_Panel *group_3 = glui->add_panel("");
	group_3->set_alignment(GLUI_ALIGN_LEFT);
//symmetry--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_symmetry = glui->add_panel_to_panel(group_3, "Symmetry");	
	glui_symmetryvisible = glui->add_checkbox_to_panel(obj_symmetry, "Visible", &qdotSymmetryVisible, Symmetry_Visible_id, control);
	glui_symmetrypop = glui->add_button_to_panel(obj_symmetry, "Edit", Symmetry_Pop_id, control);

//cuttingplane--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_cutting = glui->add_panel_to_panel(group_3, "Cutting plane");
	glui_cuttingvisible = glui->add_checkbox_to_panel(obj_cutting, "Visible", &qdotCuttingVisible, Cutting_Visible_id, control);
	glui_cuttingpop = glui->add_button_to_panel(obj_cutting,"Edit", Cutting_Pop_id, control);

	glui->add_column_to_panel(group_3, true);
	
//clustering--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_cluster = glui->add_panel_to_panel(group_3,"Clustering");
	glui_clusterpop = glui->add_button_to_panel(obj_cluster, "Edit", Cluster_Pop_id, control);	
	
//distancemap--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_distance = glui->add_panel_to_panel(group_3, "Distance map");
	glui_distancesample = glui->add_edittext_to_panel(obj_distance,"# of data for sampling: ", GLUI_EDITTEXT_INT, &qdotDistanceSample, Distance_Sample_id, control);
	glui_distancepoint = glui->add_checkbox_to_panel(obj_distance,"Select one point", &qdotDistancePoint, Distance_Point_id, control);
	glui_distancepop = glui->add_button_to_panel(obj_distance,"Edit", Distance_Pop_id, control);

//update!!!!!!!!!!!!!!!!!!!!!!!!!!
	GLUI_Button *glui_update = glui->add_button("Update", Update_id, control);


	GLUI_Panel *obj_vis = glui->add_panel("Visualization");
	obj_vis->set_alignment(GLUI_ALIGN_LEFT);
//projection----------------------------------------------------------------------------------------------
	GLUI_Panel *obj_projection = glui->add_panel_to_panel(obj_vis, "Projection");
	obj_projection->set_alignment(GLUI_ALIGN_LEFT);
	glui_projectionvisible = glui->add_checkbox_to_panel(obj_projection, "Visible", &qdotProjectionVisible, 
				Projection_Visible_id, control);	
	glui_projectionsync = glui->add_checkbox_to_panel(obj_projection, "Sync", &qdotProjectionSync, 
				Projection_Sync_id, control);
	glui->add_column_to_panel(obj_projection, true);
	GLUI_Panel *group_4 = glui->add_panel_to_panel(obj_projection, "Data type");
	group_4->set_alignment(GLUI_ALIGN_LEFT);
	glui_projectiontype[0] = glui->add_listbox_to_panel(group_4, "Left", &qdotProjectionType[0], 
				Projection_Type_id0, control);
	glui_projectiontype[0]->add_item(0, "Cutting plane");
	glui_projectiontype[0]->add_item(1, "Whole field");
	glui->add_column_to_panel(group_4, true);
	glui_projectiontype[1] = glui->add_listbox_to_panel(group_4, "Right", &qdotProjectionType[1], 
				Projection_Type_id1, control);
	glui_projectiontype[1]->add_item(0, "Cutting plane");
	glui_projectiontype[1]->add_item(1, "Whole field");

	GLUI_Panel *group_5 = glui->add_panel_to_panel(obj_vis,"");
//encode----------------------------------------------------------------------------------------------
	GLUI_Panel *obj_encode = glui->add_panel_to_panel(group_5 , "3D encoding");
	glui_encode[0] = glui->add_listbox_to_panel(obj_encode, "Left ", &qdotEncode[0], Encode_id0, control);
	glui_encode[0]->add_item(0, "Linear");
	glui_encode[0]->add_item(1, "SplitVectors");
	glui_encode[0]->add_item(2, "Uniform");
	//glui->add_column_to_panel(obj_encode, true);
	glui_encode[1] = glui->add_listbox_to_panel(obj_encode, "Right ", &qdotEncode[1], Encode_id1, control);
	glui_encode[1]->add_item(0, "Linear");
	glui_encode[1]->add_item(1, "SplitVectors");
	glui_encode[1]->add_item(2, "Uniform");
	
	glui->add_column_to_panel(group_5, true);
//other----------------------------------------------------------------------------------------------
	GLUI_Panel * obj_background = glui->add_panel_to_panel(group_5,"Background color");
	char str[20];
	sprintf(str,"%0.2f",qdotBack);
	glui_background = glui->add_statictext_to_panel(obj_background,str);
	glui_background->set_w(40);
	glui->add_column_to_panel(obj_background,true);
	GLUI_Scrollbar *sb_background = new GLUI_Scrollbar(obj_background, "Background color",  GLUI_SCROLL_HORIZONTAL,
				&qdotBack,Background_id,control);
	sb_background->set_float_limits(0., 1.);

	GLUI_Panel *obj_alpha = glui->add_panel_to_panel(group_5,"Opacity");
	sprintf(str,"%0.2f",qdotOpacity);
	glui_opacity = glui->add_statictext_to_panel(obj_alpha,str);
	glui_opacity->set_w(40);
	glui->add_column_to_panel(obj_alpha,true);
	GLUI_Scrollbar *sb_alpha = new GLUI_Scrollbar(obj_alpha, "Opacity",  GLUI_SCROLL_HORIZONTAL,&qdotOpacity,Opacity_id,control);
	sb_alpha->set_int_limits(0.1, 1);


	GLUI_Panel *group_6 = glui->add_panel_to_panel(obj_vis,"");
	GLUI_Checkbox *glui_blind = glui->add_checkbox_to_panel(group_6,"Color blind safe", &qdotBlind, Blind_id, control);

	glui->add_column_to_panel(group_6, true);
	GLUI_Panel *obj_density = glui->add_panel_to_panel(group_6,"Sampling frequency");
	char str2[20];
	sprintf(str2,"%d",qdotFrequency);
	glui_frequency = glui->add_statictext_to_panel(obj_density,str2);
	glui_frequency->set_w(40);
	glui->add_column_to_panel(obj_density,true);
	GLUI_Scrollbar * sb_density = new GLUI_Scrollbar(obj_density, "Density frequency", GLUI_SCROLL_HORIZONTAL,&qdotFrequency,Frequency_id,control);
	sb_density->set_int_limits(1, 100);	
//------------------------------------------------------------------------------------------------------------
	GLUI_Button *glui_picture = glui->add_button("Capture screenshot", Picture_id, control);
	GLUI_Button *glui_save = glui->add_button("Save settings", Save_id, control);
	GLUI_Button *glui_exit = glui->add_button("Save and exit", Exit_id, control);

//----------------------------------------------------------------------------------------------------------------
	glui_data_1();
	glui_data_2();
	glui_information_function();
	glui_contour();
//	glui_symmetry();
//	glui_cluster();
	glui_distance();
//	glui_cutting();
//	glui_roi();

	glui_data_pop1->hide();
	glui_data_pop2->hide();
	glui_distance_pop->hide();
	glui_contour_pop->hide();
//	glui_cluster_pop->hide();
//	glui_cutting_pop->hide();
//	glui_symmetry_pop->hide();
	glui_information_pop->hide();	
//	glui_roi_pop->hide();
}*/


void glui_function(int data_type)
{
	if(data_type == 0)
		glui[data_type] = GLUI_Master.create_glui("Left Menu");
	else
		glui[data_type] = GLUI_Master.create_glui("Right Menu");

//data--------------------------------------------------------------------------------------------
	GLUI_Panel *obj_data = glui[data_type]->add_panel("Data");
	obj_data->set_alignment(GLUI_ALIGN_LEFT);
	text_data[data_type] = glui[data_type]->add_statictext_to_panel(obj_data, datafile[data_type]);
	text_data[data_type]->set_w(300);
	glui[data_type]->add_column_to_panel(obj_data, true);
	glui_loaddata[data_type] = glui[data_type]->add_button_to_panel(obj_data,"Edit", Loaddata_id[data_type], control);
	glui_loaddata[data_type] -> set_w(5);

//display--------------------------------------------------------------------------------------------	
	GLUI_Panel *p1 = glui[data_type]->add_panel("");
	p1->set_alignment(GLUI_ALIGN_LEFT);
	glui_display[data_type] = glui[data_type]->add_listbox_to_panel(p1, "Display", &qdotDisplay[data_type], Display_id[data_type], control);
	glui_display[data_type]->add_item(0, "3D contour");
	glui_display[data_type]->add_item(1, "3D original data");
	glui_display[data_type]->add_item(2, "2D distance map");
	glui_display[data_type]->set_int_val(qdotDisplay[data_type]);
	//glui_display[data_type]->add_item(3, "2D projection");
	glui[data_type]->add_column_to_panel(p1, true);
	//GLUI_Panel *obj_encode = glui[data_type]->add_panel_to_panel(p1, "3D encoding");
	glui_encode[data_type] = glui[data_type]->add_listbox_to_panel(p1, "Encoding ", &qdotEncode[data_type], Encode_id[data_type], control);
	glui_encode[data_type]->add_item(0, "Linear");
	glui_encode[data_type]->add_item(1, "SplitVectors");
	//glui_encode[data_type]->add_item(2, "Uniform");

	GLUI_Panel *p2 = glui[data_type]->add_panel("");
	p2->set_alignment(GLUI_ALIGN_LEFT);
	//GLUI_Panel *obj_color = glui[data_type]->add_panel_to_panel(p2, "Color by");
	glui_color[data_type] = glui[data_type]->add_listbox_to_panel(p2, "Color by ", &qdotColorby[data_type], Color_id[data_type], control);
	//glui_color[data_type]->add_item(0, "Magnitude");
	glui_color[data_type]->add_item(0, "Cluster");
	glui_color[data_type]->add_item(1, "Atom type");
	//glui[data_type]->add_column_to_panel(p2, true);
	//GLUI_Panel *obj_shape = glui[data_type]->add_panel_to_panel(p2, "Geometry");
	//glui_shape[data_type] = glui[data_type]->add_listbox_to_panel(p2, "Geometry ", &qdotShape[data_type], Shape_id[data_type], control);
	//glui_shape[data_type]->add_item(0, "Arrow");
	//glui_shape[data_type]->add_item(1, "By atom type");

	GLUI_Panel *p3 = glui[data_type]->add_panel("");
	p3->set_alignment(GLUI_ALIGN_LEFT);
	//GLUI_Panel *obj_mesh = glui[data_type]->add_panel_to_panel(p3, "QDOT regions");
	glui_mesh[data_type] = glui[data_type]->add_listbox_to_panel(p3, "Regions ", &qdotRegion[data_type], Region_id[data_type], control);
	glui_mesh[data_type]->add_item(0, "None");

	for(int i=0;i<1;i++)//qdotRegionList[data_type].size()
	{
		char str[10];
		sprintf(str, "%d", i+1); 
		glui_mesh[data_type]->add_item(i+1, str);
	}
	glui_mesh[data_type]->set_int_val(qdotRegion[data_type]);
	//glui_mesh[data_type]->add_item(qdotRegionList[data_type].size()+1, "All");
	glui[data_type]->add_column_to_panel(p3, true);
	glui_overview[data_type] = glui[data_type]->add_checkbox_to_panel(p3, "Show overviewGlyph", &qdotOverview[data_type], Overview_id[data_type]);
	//checkpoint!!!!!!!!!!!!!!!!!!!!!!!!!!!
//contour--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_contour = glui[data_type]->add_panel("Contour");
	obj_contour->set_alignment(GLUI_ALIGN_LEFT);
	glui_contoursync[data_type] = glui[data_type]->add_checkbox_to_panel(obj_contour, "sync", &qdotContourSync, Contour_Sync_id[data_type], control);

	glui_contouredit[data_type] = new GLUI_TextBox(obj_contour, true, Contour_Edit_id[data_type], control);
	glui_contouredit[data_type] -> set_w(300);
	glui_contouredit[data_type] -> set_h(50);
	glui_contour_display(data_type);
	
//clustering--------------------------------------------------------------------------------------------

	GLUI_Panel *obj_cluster = glui[data_type]->add_panel("Clustering");
	obj_cluster->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_Panel *p6 = glui[data_type]->add_panel_to_panel(obj_cluster, "");
	glui_clustersync[data_type] = glui[data_type]->add_checkbox_to_panel(p6, "Sync", &qdotClusterSync, Cluster_Sync_id[data_type], control);
	glui[data_type]->add_column_to_panel(p6, true);
	p6->set_alignment(GLUI_ALIGN_LEFT);
	//GLUI_Panel *obj_cluster_left = glui[data_type]->add_panel_to_panel(obj_cluster, "Cluster in");
	/*glui_cuttingscroll[data_type] = new GLUI_TextBox(p6, true, Cutting_Move_id[data_type], control);
	glui_cuttingscroll[data_type] -> set_w(200);
	glui_cuttingscroll[data_type] -> set_h(40);*/
	GLUI_Button *glui_cuttingbutton = glui[data_type]->add_button_to_panel(p6, "Select cutting planes", Cuttingbutton_id[data_type], control);
	glui_cuttingpop(data_type);
	glui_cutting[data_type]->hide();

	/*glui_clustertype[data_type] = glui[data_type]->add_listbox_to_panel(p6,//obj_cluster,//obj_cluster_left,
						"Type", &qdotClusterType[data_type], Cluster_Type_id[data_type], control);
	glui_clustertype[data_type]->add_item(0, "One layer");
	glui_clustertype[data_type]->add_item(1, "Multiple layers");
	glui_clustertype[data_type]->add_item(2, "All layers");*/

	//===============================================cutting plane ================================================
	GLUI_Panel *obj_cutting_left = glui[data_type]->add_panel_to_panel(obj_cluster, "Cutting plane");
	obj_cutting_left->set_alignment(GLUI_ALIGN_LEFT);
	//glui_clusterlayer[data_type] = glui[data_type]->add_checkbox_to_panel(obj_cutting_left, "Select layers",
	//					 &qdotClusterlayer[data_type], Cluster_Layer_id[data_type], control);
	glui_cuttingedit[data_type] = new GLUI_TextBox(obj_cutting_left, true, Cutting_Edit_id[data_type], control);
	glui_cuttingedit[data_type] -> set_w(300);
	glui_cuttingedit[data_type] -> set_h(30);

	glui_cuttingdisplay(data_type);

	glui_cuttingmove[data_type] = new GLUI_EditText(obj_cutting_left, "Distance of cutting planes", &qdotMDegree[data_type], Cutting_Distance_id[data_type], control);
	
	/*glui_cuttingscroll[data_type] = new GLUI_Scrollbar(obj_cutting_left, "Edit",  GLUI_SCROLL_HORIZONTAL,
				&qdotCuttingMove[data_type], Cutting_Move_id[data_type], control);
	glui_cuttingscroll[data_type]->set_float_limits(0,1);*/
	//===============================================weighted clustering ================================================
	GLUI_Panel *p4 = glui[data_type]->add_panel_to_panel(obj_cluster,"");	
	GLUI_Panel *obj_1 = glui[data_type]->add_panel_to_panel(p4, "Step 1");

	GLUI_Panel * obj_l = glui[data_type]->add_panel_to_panel(obj_1,"Location");
	char str[20];
	sprintf(str,"%0.2f",qdotLweight[data_type][0]);
	glui_LWtext0[data_type] = glui[data_type]->add_statictext_to_panel(obj_l,str);
	glui_LWtext0[data_type] -> set_w(5);
	glui[data_type]->add_column_to_panel(obj_l, true);
	sb_location0[data_type] = new GLUI_Scrollbar(obj_l, "Location",  GLUI_SCROLL_HORIZONTAL,
				&qdotLweight[data_type][0], Lweight_id0[data_type], control);
	sb_location0[data_type]->set_w(100);
	sb_location0[data_type]->set_float_limits(0,1);

	GLUI_Panel * obj_v = glui[data_type]->add_panel_to_panel(obj_1,"Direction");
	sprintf(str,"%0.2f",qdotVweight[data_type][0]);
	glui_VWtext0[data_type] = glui[data_type]->add_statictext_to_panel(obj_v,str);
	glui_VWtext0[data_type] -> set_w(5);
	glui[data_type]->add_column_to_panel(obj_v, true);
	sb_velocity0[data_type] = new GLUI_Scrollbar(obj_v, "Direction",  GLUI_SCROLL_HORIZONTAL,
				&qdotVweight[data_type][0], Vweight_id0[data_type], control);
	sb_velocity0[data_type]->set_w(100);
	sb_velocity0[data_type]->set_float_limits(0,1);

	GLUI_Panel * obj_d = glui[data_type]->add_panel_to_panel(obj_1,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[data_type][0]);
	glui_DWtext0[data_type] = glui[data_type]->add_statictext_to_panel(obj_d,str);
	glui_DWtext0[data_type]->set_w(5);
	glui[data_type]->add_column_to_panel(obj_d, true);
	sb_density0[data_type] = new GLUI_Scrollbar(obj_d, "Magnitude",  GLUI_SCROLL_HORIZONTAL, 
				&qdotDweight[data_type][0], Dweight_id0[data_type], control);
	sb_density0[data_type]->set_w(100);
	sb_density0[data_type]->set_float_limits(0,1);

	GLUI_Panel *obj_m1 = glui[data_type]->add_panel_to_panel(obj_1,"Range of magnitudes");
	sprintf(str,"%6.2e",qdotMagRange0[data_type][0]);
	string str2(str);
	sb_magrange00[data_type] = new GLUI_EditText( obj_m1, "Min", &qdotMagRange0[data_type][0], MRange_id00[data_type], control);
	sb_magrange00[data_type]->set_w(30);
	//glui[data_type]->add_column_to_panel(obj_m1, true);
	sprintf(str,"%6.2e",qdotMagRange0[data_type][1]);
	string str3(str);
	sb_magrange01[data_type] = new GLUI_EditText( obj_m1, "Max",  &qdotMagRange0[data_type][1], MRange_id01[data_type], control);
	sb_magrange01[data_type]->set_w(30);
	/*double minmag = qdotField[data_type]->getMin();
	double maxmag = qdotField[data_type]->getMax();
	double median;
	if(minmag > 0)
	{
		median = pow(10., (log10(maxmag) + log10(minmag))/2.);
	}
	else
	{
		median = pow(10., (log10(maxmag) + log10(minmag))/2.);
	}
	float mag = qdotMagRange0[data_type][0]/qdotMagRange0[data_type][1];
	sprintf(str,"%6.1e",qdotMagRange0[data_type][0]);
	glui_Mtext00[data_type] = glui[data_type]->add_statictext_to_panel(obj_m1,str);
	glui_Mtext00[data_type]->set_w(5);
	glui[data_type]->add_column_to_panel(obj_m1, true);
	sb_magrange00[data_type] = new GLUI_Scrollbar(obj_m1, "Min(magnitude)",  GLUI_SCROLL_HORIZONTAL, 
				&mag, MRange_id00[data_type], control);
	sb_magrange00[data_type]->set_float_limits(0,1);
	sb_magrange00[data_type]->set_w(100);	

	GLUI_Panel *obj_m12 = glui[data_type]->add_panel_to_panel(obj_1,"Min (Magnitude)");
	mag = 0;
	sprintf(str,"%6.1e",qdotMagRange1[data_type][0]);
	glui_Mtext01[data_type] = glui[data_type]->add_statictext_to_panel(obj_m12,str);
	glui_Mtext01[data_type]->set_w(5);
	sb_magrange01[data_type] = new GLUI_Scrollbar(obj_m1, "Max(magnitude)",  GLUI_SCROLL_HORIZONTAL, 
				&mag, MRange_id01[data_type], control);
	sb_magrange01[data_type]->set_float_limits(0,1);	
	sb_magrange01[data_type]->set_w(100);*/

	glui_cluster_text0[data_type] = new GLUI_EditText(obj_1, "# of clusters", &qdotHCNum[data_type][0], Cluster_Num_id0[data_type], control);
	glui_cluster_text0[data_type]->set_w(8);

	glui[data_type]->add_column_to_panel(p4, true);
	//===============================================weighted clustering ================================================
	GLUI_Panel *obj_2 = glui[data_type]->add_panel_to_panel(p4, "Step 2");

	GLUI_Panel * obj_l2 = glui[data_type]->add_panel_to_panel(obj_2,"Location");
	//char str[20];
	sprintf(str,"%0.2f",qdotLweight[data_type][1]);
	glui_LWtext1[data_type] = glui[data_type]->add_statictext_to_panel(obj_l2,str);
	glui_LWtext1[data_type] -> set_w(5);
	glui[data_type]->add_column_to_panel(obj_l2, true);
	sb_location1[data_type] = new GLUI_Scrollbar(obj_l2, "Location",  GLUI_SCROLL_HORIZONTAL,
				&qdotLweight[data_type][1], Lweight_id1[data_type], control);
	sb_location1[data_type]->set_w(100);
	sb_location1[data_type]->set_float_limits(0,1);

	GLUI_Panel * obj_v2 = glui[data_type]->add_panel_to_panel(obj_2,"Direction");
	sprintf(str,"%0.2f",qdotVweight[data_type][1]);
	glui_VWtext1[data_type] = glui[data_type]->add_statictext_to_panel(obj_v2,str);
	glui_VWtext1[data_type] -> set_w(5);
	glui[data_type]->add_column_to_panel(obj_v2, true);
	sb_velocity1[data_type] = new GLUI_Scrollbar(obj_v2, "Direction",  GLUI_SCROLL_HORIZONTAL,
				&qdotVweight[data_type][1], Vweight_id1[data_type], control);
	sb_velocity1[data_type]->set_w(100);
	sb_velocity1[data_type]->set_float_limits(0,1);

	GLUI_Panel * obj_d2 = glui[data_type]->add_panel_to_panel(obj_2,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[data_type][1]);
	glui_DWtext1[data_type] = glui[data_type]->add_statictext_to_panel(obj_d2,str);
	glui_DWtext1[data_type]->set_w(5);
	glui[data_type]->add_column_to_panel(obj_d2, true);
	sb_density1[data_type] = new GLUI_Scrollbar(obj_d2, "Magnitude",  GLUI_SCROLL_HORIZONTAL, 
				&qdotDweight[data_type][1], Dweight_id1[data_type], control);
	sb_density1[data_type]->set_w(100);
	sb_density1[data_type]->set_float_limits(0,1);

	GLUI_Panel *obj_m2 = glui[data_type]->add_panel_to_panel(obj_2,"Range of magnitudes");
	sprintf(str,"%6.2e",qdotMagRange1[data_type][0]);	
	string str4(str);
	sb_magrange10[data_type] = new GLUI_EditText( obj_m2, "Min", &qdotMagRange1[data_type][0], MRange_id10[data_type], control);
	sb_magrange10[data_type]->set_w(30);
	//glui[data_type]->add_column_to_panel(obj_m2, true);
	sprintf(str,"%6.2e",qdotMagRange1[data_type][1]);
	string str5(str);
	sb_magrange11[data_type] = new GLUI_EditText( obj_m2, "Max",&qdotMagRange1[data_type][1], MRange_id11[data_type], control);
	sb_magrange11[data_type]->set_w(30);

	/*GLUI_Panel *obj_m2 = glui[data_type]->add_panel_to_panel(obj_2,"Ranges of Magnitude");
	sb_magrange10[data_type] = new GLUI_Scrollbar(obj_m2, "Min(magnitude)",  GLUI_SCROLL_HORIZONTAL, 
				&qdotMagRange1[data_type][0], MRange_id10[data_type], control);
	sb_magrange10[data_type]->set_float_limits(0,1);	
	sb_magrange10[data_type]->set_w(100);
	sb_magrange11[data_type] = new GLUI_Scrollbar(obj_m2, "Max(magnitude)",  GLUI_SCROLL_HORIZONTAL, 
				&qdotMagRange1[data_type][1], MRange_id11[data_type], control);
	sb_magrange11[data_type]->set_float_limits(0,1);	
	sb_magrange11[data_type]->set_w(100);*/

	glui_cluster_text1[data_type] = new GLUI_EditText(obj_2, "# of clusters", &qdotHCNum[data_type][1], Cluster_Num_id1[data_type], control);
	glui_cluster_text1[data_type]->set_w(8);

//distancemap--------------------------------------------------------------------------------------------	
	GLUI_Panel *obj_distance = glui[data_type]->add_panel("Distance map");
	obj_distance->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_Panel *p7 = glui[data_type]->add_panel_to_panel(obj_distance,"");	
	//glui_distancepoint[data_type]  = glui[data_type] ->add_checkbox_to_panel(p7,"Select one point", &qdotDistancePoint[data_type] , Distance_Point_id[data_type] , control);
	//glui[data_type]->add_column_to_panel(p7, true);
	glui_distancez[data_type] = glui[data_type]->add_checkbox_to_panel(p7,"Select reference layer", &qdotDistanceZ[data_type],
				Distance_Z_id[data_type], control);
	//GLUI_Panel *p8 = glui[data_type]->add_panel_to_panel(obj_distance,"");	
	glui[data_type]->add_column_to_panel(p7, true);
	glui_distancesync[data_type] = glui[data_type]->add_checkbox_to_panel(p7, "Sync", &qdotDistanceSync, Distance_Sync_id[data_type], control);
	//glui[data_type]->add_column_to_panel(p8, true);
	//glui_distancesample[data_type]  = glui[data_type] ->add_edittext_to_panel(p8,"# of data for sampling: ", GLUI_EDITTEXT_INT, &qdotDistanceSample[data_type] , Distance_Sample_id[data_type] , control);


	//GLUI_Panel *obj_distance_left = glui[data_type]->add_panel_to_panel(obj_distance, "Edit");	
	glui_distanceedit[data_type] = new GLUI_TextBox(obj_distance,true,Distance_List_id[data_type],control);
	glui_distanceedit[data_type]->set_w(300);
	glui_distanceedit[data_type]->set_h(50);

	char *contour_text = new char[1024];
	for(int i=0;i<1024;i++)
	{
		contour_text[i] = '\0';
	}
	for(int i=0;i<qdotDistanceV[data_type].size();i++)
	{
	//cerr<<qdotDistanceV[data_type][i][2]<<" "<<qdotDistanceV[data_type][i][0]<<" "<<qdotDistanceV[data_type][i][1]<<endl;
		char *str = new char [50];
			   sprintf(str, "Z %0.2f V %0.2f %0.2f \n", 
					qdotDistanceV[data_type][i][2], qdotDistanceV[data_type][i][0], qdotDistanceV[data_type][i][1]);
			    strcat(contour_text,  str);

			delete [] str;
	}

	glui_distanceedit[data_type]->set_text((const char*) contour_text);
	delete [] contour_text;	



//update!!!!!!!!!!!!!!!!!!!!!!!!!!
	GLUI_Button *glui_update = glui[data_type]->add_button("Update", Update_id[data_type], control);
}

//----------------visualization-----------------------------------------------------------------
void glui_visualization()
{
	glui_visual = GLUI_Master.create_glui("Visualization");
//----------------------------------other----------------------------------------------------------------------------------------------
	//add a function to adjust size of arrows
	GLUI_Panel * obj_background = glui_visual->add_panel("Background color");
	char str[20];
	sprintf(str,"%0.2f",qdotBack);
	glui_background = glui_visual->add_statictext_to_panel(obj_background,str);
	glui_background->set_w(50);
	GLUI_Scrollbar *sb_background = new GLUI_Scrollbar(obj_background, "Background color",  GLUI_SCROLL_HORIZONTAL,
				&qdotBack,Background_id,control);
	sb_background->set_float_limits(0., 1.);

	GLUI_Panel *obj_alpha = glui_visual->add_panel("Opacity");
	sprintf(str,"%0.2f",qdotOpacity);
	glui_opacity = glui_visual->add_statictext_to_panel(obj_alpha,str);
	glui_opacity->set_w(40);
	GLUI_Scrollbar *sb_alpha = new GLUI_Scrollbar(obj_alpha, "Opacity",  GLUI_SCROLL_HORIZONTAL,&qdotOpacity,Opacity_id,control);
	sb_alpha->set_float_limits(0.1, 1);

	GLUI_Panel *obj_length = glui_visual->add_panel("Length");
	sprintf(str,"%0.2f",qdotLength);
	glui_length = glui_visual->add_statictext_to_panel(obj_length,str);
	glui_length->set_w(40);
	GLUI_Scrollbar *sb_length = new GLUI_Scrollbar(obj_length, "Line length",  GLUI_SCROLL_HORIZONTAL,&qdotLength, Length_id,control);
	sb_length->set_float_limits(0.1, 1);

	GLUI_Panel *obj_base = glui_visual->add_panel("Arrow base");
	sprintf(str,"%0.2f",qdotArrowBase);
	glui_base = glui_visual->add_statictext_to_panel(obj_base,str);
	glui_base->set_w(40);
	GLUI_Scrollbar *sb_arrow = new GLUI_Scrollbar(obj_base, "Arrow base",  GLUI_SCROLL_HORIZONTAL,&qdotArrowBase, Arrow_id,control);
	sb_arrow->set_float_limits(0.1, 1);

	GLUI_Checkbox *glui_blind = glui_visual->add_checkbox("Color blind safe", &qdotBlind, Blind_id, control);
	GLUI_Checkbox *glui_cut= glui_visual->add_checkbox("Show cutting planes", &qdotCutVisible, Cutvisible_id, control);

	GLUI_Panel *obj_density = glui_visual->add_panel("Sampling frequency");
	char str2[20];
	sprintf(str2,"%d",qdotFrequency);
	glui_frequency = glui_visual->add_statictext_to_panel(obj_density,str2);
	glui_frequency->set_w(50);
	GLUI_Scrollbar * sb_density = new GLUI_Scrollbar(obj_density, "Density frequency", GLUI_SCROLL_HORIZONTAL,&qdotFrequency,Frequency_id,control);
	sb_density->set_int_limits(1, 100);	

	GLUI_Panel *obj_distance = glui_visual->add_panel("Distance map");
	glui_distancesample[0]  = glui_visual ->add_edittext_to_panel(obj_distance,"# of data for sampling: ", GLUI_EDITTEXT_INT, &qdotDistanceSample[0] , Distance_Sample_id[0] , control);

	GLUI_Button *glui_rerender = glui_visual->add_button("Re-render", Rerender_id, control);
//------------------------------------------------------------------------------------------------------------
	GLUI_Button *glui_picture = glui_visual->add_button("Capture screenshot", Picture_id, control);
	GLUI_Button *glui_save = glui_visual->add_button("Save settings", Save_id, control);
	GLUI_Button *glui_exit = glui_visual->add_button("Save and exit", Exit_id, control);

}
void glui_cuttingpop(int data_type)
{
	glui_cutting[data_type] = GLUI_Master.create_glui("Select cutting planes");

	glui_cuttingscroll[data_type] = new GLUI_TextBox(glui_cutting[data_type], true, Cutting_Move_id[data_type], control);
	glui_cuttingscroll[data_type] -> set_w(100);
	glui_cuttingscroll[data_type] -> set_h(40);

	GLUI_Button *glui_ok = glui_cutting[data_type]->add_button("OK", Cuttingok_id[data_type], control);

	glui_cutting[data_type]->add_column(true);

	glui_cuttinglist[data_type] = new GLUI_TextBox(glui_cutting[data_type], true, Cutting_List_id[data_type], control);
	glui_cuttinglist[data_type] -> set_w(300);
	glui_cuttinglist[data_type] -> set_h(100);	


}
void glui_data_1()
{
	glui_data_pop1 = GLUI_Master.create_glui("Data (left window)");
	
	glui_datalist[0] = new GLUI_List(glui_data_pop1,true, Loaddata_List_id[0], control);
	glui_datalist[0] ->set_w(450);
	glui_dataok[0] = glui_data_pop1->add_button("OK", Loaddata_OK_id[0], control);
	glui_datacancel[0] = glui_data_pop1->add_button("Cancel", Loaddata_Cancel_id[0], control);
	glui_data_display_1();
}
void glui_data_display_1()
{
		for(int i=0;i<datastring.size();i++)
		{
			glui_datalist[0]->delete_item(i);		
		}		
		ListFile(datadir[0]);
		int ii;
		for(int i=0;i<datastring.size();i++)
		{
			if(!strcmp(datafile[0],datastring[i].c_str()))
			{
				ii = i;break;
			}
		}
		glui_datalist[0]->set_int_val(ii);
		for(int i=0;i<datastring.size();i++)
		{
			char *str = new char[datastring[i].length()+1];
			strcpy(str,datastring[i].c_str());
			//cerr<<str<<endl;
			glui_datalist[0]->add_item(i,str);	
			//cerr<<i<<" "<<str<<endl;	
			delete [] str;
		} 	
}
void glui_data_2()
{
	glui_data_pop2 = GLUI_Master.create_glui("Data (right window)");
	
	glui_datalist[1] = new GLUI_List(glui_data_pop2,true, Loaddata_List_id[1], control);
	glui_datalist[1] ->set_w(450);
	glui_dataok[1] = glui_data_pop2->add_button("OK", Loaddata_OK_id[1], control);
	glui_datacancel[1] = glui_data_pop2->add_button("Cancel", Loaddata_Cancel_id[1], control);

	glui_data_display_2();
}
void glui_data_display_2()
{
		for(int i=0;i<datastring.size();i++)
		{
			glui_datalist[1]->delete_item(i);		
		}		
		ListFile(datadir[1]);
		int ii;
		for(int i=0;i<datastring.size();i++)
		{
			if(!strcmp(datafile[1],datastring[i].c_str()))
			{
				ii = i;break;
			}
		}
		glui_datalist[1]->set_int_val(ii);
		for(int i=0;i<datastring.size();i++)
		{
			char *str = new char[datastring[i].length()+1];
			strcpy(str,datastring[i].c_str());
			//cerr<<str<<endl;
			glui_datalist[1]->add_item(i,str);	
			//cerr<<i<<" "<<str<<endl;	
			delete [] str;
		} 	
}
/*
void glui_information_function()
{
	glui_information_pop = GLUI_Master.create_glui("Data (right window)");

	glui_information = new GLUI_TextBox(glui_information_pop, true);
	glui_information->set_alignment(GLUI_ALIGN_LEFT);
	glui_information->set_h(400);
	glui_information->set_w(400);
	glui_information_display();
}
void glui_information_display()
{
	char *text = new char[1024];
	for(int i=0;i<1024;i++)
	{
		text[i] = '\0';
	}

	svVector3 lb,rb;
	lb = qdotField[0]->getLbbox();
	rb = qdotField[0]->getRbbox();
	sprintf(text," Data in the left window \n");
	sprintf(text, "%s Name: %s\n", text, datafile[0]);
	sprintf(text, "%s Number of points: %d\n", text, qdotField[0]->getRawSize());
	sprintf(text, "%s x :[%0.2f, %0.2f]\n", text, lb[0], rb[0]);
	sprintf(text, "%s y :[%0.2f, %0.2f]\n", text, lb[1], rb[1]);
	sprintf(text, "%s z :[%0.2f, %0.2f]\n", text, lb[2], rb[2]);
	sprintf(text, "%s magnitude :[%6.2e, %6.2e]\n", 
			text, qdotField[0]->getMin(), qdotField[0]->getMax());
	sprintf(text, "%s\n", text);

	sprintf(text,"%s Data in the right window \n", text);
	lb = qdotField[1]->getLbbox();
	rb = qdotField[1]->getRbbox();
	sprintf(text, "%s Name: %s\n", text, datafile[1]);
	sprintf(text, "%s Number of points: %d\n", text, qdotField[1]->getRawSize());
	sprintf(text, "%s x :[%0.2f, %0.2f]\n", text, lb[0], rb[0]);
	sprintf(text, "%s y :[%0.2f, %0.2f]\n", text, lb[1], rb[1]);
	sprintf(text, "%s z :[%0.2f, %0.2f]\n", text, lb[2], rb[2]);
	sprintf(text, "%s magnitude :[%6.2e, %6.2e]\n", 
			text, qdotField[1]->getMin(), qdotField[1]->getMax());

	glui_information->set_text((const char*) text);	

	delete [] text;
}
void glui_roi()
{
	glui_roi_pop = GLUI_Master.create_glui("Data of interest (DOI)");
	

	//glui_roirefer = glui_roi_pop->add_checkbox("Enable referred data", &qdotROIRefer, ROI_Refer_id, control);

	glui_roisync = glui_roi_pop->add_checkbox( "Sync", &qdotROISync, ROI_Sync_id, control);
	GLUI_Panel *group_2 = glui_roi_pop->add_panel("");
	GLUI_Panel *obj_roi_left = glui_roi_pop->add_panel_to_panel(group_2, "Left window");
	glui_roirefer[0] = glui_roi_pop->add_button_to_panel(obj_roi_left, "Add DOI with referred data", ROI_Refer_id0,control);
	glui_roiadd[0] = glui_roi_pop->add_button_to_panel(obj_roi_left, "Add DOI without referred data", ROI_Add_id0,control); 
	glui_roidelete[0] = glui_roi_pop->add_button_to_panel(obj_roi_left, "Delete DOI", ROI_Delete_id0,control); 
	GLUI_Panel *obj_roi_right = glui_roi_pop->add_panel_to_panel(group_2, "Right window");	
	glui_roirefer[1] = glui_roi_pop->add_button_to_panel(obj_roi_right, "Add DOI with referred data", ROI_Refer_id1,control);
	glui_roiadd[1] = glui_roi_pop->add_button_to_panel(obj_roi_right, "Add DOI without referred data", ROI_Add_id1,control); 
	glui_roidelete[1] = glui_roi_pop->add_button_to_panel(obj_roi_right, "Delete DOI", ROI_Delete_id1,control); */
	/*glui_roiedit[0] = glui_roi_pop->add_checkbox_to_panel(obj_roi_left, "Edit", &qdotROIEdit[0], ROI_Edit_id0, control);
	glui_roiadd[0] = glui_roi_pop->add_checkbox_to_panel(obj_roi_left, "Add", &qdotROIAdd[0], ROI_Add_id0, control);
	glui_roidelete[0] = glui_roi_pop->add_checkbox_to_panel(obj_roi_left, "Delete", &qdotROIDelete[0], ROI_Delete_id0, control);
	glui_roidisplay[0] = glui_roi_pop->add_listbox_to_panel(obj_roi_left, "Display Panel", &qdotROIdisplay[0], ROI_Display_id0, control);
	glui_roidisplay[0]->add_item(0, "None");
	glui->add_column_to_panel(group_2);
	GLUI_Panel *obj_roi_right = glui_roi_pop->add_panel_to_panel(group_2, "Right window");
	glui_roiedit[1] = glui_roi_pop->add_checkbox_to_panel(obj_roi_right, "Edit", &qdotROIEdit[1], ROI_Edit_id1, control);
	glui_roiadd[1] = glui_roi_pop->add_checkbox_to_panel(obj_roi_right, "Add", &qdotROIAdd[1], ROI_Add_id1, control);
	glui_roidelete[1] = glui_roi_pop->add_checkbox_to_panel(obj_roi_right, "Delete", &qdotROIDelete[1], ROI_Delete_id1, control);
	glui_roidisplay[1] = glui_roi_pop->add_listbox_to_panel(obj_roi_right, "Display Panel", &qdotROIdisplay[1], ROI_Display_id1, control);
	glui_roidisplay[1]->add_item(0, "None");
*/
/*	glui_roiok = glui_roi_pop->add_button("OK", ROI_OK_id, control);
}
void glui_contour()
{
  	glui_contour_pop = GLUI_Master.create_glui("Contour Generation");

	glui_contoursync = glui_contour_pop->add_checkbox("sync", &qdotContourSync, Contour_Sync_id);

	GLUI_Panel *obj_contour = glui_contour_pop->add_panel("Left window");
	glui_contouredit[0] = new GLUI_TextBox(obj_contour, true, Contour_Edit_id0, control);
	glui_contouredit[0] -> set_w(200);
	glui_contouredit[0] -> set_h(100);
	//glui_contour_pop->add_column_to_panel(obj_contour, true);
	GLUI_Panel *obj_contour2 = glui_contour_pop->add_panel("Right window");
	glui_contouredit[1] = new GLUI_TextBox(obj_contour2, true, Contour_Edit_id1, control);
	glui_contouredit[1] -> set_w(200);
	glui_contouredit[1] -> set_h(100);
	glui_contourok = glui_contour_pop->add_button("OK", Contour_OK_id, control);
	//glui_contourcancel = glui_contour_pop->add_button("Cancel", Contour_Cancel_id, control);

	glui_contour_display();
}*/
void glui_contour_display(int data_type)
{
	int j = data_type;
	//for(int j=0;j<2;j++)
	//{
		char *contour_text = new char[1024];

		for(int i=0;i<1024;i++)
		{
			contour_text[i] = '\0';
		}

		for(int i=0;i<qdotContourLAP[j].size();i++)
		{
			char *str = new char [50];
			    sprintf(str, "L A [ %0.2f %0.2f ] %6.2e\n", 
						qdotContourLAP[j][i][0], qdotContourLAP[j][i][1],
						qdotContourLA[j][i]);
			    strcat(contour_text,  str);
			delete [] str;
		}
		for(int i=0;i<qdotContourLRP[j].size();i++)
		{
			char *str = new char [50];
			    sprintf(str, "L R [ %0.2f %0.2f ] %0.2f\n",
						qdotContourLRP[j][i][0], qdotContourLRP[j][i][1],
						qdotContourLR[j][i]);
			    strcat(contour_text,  str);
			delete [] str;
		}
		for(int i=0;i<qdotContourGA[j].size();i++)
		{
			char *str = new char [50];
			    sprintf(str, "G A %6.2e\n", 
						qdotContourGA[j][i]);
			    strcat(contour_text,  str);
			delete [] str;
		}
		for(int i=0;i<qdotContourGR[j].size();i++)
		{
			char *str = new char [50];
			    sprintf(str, "G R %6.2e\n", 
						qdotContourGR[j][i]);
			    strcat(contour_text,  str);
			delete [] str;
		}

		glui_contouredit[j]->set_text((const char*) contour_text);

		delete [] contour_text;	
	//}
}
/*
void glui_symmetry()
{
	glui_symmetry_pop = GLUI_Master.create_glui("Symmetry Plane");	
	
	glui_symmetrysync = glui_symmetry_pop->add_checkbox("Sync", &qdotSymmetrySync, Symmetry_Sync_id, control);
	glui_symmetrytype = glui_symmetry_pop->add_checkbox("Direct manipulation", &qdotSymmetryType, Symmetry_Edit_Type_id, control);

	GLUI_Panel *obj_symmetry_left = glui_symmetry_pop->add_panel("Left window");
	glui_symmetryedit[0] = new GLUI_TextBox(obj_symmetry_left, true, Symmetry_List_id0, control);
	glui_symmetryedit[0] ->set_w(400);
	glui_symmetryedit[0] ->set_h(100);
	//glui_symmetrytype[0] = glui_symmetry_pop->add_listbox_to_panel(obj_symmetry_left, "Type", &qdotSymmetryType[0],
	//					Symmetry_Edit_Type_id0, control);
	//glui_symmetrytype[0]->add_item(0, "Modification of context");
	//glui_symmetrytype[0]->add_item(1, "Direct manipulation");
	GLUI_Panel *obj_symmetry_right = glui_symmetry_pop->add_panel("Right window");
	glui_symmetryedit[1] = new GLUI_TextBox(obj_symmetry_right, true, Symmetry_List_id1, control);
	glui_symmetryedit[1] ->set_w(400);
	glui_symmetryedit[1] ->set_h(100);
	//glui_symmetrytype[1] = glui_symmetry_pop->add_listbox_to_panel(obj_symmetry_right, "Type", &qdotSymmetryType[1],
	//					Symmetry_Edit_Type_id1, control);
	//glui_symmetrytype[1]->add_item(0, "Modification of context");
	//glui_symmetrytype[1]->add_item(1, "Direct manipulation");
	glui_symmetryok = glui_symmetry_pop->add_button("OK", Symmetry_OK_id, control);
	glui_symmetrycancel = glui_symmetry_pop->add_button("Cancel", Symmetry_Cancel_id, control);

	glui_symmetry_display();
}
void glui_symmetry_display()
{
	for(int j=0;j<2;j++)
	{
		char *contour_text = new char[1024];

		for(int i=0;i<1024;i++)
		{
			contour_text[i] = '\0';
		}
		for(int i=0;i<qdotSymmetryV[j].size();i++)
		{
			char *str = new char [50];
			    sprintf(str, "P %0.2f %0.2f %0.2f ", 
					qdotSymmetryV[j][i][0], qdotSymmetryV[j][i][1], qdotSymmetryV[j][i][2]);
			    strcat(contour_text,  str);
			    sprintf(str, "N %0.2f %0.2f %0.2f \n", 
					qdotSymmetryN[j][i][0], qdotSymmetryN[j][i][1], qdotSymmetryN[j][i][2]);
			    strcat(contour_text,  str);
			delete [] str;
		}

		glui_symmetryedit[j]->set_text((const char*) contour_text);

		delete [] contour_text;	
	}
}
void glui_cluster()
{
	glui_cluster_pop = GLUI_Master.create_glui("Clustering");

	glui_clustertogether = glui_cluster_pop->add_checkbox("Integrated clustering", &qdotClusterTogether, Cluster_Together_id, control);	
	glui_clustersync = glui_cluster_pop->add_checkbox("Sync", &qdotClusterSync, Cluster_Sync_id, control);
	glui_clusterlayer=glui_cluster_pop->add_checkbox("By layer", &qdotClusterlayer, Cluster_Layer_id, control);
	GLUI_Panel *obj_cluster_left = glui_cluster_pop->add_panel("Left window");
	glui_clustertype[0] = glui_cluster_pop->add_listbox_to_panel(obj_cluster_left,
						"Type", &qdotClusterType[0], Cluster_Type_id0, control);
	glui_clustertype[0]->add_item(0, "Magnitude");
	glui_clustertype[0]->add_item(1, "Orientation");
	glui_clusterROI[0] = glui_cluster_pop->add_checkbox_to_panel(obj_cluster_left,
						"ROI", &qdotClusterROI[0], Cluster_ROI_id0, control);
	glui_clusternum[0] = glui_cluster_pop->add_edittext_to_panel(obj_cluster_left,
						"Number of clusters: ", GLUI_EDITTEXT_INT, &qdotClusterNum[0], Cluster_Num_id0, control);
	GLUI_Panel *obj_cluster_right = glui_cluster_pop->add_panel("Right window");
	glui_clustertype[1] = glui_cluster_pop->add_listbox_to_panel(obj_cluster_right,
						"Type", &qdotClusterType[1], Cluster_Type_id1, control);
	glui_clustertype[1]->add_item(0, "Magnitude");
	glui_clustertype[1]->add_item(1, "Orientation");
	glui_clusterROI[1] = glui_cluster_pop->add_checkbox_to_panel(obj_cluster_right,
						"ROI", &qdotClusterROI[1], Cluster_ROI_id1, control);
	glui_clusternum[1] = glui_cluster_pop->add_edittext_to_panel(obj_cluster_right,
						"Number of clusters: ", GLUI_EDITTEXT_INT, &qdotClusterNum[1], Cluster_Num_id1, control);
	glui_clusterok = glui_cluster_pop->add_button("OK",Cluster_OK_id, control);
	glui_clustercancel = glui_cluster_pop->add_button("Cancel", Cluster_Cancel_id, control);
}
void glui_distance()
{
	glui_distance_pop = GLUI_Master.create_glui("Distance map");
	
	glui_distancesync = glui_distance_pop->add_checkbox("Sync", &qdotDistanceSync, Distance_Sync_id, control);
	GLUI_Panel *obj_distance_left = glui_distance_pop->add_panel("Left window");	
	glui_distanceedit[0] = new GLUI_TextBox(obj_distance_left,true,Distance_List_id0,control);
	glui_distanceedit[0]->set_w(200);
	glui_distanceedit[0]->set_h(80);
	glui_distancez[0] = glui_distance_pop->add_checkbox_to_panel(obj_distance_left,"Select reference layer", &qdotDistanceZ[0],
				Distance_Z_id0, control);
	GLUI_Panel *obj_distance_right = glui_distance_pop->add_panel("Right window");	
	glui_distanceedit[1] = new GLUI_TextBox(obj_distance_right,true,Distance_List_id1,control);
	glui_distanceedit[1]->set_w(200);
	glui_distanceedit[1]->set_h(80);
	glui_distancez[1] = glui_distance_pop->add_checkbox_to_panel(obj_distance_right,"Select reference layer", &qdotDistanceZ[1],
				Distance_Z_id1, control);
	glui_distanceok= glui_distance_pop->add_button("OK", Distance_OK_id, control);
	glui_distancecancel = glui_distance_pop->add_button("Cancel", Distance_Cancel_id, control);

	glui_distance_display();
}
void glui_distance_display()
{
	for(int j=0;j<2;j++)
	{
		char *contour_text = new char[1024];

		for(int i=0;i<1024;i++)
		{
			contour_text[i] = '\0';
		}
		for(int i=0;i<qdotDistanceV[j].size();i++)
		{
			char *str = new char [50];
			    sprintf(str, "Z %0.2f V %0.2f %0.2f \n", 
					qdotDistanceV[j][i][2], qdotDistanceV[j][i][0], qdotDistanceV[j][i][1]);
			    strcat(contour_text,  str);
			delete [] str;
		}

		glui_distanceedit[j]->set_text((const char*) contour_text);

		delete [] contour_text;	
	}
}*/
/*void glui_cutting()
{
	glui_cutting_pop = GLUI_Master.create_glui("Cutting plane");

	GLUI_Panel *group = glui_cutting_pop->add_panel("");
	group->set_alignment(GLUI_ALIGN_LEFT);
	glui_cuttingedit = glui_cutting_pop->add_checkbox_to_panel(group, "Direct manipulation", &qdotCuttingEdit, Cutting_Edit_id, control);
	//glui_cutting_pop->add_column_to_panel(group, true);
	glui_cuttingsync = glui_cutting_pop->add_checkbox_to_panel(group, "Sync", &qdotCuttingSync, Cutting_Sync_id, control);

	GLUI_Panel *obj_cutting_left = glui_cutting_pop->add_panel( "Left window");
	GLUI_Panel *obj1 = glui_cutting_pop->add_panel_to_panel(obj_cutting_left,"Point");
	glui_cuttingvx[0] = glui_cutting_pop->add_edittext_to_panel(obj1, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[0][0], Cutting_Vector_id0, control);
	glui_cuttingvx[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj1, true);
	glui_cuttingvy[0] = glui_cutting_pop->add_edittext_to_panel(obj1, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[0][1], Cutting_Vector_id0, control);
	glui_cuttingvy[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj1, true);
	glui_cuttingvz[0] = glui_cutting_pop->add_edittext_to_panel(obj1, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[0][2], Cutting_Vector_id0, control);
	glui_cuttingvz[0]->set_w(50);
	GLUI_Panel *obj2 = glui_cutting_pop->add_panel_to_panel(obj_cutting_left,"Normal");
	glui_cuttingnx[0] = glui_cutting_pop->add_edittext_to_panel(obj2, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[0][0], Cutting_Normal_id0, control);
	glui_cuttingnx[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj2, true);
	glui_cuttingny[0] = glui_cutting_pop->add_edittext_to_panel(obj2, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[0][1], Cutting_Normal_id0, control);
	glui_cuttingny[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj2, true);
	glui_cuttingnz[0] = glui_cutting_pop->add_edittext_to_panel(obj2, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[0][2], Cutting_Normal_id0, control);
	glui_cuttingnz[0]->set_w(50);

	GLUI_Panel *obj_cutting_right = glui_cutting_pop->add_panel("Right window");
	GLUI_Panel *obj3 = glui_cutting_pop->add_panel_to_panel(obj_cutting_right,"Point");
	glui_cuttingvx[1] = glui_cutting_pop->add_edittext_to_panel(obj3, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[1][0], Cutting_Vector_id1, control);
	glui_cuttingvx[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj3, true);
	glui_cuttingvy[1] = glui_cutting_pop->add_edittext_to_panel(obj3, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[1][1], Cutting_Vector_id1, control);
	glui_cuttingvy[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj3, true);
	glui_cuttingvz[1] = glui_cutting_pop->add_edittext_to_panel(obj3, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[1][2], Cutting_Vector_id1, control);
	glui_cuttingvz[1]->set_w(50);
	GLUI_Panel *obj4 = glui_cutting_pop->add_panel_to_panel(obj_cutting_right,"Normal");
	glui_cuttingnx[1] = glui_cutting_pop->add_edittext_to_panel(obj4, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[1][0], Cutting_Normal_id1, control);
	glui_cuttingnx[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj4, true);
	glui_cuttingny[1] = glui_cutting_pop->add_edittext_to_panel(obj4, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[1][1], Cutting_Normal_id1, control);
	glui_cuttingny[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj4, true);
	glui_cuttingnz[1] = glui_cutting_pop->add_edittext_to_panel(obj4, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[1][2], Cutting_Normal_id1, control);
	glui_cuttingnz[1]->set_w(50);
}*/
void glui_cuttingdisplay(int data_type)
{
	int j = data_type;

		char *contour_text = new char[1024];

		for(int i=0;i<1024;i++)
		{
			contour_text[i] = '\0';
		}
		for(int i=0;i<1;i++)
		{
			char *str = new char [50];
			    sprintf(str, "P [ %0.2f, %0.2f, %0.2f ], N [ %0.2f, %0.2f, %0.2f ] \n", 
					qdotCuttingV[j][0], qdotCuttingV[j][1], qdotCuttingV[j][2],
					qdotCuttingN[j][0], qdotCuttingN[j][1], qdotCuttingN[j][2]);
			    strcat(contour_text,  str);
			delete [] str;
		}
		glui_cuttingedit[j]->set_text((const char*) contour_text);
//===========================

		for(int i=0;i<1024;i++)
		{
			contour_text[i] = '\0';
		}
		for(int i=0;i<qdotCuttingSingle[data_type].size();i++)
		{
			char *str = new char [50];
			sprintf(str, "S %d \n", qdotCuttingSingle[data_type][i]);
			strcat(contour_text,  str);
			delete [] str;
		}
		for(int i=0;i<qdotCuttingMulti[data_type].size();i++)
		{
			char *str = new char [50];
			sprintf(str, "M [ %d %d ] \n", 
					qdotCuttingMulti[data_type][i][0], 
					qdotCuttingMulti[data_type][i][1]);
			strcat(contour_text,  str);
			delete [] str;
		}
		//cerr<<contour_text<<" "<<qdotCuttingSingle[data_type].size()<<endl;
		glui_cuttingscroll[j]->set_text((const char*) contour_text);
//===========================
		delete [] contour_text;	

		contour_text = new char[8000];
		vector<svVector3> pos = qdotCutSlice[data_type]->getPlanePos();
		for(int i=0;i<8000;i++)
		{
			contour_text[i] = '\0';
		}
		for(int ii=0;ii<pos.size();ii++)
		{
			char *str = new char [20];
			sprintf(str, "%d (%0.2f,%0.2f,%0.2f)\n", ii, pos[ii][0], pos[ii][1], pos[ii][2]);
			strcat(contour_text,  str);
			delete [] str;

			glui_cuttinglist[j]->set_text((const char*) contour_text);
		}
		//glui_cuttinglist[j]->disable();
		delete [] contour_text;	
		pos.clear();
}
/*
void glui_cutting()
{
	glui_cutting_pop = GLUI_Master.create_glui("Cutting plane");

	GLUI_Panel *group = glui_cutting_pop->add_panel("");
	group->set_alignment(GLUI_ALIGN_LEFT);
	glui_cuttingsync = glui_cutting_pop->add_checkbox_to_panel(group, "Sync", &qdotCuttingSync, Cutting_Sync_id, control);

	GLUI_Panel *obj_cutting_left = glui_cutting_pop->add_panel( "Left window");
	GLUI_Panel *obj1 = glui_cutting_pop->add_panel_to_panel(obj_cutting_left,"Point");
	glui_cuttingvx[0] = glui_cutting_pop->add_edittext_to_panel(obj1, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[0][0], Cutting_Vector_id0, control);
	glui_cuttingvx[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj1, true);
	glui_cuttingvy[0] = glui_cutting_pop->add_edittext_to_panel(obj1, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[0][1], Cutting_Vector_id0, control);
	glui_cuttingvy[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj1, true);
	glui_cuttingvz[0] = glui_cutting_pop->add_edittext_to_panel(obj1, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[0][2], Cutting_Vector_id0, control);
	glui_cuttingvz[0]->set_w(50);
	GLUI_Panel *obj2 = glui_cutting_pop->add_panel_to_panel(obj_cutting_left,"Normal");
	glui_cuttingnx[0] = glui_cutting_pop->add_edittext_to_panel(obj2, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[0][0], Cutting_Normal_id0, control);
	glui_cuttingnx[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj2, true);
	glui_cuttingny[0] = glui_cutting_pop->add_edittext_to_panel(obj2, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[0][1], Cutting_Normal_id0, control);
	glui_cuttingny[0]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj2, true);
	glui_cuttingnz[0] = glui_cutting_pop->add_edittext_to_panel(obj2, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[0][2], Cutting_Normal_id0, control);
	glui_cuttingnz[0]->set_w(50);

	GLUI_Panel *obj_cutting_right = glui_cutting_pop->add_panel("Right window");
	GLUI_Panel *obj3 = glui_cutting_pop->add_panel_to_panel(obj_cutting_right,"Point");
	glui_cuttingvx[1] = glui_cutting_pop->add_edittext_to_panel(obj3, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[1][0], Cutting_Vector_id1, control);
	glui_cuttingvx[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj3, true);
	glui_cuttingvy[1] = glui_cutting_pop->add_edittext_to_panel(obj3, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[1][1], Cutting_Vector_id1, control);
	glui_cuttingvy[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj3, true);
	glui_cuttingvz[1] = glui_cutting_pop->add_edittext_to_panel(obj3, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingV[1][2], Cutting_Vector_id1, control);
	glui_cuttingvz[1]->set_w(50);
	GLUI_Panel *obj4 = glui_cutting_pop->add_panel_to_panel(obj_cutting_right,"Normal");
	glui_cuttingnx[1] = glui_cutting_pop->add_edittext_to_panel(obj4, "x:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[1][0], Cutting_Normal_id1, control);
	glui_cuttingnx[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj4, true);
	glui_cuttingny[1] = glui_cutting_pop->add_edittext_to_panel(obj4, "y:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[1][1], Cutting_Normal_id1, control);
	glui_cuttingny[1]->set_w(50);
	glui_cutting_pop->add_column_to_panel(obj4, true);
	glui_cuttingnz[1] = glui_cutting_pop->add_edittext_to_panel(obj4, "z:", GLUI_EDITTEXT_FLOAT,
					&qdotCuttingN[1][2], Cutting_Normal_id1, control);
	glui_cuttingnz[1]->set_w(50);
}

void glui_layerclustering()
{
  	glui_lcluster = GLUI_Master.create_glui_subwindow("Clustering");
//----------------------------------------------------------------
	
	GLUI_Panel *obj_data = glui_lcluster->add_panel("Weighted K-means clustering");

//----------------------------------------------------------------
	GLUI_Panel *obj_1 = glui->add_panel_to_panel(obj_data,"Step 1");

	GLUI_Panel * obj_l = glui->add_panel_to_panel(obj_1,"Location");
	char str[20];
	sprintf(str,"%0.2f",qdotLweight[0][0]);
	glui_LWtext00 = glui_lcluster->add_statictext_to_panel(obj_l,str);
	glui_LWtext00 -> set_w(5);
	glui_lcluster->add_column_to_panel(obj_l, true);
	GLUI_Scrollbar *sb_location = new GLUI_Scrollbar(obj_l, "Location",  GLUI_SCROLL_HORIZONTAL,&qdotLweight[0][0], Lweight_id00, control);
	sb_location->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_1, true);

	GLUI_Panel * obj_v = glui->add_panel_to_panel(obj_1,"Direction");
	sprintf(str,"%0.2f",qdotVweight[0][0]);
	glui_VWtext00 = glui_lcluster->add_statictext_to_panel(obj_v,str);
	glui_VWtext00 -> set_w(5);
	glui_lcluster->add_column_to_panel(obj_v, true);
	GLUI_Scrollbar *sb_velocity = new GLUI_Scrollbar(obj_v, "Direction",  GLUI_SCROLL_HORIZONTAL,&qdotVweight[0][0], Vweight_id00, control);
	sb_velocity->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_1, true);

	GLUI_Panel * obj_d = glui->add_panel_to_panel(obj_1,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[0][0]);
	glui_DWtext00 = glui_lcluster->add_statictext_to_panel(obj_d,str);
	glui_DWtext00->set_w(5);
	glui_lcluster->add_column_to_panel(obj_d, true);
	GLUI_Scrollbar *sb_density = new GLUI_Scrollbar(obj_d, "Magnitude",  GLUI_SCROLL_HORIZONTAL, &qdotDweight[0][0], Dweight_id00, control);
	sb_density->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_1, true);
	GLUI_EditText *glui_text = new GLUI_EditText(obj_1, "# of clusters", &qdotHCNum[0][0]);
	glui_text->set_w(8);

//----------------------------------------------------------------
	GLUI_Panel *obj_2 = glui->add_panel_to_panel(obj_data,"Step 2");

	GLUI_Panel * obj_l2 = glui->add_panel_to_panel(obj_2,"Location");
	//char str[20];
	sprintf(str,"%0.2f",qdotLweight[0][1]);
	glui_LWtext01 = glui_lcluster->add_statictext_to_panel(obj_l2,str);
	glui_LWtext01->set_w(5);
	glui_lcluster->add_column_to_panel(obj_l2, true);
	GLUI_Scrollbar *sb_location1 = new GLUI_Scrollbar(obj_l2, "Location",  GLUI_SCROLL_HORIZONTAL,&qdotLweight[0][1], Lweight_id01, control);
	sb_location->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_2, true);

	GLUI_Panel * obj_v2 = glui->add_panel_to_panel(obj_2,"Direction");
	sprintf(str,"%0.2f",qdotVweight[0][1]);
	glui_VWtext01 = glui_lcluster->add_statictext_to_panel(obj_v2,str);
	glui_VWtext01->set_w(5);
	glui_lcluster->add_column_to_panel(obj_v2, true);
	GLUI_Scrollbar *sb_velocity1 = new GLUI_Scrollbar(obj_v2, "Direction",  GLUI_SCROLL_HORIZONTAL,&qdotVweight[0][1], Vweight_id01, control);
	sb_velocity->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_2, true);

	GLUI_Panel * obj_d2 = glui->add_panel_to_panel(obj_2,"Magnitude");
	sprintf(str,"%0.2f",qdotDweight[0][1]);
	glui_DWtext01 = glui_lcluster->add_statictext_to_panel(obj_d2,str);
	glui_DWtext01->set_w(5);
	glui_lcluster->add_column_to_panel(obj_d2, true);
	GLUI_Scrollbar *sb_density1 = new GLUI_Scrollbar(obj_d2, "Magnitude",  GLUI_SCROLL_HORIZONTAL, &qdotDweight[0][1], Dweight_id01, control);
	sb_density->set_float_limits(0,1);
	glui_lcluster->add_column_to_panel(obj_2, true);
	GLUI_EditText *glui_text3 = new GLUI_EditText(obj_2, "# of clusters", &qdotHCNum[0][1]);
	glui_text3->set_w(5);

	glui_lcluster->add_column_to_panel(obj_data, true);
//----------------------------------------------------------------
	GLUI_Button *sb_update = new GLUI_Button(obj_data, "Update", Uweight_id0, control);
}*/
  	
