#include "qdotFunction.h"

void redisplay_all(void)
{
    glutSetWindow(window);
    glutPostRedisplay();

    glutSetWindow(left_window);
    glutPostRedisplay();

    glutSetWindow(right_window);
    glutPostRedisplay();
}

void fdataok(int id)
{
	ListFile(datadir[id]);
	
	int ii = glui_datalist[id]->get_current_item();
	//cerr<<ii<<endl;

	if(!strcmp(datafile[id],datastring[ii].c_str()))
	{
		
	}
	else
	{
		sprintf(datafile[id], "%s", datastring[ii].c_str());
		//text_data[data_type] = glui[data_type]->add_statictext_to_panel(obj_data, datafile[data_type]);

		qdotNewdata[id] = true;	
		qdotNewContour[id] = true;
		qdotNewLayerContour[id] = true;
		qdotNewContourCluster[id] = true;
		qdotNewCluster[id] =true;
		qdotNewDistance[id] = true;
		qdotNewArrow[id] =true;
		qdotNewArrowData[id] =true;
		qdotNewArrowContour[id] =true;
	}
}

void fdatacancel(int id)
{	
	ListFile(datadir[id]);
	int ii;
	for(int i=0;i<datastring.size();i++)
	{
		if(!strcmp(datafile[id],datastring[i].c_str()))
		{
			ii = i;break;
		}
	}
	glui_datalist[id]->set_int_val(ii);
	/*
		for(int i=0;i<datastring.size();i++)
		{
			char *str = new char[datastring[i].length()+1];
			strcpy(str,datastring[i].c_str());
			//cerr<<str<<endl;
			hah[data_type]->add_item(i,str);	
			//cerr<<i<<" "<<str<<endl;	
			delete [] str;
		}*/		
}

void fdataprocessing()
{
	for(int i=0;i<2;i++)
	{
		if(qdotNewdata[i])
		{
			cerr<<"start load new"<<endl;
			qdotField[i]->sortData(datadir[i], datafile[i], tmpfiledir);
			cerr<<"set data"<<endl;
			qdotField[i]->setCellRegion(1);
			qdotField[i]->setData();
			cerr<<"set format"<<endl;
			qdotField[i]->setFormat();
			cerr<<"set map"<<endl;
			qdotRegionList[i] = qdotField[i]->getRegionIndex();
			qdotMap[i]->New(qdotField[i]->getRawPos(), qdotField[i]->getRawSize(),
						qdotField[i]->getLbbox(), qdotField[i]->getRbbox(),
						qdotField[i]->getXd(), qdotField[i]->getYd(),
						qdotField[i]->getXl(), qdotField[i]->getYl(),
						qdotField[i]->getXnum(), qdotField[i]->getYnum(),
						qdotField[i]->getZdistance());
			qdotCutSlice[i]->SetData(qdotField[i]->getLbbox(), qdotField[i]->getRbbox()); 
		}
	}
	qdotScalar = max(qdotField[0]->getScalar(), qdotField[1]->getScalar());
}

void farrowprocessing()
{
	for(int i=0;i<2;i++)
	{
		if(qdotDisplay[i] == 0)// && qdotNewArrowContour[i])//D_Contour)
		{
			cerr<<"update data 1 "<<endl;
			qdotArrowSelection[i]->SetData(qdotField[i]->getLayerContourPos(), 
								qdotField[i]->getLayerContourVel(),
								qdotField[i]->getLayerContourDen(),
								qdotField[i]->getLayerContourExp(),
								qdotField[i]->getLayerContourCoe(),
								qdotField[i]->getLayerContourAtom(),
								qdotField[i]->getLayerContourSize());
			qdotArrowSelection[i]->setMaxMag(qdotField[i]->getMax());
			qdotArrowSelection[i]->SetInbox(NULL);
			qdotDisc[i]->SetData(qdotField[i]->getLayerContourPos(), 
							qdotField[i]->getLayerContourVel(),
							qdotField[i]->getLayerContourDen(),
							qdotField[i]->getLayerContourExp(),
							qdotField[i]->getLayerContourCoe(),
							qdotField[i]->getLayerContourSize());
			qdotArrowSelection[i]->ReadDataFormat(qdotField[i]->getFormat());
			qdotNewContourCluster[i] = true;
		}
		else if(qdotDisplay[i] == 1)// && qdotNewArrowData[i])
		{
			cerr<<"update data 2"<<endl;
			qdotArrowSelection[i]->SetData(qdotField[i]->getRawPos(), 
								qdotField[i]->getRawVel(),
								qdotField[i]->getRawDen(),
								qdotField[i]->getRawExp(),
								qdotField[i]->getRawCoe(),
								qdotField[i]->getRawAtom(),
								qdotField[i]->getRawSize());
			qdotArrowSelection[i]->setMaxMag(qdotField[i]->getMax());
			qdotArrowSelection[i]->SetInbox(qdotField[i]->getRawInbox());
			qdotDisc[i]->SetData(qdotField[i]->getLayerPos(), 
							qdotField[i]->getLayerVel(),
							qdotField[i]->getLayerDen(),
							qdotField[i]->getLayerExp(),
							qdotField[i]->getLayerCoe(),
							qdotField[i]->getLayerSize());
			qdotArrowSelection[i]->ReadDataFormat(qdotField[i]->getFormat());
			qdotNewCluster[i] = true;

		}


	}
	for(int i=0;i<2;i++)
	{
		if(qdotDisplay[i] == 0)// && qdotNewContourCluster[i])//D_Contour)
		{
			cerr<<"update cluster 1 "<<endl;
			//qdotArrowSelection[i]->SetInbox(NULL);
			qdotDisc[i]->SetClusters(qdotField[i]->getLayerContourCluster(),
							qdotHCNum[i][0]+qdotHCNum[i][1]);
			qdotDisc[i]->computeMean(qdotField[i]->getLayerDenContourCluster(),qdotField[i]->getMaglevel());

			qdotNewArrow[i] = true;
		}
		else if(qdotDisplay[i] == 1)// && qdotNewCluster[i])
		{
			cerr<<"update cluster 2"<<endl;
			//qdotArrowSelection[i]->SetInbox(qdotField[i]->getRawInbox());
			qdotDisc[i]->SetClusters(qdotField[i]->getLayerCluster(),
							qdotHCNum[i][0]+qdotHCNum[i][1]);
			qdotDisc[i]->computeMean(qdotField[i]->getLayerDenCluster(),qdotField[i]->getMaglevel());
			qdotNewArrow[i] = true;
		}
	}

	for(int i=0;i<2;i++)
	{
		if(qdotNewArrow[i])
		{

		}

		if(qdotDisplay[i] == 0)// && qdotNewArrow[i])//D_Contour)
		{
			cerr<<"update render 1"<<endl;
			qdotArrowSelection[i]->SetSample(qdotFrequency, qdotDisplay[i]);
			qdotArrowSelection[i]->SetFontColor(qdotFont);
			qdotArrowSelection[i]->SetEncodeMode(qdotEncode[i]);
			qdotArrowSelection[i]->SetScalar(qdotScalar);
			cerr<<qdotLength<<endl;
			if(qdotEncode[i] == 0)
			{	qdotArrowSelection[i]->setLength(qdotLength * 5e-11);}
			else
			{	qdotArrowSelection[i]->setLength(qdotLength);}
			qdotArrowSelection[i]->setArrowBase(qdotArrowBase);
			qdotArrowSelection[i]->SetArrows();
			qdotArrowSelection[i]->displayList = i+1;
			if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourCluster(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourAtom(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());
		}
		else if(qdotDisplay[i] == 1)// && qdotNewArrow[i])
		{
			cerr<<"update render 2"<<endl;
			qdotArrowSelection[i]->SetSample(qdotFrequency, qdotDisplay[i]);
			qdotArrowSelection[i]->SetFontColor(qdotFont);
			qdotArrowSelection[i]->SetEncodeMode(qdotEncode[i]);
			qdotArrowSelection[i]->SetScalar(qdotScalar);
			if(qdotEncode[i] == 0)
			{	qdotArrowSelection[i]->setLength(qdotLength * 5e-11);}
			else
			{	qdotArrowSelection[i]->setLength(qdotLength);}
			qdotArrowSelection[i]->setArrowBase(qdotArrowBase);
			qdotArrowSelection[i]->SetArrows();
			qdotArrowSelection[i]->displayList = i+1;
			if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());

		}

		//qdotArrowGlyphRenderArrow[i]->svArrowGlyphRenderArrow(qdotOpacity);
	}

	if(qdotNewArrow[0])
	{
		glutSetWindow(left_window);
		qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
		glutPostRedisplay();
	}
	if(qdotNewArrow[1])
	{
		glutSetWindow(right_window);
		qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);
		glutPostRedisplay();
	}
}
/*
void fdata()
{
	//glui_load
}
void fsymmetrymove()
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
}
void fsymmetryenable()
{
	if(qdotSymmetryType)
	{
		for(int i=0;i<2;i++)
		{
			glui_symmetryedit[i]->disable();
		}
	}
	else
	{
		for(int i=0;i<2;i++)
		{
			glui_symmetryedit[i]->enable();
		}
	}	
}
void fsymmetryedit(int id)
{
	qdotNewSymmetry[id] = true;
	if(qdotSymmetrySync)
	{
		glui_symmetryedit[1-id]->set_text(glui_symmetryedit[id]->get_text());
		qdotNewSymmetry[1-id] = true;
	}
}
void fsymmetryparser()
{
	for(int i=0;i<2;i++)
	{
		if(qdotNewSymmetry[i])
		{
			qdotSymmetryN[i].clear();
			qdotSymmetryN[i].clear();

			char *contour_text = new char[1024];
			contour_text = (char *)(glui_symmetryedit[i]->get_text());
			char str[32];
			char *pch2 = strtok(contour_text, " []\n");
			while (pch2 !=NULL)
			{
				cerr<<pch2<<endl;
				if(!strcmp(pch2,"P"))
				{
					pch2 = strtok(NULL, " []\n");
					svVector3 n;
					n[0] = atof(pch2);
					pch2 = strtok(NULL, " []\n");
					n[1] = atof(pch2);
					pch2 = strtok(NULL, " []\n");
					n[2] = atof(pch2);
					qdotSymmetryV[i].push_back(n);
				}
				else
				{
					pch2 = strtok(NULL, " []\n");
					svVector3 n;
					n[0] = atof(pch2);
					pch2 = strtok(NULL, " []\n");
					n[1] = atof(pch2);
					pch2 = strtok(NULL, " []\n");
					n[2] = atof(pch2);
					qdotSymmetryN[i].push_back(n);			
				}
				pch2 = strtok(NULL, " []\n");
			}	
			delete [] contour_text;
			delete pch2;
		}
	}
}
void fsymmetryprocessing()
{
	fsymmetryparser();

	for(int i=0;i<2;i++)
	{
		if(qdotNewSymmetry[i])
			qdotField[i]->symmetryGeneration(qdotSymmetryV[i], qdotSymmetryN[i]);
	}
}*/
/*void flayerclusteredit(int id)
{
	switch (id)
	{
		case Lweight_id00:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotLweight[0][0]);
			glui_LWtext00->set_text(str);
			break;
		}
		case Vweight_id00:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotVweight[0][0]);
			glui_VWtext00->set_text(str);
			break;
		}
		case Dweight_id00:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotDweight[0][0]);
			glui_DWtext00->set_text(str);
			break;
		}
		case Lweight_id01:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotLweight[0][1]);
			glui_LWtext01->set_text(str);
			break;
		}
		case Vweight_id01:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotVweight[0][1]);
			glui_VWtext01->set_text(str);
			break;
		}
		case Dweight_id01:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotDweight[0][1]);
			glui_DWtext01->set_text(str);
			break;
		}
		case Lweight_id10:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotLweight[1][0]);
			glui_LWtext10->set_text(str);
			break;
		}
		case Vweight_id10:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotVweight[1][0]);
			glui_VWtext10->set_text(str);
			break;
		}
		case Dweight_id10:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotDweight[1][0]);
			glui_DWtext10->set_text(str);
			break;
		}
		case Lweight_id11:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotLweight[1][1]);
			glui_LWtext11->set_text(str);
			break;
		}
		case Vweight_id11:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotVweight[1][1]);
			glui_VWtext11->set_text(str);
			break;
		}
		case Dweight_id11:
		{
			char str[20];
			sprintf(str,"%0.2f",qdotDweight[1][1]);
			glui_DWtext11->set_text(str);
			break;
		}

	}

	//	cout<<qdotLweight[0][0]<<" "<<qdotVweight[0][0]<<" "<<qdotDweight[0][0]<<" "
	//		<<qdotLweight[0][1]<<" "<<qdotVweight[0][1]<<" "<<qdotDweight[0][1]<<endl;
}
void flayerclusterupdate(int id)
{
	if(id = Uweight_id0)
	{
		//cerr<<"newcluster"<<endl;
		double mag[2];
		mag[0] = qdotHCmag[0][0];
		mag[1] = qdotHCmag[0][1];
		double ws[2], wv[2], wd[2];
		ws[0]=qdotLweight[0][0];ws[1]=qdotLweight[0][1];
		wv[0]=qdotVweight[0][0];wv[1]=qdotVweight[0][1];
		wd[0]=qdotDweight[0][0];wd[1]=qdotDweight[0][1];
		//qdotField[0]->layerClustering(qdotHCNum[0],mag, ws,wv,wd,false);
		//cerr<<"disc"<<" "<<qdotClusterNum[0]<<endl;//checkpoint!!!!!!!!!!!!!!!!!
		qdotDisc[0]->SetClusters(qdotField[0]->getRawLayerVelcluster(),
						qdotHCNum[0][0]+qdotHCNum[0][1], 
						qdotClusterlayer,qdotField[0]->getZlayer(),
						qdotField[0]->getZmin(),qdotField[0]->getZdistance());
		//cerr<<"disc"<<endl;//checkpoint!!!!!!!!!!!!!!!!!
		qdotDisc[0]->computeMean(qdotField[0]->getRawMagcluster(), 
						qdotField[0]->getMaglevel());
		qdotArrowSelection[0]->SetExpand(qdotDisc[0]->GetROI());
		//cerr<<"color"<<endl;
		qdotArrowSelection[0]->SetColors(qdotColor->getColors(qdotBlind, 
								qdotClusterType[0], qdotHCNum[0][0]+qdotHCNum[0][1], //qdotClusterNum[0], 
								qdotField[0]->getRawLayerVelcluster(),
								qdotField[0]->getRawSize(), qdotFont));
		qdotDisc[0]->setColors(qdotColor->getColors(qdotBlind, 
								0, qdotField[0]->getMaglevel()),
								qdotColor->getColors(qdotBlind, 
								qdotClusterType[0], qdotHCNum[0][0]+qdotHCNum[0][1]),//qdotClusterNum[0]), 
								qdotField[0]->getMaglevel(), qdotClusterlayer, qdotField[0]->getZlayer());
		//cerr<<"displaylist"<<endl;
		glutSetWindow(left_window);
		qdotArrowSelection[0]->RenderArrow(qdotOpacity, renderzmin, renderzmax);
	}
	else
	{
		//cerr<<"newcluster"<<endl;
		double mag[2];
		mag[0] = qdotHCmag[1][0];
		mag[1] = qdotHCmag[1][1];
		double ws[2], wv[2], wd[2];
		ws[0]=qdotLweight[1][0];ws[1]=qdotLweight[1][1];
		wv[0]=qdotVweight[1][0];wv[1]=qdotVweight[1][1];
		wd[0]=qdotDweight[1][0];wd[1]=qdotDweight[1][1];
		//qdotField[1]->layerClustering(qdotHCNum[1],mag, ws,wv,wd,true);
		//cerr<<"disc"<<" "<<qdotClusterNum[1]<<endl;//checkpoint!!!!!!!!!!!!!!!!!
		qdotDisc[1]->SetClusters(qdotField[1]->getRawLayerVelcluster(),
						qdotHCNum[1][0]+qdotHCNum[1][1], 
						qdotClusterlayer,qdotField[1]->getZlayer(),
						qdotField[1]->getZmin(),qdotField[1]->getZdistance());
		//cerr<<"disc"<<endl;//checkpoint!!!!!!!!!!!!!!!!!
		qdotDisc[1]->computeMean(qdotField[1]->getRawMagcluster(), 
						qdotField[1]->getMaglevel());
		qdotArrowSelection[1]->SetExpand(qdotDisc[1]->GetROI());
		//cerr<<"color"<<endl;
		qdotArrowSelection[1]->SetColors(qdotColor->getColors(qdotBlind, 
								qdotClusterType[1], qdotHCNum[1][0]+qdotHCNum[1][1], //qdotClusterNum[0], 
								qdotField[1]->getRawLayerVelcluster(),
								qdotField[1]->getRawSize(), qdotFont));
		qdotDisc[1]->setColors(qdotColor->getColors(qdotBlind, 
								0, qdotField[1]->getMaglevel()),
								qdotColor->getColors(qdotBlind, 
								qdotClusterType[1], qdotHCNum[1][0]+qdotHCNum[1][1]),//qdotClusterNum[0]), 
								qdotField[1]->getMaglevel(), qdotClusterlayer, qdotField[1]->getZlayer());
		cerr<<"displaylist"<<endl;
		glutSetWindow(left_window);
		qdotArrowSelection[1]->RenderArrow(qdotOpacity, renderzmin, renderzmax);
	}
}
*/

/*
void fclusteredit(int id)
{
	if(id < 0)
	{
		qdotNewCluster[0] = true;
		qdotNewCluster[1] = true;
	}
	else
	{	
		qdotNewCluster[id] = true;
		if(qdotClusterSync)
		{
			switch (id)
			{
				case Cluster_Type_id0: glui_clustertype[1]->set_int_val(glui_clustertype[0]->get_int_val());
							qdotNewCluster[1] = true; break;
				case Cluster_Type_id1: glui_clustertype[0]->set_int_val(glui_clustertype[1]->get_int_val());
							qdotNewCluster[0] = true; break;
				case Cluster_ROI_id0: glui_clusterROI[1]->set_int_val(glui_clusterROI[0]->get_int_val());
							qdotNewCluster[1] = true; break;
				case Cluster_ROI_id1: glui_clusterROI[0]->set_int_val(glui_clusterROI[1]->get_int_val());
							qdotNewCluster[0] = true; break;
				case Cluster_Num_id0: glui_clusternum[1]->set_int_val(glui_clusternum[0]->get_int_val());
							qdotNewCluster[1] = true; break;
				case Cluster_Num_id1: glui_clusternum[0]->set_int_val(glui_clusternum[1]->get_int_val());
							qdotNewCluster[0] = true; break;
			}
		}
	}
}
void fclusterparser()
{
	for(int i=0;i<2;i++)
	{
		int ni = qdotClusterType[i];
		int nj = qdotClusterROI[i];
		qdotCluster[i][nj][ni] = qdotClusterNum[i];
	}
}*/
/*
void fclusterprocessing()
{
	fclusterparser();
*/
	/*if(qdotClusterTogether)
	{
		if(qdotNewCluster[0] || (qdotClusterType[0]==1 && (qdotNewSymmetry[0]||qdotNewSymmetry[1])))
		{
			int *cluster = new int [qdotField[1]->getRawSize()];
			qdotField[0]->clusterGeneration(qdotClusterROI[0], qdotClusterType[0],qdotCluster[0][qdotClusterROI[0]][qdotClusterType[0]],
					datafile[1], qdotField[1]->getRawInbox(), qdotField[1]->getRawSize(), 
					qdotSymmetryV[1], qdotSymmetryN[1],
					cluster);
			qdotField[1]->setCluster(qdotClusterType[0],cluster, qdotField[0]->getMagrange());
			delete [] cluster;
		}
	}
	else
	{*/
		/*if(qdotNewCluster[0]|| (qdotClusterType[0]==1 && qdotNewSymmetry[0]))
		qdotField[0]->clusterGeneration(qdotClusterROI[0], qdotClusterType[0],qdotCluster[0][qdotClusterROI[0]][qdotClusterType[0]],
				NULL, NULL, qdotField[1]->getRawSize(), 
				qdotSymmetryV[1], qdotSymmetryN[1],
				NULL);
		if(qdotNewCluster[1]|| (qdotClusterType[1]==1 && qdotNewSymmetry[1]))
		qdotField[1]->clusterGeneration(qdotClusterROI[1], qdotClusterType[1],qdotCluster[1][qdotClusterROI[1]][qdotClusterType[1]],
				NULL, NULL, qdotField[0]->getRawSize(), 
				qdotSymmetryV[0], qdotSymmetryN[0],
				NULL);*/

		//qdotField[0]->layerClustering(qdotClusterNum[0],1,1,1);
		//qdotField[1]->layerClustering(qdotClusterNum[1],1,1,1);
	//}
	/*if(qdotNewCluster[0])
	{
		qdotField[0]->contourClustering( qdotClusterType[0],qdotClusterROI[0]);
		qdotField[0]->planeClustering( qdotClusterType[0],qdotClusterROI[0]);
	}
	if(qdotNewCluster[1])
	{
		qdotField[1]->contourClustering( qdotClusterType[1],qdotClusterROI[1]);
		qdotField[1]->planeClustering( qdotClusterType[1],qdotClusterROI[1]);	
	}*/
//}
void fcontoursync(int id)
{
	glui_contoursync[1-id]->set_int_val(glui_contoursync[id]->get_int_val());
	qdotContourSync = glui_contoursync[id]->get_int_val();
}
void fcontouredit(int id)
{
	qdotNewContour[id] = true;
	qdotNewContourCluster[id] = true;
	qdotNewArrow[id] = true;
	qdotNewArrowContour[id] = true;
	qdotNewLayerContour[id] = true;
	if(qdotContourSync)
	{	
		glui_contouredit[1-id]->set_text(glui_contouredit[id]->get_text());
		qdotNewContour[1-id] = true;
		qdotNewContourCluster[1-id] = true;
		qdotNewArrow[1-id] = true;
		qdotNewArrowContour[1-id] = true;
		qdotNewLayerContour[1-id] = true;
	}	
}
void fcontourparser()
{
	for(int i=0;i<2;i++)
	{
		if(qdotNewContour[i])
		{
			qdotContourLA[i].clear();
			for(int j=0;j<qdotContourLA[i].size();j++)qdotContourLAP[i][j].clear();
			qdotContourLAP[i].clear();
			qdotContourLR[i].clear();
			for(int j=0;j<qdotContourLR[i].size();j++)qdotContourLRP[i][j].clear();
			qdotContourLRP[i].clear();
			qdotContourGR[i].clear();
			qdotContourGA[i].clear();

			//char *contour_text = new char[1024];
			//contour_text = (char *)(glui_contouredit[i]->get_text());
			char str[32];
			char *pch2 = strtok((char *)(glui_contouredit[i]->get_text()), " []\n");
			while (pch2 !=NULL)
			{
				cerr<<pch2<<endl;
				if(!strcmp(pch2,"G"))
				{
					pch2 = strtok(NULL, " []\n");
					if(!strcmp(pch2,"A"))
					{
						pch2 = strtok(NULL," []\n");
						qdotContourGA[i].push_back(atof(pch2));
					}
					else
					{
						pch2 = strtok(NULL, " []\n");
						qdotContourGR[i].push_back(atof(pch2));
					}
				}
				else
				{
					pch2 = strtok(NULL, " []\n");
					if(!strcmp(pch2,"A"))
					{
						vector<double> store3(2);
						pch2 = strtok(NULL, " []\n");
						store3[0] = atof(pch2);
						pch2 = strtok(NULL, " []\n");
						store3[1] = atof(pch2);
						qdotContourLAP[i].push_back(store3);

						pch2 = strtok(NULL, " []\n");
						qdotContourLA[i].push_back(atof(pch2));
					}
					else
					{
						vector<double> store3(2);
						pch2 = strtok(NULL, " []\n");
						store3[0] = atof(pch2);
						pch2 = strtok(NULL, " []\n");
						store3[1] = atof(pch2);
						qdotContourLRP[i].push_back(store3);

						pch2 = strtok(NULL, " []\n");
						qdotContourLR[i].push_back(atof(pch2));
					}			
				}
				pch2 = strtok(NULL, " []\n");
			}	
			//delete [] contour_text;
			//delete pch2;
		}
	}

	for(int j=0;j<2;j++)
	{
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
	}
}
void fcontourprocessing()
{
	fcontourparser();

	for(int i=0;i<2;i++)
	{
		if(qdotNewContour[i])
		{
			qdotField[i]->contourGeneration(qdotContourLRP[i], qdotContourLR[i],
						qdotContourLAP[i], qdotContourLA[i],
						qdotContourGR[i], qdotContourGA[i]);	
			//qdotField[i]->layerContourGeneration(qdotContourLRP[i], qdotContourLR[i],
			//			qdotContourLAP[i], qdotContourLA[i],
			//			qdotContourGR[i], qdotContourGA[i],qdotCutSlice[i]->getSelected());

		}
		//if(qdotNewContour[i] || qdotNewCluster[i])
		//	qdotField[i]->contourClustering( qdotClusterType[i],qdotClusterROI[i], qdotClusterlayer);	
		//if(qdotNewContour[i])
		//{
			//qdotNewArrow[i] = true;
			//qdotNewContour[i] = false;
		//}
	}
}
void fclustersync(int id)
{
	glui_clustersync[1-id]->set_int_val(glui_clustersync[id]->get_int_val());
	qdotClusterSync = glui_clustersync[id]->get_int_val();
}

void fclustertype(int id)
{
	int type = glui_clustertype[id]->get_int_val();
	if(type == 0)
	{
		/*glui_clusterlayer[id]->disable();
		glui_cuttingmove[id]->enable();
		glui_cuttingscroll[id]->enable();*/
	}
	else if(type == 1)
	{/*
		glui_clusterlayer[id]->enable();
		glui_cuttingmove[id]->disable();
		glui_cuttingscroll[id]->enable();*/
	}
	else
	{
		/*glui_clusterlayer[id]->disable();
		glui_cuttingmove[id]->disable();*/
		glui_cuttingscroll[id]->disable();
	}
	qdotClusterType[id] = type;
	qdotNewCluster[id] = true;
	qdotNewContourCluster[id] = true;
	qdotNewArrow[id] =true;

	if(qdotClusterSync)
	{
		glui_clustertype[1-id]->set_int_val(type);
		if(type == 0)
		{
			/*glui_clusterlayer[1-id]->disable();
			glui_cuttingmove[1-id]->enable();
			glui_cuttingscroll[1-id]->enable();*/
		}
		else if(type == 1)
		{
			/*glui_clusterlayer[1-id]->enable();
			glui_cuttingmove[1-id]->disable();
			glui_cuttingscroll[1-id]->disable();*/
		}
		else
		{
			/*glui_clusterlayer[1-id]->disable();
			glui_cuttingmove[1-id]->disable();*/
			glui_cuttingscroll[1-id]->disable();
		}
		qdotClusterType[1-id] = type;	
		qdotNewCluster[1-id] = true;
		qdotNewContourCluster[1-id] = true;
		qdotNewArrow[1-id] =true;	
	}
}
void fcuttingedit(int id)
{
	qdotNewCluster[id] = true;
	qdotNewContourCluster[id] = true;
	qdotNewArrow[id] = true;
	qdotNewArrowData[id] = true;
	qdotNewArrowContour[id] = true;
	qdotNewLayerContour[id] = true;
	if(qdotClusterSync)
	{	
		

		qdotNewCluster[1-id] = true;
		qdotNewContourCluster[1-id] = true;
		qdotNewArrow[1-id] = true;
		qdotNewArrowData[1-id] = true;
		qdotNewArrowContour[1-id] = true;
		qdotNewLayerContour[1-id] = true;
	//if(qdotClusterSync)
	//{
		glui_cuttingedit[1-id]->set_text(glui_cuttingedit[id]->get_text());
	//}
	}	

	redisplay_all();
}

void fcuttingselection()
{

}

void fcuttingprocessing()
{
	for(int id=0;id<2;id++)
	{
		if(qdotNewCluster[id])
		{ 

			fcuttingeditpaser(id);
			fcuttingpaser(id);
			qdotCutSlice[id]->SetPlane(qdotCuttingV[id], qdotCuttingN[id], qdotMDegree[id]);
			qdotCutSlice[id]->setSelected(qdotCuttingSingle[id], qdotCuttingMulti[id]);
		}
	}
	for(int i=0;i<2;i++)
	{
		if(qdotNewCluster[i])
		{ 
			qdotField[i]->getPlane(qdotCuttingV[i], qdotCuttingN[i], qdotMDegree[i],qdotCutSlice[i]->getSelected());
		}
	}
}

void fcuttingdistance(int id) //only works for one layer selection
{
	qdotNewCluster[id] = true;
	qdotNewContourCluster[id] = true;
	qdotNewArrow[id] = true;	
	qdotNewArrowData[id] = true;
	qdotNewArrowContour[id] = true;
	qdotNewLayerContour[id] = true;

	qdotCutSlice[id]->SetPlane(qdotCuttingV[id], qdotCuttingN[id], qdotMDegree[id]);
	//qdotCuttingMove[id] = ((float)qdotCutSlice[id]->getCurrentPlane())/((float)qdotCutSlice[id]->getPlaneSize());

	if(qdotClusterSync)
	{	
		glui_cuttingmove[1-id]->set_float_val(glui_cuttingmove[id]->get_float_val());

		qdotMDegree[1-id] = qdotMDegree[id];
		qdotNewCluster[1-id] = true;
		qdotNewContourCluster[1-id] = true;
		qdotNewArrow[1-id] = true;
		qdotNewArrowData[1-id] = true;
		qdotNewArrowContour[1-id] = true;
		qdotNewLayerContour[1-id] = true;

		qdotCutSlice[1-id]->SetPlane(qdotCuttingV[1-id], qdotCuttingN[1-id], qdotMDegree[1-id]);
		//qdotCuttingMove[1-id] = ((float)qdotCutSlice[1-id]->getCurrentPlane())/((float)qdotCutSlice[1-id]->getPlaneSize());
	}

	redisplay_all();
}

void fcuttingmove(int id) 
{
	qdotNewCluster[id] = true;
	qdotNewContourCluster[id] = true;
	qdotNewArrow[id] = true;
	qdotNewArrowData[id] = true;
	qdotNewArrowContour[id] = true;
	qdotNewLayerContour[id] = true;

	/*int index = (int)(qdotCuttingMove[id]*qdotCutSlice[id]->getPlaneSize()) - qdotCutSlice[id]->getCurrentPlane();

	for(int i=0;i<3;i++)
	{
		qdotCuttingV[id][i] = qdotCuttingV[id][i] 
			+ qdotCuttingN[id][i] * (float)(index) * qdotMDegree[id];
	}*/
	/*fcuttingeditpaser(id);

	if(qdotCuttingSingle[id].size()>0)
		qdotCutSlice[id]->setCurrentPlane(qdotCuttingSingle[id][0]);//qdotCuttingMove[id]);
	else if (qdotCuttingMulti[id].size()>0)
		qdotCutSlice[id]->setCurrentPlane(qdotCuttingMulti[id][0][0]);//qdotCuttingMove[id]);

	char *contour_text = new char[1024];
	for(int i=0;i<1024;i++)
	{
		contour_text[i] = '\0';
	}
	char *str = new char [50];
	sprintf(str, "P [ %0.2f, %0.2f, %0.2f ], N [ %0.2f, %0.2f, %0.2f ] \n", 
				qdotCuttingV[id][0], qdotCuttingV[id][1], qdotCuttingV[id][2],
				qdotCuttingN[id][0], qdotCuttingN[id][1], qdotCuttingN[id][2]);
	strcat(contour_text,  str);
	delete [] str;
	glui_cuttingedit[id]->set_text((const char*) contour_text);
	delete [] contour_text;	
*/
	if(qdotClusterSync)
	{	
		//fcuttingeditpaser(id);

		//glui_cuttingedit[1-id]->set_text(glui_cuttingedit[id]->get_text());
		glui_cuttingscroll[1-id]->set_text(glui_cuttingscroll[id]->get_text());
		//qdotCuttingMove[1-id] = qdotCuttingMove[id];

		qdotNewCluster[1-id] = true;
		qdotNewContourCluster[1-id] = true;
		qdotNewArrow[1-id] = true;
		qdotNewArrowData[1-id] = true;
		qdotNewArrowContour[1-id] = true;
		qdotNewLayerContour[1-id] = true;
		/*index = (int)(qdotCuttingMove[1-id]*qdotCutSlice[1-id]->getPlaneSize()) - qdotCutSlice[1-id]->getCurrentPlane();
		for(int i=0;i<3;i++)
		{
			qdotCuttingV[1-id][i] = qdotCuttingV[1-id][i] 
				+ qdotCuttingN[1-id][i] * (float)(index) * qdotMDegree[1-id];
		}
		qdotCutSlice[1-id]->setCurrentPlane(qdotCuttingMove[1-id]);*/	
	}	

	redisplay_all();
}
/*
void fcuttingenable()
{
	if(qdotCuttingEdit)
	{
		for(int i=0;i<2;i++)
		{
			glui_cuttingvx[i]->disable();
			glui_cuttingvy[i]->disable();
			glui_cuttingvz[i]->disable();
			glui_cuttingnx[i]->disable();
			glui_cuttingny[i]->disable();
			glui_cuttingnz[i]->disable();
		}
	}
	else
	{
		for(int i=0;i<2;i++)
		{
			glui_cuttingvx[i]->enable();
			glui_cuttingvy[i]->enable();
			glui_cuttingvz[i]->enable();
			glui_cuttingnx[i]->enable();
			glui_cuttingny[i]->enable();
			glui_cuttingnz[i]->enable();
		}
	}
}*/
/*
void fcuttingedit(int id)
{
	if(qdotCuttingSync && !qdotCuttingEdit)
	{
		switch (id)
		{
			case Cutting_Vector_id0: glui_cuttingvx[1]->set_float_val(glui_cuttingvx[0]->get_float_val());
						 glui_cuttingvy[1]->set_float_val(glui_cuttingvy[0]->get_float_val());
						 glui_cuttingvz[1]->set_float_val(glui_cuttingvz[0]->get_float_val());break;
			case Cutting_Vector_id1: glui_cuttingvx[0]->set_float_val(glui_cuttingvx[1]->get_float_val());
						 glui_cuttingvy[0]->set_float_val(glui_cuttingvy[1]->get_float_val());
						 glui_cuttingvz[0]->set_float_val(glui_cuttingvz[1]->get_float_val());break;
			case Cutting_Normal_id0: glui_cuttingnx[1]->set_float_val(glui_cuttingnx[0]->get_float_val());
						 glui_cuttingny[1]->set_float_val(glui_cuttingny[0]->get_float_val());
						 glui_cuttingnz[1]->set_float_val(glui_cuttingnz[0]->get_float_val());break;
			case Cutting_Normal_id1: glui_cuttingnx[0]->set_float_val(glui_cuttingnx[1]->get_float_val());
						 glui_cuttingny[0]->set_float_val(glui_cuttingny[1]->get_float_val());
						 glui_cuttingnz[0]->set_float_val(glui_cuttingnz[1]->get_float_val());break;
		}
	}	
}*/
void fcuttingpop(int id)
{		
		char *contour_text = new char[8000];
		vector<svVector3> pos = qdotCutSlice[id]->getPlanePos();
		for(int i=0;i<8000;i++)
		{
			contour_text[i] = '\0';
		}
		for(int ii=0;ii<pos.size();ii++)
		{
			char str[20];
			sprintf(str, "%d (%0.2f,%0.2f,%0.2f)\n", ii, pos[ii][0], pos[ii][1], pos[ii][2]);
			strcat(contour_text,  str);
			//delete [] str;
		
			//cerr<<pos[ii][0]<<" "<<pos[ii][1]<<" "<<pos[ii][2]<<endl;
		}
		//cerr<<contour_text<<endl;
		glui_cuttinglist[id]->set_text((const char*) contour_text);
	//	}
		//glui_cuttinglist[j]->disable();
		delete [] contour_text;	
		pos.clear();
}

void fcuttingpaser(int id)
{
	cerr<<"fcuttingpaser"<<endl;
	int i = id;
	//for(int i=0;i<2;i++)
	//{
		if(qdotNewCluster[i])
		{
			//char *contour_text = new char[128];
			//contour_text = (char *)(glui_cuttingedit[i]->get_text());
			//for(int ii=0;ii<strlen(contour_text);ii++)
			//	cerr<<contour_text[ii]<<" ";
			//cerr<<endl;
			char str[32];

			char *pch2 = strtok((char *)(glui_cuttingedit[i]->get_text()), " [],\n"); //"p"
			//cerr<<pch2<<endl;
			pch2 = strtok(NULL, " [],\n");
			qdotCuttingV[i][0] = atof(pch2);
			pch2 = strtok(NULL, " [],\n");
			qdotCuttingV[i][1] = atof(pch2);
			pch2 = strtok(NULL, " [],\n");
			qdotCuttingV[i][2] = atof(pch2);

			pch2 = strtok(NULL, " [],\n"); //"n"
			pch2 = strtok(NULL, " [],\n");
			qdotCuttingN[i][0] = atof(pch2);
			pch2 = strtok(NULL, " [],\n");
			qdotCuttingN[i][1] = atof(pch2);
			pch2 = strtok(NULL, " [],\n");
			qdotCuttingN[i][2] = atof(pch2);
	
			pch2 = strtok(NULL, " [],\n");
			//delete [] contour_text;
			//delete pch2;
		}

		float sum = sqrt(qdotCuttingN[i][0] * qdotCuttingN[i][0]
			+ qdotCuttingN[i][1]*qdotCuttingN[i][1]
			+ qdotCuttingN[i][2]*qdotCuttingN[i][2]);
		qdotCuttingN[i][0] = qdotCuttingN[i][0]/sum;
		qdotCuttingN[i][1] = qdotCuttingN[i][1]/sum;
		qdotCuttingN[i][2] = qdotCuttingN[i][2]/sum;

	/*if(qdotClusterSync)
	{
		glui_cuttingedit[1-id]->set_text(glui_cuttingedit[id]->get_text());
	}*/
	cerr<<qdotCuttingV[i][0]<<" "<<qdotCuttingV[i][1]<<" "<<qdotCuttingV[i][2]<<" "<<qdotCuttingN[i][0]<<" "<<qdotCuttingN[i][1]<<" "<<qdotCuttingN[i][2]<<endl; //check point!
	//cannot type the cutting planes correctly

	cerr<<"fcuttingpaser"<<endl;

		int data_type = id;
		char *contour_text = new char[1024];

		for(int ii=0;ii<1024;ii++)
		{
			contour_text[ii] = '\0';
		}
		for(int ii=0;ii<1;ii++)
		{
			char *str = new char [50];
			    sprintf(str, "P [ %0.2f, %0.2f, %0.2f ], N [ %0.2f, %0.2f, %0.2f ] \n", 
					qdotCuttingV[data_type][0], qdotCuttingV[data_type][1], qdotCuttingV[data_type][2],
					qdotCuttingN[data_type][0], qdotCuttingN[data_type][1], qdotCuttingN[data_type][2]);
			    strcat(contour_text,  str);
			delete [] str;
		}
		glui_cuttingedit[data_type]->set_text((const char*) contour_text);

	delete [] contour_text;	




	//}
/*
	for(int i=0;i<2;i++)
	{
		//qdotNewCutting[i] = false;

		if(fabs(glui_cuttingvx[i]->get_float_val() - qdotCuttingV[i][0])>1e-10) qdotNewCutting[i] = true;
		if(fabs(glui_cuttingvy[i]->get_float_val() - qdotCuttingV[i][1])>1e-10) qdotNewCutting[i] = true;
		if(fabs(glui_cuttingvz[i]->get_float_val() - qdotCuttingV[i][2])>1e-10) qdotNewCutting[i] = true;

		if(fabs(glui_cuttingnx[i]->get_float_val() - qdotCuttingN[i][0])>1e-10) qdotNewCutting[i] = true;
		if(fabs(glui_cuttingny[i]->get_float_val() - qdotCuttingN[i][1])>1e-10) qdotNewCutting[i] = true;
		if(fabs(glui_cuttingnz[i]->get_float_val() - qdotCuttingN[i][2])>1e-10) qdotNewCutting[i] = true;
	}*/
}
void fcuttingeditpaser(int data_type) //check point!! cannot re-show the test properly 
{
	int i = data_type;

	qdotCuttingSingle[i].clear();
	for(int j=0;j<qdotCuttingMulti[i].size();j++)
	{
		qdotCuttingMulti[i][j].clear();
	}
	qdotCuttingMulti[i].clear();

	//char *contour_text = new char[128];
	//contour_text = (char *)(glui_cuttingscroll[i]->get_text());
	char str[32];
	char *pch2 = strtok((char *)(glui_cuttingscroll[i]->get_text()), " [],\n");
	while (pch2 !=NULL)
	{
		//cerr<<"start"<<endl;
		cerr<<pch2<<endl;
		if(!strcmp(pch2,"S"))
		{
			pch2 = strtok(NULL," [],\n");
			qdotCuttingSingle[i].push_back(atof(pch2));
		}
		else
		{
			pch2 = strtok(NULL, " [],\n");
			cerr<<pch2<<endl;
			vector<int> store3(2);
			store3[0] = atof(pch2);
			cerr<<pch2<<endl;
			pch2 = strtok(NULL, " [],\n");
			store3[1] = atof(pch2);
			cerr<<pch2<<endl;
			qdotCuttingMulti[i].push_back(store3);
			
		}
		cerr<<pch2<<endl;
		pch2 = strtok(NULL, " []\n");
		//cerr<<pch2<<endl;
	}

	cerr<<"fcuttingeditpaser"<<endl;
	//delete [] contour_text;	
	//delete pch2;
	cerr<<"fcuttingeditpaser"<<endl;

		char *contour_text = new char[1024];

		for(int ii=0;ii<1024;ii++)
		{
			contour_text[ii] = '\0';
		}
		for(int ii=0;ii<qdotCuttingSingle[data_type].size();ii++)
		{
			char *str = new char [50];
			sprintf(str, "S %d \n", qdotCuttingSingle[data_type][ii]);
			strcat(contour_text,  str);
			delete [] str;
		}
		for(int ii=0;ii<qdotCuttingMulti[data_type].size();ii++)
		{
			char *str = new char [50];
			sprintf(str, "M [ %d %d ] \n", 
					qdotCuttingMulti[data_type][ii][0], 
					qdotCuttingMulti[data_type][ii][1]);
			strcat(contour_text,  str);
			delete [] str;
		}
		//cerr<<contour_text<<" "<<qdotCuttingSingle[data_type].size()<<endl;
		glui_cuttingscroll[data_type]->set_text((const char*) contour_text);

	delete [] contour_text;	
}
void fclusterpaser(int id)
{
		if(qdotNewCluster[id])
		{

			char *pch2 = strtok((char *)(sb_magrange00[id]->get_text()), " [],\n"); //"p"
			qdotMagRange0[id][0] = atof(pch2);

			pch2 = strtok((char *)(sb_magrange01[id]->get_text()), " [],\n"); //"p"
			qdotMagRange0[id][1] = atof(pch2);

			pch2 = strtok((char *)(sb_magrange10[id]->get_text()), " [],\n"); //"p"
			qdotMagRange1[id][0] = atof(pch2);

			pch2 = strtok((char *)(sb_magrange11[id]->get_text()), " [],\n"); //"p"
			qdotMagRange1[id][1] = atof(pch2);
		}
}
//	glui_cluster_text0[data_type] = new GLUI_EditText(obj_1, "# of clusters", &qdotHCNum[data_type][0], Cluster_Num_id[0], control);
void fclusteredit(int data_type, int id)
{
	qdotNewCluster[data_type] = true;
	qdotNewContourCluster[data_type]=true;
	qdotNewArrow[data_type]= true;
	qdotNewArrowData[data_type]=true;
	qdotNewArrowContour[data_type]=true;
	qdotNewLayerContour[data_type] = true;

	if(qdotClusterSync)
	{
		qdotNewCluster[1-data_type] = true;
		qdotNewContourCluster[1-data_type]=true;
		qdotNewArrow[1-data_type]= true;
		qdotNewArrowData[1-data_type]=true;
		qdotNewArrowContour[1-data_type]=true;
		qdotNewLayerContour[1-data_type] = true;

		if(id == Cluster_Num_id0[data_type])
		{
			glui_cluster_text0[1-data_type]->set_int_val(qdotHCNum[data_type][0]);
			qdotHCNum[1-data_type][0] = qdotHCNum[data_type][0];
		} 
		else if(id == Cluster_Num_id1[data_type])
		{
			glui_cluster_text1[1-data_type]->set_int_val(qdotHCNum[data_type][1]);
			qdotHCNum[1-data_type][1] = qdotHCNum[data_type][1];
		}
	}
}

void fclusterlayer(int id)
{
	glui_clusterlayer[1-id]->set_int_val(glui_clusterlayer[id]->get_int_val());
}
/*		case Lwegith_id0[0]:
		case Lwegith_id1[0]:
		case Vwegith_id0[0]:
		case Vwegith_id1[0]:
		case Dwegith_id0[0]:
		case Dwegith_id1[0]:
		case MRange_id00[0]:
		case MRange_id01[0]:
		case MRange_id10[0]:
		case MRange_id11[0]:fclusterweight(0, id);break;
	sb_location0[data_type] = new GLUI_Scrollbar(obj_l, "Location",  GLUI_SCROLL_HORIZONTAL,
				&qdotLweight[data_type][0], Lweight_id0[data_type], control);
	sb_magrange00[data_type] = new GLUI_Scrollbar(obj_d, "Min(magnitude)",  GLUI_SCROLL_HORIZONTAL, 
				&qdotMagRange0[data_type][0], MRange_id00[data_type], control);
glui_LWtext0*/
void fclusterweight(int data_type, int id)
{
	qdotNewCluster[data_type] = true;
	qdotNewContourCluster[data_type]=true;
	qdotNewArrow[data_type]= true;
	qdotNewArrowData[data_type]=true;
	qdotNewArrowContour[data_type]=true;
	qdotNewLayerContour[data_type] = true;
	
	char str[20];

	sprintf(str,"%0.2f",qdotLweight[data_type][0]);
	glui_LWtext0[data_type]->set_text(str);
	sprintf(str,"%0.2f",qdotLweight[data_type][1]);
	glui_LWtext1[data_type]->set_text(str);
	sprintf(str,"%0.2f",qdotVweight[data_type][0]);
	glui_VWtext0[data_type]->set_text(str);
	sprintf(str,"%0.2f",qdotVweight[data_type][1]);
	glui_VWtext1[data_type]->set_text(str);
	sprintf(str,"%0.2f",qdotDweight[data_type][0]);
	glui_DWtext0[data_type]->set_text(str);
	sprintf(str,"%0.2f",qdotDweight[data_type][1]);
	glui_DWtext1[data_type]->set_text(str);

	if(qdotMagRange1[data_type][0] < qdotMagRange0[data_type][1])
	{
		qdotMagRange1[data_type][0] = qdotMagRange0[data_type][1];
	}
	if(qdotMagRange1[data_type][1] > qdotMagRange1[data_type][1])
	{
		qdotMagRange1[data_type][1] = qdotMagRange1[data_type][1];
	}

	if(qdotClusterSync)
	{
		qdotNewCluster[1-data_type] = true;
		qdotNewContourCluster[1-data_type]=true;
		qdotNewArrow[1-data_type]= true;
		qdotNewArrowData[1-data_type]=true;
		qdotNewArrowContour[1-data_type]=true;
		qdotNewLayerContour[1-data_type] = true;

		sb_location0[1-data_type]->set_float_val(qdotLweight[data_type][0]);
		sb_location1[1-data_type]->set_float_val(qdotLweight[data_type][1]);
		//glui_LWtext0[1-data_type]->set_float_val(qdotLweight[data_type][0]);
		//glui_LWtext1[1-data_type]->set_float_val(qdotLweight[data_type][1]);
		sb_velocity0[1-data_type]->set_float_val(qdotVweight[data_type][0]);
		sb_velocity1[1-data_type]->set_float_val(qdotVweight[data_type][1]);
		//glui_VWtext0[1-data_type]->set_float_val(qdotVweight[data_type][0]);
		//glui_VWtext1[1-data_type]->set_float_val(qdotVweight[data_type][1]);
		sb_density0[1-data_type]->set_float_val(qdotDweight[data_type][0]);
		sb_density1[1-data_type]->set_float_val(qdotDweight[data_type][1]);
		//glui_DWtext0[1-data_type]->set_float_val(qdotDweight[data_type][0]);
		//glui_DWtext1[1-data_type]->set_float_val(qdotDweight[data_type][1]);
		sb_magrange00[1-data_type]->set_float_val(sb_magrange00[data_type]->get_float_val());
		sb_magrange01[1-data_type]->set_float_val(sb_magrange01[data_type]->get_float_val());
		sb_magrange10[1-data_type]->set_float_val(sb_magrange10[data_type]->get_float_val());
		sb_magrange11[1-data_type]->set_float_val(sb_magrange11[data_type]->get_float_val());

		sprintf(str,"%0.2f",qdotLweight[data_type][0]);
		glui_LWtext0[1-data_type]->set_text(str);
		sprintf(str,"%0.2f",qdotLweight[data_type][1]);
		glui_LWtext1[1-data_type]->set_text(str);
		sprintf(str,"%0.2f",qdotVweight[data_type][0]);
		glui_VWtext0[1-data_type]->set_text(str);
		sprintf(str,"%0.2f",qdotVweight[data_type][1]);
		glui_VWtext1[1-data_type]->set_text(str);
		sprintf(str,"%0.2f",qdotDweight[data_type][0]);
		glui_DWtext0[1-data_type]->set_text(str);
		sprintf(str,"%0.2f",qdotDweight[data_type][1]);
		glui_DWtext1[1-data_type]->set_text(str);

		qdotLweight[1-data_type][0] = qdotLweight[data_type][0];
		qdotVweight[1-data_type][0] = qdotVweight[data_type][0];
		qdotDweight[1-data_type][0] = qdotDweight[data_type][0];
		qdotLweight[1-data_type][1] = qdotLweight[data_type][1];
		qdotVweight[1-data_type][1] = qdotVweight[data_type][1];
		qdotDweight[1-data_type][1] = qdotDweight[data_type][1];
		qdotMagRange0[1-data_type][0] = qdotMagRange0[data_type][0];
		qdotMagRange0[1-data_type][1] = qdotMagRange0[data_type][1];
		qdotMagRange1[1-data_type][0] = qdotMagRange1[data_type][0];
		qdotMagRange1[1-data_type][1] = qdotMagRange1[data_type][1];
	}
}

void fclusterprocessing()
{
	fcuttingprocessing();
	fclusterpaser(0);
	fclusterpaser(1);

	//if(qdotClusterType[0] == 2)
	//	qdotCutSlice[0]->setCurrentPlane();
	//if(qdotClusterType[1] == 2)
	//	qdotCutSlice[1]->setCurrentPlane();
	//qdotCutSlice[0]->setSelected(qdotCuttingSingle[0], qdotCuttingMulti[0]);
	//qdotCutSlice[1]->setSelected(qdotCuttingSingle[1], qdotCuttingMulti[1]);

	double mag[2];
	mag[0] = qdotMagRange0[0][0];
	mag[1] = qdotMagRange0[0][1];
	double mag2[2];
	mag2[0] = qdotMagRange1[0][0];
	mag2[1] = qdotMagRange1[0][1];
	double ws[2], wv[2], wd[2];
	ws[0]=qdotLweight[0][0];ws[1]=qdotLweight[0][1];
	wv[0]=qdotVweight[0][0];wv[1]=qdotVweight[0][1];
	wd[0]=qdotDweight[0][0];wd[1]=qdotDweight[0][1];
	//cerr<<qdotNewCluster[0]<<endl;
	if(qdotNewCluster[0])
	{
		qdotField[0]->layerClustering(qdotHCNum[0],mag,mag2,
					ws,wv,wd, 
					qdotCutSlice[0]->getSelected(), qdotMDegree[0]);
		cerr<<"	qdotNewLayerClustering"<<endl;
	}
	if(qdotNewLayerContour[0])
	{
		qdotField[0]->layerContourGeneration(qdotContourLRP[0], qdotContourLR[0],
					qdotContourLAP[0], qdotContourLA[0],
					qdotContourGR[0], qdotContourGA[0],qdotCutSlice[0]->getSelected());
		cerr<<"	qdotNewLayerContour"<<endl;
		
	}
	if(qdotNewContourCluster[0])
	{
		qdotField[0]->layerContourClustering(mag,mag2,
					ws,wv,wd, 
					qdotCutSlice[0]->getSelected());
	}

	mag[0] = qdotMagRange0[1][0];
	mag[1] = qdotMagRange0[1][1];
	mag2[0] = qdotMagRange1[1][0];
	mag2[1] = qdotMagRange1[1][1];
	ws[0]=qdotLweight[1][0];ws[1]=qdotLweight[1][1];
	wv[0]=qdotVweight[1][0];wv[1]=qdotVweight[1][1];
	wd[0]=qdotDweight[1][0];wd[1]=qdotDweight[1][1];

	if(qdotNewCluster[1])
	{

		qdotField[1]->layerClustering(qdotHCNum[1],mag,mag2,
					ws,wv,wd,  
					qdotCutSlice[1]->getSelected(),qdotMDegree[1]);
	}
	if(qdotNewLayerContour[1])
	{
		qdotField[1]->layerContourGeneration(qdotContourLRP[1], qdotContourLR[1],
					qdotContourLAP[1], qdotContourLA[1],
					qdotContourGR[1], qdotContourGA[1],qdotCutSlice[1]->getSelected());	
	}
	if(qdotNewContourCluster[1])
	{	
		qdotField[1]->layerContourClustering(mag,mag2,
					ws,wv,wd, 
					qdotCutSlice[1]->getSelected());
	}
}
/*void fcuttingmove()
{
	for(int i=0;i<2;i++)
	{
		vector<svVector3> pn(1);
		vector<svVector3> pv(1);
		pv = qdotCutSlice[i]->GetPoint();
		pn = qdotCutSlice[i]->GetDir();
		qdotCuttingV[i] = pv[0];
		qdotCuttingN[i] = pn[0];

		glui_cuttingvx[i]->set_float_val(qdotCuttingV[i][0]);
		glui_cuttingvy[i]->set_float_val(qdotCuttingV[i][1]);
		glui_cuttingvz[i]->set_float_val(qdotCuttingV[i][2]);

		glui_cuttingnx[i]->set_float_val(qdotCuttingN[i][0]);
		glui_cuttingny[i]->set_float_val(qdotCuttingN[i][1]);
		glui_cuttingnz[i]->set_float_val(qdotCuttingN[i][2]);
	}
}*/
/*
void fcuttingprocessing()
{
	fcuttingparser();

	for(int i=0;i<2;i++)
	{
		vector<svVector3> pv(1);pv[0] = qdotCuttingV[i];
		vector<svVector3> pn(1);pn[0] = qdotCuttingN[i];
		if(qdotNewCutting[i])
		{
			qdotCutSlice[i]->SetPlane(pv, pn);

			if(!qdotProjectionType[i])
			{
				qdotField[i]->getPlane(qdotProjectionType[i], qdotDisplay[i], 
					      qdotCuttingV[i], qdotCuttingN[i]);//cerr<<"1"<<endl;
				qdotField[i]->getPlaneROI(qdotSSlider[i]->getMagrange(),
						qdotSSlider[i]->getZrange(),
						qdotSSlider[i]->getXYrange(),
						qdotSSlider[i]->getSpacerange());//cerr<<"1"<<endl;
			}

		}
		if((qdotNewCutting[i] && !qdotProjectionType[i]) || qdotNewCluster[i])
		{		
				qdotField[i]->planeClustering( qdotClusterType[i],qdotClusterROI[i]);	
		}
		if(qdotNewCutting[i])
		{
			if(!qdotProjectionType[i])
			{
				if(qdotClusterType[i] == 0)
				{
					qdotCut[i]->SetCut(qdotField[i]->getPlanePos(),qdotField[i]->getPlaneVel(),
						qdotField[i]->getPlaneDen(),
						qdotColor->getColors(qdotBlind, 
									qdotClusterType[i], qdotClusterNum[i], 
									qdotField[i]->getPlaneMagcluster(),
									qdotField[i]->getPlaneSize(), qdotFont),
						qdotField[i]->getPlaneSize());
				}
				else
				{
					qdotCut[i]->SetCut(qdotField[i]->getPlanePos(),qdotField[i]->getPlaneVel(),
						qdotField[i]->getPlaneDen(),
						qdotColor->getColors(qdotBlind, 
									qdotClusterType[i], qdotClusterNum[i], 
									qdotField[i]->getPlaneVelcluster(),
									qdotField[i]->getPlaneSize(), qdotFont),
						qdotField[i]->getPlaneSize());//cerr<<"2"<<endl;
				}
				qdotCut[i]->SetInbox(qdotField[i]->getPlaneInbox(),qdotField[i]->getPlaneSize());
			}
		}
		//qdotNewCutting[i] = false;
	}
}*/
void fdistancesync(int id)
{
	glui_distancesync[1-id]->set_int_val(glui_distancesync[id]->get_int_val());
}
void fdistancesample()
{
	//if(qdotDistanceSync)
	//{
		qdotDistanceSample[0] = qdotDistanceSample[0];
		qdotDistanceSample[1] = qdotDistanceSample[0];
		//glui_distancesample[0]->set_int_val(glui_distancesample[id]->get_int_val());
	//}
	redisplay_all();
}
void fdistancez(int id)
{

}
void fdistancepaser()
{
	for(int i=0;i<2;i++)
	{
		if(qdotNewDistance[i])
		{
			qdotDistanceV[i].clear();

			//char *contour_text = new char[1024];
			//contour_text = (char *)(glui_distanceedit[i]->get_text());
			char str[32];
			char *pch2 = strtok((char *)(glui_distanceedit[i]->get_text()), " [],\n");
			while (pch2 !=NULL)
			{
				cerr<<pch2<<endl;

				pch2 = strtok(NULL, " [],\n");
				cerr<<pch2<<endl;
				svVector3 n;
				n[2] = atof(pch2);
				pch2 = strtok(NULL, " [],\n");
				cerr<<pch2<<endl;
				pch2 = strtok(NULL, " [],\n");
				cerr<<pch2<<endl;
				n[0] = atof(pch2);
				pch2 = strtok(NULL, " [],\n");
				cerr<<pch2<<endl;
				n[1] = atof(pch2);
				qdotDistanceV[i].push_back(n);
				pch2 = strtok(NULL, " [],\n");

			}	
	cerr<<"done paser"<<endl;
			char *contour_text = new char[1024];
			for(int ii=0;ii<1024;ii++)
			{
				contour_text[ii] = '\0';
			}
			for(int ii=0;ii<qdotDistanceV[i].size();ii++)
			{
			cerr<<qdotDistanceV[i][ii][2]<<" "<<qdotDistanceV[i][ii][0]<<" "<<qdotDistanceV[i][ii][1]<<endl;
				char str[50];
					   sprintf(str, "Z %0.2f V %0.2f %0.2f \n", 
							qdotDistanceV[i][ii][2], qdotDistanceV[i][ii][0], qdotDistanceV[i][ii][1]);
					    strcat(contour_text,  str);

					//delete [] str;
			}
	cerr<<"done paser"<<endl;
			glui_distanceedit[i]->set_text((const char*) contour_text);
			delete [] contour_text;	
			//delete [] contour_text;
			//delete pch2;
		}
	}
}
void fdistanceedit(int id)
{
	qdotNewDistance[id] = true;
	if(qdotDistanceSync)
	{
		glui_distanceedit[1-id]->set_text(glui_distanceedit[id]->get_text());
		qdotNewDistance[1-id] = true;
	}
}
void fdistanceprocessing()
{
	fdistancepaser();

	cerr<<"done paser"<<endl;

	for(int i=0;i<2;i++)
	{
		if(qdotNewDistance[i])
		{
			vector<svScalar> z;
			for(int j=0;j<qdotDistanceV[i].size();j++) z.push_back(qdotDistanceV[i][j][2]);
			qdotField[i]->distanceGeneration(qdotMap[i]->GetSelectedLayer(), 
						qdotDistanceV[i], z);
			qdotMap[i]->DecideSelectedPoint();
			z.clear();
			qdotMap[i]->SetDistance(qdotField[i]->getRawDis());

			qdotMap[i]->SetFontColor(qdotFont);
		}

		if(qdotNewDistance[i] || qdotNewCluster[i])
		{
			if(qdotColorby[i] == 0)
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}	
		}
	}
}
/*void froimove(int id)
{

		GLfloat **tb_transform = new GLfloat*[4];
		for(int i=0;i<4;i++)
		{
			tb_transform[i] = new GLfloat[4];
			for(int j=0;j<4;j++)
			{
				tb_transform[i][j] = m[id][j+i*4];
			}
		}

	if(qdotROIRefer[id])
	{
			qdotSSlider[id]->AddPoints(qdotArrowSelection[id]->GetSelectedPoint(),
					qdotArrowSelection[id]->GetSelectedPointMag(),
					qdotArrowSelection[id]->GetSelectedPointVel(),
					qdotROIRefer[id], qdotArrowSelection[id]->GetSelected2DPos(m[id]),
					image_width/2, image_height, true);
			cerr<<"slider1"<<endl;
			qdotSSlider[id]->AddQuad();
			//cerr<<"slider2"<<endl;
			//if(qdotROISync) //checkpoint!!!!!!!!!!!!!!!!!!
			//{
				//if(qdotArrowSelection[id]->SELECTED_CURRENT)
				//	qdotArrowSelection[1-id]->AddCurrentIndex(qdotArrowSelection[id]->GetSelectedPoint());
				//else 
			if(qdotArrowSelection[id]->SELECTED)
				qdotArrowSelection[1-id]->AddMarker(qdotArrowSelection[id]->GetSelectedPoint());

			qdotSSlider[1-id]->AddPoints(qdotArrowSelection[1-id]->GetSelectedPoint(),
							qdotArrowSelection[1-id]->GetSelectedPointMag(),
							qdotArrowSelection[1-id]->GetSelectedPointVel(),
							qdotROIRefer[1-id], qdotArrowSelection[1-id]->GetSelected2DPos(m[1-id]),
							image_width/2, image_height,qdotROISync);
			qdotSSlider[1-id]->AddQuad();
				//if(qdotArrowSelection[id]->SELECTED)
				//	qdotArrowSelection[1-id]->RenderArrow(qdotOpacity);*/
			/*}
			else
			{
				if(qdotArrowSelection[id]->SELECTED_CURRENT)
					qdotArrowSelection[1-id]->AddCurrentIndex(qdotArrowSelection[id]->GetSelectedPoint());
				else if(qdotArrowSelection[id]->SELECTED)
					qdotArrowSelection[1-id]->AddMarker(qdotArrowSelection[id]->GetSelectedPoint());

				qdotSSlider[1-id]->AddPoints(qdotArrowSelection[1-id]->GetSelectedPoint(),
							qdotArrowSelection[1-id]->GetSelectedPointMag(),
							qdotArrowSelection[1-id]->GetSelectedPointVel(),
							qdotROIRefer[1-id], qdotArrowSelection[1-id]->GetSelected2DPos(m[1-id]),
							image_width/2, image_height,false);
				qdotSSlider[1-id]->AddQuad();
				//if(qdotArrowSelection[id]->SELECTED)
				//	qdotArrowSelection[1-id]->RenderArrow(qdotOpacity);
			}*/

			//cerr<<"3"<<endl;
/*			if(id == 0)
	   			glutSetWindow(left_window);
			else
	   			glutSetWindow(right_window);
			if(qdotArrowSelection[id]->SELECTED)
				qdotArrowSelection[id]->RenderArrow(qdotOpacity, renderzmin, renderzmax);
			if(id == 1)
	   			glutSetWindow(left_window);
			else
	   			glutSetWindow(right_window);
			if(qdotROISync)
			{
				if(qdotArrowSelection[id]->SELECTED)
					qdotArrowSelection[1-id]->RenderArrow(qdotOpacity, renderzmin, renderzmax);
			}

		qdotROIRefer[0] = 0;
		qdotROIRefer[1] = 0;
	}
	if(qdotROIAdd[id])
	{
		svVector3 tmp;
		tmp[0] = 0;
		tmp[1] = 0;
		tmp[2] = 0;
		qdotSSlider[id]->AddPoints(tmp, 0, tmp, 
					qdotROIRefer[id], tmp,
					image_width/2, image_height,true);
		qdotSSlider[id]->AddQuad();

		if(qdotROISync)
		{
			qdotSSlider[1-id]->AddPoints(tmp, 0, tmp, 
					qdotROIRefer[1-id], tmp,
					image_width/2, image_height,qdotROISync);
			qdotSSlider[1-id]->AddQuad();
					//if(qdotArrowSelection[id]->SELECTED)
					//	qdotArrowSelection[1-id]->RenderArrow(qdotOpacity);
		}
		qdotROIAdd[0] = 0;
		qdotROIAdd[1] = 0;

		redisplay_all();
			//cerr<<1-id<<endl;
	}
	if(qdotROIDelete[id])
	{	
		bool flag = qdotSSlider[1-id]->getEnable(qdotSSlider[id]->getQuad());
		if(qdotROISync)
		{
			//delete both of them
			qdotSSlider[1-id]->DeletePoints(qdotSSlider[id]->getQuad());
			qdotSSlider[id]->DeletePoints(false);
		}
		else
		{
			if(flag)
			{
				//just make it unvisible
				qdotSSlider[id]->DeletePoints(true);
			}
			else
			{
				//delete both of them
				qdotSSlider[1-id]->DeletePoints(qdotSSlider[id]->getQuad());
				qdotSSlider[id]->DeletePoints(false);
			}
		}
		qdotROIDelete[0] = 0;
		qdotROIDelete[1] = 0;
	}*/
	/*if(qdotROIDelete[id])
	{
		if(qdotROISync)
		{
			if(qdotArrowSelection[id]->SELECTED_CURRENT)
			{
				qdotArrowSelection[1-id]->DeleteMarker(qdotArrowSelection[id]->GetSelectedPoint());	
			}
		}

		if(qdotArrowSelection[id]->SELECTED_CURRENT)
		{
			qdotArrowSelection[id]->DeleteMarker();
		}

		if(id == 0)
   			glutSetWindow(left_window);
		else
   			glutSetWindow(right_window);
		if(qdotArrowSelection[id]->SELECTED)
			qdotArrowSelection[id]->RenderArrow(qdotOpacity);
		if(id == 1)
   			glutSetWindow(left_window);
		else
   			glutSetWindow(right_window);
		if(qdotROISync)
		{
			if(qdotArrowSelection[id]->SELECTED)
				qdotArrowSelection[1-id]->RenderArrow(qdotOpacity);
		}
	}	
	if(qdotROIEdit[id])
	{
		if(qdotROISync)
		{
			if(qdotArrowSelection[id]->SELECTED_CURRENT)
			{
				qdotArrowSelection[1-id]->AddCurrentIndex(qdotArrowSelection[id]->GetSelectedPoint());
				qdotArrowSelection[1-id]->RenderArrow(qdotOpacity);
			}
		}
		if(qdotArrowSelection[id]->SELECTED_CURRENT)
			qdotArrowSelection[id]->RenderArrow(qdotOpacity);
	}*/

//}
/*
void fclusterdisc()
{
	if(qdotDisplay[0] == D_Contour)
	{
	}
	else
	{
		if(!qdotClusterlayer)
		{

		}
		else
		{
			if(qdotClusterType[0] == 0)
			{

			}
			else
			{
				qdotDisc[0]->SetClusters(qdotField[0]->getRawLayerVelcluster(),
							qdotClusterNum[0], qdotClusterlayer,qdotField[0]->getZlayer(),
							qdotField[0]->getZmin(),qdotField[0]->getZdistance());
				qdotDisc[1]->SetClusters(qdotField[1]->getRawLayerVelcluster(),
							qdotClusterNum[1], qdotClusterlayer,qdotField[1]->getZlayer(),
							qdotField[1]->getZmin(),qdotField[1]->getZdistance());
				qdotDisc[0]->computeMean(qdotField[0]->getRawMagcluster(), 
						qdotCluster[0][qdotClusterROI[0]][0]);
				qdotDisc[1]->computeMean(qdotField[1]->getRawMagcluster(), 
						qdotCluster[1][qdotClusterROI[1]][0]);
				qdotArrowSelection[0]->SetExpand(qdotDisc[0]->GetROI());
				qdotArrowSelection[1]->SetExpand(qdotDisc[1]->GetROI());
			}
		}
	}
}
*/
void fbackground()
{
	char str[20];
	sprintf(str,"%0.2f",qdotBack);
	glui_background->set_text(str);

	if(qdotBack > 0.5)
	{
		qdotFont[0] = 0;
		qdotFont[1] = 0;
		qdotFont[2] = 0;
	}
	else
	{
		qdotFont[0] = 1;
		qdotFont[1] = 1;
		qdotFont[2] = 1;
	}
	
	qdotArrowSelection[0]->SetFontColor(qdotFont);
	qdotOutline[0]->SetFontColor(qdotFont);
	qdotMap[0]->SetFontColor(qdotFont);
	qdotCutSlice[0]->SetFontColor(qdotFont);

	qdotArrowSelection[1]->SetFontColor(qdotFont);
	qdotOutline[1]->SetFontColor(qdotFont);
	qdotMap[1]->SetFontColor(qdotFont);
	qdotCutSlice[1]->SetFontColor(qdotFont);

	redisplay_all();
}	

void fopacity()
{
	char str[20];
	sprintf(str,"%0.2f",qdotOpacity);
	glui_opacity->set_text(str);

	////qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
	//qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);

	//redisplay_all();
	//fsetdisplaylist_arrow();
	//fsetdisplaylist();
	redisplay_all();
}

void flength()
{
	if(qdotEncode[0] == 0)
	{
		qdotArrowSelection[0]->extendLength(qdotLength * 5e-11);
	}
	else if(qdotEncode[0] == 1)
	{
		qdotArrowSelection[0]->extendLength(qdotLength);
	}

	if(qdotEncode[1] == 0)
	{
		qdotArrowSelection[1]->extendLength(qdotLength * 5e-11);
	}
	else if(qdotEncode[1] == 1)
	{
		qdotArrowSelection[1]->extendLength(qdotLength);
	}

	//qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
	//qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);

	//fsetdisplaylist_arrow();
}

void farrow()
{
	qdotArrowSelection[0]->setArrowBase(qdotArrowBase);
	qdotArrowSelection[1]->setArrowBase(qdotArrowBase);

	//qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
	//qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);

	//fsetdisplaylist_arrow();
}

void fblind()
{
	//fsetcolor();
	for(int i=0;i<2;i++)
	{
		if(qdotDisplay[i] == 0)//D_Contour)
		{
			if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourCluster(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourAtom(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());
			qdotNewArrow[i] = true;
		}
		else if (qdotDisplay[i] == 1)
		{
			if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());
			qdotNewArrow[i] = true;

		}
		else
		{
			if(qdotColorby[i] == 0)
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
		}
	}
	//fsetdisplaylist_arrow();
}

void ffrequency()
{
	char str[20];
	sprintf(str,"%d",qdotFrequency);
	glui_frequency->set_text(str);

	qdotArrowSelection[0]->SetSample(qdotFrequency, qdotDisplay[0]);
	qdotArrowSelection[1]->SetSample(qdotFrequency, qdotDisplay[1]);

	//qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
	//qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);

	//fsetdisplaylist_arrow();
}

void fencode(int id)
{
	qdotNewArrow[id]=true;
	//qdotArrowSelection[id]->SetEncodeMode(qdotEncode[id]);
	//qdotArrowSelection[1]->SetEncodeMode(qdotEncode[1]);

	//fsetdisplaylist_arrow();

	//qdotArrowSelection[id]->svArrowGlyphRenderArrow(qdotOpacity);
	//qdotArrowGlyphRenderArrow[1]->svArrowGlyphRenderArrow(qdotOpacity);
}

/*void fresetarrow()
{
	qdotArrowSelection[0]->SetArrows();
	qdotArrowSelection[1]->SetArrows();

	qdotArrowGlyphRenderArrow[0]->svArrowGlyphRenderArrow(qdotOpacity);
	qdotArrowGlyphRenderArrow[1]->svArrowGlyphRenderArrow(qdotOpacity);
}*/
void fsetcolor()
{
	for(int i=0;i<2;i++)
	{
		if(qdotDisplay[i] == 0)//D_Contour)
		{
			/*if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourCluster(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourAtom(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());*/
			qdotNewArrow[i] = true;
		}
		else if (qdotDisplay[i] == 1)
		{
			/*if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());*/
			qdotNewArrow[i] = true;

		}
		else
		{
			if(qdotColorby[i] == 0)
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
		}
	}	
}
/*void fsetcolor()
{
	if(qdotDisplay[0] == D_Contour)
	{
		if(qdotClusterType[0] == 0)
		{
			qdotArrowSelection[0]->SetColors(qdotColor->getColors(qdotBlind, 
						qdotClusterType[0], qdotClusterNum[0], 
						qdotField[0]->getContourMagcluster(),
						qdotField[0]->getContourSize(), qdotFont));
			qdotArrowSelection[1]->SetColors(qdotColor->getColors(qdotBlind, 
						qdotClusterType[1], qdotClusterNum[1], 
						qdotField[1]->getContourMagcluster(),
						qdotField[1]->getContourSize(), qdotFont));
		}
		else
		{
			qdotArrowSelection[0]->SetColors(qdotColor->getColors(qdotBlind, 
						qdotClusterType[0], qdotClusterNum[0], 
						qdotField[0]->getContourVelcluster(),
						qdotField[0]->getContourSize(), qdotFont));
			qdotArrowSelection[1]->SetColors(qdotColor->getColors(qdotBlind, 
						qdotClusterType[1], qdotClusterNum[1], 
						qdotField[1]->getContourVelcluster(),
						qdotField[1]->getContourSize(), qdotFont));
		}
	}
	else
	{
		//cerr<<qdotClusterNum[0]<<"qdotclusternum"<<endl;
				qdotArrowSelection[0]->SetColors(qdotColor->getColors(qdotBlind, 
							qdotClusterType[0], qdotClusterNum[0], 
							qdotField[0]->getRawLayerVelcluster(),
							qdotField[0]->getRawSize(), qdotFont));
				qdotArrowSelection[1]->SetColors(qdotColor->getColors(qdotBlind, 
							qdotClusterType[1], qdotClusterNum[1], 
							qdotField[1]->getRawLayerVelcluster(),
							qdotField[1]->getRawSize(), qdotFont));
				qdotDisc[0]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotCluster[0][qdotClusterROI[0]][0]),
							qdotColor->getColors(qdotBlind, 
							qdotClusterType[0], qdotClusterNum[0]), 
							qdotCluster[0][qdotClusterROI[0]][0], qdotClusterlayer, qdotField[0]->getZlayer());
				qdotDisc[1]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotCluster[1][qdotClusterROI[1]][0]),
							qdotColor->getColors(qdotBlind, 
							qdotClusterType[1], qdotClusterNum[1]), 
							qdotCluster[1][qdotClusterROI[1]][0], qdotClusterlayer, qdotField[1]->getZlayer());
		
	}
}*/
void fsetdisplaylist_arrow()
{
	glutSetWindow(left_window);
	//qdotArrowSelection[0]->RenderArrow(qdotOpacity, renderzmin, renderzmax);
	qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
    	glutPostRedisplay();
//======================================================================================================
	glutSetWindow(right_window);
	//qdotArrowSelection[1]->RenderArrow(qdotOpacity, renderzmin, renderzmax);
	qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);
    	glutPostRedisplay();
}
void fsetdisplaylist_mesh() //checkpoint
{
	if(qdotDisplay[0] <2)
	{
		glutSetWindow(left_window);
		/*vector<bool> rindex;
		rindex.resize(qdotRegionList[0].size());
		for(int i=0;i<rindex.size();i++)
		{
			rindex[i] = false;
		}
		if(qdotRegion[0]==qdotRegionList[0].size()+1)
		{
			for(int i=0;i<rindex.size();i++)
			{
				rindex[i] = true;
			}
		}
		else
		{
			rindex[qdotRegion[0]-1] = true;
		}*/
/*
		qdotMesh[0]->generateList(qdotField[0]->getCellPosVec(), 
								qdotField[0]->getCellVelVec(),
								qdotField[0]->getCellDenVec(),
								qdotField[0]->getCellIndex(),
								true, 3);
*/

	    	glutPostRedisplay();
	}
//======================================================================================================
	if(qdotDisplay[1] <2)
	{
		glutSetWindow(right_window);
		/*rindex.resize(qdotRegionList[1].size());
		for(int i=0;i<rindex.size();i++)
		{
			rindex[i] = false;
		}
		if(qdotRegion[1]==qdotRegionList[1].size()+1)
		{
			for(int i=0;i<rindex.size();i++)
			{
				rindex[i] = true;
			}
		}
		else
		{
			rindex[qdotRegion[1]-1] = true;
		}*/
/*
		qdotMesh[1]->generateList(qdotField[1]->getCellPosVec(), 
								qdotField[1]->getCellVelVec(),
								qdotField[1]->getCellDenVec(),
								qdotField[1]->getCellIndex(),
								true, 4);
	    	glutPostRedisplay();
*/
	}

	//rindex.clear();

}
void update()
{
	fdataprocessing();cerr<<"fdataprocessing();"<<endl;
	fcontourprocessing();cerr<<"fcontourprocessing();"<<endl;//check point! cannot generate new contours successfully
	fclusterprocessing();cerr<<"fclusterprocessing();"<<endl;
	fdistanceprocessing();cerr<<"fdistanceprocessing();"<<endl;
	farrowprocessing();cerr<<"farrowprocessing();"<<endl;

	//fsetdisplaylist_arrow();cerr<<"displaylist_arrow"<<endl;
	fsetdisplaylist_mesh();

	for(int i=0;i<2;i++)
	{
		qdotNewdata[i] = false;
		qdotNewCluster[i] = false;
		qdotNewContourCluster[i] = false;
		qdotNewContour[i] = false;
		qdotNewLayerContour[i] =false;
		qdotNewDistance[i] = false;
		qdotNewArrowData[i]=false;
		qdotNewArrowContour[i]=false;
		qdotNewArrow[i] = false;
	}
	cerr<<"update done"<<endl;

}
void frefreshrendering()
{
	
}
void init()
{
	mkdir(tmpfiledir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  
cerr<<"-----------------------------Initialization---------------------------------------"<<endl;
	qdotField[0] = new svQDOT(datadir[0], datafile[0], tmpfiledir);
	qdotField[1] = new svQDOT(datadir[1], datafile[1], tmpfiledir);
/*comment for future use*/
/*
	qdotSlider[0] = new svSlider();
	qdotSlider[1] = new svSlider();
*/
cerr<<"-----------------------------Initialization of parameters---------------------------------------"<<endl;
	qdotScalar = max(qdotField[0]->getScalar(), qdotField[1]->getScalar());
	qdotMagRange0[0][0] = qdotField[0]->getMin();
	qdotMagRange0[0][1] = qdotField[0]->getMax();	
	qdotMagRange0[1][0] = qdotField[1]->getMin();
	qdotMagRange0[1][1] = qdotField[1]->getMax();
	qdotMagRange1[0][0] = 0;
	qdotMagRange1[0][1] = 0;	
	qdotMagRange1[1][0] = 0;
	qdotMagRange1[1][1] = 0;

	qdotRegionList[0] = qdotField[0]->getRegionIndex();
	qdotRegionList[1] = qdotField[1]->getRegionIndex();

//cerr<<"-----------------------------Initialization---------------------------------------"<<endl;
/*comment for future use*/
/*
	qdotSlider[0]->SetData(qdotField[0]->getMinNozero(),qdotField[0]->getMax(),qdotField[0]->getMinNozero(),qdotField[0]->getMax());
	qdotSlider[1]->SetData(qdotField[1]->getMinNozero(),qdotField[1]->getMax(),qdotField[1]->getMinNozero(),qdotField[1]->getMax());
	qdotSlider[0]->SetAngle(0,360,0,180);
	qdotSlider[1]->SetAngle(0,360,0,180);
	qdotSlider[0]->SetWidget();
	qdotSlider[1]->SetWidget();
	qdotSlider[0]->SetFontColor(qdotFont);
	qdotSlider[1]->SetFontColor(qdotFont);*/
/*comment for future use*/
cerr<<"-----------------------------init map--------------------------------------"<<endl;
	qdotMap[0] = new svQDOTdistancemap(qdotField[0]->getRawPos(), qdotField[0]->getRawSize(),
						qdotField[0]->getLbbox(), qdotField[0]->getRbbox(),
						qdotField[0]->getXd(), qdotField[0]->getYd(),
						qdotField[0]->getXl(), qdotField[0]->getYl(),
						qdotField[0]->getXnum(), qdotField[0]->getYnum(),
						qdotField[0]->getZdistance());
	qdotMap[1] = new svQDOTdistancemap(qdotField[1]->getRawPos(), qdotField[1]->getRawSize(),
						qdotField[1]->getLbbox(), qdotField[1]->getRbbox(),
						qdotField[1]->getXd(), qdotField[1]->getYd(),
						qdotField[1]->getXl(), qdotField[1]->getYl(),
						qdotField[1]->getXnum(), qdotField[1]->getYnum(),
						qdotField[1]->getZdistance());
/*comment for future use*/	
/*cerr<<"-----------------------------------init roi-----------------------------------------------------"<<endl;
	qdotSSlider[0] = new svSpaceSlider(qdotField[0]->getLbbox(), qdotField[0]->getRbbox(),
					qdotField[0]->getMin(), qdotField[0]->getMax(),
					qdotField[0]->getMinNozero());
	qdotSSlider[1] = new svSpaceSlider(qdotField[1]->getLbbox(), qdotField[1]->getRbbox(),
					qdotField[1]->getMin(), qdotField[1]->getMax(),
					qdotField[1]->getMinNozero());*/
/*comment for future use*/
/*cerr<<"-----------------------------------set roi-----------------------------------------------------"<<endl;
	qdotField[0]->getROI(qdotSSlider[0]->getMagrange(),
			qdotSSlider[0]->getZrange(),
			qdotSSlider[0]->getXYrange(),
			qdotSSlider[0]->getSpacerange());
	qdotField[1]->getROI(qdotSSlider[1]->getMagrange(),
			qdotSSlider[1]->getZrange(),
			qdotSSlider[1]->getXYrange(),
			qdotSSlider[1]->getSpacerange());
	qdotField[0]->printROI();
	qdotField[1]->printROI();*/
/*comment for future use*/

cerr<<"-------------------------------------init cutting plane-----------------------------------------------------"<<endl;
/*	qdotField[0]->getPlane(qdotProjectionType[0], qdotDisplay[0], 
		      qdotCuttingV[0], qdotCuttingN[0]);
	qdotField[1]->getPlane(qdotProjectionType[1], qdotDisplay[1], 
		      qdotCuttingV[1], qdotCuttingN[1]);
	qdotField[0]->getPlaneROI(qdotSSlider[0]->getMagrange(),
			qdotSSlider[0]->getZrange(),
			qdotSSlider[0]->getXYrange(),
			qdotSSlider[0]->getSpacerange());
	qdotField[1]->getPlaneROI(qdotSSlider[1]->getMagrange(),
			qdotSSlider[1]->getZrange(),
			qdotSSlider[1]->getXYrange(),
			qdotSSlider[1]->getSpacerange());	*/


cerr<<"-------------------------------------init cutting plane-----------------------------------------------------"<<endl;
	qdotCutSlice[0] = new svSlice(qdotField[0]->getLbbox(), qdotField[0]->getRbbox()); 
	qdotCutSlice[0]->SetPlane(qdotCuttingV[0], qdotCuttingN[0], qdotMDegree[0]);
	qdotCutSlice[1] = new svSlice(qdotField[1]->getLbbox(), qdotField[1]->getRbbox()); 
	qdotCutSlice[1]->SetPlane(qdotCuttingV[1], qdotCuttingN[1], qdotMDegree[1]);
	qdotCutSlice[0]->SetFontColor(qdotFont);
	qdotCutSlice[1]->SetFontColor(qdotFont);
	//qdotCuttingSingle[0].push_back(qdotCutSlice[0]->getCurrentPlane());
	//qdotCuttingSingle[1].push_back(qdotCutSlice[1]->getCurrentPlane());
	//qdotCuttingSingle[0].push_back(0);
	//qdotCuttingSingle[1].push_back(0);
	vector<int> index;
	index.push_back(25);
	index.push_back(50);
	qdotCuttingMulti[0].push_back(index);
	qdotCuttingMulti[1].push_back(index);
	index.clear();
		/*for(int i=0;i<qdotCuttingMulti[0].size();i++)
		{
			
			cerr<<qdotCuttingMulti[0][i][0]<<" "<<qdotCuttingMulti[0][i][1]<<endl;
			
		}*/	
	//index.clear();
cerr<<"-------------------------------------init cutting plane-----------------------------------------------------"<<endl;
	qdotCutSlice[0]->setSelected(qdotCuttingSingle[0], qdotCuttingMulti[0]);
	qdotCutSlice[1]->setSelected(qdotCuttingSingle[1], qdotCuttingMulti[1]);
cerr<<"-------------------------------------init cutting plane-----------------------------------------------------"<<endl;
	qdotField[0]->getPlane(qdotCuttingV[0], qdotCuttingN[0], qdotMDegree[0],qdotCutSlice[0]->getSelected());

	qdotField[1]->getPlane(qdotCuttingV[1], qdotCuttingN[1], qdotMDegree[1],qdotCutSlice[1]->getSelected());
cerr<<"-------------------------------------init cutting plane-----------------------------------------------------"<<endl;
	//qdotCuttingMove[0] = ((float)qdotCutSlice[0]->getCurrentPlane())/((float)qdotCutSlice[0]->getPlaneSize());
	//qdotCuttingMove[1] = ((float)qdotCutSlice[1]->getCurrentPlane())/((float)qdotCutSlice[1]->getPlaneSize());
//	cerr<<qdotCuttingMove[0]<<endl;
	//glui_cuttingscroll[0]->set_float_val(qdotCuttingMove[0]);
	//glui_cuttingscroll[1]->set_float_val(qdotCuttingMove[1]);

cerr<<"--------------------------------------init contour-------------------------------------------------------"<<endl;
	qdotField[0]->contourGeneration(qdotContourLRP[0], qdotContourLR[0],
				qdotContourLAP[0], qdotContourLA[0],
				qdotContourGR[0], qdotContourGA[0]);	
	qdotField[1]->contourGeneration(qdotContourLRP[1], qdotContourLR[1],
				qdotContourLAP[1], qdotContourLA[1],
				qdotContourGR[1], qdotContourGA[1]);	
cerr<<"--------------------------------------init contour-------------------------------------------------------"<<endl;
	qdotField[0]->layerContourGeneration(qdotContourLRP[0], qdotContourLR[0],
				qdotContourLAP[0], qdotContourLA[0],
				qdotContourGR[0], qdotContourGA[0],qdotCutSlice[0]->getSelected());	
	qdotField[1]->layerContourGeneration(qdotContourLRP[1], qdotContourLR[1],
				qdotContourLAP[1], qdotContourLA[1],
				qdotContourGR[1], qdotContourGA[1],qdotCutSlice[1]->getSelected());	
	/*qdotField[0]->getContourROI(qdotSSlider[0]->getMagrange(),
			qdotSSlider[0]->getZrange(),
			qdotSSlider[0]->getXYrange(),
			qdotSSlider[0]->getSpacerange());
	qdotField[1]->getContourROI(qdotSSlider[1]->getMagrange(),
			qdotSSlider[1]->getZrange(),
			qdotSSlider[1]->getXYrange(),
			qdotSSlider[1]->getSpacerange());*/	

/*comment for future use*/
/*
cerr<<"--------------------------------------init symmetry and cluster------------------------------------------------------"<<endl;
	qdotField[0]->symmetryGeneration(qdotSymmetryV[0], qdotSymmetryN[0]);
cerr<<"--------------------------------------init symmetry and cluster------------------------------------------------------"<<endl;
	qdotField[1]->symmetryGeneration(qdotSymmetryV[1], qdotSymmetryN[1]);
*/
cerr<<"--------------------------------------------color-------------------------------------------------"<<endl;
	qdotColor = new svColor();
	qdotColor->SetD3Color();
	qdotColor->SetSpiralColor();
/*comment for future use*/
/*cerr<<"--------------------------------------init cluster------------------------------------------------------"<<endl;
	if(qdotClusterTogether)
	{

		int *cluster = new int [qdotField[1]->getRawSize()];
		qdotField[0]->clusterGeneration(qdotClusterROI[0], qdotClusterType[0],qdotCluster[0][qdotClusterROI[0]][qdotClusterType[0]],
				datafile[1], qdotField[1]->getRawInbox(), qdotField[1]->getRawSize(), 
				qdotSymmetryV[1], qdotSymmetryN[1],
				cluster);
		qdotField[1]->setCluster(qdotClusterType[0],cluster, qdotField[0]->getMagrange());

		qdotField[0]->layerMatlabClustering(qdotCluster[0][qdotClusterROI[0]][qdotClusterType[0]]);
				//qdotClusterROI[0], qdotClusterType[0],qdotClusterType[0],qdotCluster[0][qdotClusterROI[0]][qdotClusterType[0]],
				//datafile[1], qdotField[1]->getRawInbox(), qdotField[1]->getFormat(), 
				//qdotSymmetryV[1], qdotSymmetryN[1],
				//cluster,1);
		qdotField[1]->setLayerCluster(qdotClusterType[0],cluster, qdotField[0]->getLayerMagrange());
		delete [] cluster;
	}
	else
	{
cerr<<"--------------------------------------init symmetry and cluster------------------------------------------------------"<<endl;
		int *cluster = new int [qdotField[1]->getRawSize()];
		qdotField[0]->clusterGeneration(qdotClusterROI[0], qdotClusterType[0],qdotCluster[0][qdotClusterROI[0]][qdotClusterType[0]],
				NULL, NULL, qdotField[1]->getRawSize(), 
				qdotSymmetryV[0], qdotSymmetryN[0],
				cluster);
		qdotField[1]->clusterGeneration(qdotClusterROI[1], qdotClusterType[1],qdotCluster[1][qdotClusterROI[1]][qdotClusterType[1]],
				NULL, NULL, qdotField[1]->getRawSize(), 
				qdotSymmetryV[1], qdotSymmetryN[1],
				cluster);
		qdotField[0]->clusterGeneration(qdotClusterROI[0], 1-qdotClusterType[0],qdotCluster[0][qdotClusterROI[0]][1-qdotClusterType[0]],
				NULL, NULL, qdotField[1]->getRawSize(), 
				qdotSymmetryV[0], qdotSymmetryN[0],
				cluster);
		qdotField[1]->clusterGeneration(qdotClusterROI[1], 1-qdotClusterType[1],qdotCluster[1][qdotClusterROI[1]][1-qdotClusterType[1]],
				NULL, NULL, qdotField[1]->getRawSize(), 
				qdotSymmetryV[1], qdotSymmetryN[1],
				cluster);
*/
cerr<<"--------------------------------------init cluster------------------------------------------------------"<<endl;
		double mag[2];
		mag[0] = qdotMagRange0[0][0];
		mag[1] = qdotMagRange0[0][1];
		double mag2[2];
		mag2[0] = qdotMagRange1[0][0];
		mag2[1] = qdotMagRange1[0][1];
		double ws[2], wv[2], wd[2];
		ws[0]=qdotLweight[0][0];ws[1]=qdotLweight[0][1];
		wv[0]=qdotVweight[0][0];wv[1]=qdotVweight[0][1];
		wd[0]=qdotDweight[0][0];wd[1]=qdotDweight[0][1];
		cerr<<mag[0]<<" "<<mag[1]<<" "<<mag2[0]<<" "<<mag2[1]<<" "<<qdotHCNum[0][0]<<" "<<qdotHCNum[0][1]<<" "
			<<ws[0]<<" "<<ws[1]<<" "<<wv[0]<<" "<<wv[1]<<" "<<wd[0]<<" "<<wd[1]<<endl;
		qdotField[0]->layerClustering(qdotHCNum[0],mag,mag2,
					ws,wv,wd, 
					qdotCutSlice[0]->getSelected(),qdotMDegree[0]); 
cerr<<"--------------------------------------init cluster------------------------------------------------------"<<endl;
		qdotField[0]->layerContourClustering(mag,mag2,
					ws,wv,wd, 
					qdotCutSlice[0]->getSelected());
cerr<<"--------------------------------------init cluster------------------------------------------------------"<<endl;
		mag[0] = qdotMagRange0[1][0];
		mag[1] = qdotMagRange0[1][1];
		mag2[0] = qdotMagRange1[1][0];
		mag2[1] = qdotMagRange1[1][1];
		ws[0]=qdotLweight[1][0];ws[1]=qdotLweight[1][1];
		wv[0]=qdotVweight[1][0];wv[1]=qdotVweight[1][1];
		wd[0]=qdotDweight[1][0];wd[1]=qdotDweight[1][1];
		cerr<<mag[0]<<" "<<mag[1]<<" "<<mag2[0]<<" "<<mag2[1]<<" "<<qdotHCNum[1][0]<<" "<<qdotHCNum[1][1]<<" "
			<<ws[0]<<" "<<ws[1]<<" "<<wv[0]<<" "<<wv[1]<<" "<<wd[0]<<" "<<wd[1]<<endl;
		qdotField[1]->layerClustering(qdotHCNum[1],mag,mag2,
					ws,wv,wd,  
					qdotCutSlice[1]->getSelected(),qdotMDegree[1]);
		qdotField[1]->layerContourClustering(mag,mag2,
					ws,wv,wd, 
					qdotCutSlice[1]->getSelected());
/*comment for future use*/
/*cerr<<"--------------------------------------init cluster------------------------------------------------------"<<endl;
		delete [] cluster;
	}
	qdotField[0]->contourClustering( qdotClusterType[0],qdotClusterROI[0],(bool)qdotClusterlayer);
	qdotField[1]->contourClustering( qdotClusterType[1],qdotClusterROI[1],(bool)qdotClusterlayer);
	qdotField[0]->planeClustering( qdotClusterType[0],qdotClusterROI[0]);
	qdotField[1]->planeClustering( qdotClusterType[1],qdotClusterROI[1]);
*/
/*comment for future use*/
cerr<<"--------------------------------------------init distancemap-------------------------------------------------"<<endl;
	vector<svScalar> z;
	for(int i=0;i<qdotDistanceV[0].size();i++) z.push_back(qdotDistanceV[0][i][2]);
	qdotField[0]->distanceGeneration(qdotMap[0]->GetSelectedLayer(), 
				qdotDistanceV[0], z);
	z.clear();
	for(int i=0;i<qdotDistanceV[1].size();i++) z.push_back(qdotDistanceV[1][i][2]);
	qdotField[1]->distanceGeneration(qdotMap[1]->GetSelectedLayer(), 
				qdotDistanceV[1], z);
	z.clear();

cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
	qdotDisc[0] = new svDisc();
	qdotDisc[1] = new svDisc();
cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
/*comment for future use*/
	for(int i=0;i<2;i++)
	{
		cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
		if(qdotDisplay[i] == 0)//D_Contour)
		{
		cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
			qdotArrowSelection[i] = new svArrowSelection(qdotField[i]->getLayerContourPos(), 
								qdotField[i]->getLayerContourVel(),
								qdotField[i]->getLayerContourDen(),
								qdotField[i]->getLayerContourExp(),
								qdotField[i]->getLayerContourCoe(),
								qdotField[i]->getLayerContourAtom(),
								qdotField[i]->getLayerContourSize(),
								qdotField[i]->getFormat());
		cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
			qdotArrowSelection[i]->displayList = i+1;
			qdotArrowSelection[i]->SetInbox(NULL);
		cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
			if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourCluster(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getLayerContourAtom(),
										qdotFont, 
										qdotField[i]->getLayerContourSize()));
			}
		cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
			qdotDisc[i]->SetData(qdotField[i]->getLayerContourPos(), 
							qdotField[i]->getLayerContourVel(),
							qdotField[i]->getLayerContourDen(),
							qdotField[i]->getLayerContourExp(),
							qdotField[i]->getLayerContourCoe(),
							qdotField[i]->getLayerContourSize());
			qdotDisc[i]->SetClusters(qdotField[i]->getLayerContourCluster(),
							qdotHCNum[i][0]+qdotHCNum[i][1]);
			qdotDisc[i]->computeMean(qdotField[i]->getLayerDenContourCluster(),qdotField[i]->getMaglevel());
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());
		cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
		}
		else
		{
		cerr<<"--------------------------------------------3d arrowstart-------------------------------------------------"<<endl;
			qdotArrowSelection[i] = new svArrowSelection(qdotField[i]->getRawPos(), 
								qdotField[i]->getRawVel(),
								qdotField[i]->getRawDen(),
								qdotField[i]->getRawExp(),
								qdotField[i]->getRawCoe(),
								qdotField[i]->getRawAtom(),
								qdotField[i]->getRawSize(),
								qdotField[i]->getFormat());
			qdotArrowSelection[i]->SetInbox(qdotField[i]->getRawInbox());
			qdotArrowSelection[i]->displayList = i+1;
		cerr<<"--------------------------------------------3d arrowcolor-------------------------------------------------"<<endl;
			if(qdotColorby[i] == 0)
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotArrowSelection[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
		cerr<<"--------------------------------------------3d arrowstart-------------------------------------------------"<<endl;
			qdotDisc[i]->SetData(qdotField[i]->getLayerPos(), 
							qdotField[i]->getLayerVel(),
							qdotField[i]->getLayerDen(),
							qdotField[i]->getLayerExp(),
							qdotField[i]->getLayerCoe(),
							qdotField[i]->getLayerSize());
		cerr<<"--------------------------------------------3d arrowstart-------------------------------------------------"<<endl;
			qdotDisc[i]->SetClusters(qdotField[i]->getLayerCluster(),
							qdotHCNum[i][0]+qdotHCNum[i][1]);
			qdotDisc[i]->computeMean(qdotField[i]->getLayerDenCluster(),qdotField[i]->getMaglevel());
			qdotDisc[i]->setColors(qdotColor->getColors(qdotBlind, 
							0, qdotField[i]->getMaglevel()),
							qdotColor->getColors(qdotBlind, 
							true, qdotHCNum[i][0]+qdotHCNum[i][1]), 
							qdotField[i]->getMaglevel(), false, qdotField[i]->getlayer());

		}
		qdotArrowSelection[i]->setMaxMag(qdotField[i]->getMax());
		qdotArrowSelection[i]->SetSample(qdotFrequency, qdotDisplay[i]);
		qdotArrowSelection[i]->SetFontColor(qdotFont);
		qdotArrowSelection[i]->SetScalar(qdotScalar);
cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
		if(qdotEncode[i] == 0)
			qdotArrowSelection[i]->setLength(qdotLength * 5e-11);
		else
			qdotArrowSelection[i]->setLength(qdotLength);
cerr<<"--------------------------------------------3d arrow-------------------------------------------------"<<endl;
		qdotArrowSelection[i]->setArrowBase(qdotArrowBase);
cerr<<"--------------------------------------------3d arrow2-------------------------------------------------"<<endl;
		qdotArrowSelection[i]->SetEncodeMode(qdotEncode[i]);
cerr<<"--------------------------------------------3d arrow3-------------------------------------------------"<<endl;
		qdotArrowSelection[i]->SetArrows();
cerr<<"--------------------------------------------3d arrow4-------------------------------------------------"<<endl;
		//qdotArrowSelection[i]->svArrowGlyphRenderArrow(qdotOpacity);
	}

cerr<<"--------------------------------------------outline-------------------------------------------------"<<endl;
	qdotOutline[0] = new svOutline();
	qdotOutline[1] = new svOutline();
	qdotOutline[0]->SetFontColor(qdotFont);
	qdotOutline[1]->SetFontColor(qdotFont);
cerr<<"--------------------------------------------Mesh-------------------------------------------------"<<endl;
//JC17
	//qdotMesh[0] = new svMesh();
	//qdotMesh[1] = new svMesh();

	/*vector<bool> rindex;
	rindex.resize(qdotRegionList[0].size());
	for(int i=0;i<rindex.size();i++)
	{
		rindex[i] = false;
	}
	if(qdotRegion[0]==qdotRegionList[0].size()+1)
	{
		for(int i=0;i<rindex.size();i++)
		{
			rindex[i] = true;
		}
	}
	else
	{
		rindex[qdotRegion[0]-1] = true;
	}*/

	/*rindex.resize(qdotRegionList[1].size());
	for(int i=0;i<rindex.size();i++)
	{
		rindex[i] = false;
	}
	if(qdotRegion[1]==qdotRegionList[1].size()+1)
	{
		for(int i=0;i<rindex.size();i++)
		{
			rindex[i] = true;
		}
	}
	else
	{
		rindex[qdotRegion[1]-1] = true;
	}*/


							
/*comment for future use*/

/*comment for future use*/
/*
cerr<<"-------------------------------------------symmetry-------------------------------------------------"<<endl;
	qdotSymmetrySlice[0] = new svSlice(qdotField[0]->getLbbox(), qdotField[0]->getRbbox(),
					qdotSymmetryV[0], qdotSymmetryN[0]); 
	qdotSymmetrySlice[1] = new svSlice(qdotField[1]->getLbbox(), qdotField[1]->getRbbox(),
					qdotSymmetryV[1], qdotSymmetryN[1]); 
	qdotSSlider[0]->SetImage(image_width, image_height);*/
/*comment for future use*/
/*
cerr<<"-------------------------------------------projection-------------------------------------------------"<<endl;
	qdotCut[0] = new svCut();
	qdotCut[1] = new svCut();
	qdotCut[0]->SetData(qdotField[0]->getZmin(), qdotField[0]->getZdistance(),
				qdotField[0]->getZlayer(),
				qdotField[0]->getLbbox(), qdotField[0]->getRbbox());
	qdotCut[1]->SetData(qdotField[1]->getZmin(), qdotField[1]->getZdistance(),
				qdotField[1]->getZlayer(),
				qdotField[1]->getLbbox(), qdotField[1]->getRbbox());
	if(qdotClusterType[0] == 0)
	{
		qdotCut[0]->SetCut(qdotField[0]->getPlanePos(),qdotField[0]->getPlaneVel(),
			qdotField[0]->getPlaneDen(),
			qdotColor->getColors(qdotBlind, 
						qdotClusterType[0], qdotClusterNum[0], 
						qdotField[0]->getPlaneMagcluster(),
						qdotField[0]->getPlaneSize(), qdotFont),
			qdotField[0]->getPlaneSize());
		qdotCut[1]->SetCut(qdotField[1]->getPlanePos(),qdotField[1]->getPlaneVel(),
				qdotField[1]->getPlaneDen(),
				qdotColor->getColors(qdotBlind, 
							qdotClusterType[1], qdotClusterNum[1], 
							qdotField[1]->getPlaneMagcluster(),
							qdotField[1]->getPlaneSize(), qdotFont),
				qdotField[1]->getPlaneSize());
	}
	else
	{
		qdotCut[0]->SetCut(qdotField[0]->getPlanePos(),qdotField[0]->getPlaneVel(),
			qdotField[0]->getPlaneDen(),
			qdotColor->getColors(qdotBlind, 
						qdotClusterType[0], qdotClusterNum[0], 
						qdotField[0]->getPlaneVelcluster(),
						qdotField[0]->getPlaneSize(), qdotFont),
			qdotField[0]->getPlaneSize());
		qdotCut[1]->SetCut(qdotField[1]->getPlanePos(),qdotField[1]->getPlaneVel(),
				qdotField[1]->getPlaneDen(),
				qdotColor->getColors(qdotBlind, 
							qdotClusterType[1], qdotClusterNum[1], 
							qdotField[1]->getPlaneVelcluster(),
							qdotField[1]->getPlaneSize(), qdotFont),
				qdotField[1]->getPlaneSize());
	}
	qdotCut[0]->SetInbox(qdotField[0]->getPlaneInbox(),qdotField[0]->getPlaneSize());
	qdotCut[1]->SetInbox(qdotField[1]->getPlaneInbox(),qdotField[1]->getPlaneSize());
	qdotCut[0]->SetFontColor(qdotFont);
	qdotCut[1]->SetFontColor(qdotFont);*/

/*comment for future use*/

cerr<<"------------------------------------------distance-------------------------------------------------"<<endl;
	qdotMap[0]->SetDistance(qdotField[0]->getRawDis());
	qdotMap[1]->SetDistance(qdotField[1]->getRawDis());
	qdotMap[0]->SetFontColor(qdotFont);
	qdotMap[1]->SetFontColor(qdotFont);
	for(int i=0;i<2;i++)
	{
			if(qdotColorby[i] == 0)
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawCluster(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}
			else
			{
				qdotMap[i]->SetColors(qdotColor->getColors(qdotBlind, 
										qdotField[i]->getRawAtom(),
										qdotFont, 
										qdotField[i]->getRawSize()));
			}	
	}
	/*qdotMap[0]->SetColors(qdotColor->getColors(qdotBlind, 
						qdotClusterType[0], qdotClusterNum[0], 
						qdotField[0]->getRawVelcluster(),
						qdotField[0]->getRawSize(), qdotFont));
	qdotMap[1]->SetColors(qdotColor->getColors(qdotBlind, 
						qdotClusterType[1], qdotClusterNum[1], 
						qdotField[1]->getRawVelcluster(),
						qdotField[1]->getRawSize(), qdotFont));*/
/*cerr<<"------------------------------------------bubble-------------------------------------------------"<<endl;
	qdotBubble[0] = new svBubbleSet(qdotField[0]->getRawPos(), qdotField[0]->getRawSize(),
						qdotField[0]->getLbbox(), qdotField[0]->getRbbox(),
						qdotField[0]->getXd(), qdotField[0]->getYd(),
						qdotField[0]->getXl(), qdotField[0]->getYl(),
						qdotField[0]->getXnum(), qdotField[0]->getYnum(),
						qdotField[0]->getZdistance());
	qdotBubble[1] = new svBubbleSet(qdotField[1]->getRawPos(), qdotField[1]->getRawSize(),
						qdotField[1]->getLbbox(), qdotField[1]->getRbbox(),
						qdotField[1]->getXd(), qdotField[1]->getYd(),
						qdotField[1]->getXl(), qdotField[1]->getYl(),
						qdotField[1]->getXnum(), qdotField[1]->getYnum(),
						qdotField[1]->getZdistance());
	qdotBubble[0]->setROI(qdotField[0]->getRawROI());
	qdotBubble[1]->setROI(qdotField[1]->getRawROI());*/
	//bubble color modifcation
cerr<<"------------------------------------------Picking-------------------------------------------------"<<endl;
	qdotPicking[0] = new svPickingray();
	qdotPicking[1] = new svPickingray();
cerr<<"------------------------------------------Done-------------------------------------------------"<<endl;
}
/*void newcluster()
{
	ifstream infile("newcluster");//hard_code!!!!!

	char str[50];
	float s;

	infile>>str;
	infile>>s;
	//if(s!=qdotClusterNum[0])
	//{
		qdotClusterNum[0] = s;
		qdotClusterNum[1] = qdotClusterNum[0];
		fclusterprocessing();
		fclusterdisc();
		fsetcolor();
		fsetdisplaylist();
	//}
	
	infile.close();

}
void increasescaling()
{
	qdotLength = qdotLength*2.;
	fscalar();
	fsetdisplaylist();
}
void decreasescaling()
{
	qdotLength = qdotLength/2.;
	fscalar();
	fsetdisplaylist();
}
void increasearrowbase()
{
	qdotArrowBase = qdotArrowBase*2.;
	farrowbase();
	fsetdisplaylist();
}
void decreasearrowbase()
{
	qdotArrowBase = qdotArrowBase/2.;
	farrowbase();
	fsetdisplaylist();
}
void toggleencode()
{
	if(qdotEncode[0] == SPLITVECTORS) qdotEncode[0] = LINEAR;
	else qdotEncode[0] = SPLITVECTORS;
	if(qdotEncode[1] == SPLITVECTORS) qdotEncode[1] = LINEAR;
	else qdotEncode[1] = SPLITVECTORS;
	
	fencode();
	fresetarrow();
	fsetdisplaylist();
}
void increasealpha()
{
	if(qdotOpacity>=1)qdotOpacity=1;
	else qdotOpacity = qdotOpacity + 0.1;
	cerr<<"qdotOpacity "<<qdotOpacity<<endl;
	fsetdisplaylist();
}
void decreasealpha()
{
	if(qdotOpacity<=0)qdotOpacity=0;
	else qdotOpacity = qdotOpacity - 0.1;
	fsetdisplaylist();
}*/

void control(int id)
{
	switch (id)
	{
		case 1: glui_data_pop1->show(); break;//Loaddata_id[0]
		case 2: glui_data_pop2->show(); break; //Loaddata_id[1]:
		case 3: cerr<<glui_datalist[0]->get_current_item();break;//Loaddata_List_id[0]=3;
		case 4: cerr<<glui_datalist[1]->get_current_item();break;//Loaddata_List_id[1]=4;
		case 5: fdataok(0);glui_data_pop1->hide(); break;//Loaddata_OK_id[0]
		case 6: fdataok(1);glui_data_pop2->hide(); break;//Loaddata_OK_id[1]
		case 7: fdatacancel(0); glui_data_pop1->hide(); break;//Loaddata_Cancel_id[0]
		case 8: fdatacancel(1); glui_data_pop2->hide(); break;//Loaddata_Cancel_id[1]

		case 9:{if(qdotDisplay[0] <2)
			{
				qdotNewArrowData[0]=true;qdotNewArrowContour[0]=true;qdotNewArrow[0]=true;
			}break;
			}//Display_id[0]
		case 10:{cerr<<"qdotdisplay "<<qdotDisplay[1]<<endl;
			if(qdotDisplay[1] <2)
			{
				qdotNewArrowData[1]=true;qdotNewArrowContour[1]=true;qdotNewArrow[1]=true;
			}break;
			}//Display_id[1]
		case 11:fencode(0);break;//Encode_id[0]
		case 12:fencode(1);break;//Encode_id[1]
		case 13:fsetcolor();break;//Color_id[0]
		case 14:fsetcolor();break;//Color_id[1]
		case 17:redisplay_all();break;//Overview_id[0]
		case 18:redisplay_all();break;//Overview_id[1]

		case 19: fcontoursync(0);break;//Contour_Sync_id[0]
		case 75: fcontoursync(1);break;//Contour_Sync_id[1]
		case 20: fcontouredit(0);break;//Contour_Edit_id[0]
	 	case 21: fcontouredit(1);break;//Contour_Edit_id[1]
	
		case 22: fclustersync(0);break;//Cluster_Sync_id[0]
		case 76: fclustersync(1);break;//Cluster_Sync_id[1]
		case 23: fclustertype(0);break;//Cluster_Type_id[0]
		case 37:fclustertype(1);break;//Cluster_Type_id[1]
		case 25:fcuttingedit(0);break;//Cutting_Edit_id[0]
		case 39:fcuttingedit(1);break;//Cutting_Edit_id[1]
		case 78:fcuttingdistance(0);break;//Cutting_Distance_id[0]
		case 79:fcuttingdistance(1);break;//Cutting_Distance_id[1]
		case 26:fcuttingmove(0);break;//Cutting_Move_id[0]
		case 40:fcuttingmove(1);break;//Cutting_Move_id[1]
		case 73: fclusteredit(0, Cluster_Num_id0[0]);break;//Cluster_Num_id0[0]
		case 74: fclusteredit(1, Cluster_Num_id0[1]);break;//Cluster_Num_id0[1]
		case 80: fclusteredit(0, Cluster_Num_id1[0]);break;//Cluster_Num_id1[0]
		case 81: fclusteredit(1, Cluster_Num_id1[1]);break;//Cluster_Num_id1[1]
		case 84: glui_cutting[0]->hide();break;//Cuttingok_id[0] = 84;
		case 85: glui_cutting[1]->hide();break;//Cuttingok_id[1] = 85;
		case 86: glui_cutting[0]->show();fcuttingpop(0);break;//Cuttingbutton_id[0] = 84;
		case 87: glui_cutting[1]->show();fcuttingpop(1);break;//Cuttingbutton_id[1] = 85;
		case 24: fclusterlayer(0);break;//Cluster_Layer_id[0]
		case 38: fclusterlayer(1);break;//Cluster_Layer_id[1]
		case 27:fclusterweight(0, id);break;//Lwegith_id0[0]:
		case 32:fclusterweight(0, id);break;//Lwegith_id1[0]:
		case 28:fclusterweight(0, id);break;//Vwegith_id0[0]:
		case 33:fclusterweight(0, id);break;//Vwegith_id1[0]:
		case 29:fclusterweight(0, id);break;//Dwegith_id0[0]:
		case 34:fclusterweight(0, id);break;//Dwegith_id1[0]:
		case 30:fclusterweight(0, id);break;//MRange_id00[0]:
		case 31:fclusterweight(0, id);break;//MRange_id01[0]:
		case 35:fclusterweight(0, id);break;//MRange_id10[0]:
		case 36:fclusterweight(0, id);break;//MRange_id11[0]
		case 41:fclusterweight(1, id);break;//Lwegith_id0[1]:
		case 46:fclusterweight(1, id);break;//Lwegith_id1[1]:
		case 42:fclusterweight(1, id);break;//Vwegith_id0[1]:
		case 47:fclusterweight(1, id);break;//Vwegith_id1[1]:
		case 43:fclusterweight(1, id);break;//Dwegith_id0[1]:
		case 48:fclusterweight(1, id);break;//Dwegith_id1[1]:
		case 44:fclusterweight(1, id);break;//MRange_id00[1]:
		case 45:fclusterweight(1, id);break;//MRange_id01[1]:
		case 49:fclusterweight(1, id);break;//MRange_id10[1]:
		case 50:fclusterweight(1, id);break;//MRange_id11[1]:

		case 51: fdistancesync(0);break;//Distance_Sync_id[0]
		case 77: fdistancesync(1);break;//Distance_Sync_id[1]
		case 54: fdistanceedit(0);break;//Distance_List_id[0]
		case 58: fdistanceedit(1);break;//Distance_List_id[1]
		case 52: fdistancesample();break;//Distance_Sample_id[0]
		//case 56: fdistancesample(1);break;//Distance_Sample_id[1]
		case 55: fdistancez(0);break;//Distance_Z_id[0]
		case 59: fdistancez(1);break;//Distance_Z_id[1]

		case 60:fsetdisplaylist_mesh();break;//Region_id[0]
		case 62:fsetdisplaylist_mesh();break;//Region_id[1]fsetdisplaylist_mesh();

		case 61: update();break;//Update_id[0]
		case 63: update();break;//Update_id[1]

		case 64: fbackground();break;//Background_id
		case 65: fopacity();break;//Opacity_id
		case 66: {	char str[20];
				sprintf(str,"%0.2f",qdotArrowBase);
				glui_base->set_text(str);}break;//Arrow_id
		case 67: {	char str[20];
				sprintf(str,"%0.2f",qdotLength);
				glui_length->set_text(str);}break;//Length_id
		case 68: fblind();break;//Blind_id
		case 69: ffrequency();break;//Frequency_id
		case 82: redisplay_all();break;//Cutvisible_id;
		case 83: flength();farrow();fsetdisplaylist_arrow();break;//Cutvisible_id;

//Cutting_List_id[0] = 88;
//Cutting_List_id[1] = 89;
		//case 70: fsavepicture();break;//Picture_id
		//case 71: fsave();break;//Save_id
		//case 72: exit(0);break;//Exit_id


	}
		redisplay_all();
}
