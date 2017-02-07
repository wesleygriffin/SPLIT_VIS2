/*  svGlyph
 */

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "svGlyph.h"
#include "svException.h"
#include "svUtil.h"

// TODO: add renderproperty

using namespace std;

namespace __svl_lib {

svGlyph::svGlyph(svScalarArray xpos, svScalarArray ypos, 
                 svScalarArray zpos, svVector3Array d)
    :svPrimitive(1)
{
  int ysize=ypos.size(), zsize=zpos.size();

  glyph = new svVector3Array[1];
  dir = new svVector3Array[1];

  for(int zi=0; zi<zsize; zi++) {
    for(int yi=0; yi<ysize; yi++) {
      glyph[0].add(svVector3(0,ypos[yi],zpos[zi]));
      dir[0].add(d[zi*ysize+yi].getValue());
    };// end for(yi)
  }; // end for(zi)

  glyphColors = NULL;
  glyphWidth = NULL;
}

svGlyph::svGlyph(svChar *infname, svInt _seed_num): svPrimitive(_seed_num)
{
  infile = strdup(infname);
  glyph = NULL;
  dir=NULL;
  glyphColors = NULL;
  glyphWidth = NULL;
}

svGlyph::svGlyph(svChar *dir, svChar *inf1, svChar *inff2,
            svVectorField* f, int numPlane)
          :svPrimitive(f);
{
      fileDir = strdup(dir);
      fileName1 = strdup(inf1);
      fileName2 = strdup(inf2);
      
      seed_num = numPlane;

      SetKmeansProperty();
      SetContourProperty();

      glyph=NULL;
      dir = NULL;
      mag = NULL;
     
      clusterLabel = NULL;
      roiLabel = NULL;
      visibleLabel = NULL;

      glyphColors = NULL;
      glyphWidth = NULL;

      isContour = false;
}

svGlyph::svGlyph(svVectorField* f, int numPlane)
        : svPrimitive(f)
{
	//cerr<<"init"<<endl;
        glyph = NULL;//new svVector3Array[numPlane];
        dir = NULL;//new svVector3Array[numPlane];
        mag = NULL;//new svScalarArray[numPlane];

        visibleLabel = NULL;
        clusterLabel = NULL;
        contourLabel = NULL;
        roiLabel = NULL;//new svIntArray[numPlane];
        glyphColors = NULL;//new svVector4Array[numPlane];
        glyphWidth = NULL;

        seed_num = numPlane;

        isContour=false;
}


svGlyph::svGlyph(svVector3 _lbbox, svVector3 _rbbox, svChar *inf, svInt _seed_num) 
            : svPrimitive(_seed_num)
{
/*
  lbbox=_lbbox;
  rbbox=_rbbox;

  infile = strdup(inf);

  glyph = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;

  glyph_radius = DEFAULT_GLYPH_RADIUS;
  glyph_height = DEFAULT_GLYPH_HEIGHT;
  glyph_frequency= DEFAULT_GLYPH_FREQUENCY;
  glyph_section_num = DEFAULT_GLYPH_SECTION_NUM;
  tube_radius = glyph_radius * 0.5;
*/
}

/*
svGlyph::svGlyph(svVectorField* f, 
              svParticle* sd, svInt sd_num, svUint mode)
  : svPrimitive(f,sd, sd_num, mode)
{
  glyph   = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;

  glyph_radius = DEFAULT_GLYPH_RADIUS;
  glyph_height = DEFAULT_GLYPH_HEIGHT;
  glyph_frequency= DEFAULT_GLYPH_FREQUENCY;
  glyph_section_num = DEFAULT_GLYPH_SECTION_NUM;
  tube_radius = glyph_radius * 0.5;
}

svGlyph::svGlyph(svVectorField* f, 
              svVector3 _lbbox, svVector3 _rbbox,
              svParticle* sd, svInt sd_num, svUint mode)
  : svPrimitive(f,sd, sd_num, mode)
{
  svPrimitive::field->SetPhysicalDimension(_lbbox,_rbbox);
  glyph   = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;

  glyph_radius = DEFAULT_GLYPH_RADIUS;
  glyph_height = DEFAULT_GLYPH_HEIGHT;
  glyph_frequency= DEFAULT_GLYPH_FREQUENCY;
  glyph_section_num = DEFAULT_GLYPH_SECTION_NUM;
  tube_radius = glyph_radius * 0.5;
}
*/

/*
svGlyph::svGlyph(svVector3Array *vec3in, svVectorField *f, svInt num)
   : svPrimitive(f, NULL, num, SV_STORE)
{
  glyph   = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;
  //glyph_radius = DEFAULT_GLYPH_RADIUS;
  //glyph_height = DEFAULT_GLYPH_HEIGHT;
  //glyph_frequency= DEFAULT_GLYPH_FREQUENCY;
  //glyph_section_num = DEFAULT_GLYPH_SECTION_NUM;
  //tube_radius = glyph_radius * 0.5;
  //Generate(vec3in, color4in);
  //Generate(vec3in);
  //ENABLE_COLOR = true;
}
*/

/*
svGlyph::svGlyph(svVector3Array *vec3in, svVector4Array *color4in, svInt num)
   : svPrimitive(NULL, NULL, num, SV_STORE)
{
  glyph   = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;
  //glyph_radius = DEFAULT_GLYPH_RADIUS;
  //glyph_height = DEFAULT_GLYPH_HEIGHT;
  //glyph_frequency= DEFAULT_GLYPH_FREQUENCY;
  //glyph_section_num = DEFAULT_GLYPH_SECTION_NUM;
  //tube_radius = glyph_radius * 0.5;
  //Generate(vec3in, color4in);
  ENABLE_COLOR = true;
}
*/

void svGlyph::New(svChar *dir, svChar *inf1, svChar *inff2,
            svVectorField* f, int numPlane)
{
      clean();

      seed_num = numPlane;

     glyph = new svVector3Array[seed_num];
     dir = new svVector3Array[seed_num];
     mag = new svScalarArray[seed_num];
 
     clusterLabel = new svIntArray[seed_num];
     roiLabel = new svIntArray[seed_num];
     visibleLabel = new svIntArray[seed_num];
     
     glyphColors = new svVector4Array[seed_num];
     glyphWidth = new svScalarArray[seed_num];

     SetKmeansProperty();
     SetContourProperty();
}

void svGlyph::SetData(char *infName, int seed)
{
        int layer = seed;
        ifstream infile(infName);
        int num;
        infile>>num;
        for(int i=0;i<num;i++)
        {
                svVector3 pos, vec;
                svScalar den;

                infile>>pos[0]>>pos[1]>>pos[2]
                                >>vec[0]>>vec[1]>>vec[2]
                                >>den;

                glyph[layer].add(pos);
                dir[layer].add(vec);
                mag[layer].add(den);

                clusterLabel[layer].add(-1);
                roiLabel[layer].add(true);
                visibleLabel[layer].add(true);
              
               svVector4 color;
               color[0]=1;color[1]=1;color[2]=1;color[3]=1;
               glyphColors[layer].add(color);
        }

        infile.close();

}

/*
void svGlyph::New(int numPlane)
{
     clean();

     seed_num = numPlane;

     glyph = new svVector3Array[seed_num];
     dir = new svVector3Array[seed_num];
     mag = new svScalarArray[seed_num];
 
     clusterLabel = new svIntArray[seed_num];
     roiLabel = new svIntArray[seed_num];
     visibleLabel = new svIntArray[seed_num];
     
     contourLabel = new svBool[seed_num];
     glyphColors = new svVector4Array[seed_num];
     contourValues = new svScalarArray[seed_num];
}
*/
/*
 * void svGlyph::SetAllData()
{
       for(int i=0;i<seed_num;i++)
        {
             SetRawData(i);
        }
}
*/
/*
void svGlyph::SetRawData(int layer)
{
	char *dataFile = new char[200];
	sprintf(dataFile, "%s/%d.txt", tmpDir, layer);
	
	
	ifstream infile(dataFile);
	int num;
	infile>>num;
	for(int i=0;i<num;i++)
	{
		svVector3 pos, vec;
		svScalar den;
		
		infile>>pos[0]>>pos[1]>>pos[2]
				>>vec[0]>>vec[1]>>vec[2]
				>>den;
				
		glyph[layer].add(pos);
		dir[layer].add(vec);
		mag[layer].add(den);
		clusterLabel[layer].add(-1);
                roiLabel[layer].add(true);
                visibleLabel[layer].add(true);
                               svVector4 color;
               color[0]=1;color[1]=1;color[2]=1;color[3]=1;
               glyphColors[layer].add(color);
	}
	infile.close();
	
	delete [] dataFile;
}

void svGlyph::SetContourData(int layer)
{
	ifstream infile(contourFile);
	int num;
	infile>>num;
	for(int i=0;i<num;i++)
	{
		svVector3 pos, vec;
		svScalar den;
		
		infile>>pos[0]>>pos[1]>>pos[2]
				>>vec[0]>>vec[1]>>vec[2]
				>>den;
				
		glyph[layer].add(pos);
		dir[layer].add(vec);
		mag[layer].add(den);
		
                clusterLabel[layer].add(-1);
                roiLabel[layer].add(true);
                visibleLabel[layer].add(true);
                               svVector4 color;
               color[0]=1;color[1]=1;color[2]=1;color[3]=1;
               glyphColors[layer].add(color);
	}
	
	infile.close();
}
*/
void svGlyph::SaveToFile()//, char *fname)
{
  ofstream outf(glyphFile);

  clusterDataSize = 0;

  for(int i=0;i< clusterLayer.size();i++)
  {
	  int layer = clusterLayer[i];
    int s = glyph[layer].size();
   // outf << s << endl;
    for(int j=0; j<s; j++)
    {
		if(roiLabel[clusterLayer[i]][j])
		{
		if(dimension == 7)
		{
	      outf << glyph[layer][j][0] << " "
           << glyph[layer][j][1] << " "
	   << glyph[layer][j][2] << " ";
	   outf <<dir[layer][j][0]<<" "
	   <<dir[layer][j][1]<<" "
	   <<dir[layer][j][2]<<" ";
	   outf<<mag[layer][j]<<endl;		
		}
		else if(dimension == 6)
		{
	      outf << glyph[layer][j][0] << " "
           << glyph[layer][j][1] << " "
	   << glyph[layer][j][2] << " ";
	   outf <<dir[layer][j][0]<<" "
	   <<dir[layer][j][1]<<" "
	   <<dir[layer][j][2]<<endl;			
		}
		else if(dimension == 3)
		{
	      outf << glyph[layer][j][0] << " "
           << glyph[layer][j][1] << " "
	   << glyph[layer][j][2] << endl;			
		}
                clusterDataSize ++;
		}
    };
  }

  outf.close();
}

void svGlyph::SaveToFile(char *fname)
{
  ofstream outf(fname);
  outf << seed_num << endl;
  for(int i=0; i<seed_num; i++)
  {
    int s = glyph[i].size();
    outf << s << endl;
    for(int j=0; j<s; j++)
    {
      outf << glyph[i][j][0] << "  "
           << glyph[i][j][1] << "  "
	   << glyph[i][j][2] << endl;
    };
  }
  outf.close();
}

void svGlyph::ResetData(int seed)
{
       int layer = seed;

	glyph[layer].free();
	dir[layer].free();
	mag[layer].free();
	
	clusterLabel[layer].free();
	roiLabel[layer].free();
	visibleLabel[layer].free();
	
	glyphColors[layer].free();

  //     ResetContourLabel(layer);
  //    ResetContour(layer);
}

void svGlyph::SetContourProperty(ContourProperty & property)
{
         if(contourproperty.contourValues == NULL)
         {
             contourproperty.seed_num = property.seed_num;
             contourproperty.contourValues = new svScalarArray[property.seed_num];
             for(int i=0;i<property.seed_num;i++)
             {
                 for(int j=0;j<property.contourValues[i].size();j++)
                 {
                   contourproperty.contourValues[i].add(property.contourValues[i][j]);
                  }
                   contourproperty.isUpdate.add(1); 
             }
         }
        else
        {
               for(int i=0;i<property.seed_num;i++)
               {
                  bool flag = false;
                  if(property.contourValues[i].size()!=contourproperty.contourValues[i].size())
                 {
                       flag = true;
                 } 
                 else 
                 {
                     for(int j=0;j<property.contourValues[i].size();j++)
                     {
                          for(int t =0;t<contourproperty.contourValues[i].size();t++)
                          {
                                if(!fabs(property.contourValues[i][j] - contourproperty.contourValues[i][j])<1e-20)
                                 {
                                     flag = true;break; 
                                  }
                          }
                     }
                 }
                 if(flag)
                 {
                     contourproperty.contourValues[i].free();
                     contourproperty.isUpdate[i] = 1;
                     for(int j=0;j<property.contourValues[i].size();j++)
                     {
                        contourproperty.contourValues[i].add(property.contourValues[i][j]);
                     }
                 }
                 else
                 {
                        contourproperty.isUpdate[i] = 0;
                 }
               }
         }
}

void svGlyph::GenerateContours()
{
    for(int j=0;j<seed_num;j++)
    {
       if(contourproperty.isUpdate[j] == 1)
       {
        for(int i=0;i<contourproperty.contourValues[j].size();i++)
        {
                GenerateContour(j, contourproperty.contourValues[j][i]);
         }
        }
     }
                                  
}

/*
void svGlyph::ResetContour(int layer)
{
	contourValues[layer].free();
}

void svGlyph::ResetContourLabel(int layer)
{
    contourLabel[layer] = false;
}
*/

/*
 * void svGlyph::AddContour(int layer, float contourValue)
{
	bool flag = true;
	for(int i=0;i<contourValues[layer].size();i++)
	{
		if(fabs(contourValues[layer][i]-contourValue)<1e-20)
		{
			flag = false;
			break;
		}
	}
	if(flag || contourValues[layer].size()==0)
	{
		contourValues[layer].add(contourValue);
		contourLabel[layer] = true;
		//ComputeContours(layer, contourValue);
	}	
}

void svGlyph::AddContour(char *input)
{
	ifstream infile(input);

	int num;
	infile>>num;
	for(int i=0;i<num;i++)
	{
		int layer;
		infile>>layer;
		
		int contourNum;
		infile>>contourNum;
		
		for(int j=0;j<contourNum;j++)
		{
			float value;
			infile>>value;
			AddContour(layer, value);
		}
	}
	
    infile.close();	
}
*/

void svGlyph::GenerateContour(int layer, float contourValue)//, char *tempContourFile)
{
		char *vtkName = new char[200];
		sprintf(vtkName, "%s/%d.vtk", vtkDir, layer);
		// = new char[200];
		//sprintf(tempContourFile, "%s/temp_contour.txt", tmpDir);
		
		svContour *contourField = new svContour(field);
		contourField->ComputeContours(vtkName, contourFile, contourValue);
		
		delete contourField;
		delete vtkName;
		
		SetContourData(layer);
}
/*
void svGlyph::ComputeContours()
{
    for(int j=0;j<seed_num;j++)
    {
       if(contourLabel[j] == true)
       {
	for(int i=0;i<contourValues[j].size();i++)
	{
		ComputeContour(j, contourValues[j][i]);
		//SetContourData(i);
	}
      }
    }
}

void svGlyph::SetClusterWeight(svScalarArray weight)
{
	clusterWeight.free();
	
	int num;
	num = weight.size();
	
	for(int i=0;i<num;i++)
	{
		clusterWeight.add(weight[i]);
	}
}

void svGlyph::SetClusters()
{
	ifstream infile(clusterFile);
    for(int i=0;i<clusterLayer.size();i++)
	{
		for(int j=0;j<glyph[clusterLayer[i]].size();j++)
		{
			if(roiLabel[clusterLayer[i]][j])
			{
		        int cluster;
		        infile>>cluster;
                clusterLabel[clusterLayer[i]][j] = cluster;			 
			}
		}
	}
	infile.close();
}
*/
void svGlyph::SetByWhole()
{
    clusterLayer.free();

    for(int i=0;i<seed_num;i++)
    {
       clusterLayer.add(i);
    }	
    SaveToFile();
}

void svGlyph::SetByLayer(int layer)
{   
   clusterLayer.free();
   clusterLayer.add(layer);
   SaveToFile();
}
void svGlyph::SetByLayer(svIntArray layerArray)
{
	cerr<<"bylayer"<<endl;
	if(clusterLayer.size()>0)
   clusterLayer.free();
   for(int i=0;i<layerArray.size();i++)
   {
       clusterLayer.add(layerArray[i]);
       cerr<<layerArray[i]<<" "<<clusterLayer.size()<<endl;
   }
   SaveToFile();
}
void svGlyph::SetByEvenLayer()
{
    clusterLayer.free();
    for(int i=0;i<seed_num;)
    {
        clusterLayer.add(i);
        i = i + 2;
    }
    SaveToFile();
}
void svGlyph::SetByOddLayer()
{
    clusterLayer.free();
    for(int i=1;i<seed_num;)
    {
        clusterLayer.add(i);
        i = i + 2;
    }
    SaveToFile();
}
void svGlyph::ComputeClusters(int numCluster)
{
	svKmeans *clusterField = new svKmeans(field);

	if(isNormalize)
	{
		clusterField->SetWeight(clusterWeight);
		clusterField->EnableNormalization();
	}
	clusterField->ComputeClusters(glyphFile, clusterFile,
					dimension, clusterDataSize ,numCluster);
					
	delete clusterField;
}
void svGlyph::ComputeClustersByWhole(int numCluster)
{
    SetByWhole();
    SaveToFile();
    ComputeClusters(numCluster);
    SetClusters();
}
void svGlyph::ComputeClustersByLayer(int numCluster)
{
    for(int i=0;i<seed_num;i++)
    {
       SetByLayer(i);
       SaveToFile();
       ComputeClusters(numCluster);
       SetClusters();
    }
}
void svGlyph::ComputeClusterByLayer(svIntArray layerArray, int numCluster)
{
	if(isWhole)
	{
          SetByLayer(layerArray);cerr<<"done"<<endl;
          SaveToFile();cerr<<"done save to file"<<endl;
          ComputeClusters(numCluster);cerr<<"done"<<endl;
          SetClusters();cerr<<"done"<<endl;
    }
    else
    {
		for(int i=0;i<layerArray.size();i++)
		{
			int index = layerArray[i];
           SetByLayer(index);
           SaveToFile();
           ComputeClusters(numCluster);
           SetClusters();			
		}
	}
}
void svGlyph::ComputeClusterByEvenLayer(int numCluster)
{
    SetByEvenLayer();
    SaveToFile();
    ComputeClusters(numCluster);
    SetClusters();
}
void svGlyph::ComputeClusterByOddLayer(int numCluster)
{
    SetByOddLayer();
    SaveToFile();
    ComputeClusters(numCluster);
    SetClusters();
}
void svGlyph::SetROI()
{
	for(int i=0;i<seed_num;i++)
	{
		for(int j=0;j<glyph[i].size();j++)
		{
			roiLabel[i][j] = true;
		}
	}	
}

void svGlyph::SetROI(svScalar mag1, svScalar mag2)
{
    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
               if(mag[i][j] > mag1 && mag[i][j] < mag2)
               {
                  roiLabel[i][j] = true;
               }
               else {
                  roiLabel[i][j] = false;
               }
         }
     }
}

void svGlyph::SetLineColorFromVec3(svVector3Array *vec3in)
{
 for(int i=0; i<seed_num; i++)
 {
     int size = vec3in[i].size();
     for(int j=0; j<size; j++) {
       svVector4 col = CalcLineColor(vec3in[i][j].getValue());
       glyphColors[i].add(col);
     }; // loop of size
 }; // loop of seed_num
}


void svGlyph::Generate(svVector3Array *vec3in)
{
/*
  glyph = new svVector3Array[seed_num];
  glyphColors = new svVector4Array[seed_num];
  for(int i=0; i<seed_num; i++) // copy vec3in
  {
    int ss= vec3in[i].size();
    for(int j=0; j<ss; j++) {
      glyph[i].add(vec3in[i][j]);
    };
  }; // loop of seed_num
  //SetLineColorFromVec3(glyph);
  SetLineColorFromVec3(vec3in);
  */
}

void svGlyph::Generate(svVector3Array *vec3in, svVector4Array *color4in)
{
/*
  glyph = new svVector3Array[seed_num];
  glyphColors = new svVector4Array[seed_num];
  for(int i=0; i<seed_num; i++) // copy vec3in
  {
    int ss= vec3in[i].size();
    for(int j=0; j<ss; j++) {
      glyph[i].add(vec3in[i][j]);
      glyphColors[i].add(color4in[i][j]);
      //color4in[i][j].print();
    };
  }; // loop of seed_num
  */
}

svGlyph::~svGlyph()
{
  clean();
  cleanDisplayList(SV_IMMEDIATE);

  delete [] tmpDir;
  delete [] rawDir;
  delete [] vtkDir;
  delete [] contourFile;
  delete [] clusterFile;
  delete [] glyphFile;
}

void svGlyph::clean()
{
  if (glyph != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyph[i].free();
    delete [] glyph;
    glyph=NULL;
  };
  if (dir!=NULL) {
    for(int i=0; i<seed_num; i++)
      dir[i].free();
    delete [] dir;
    dir=NULL;
  };
  if (mag!=NULL) {
    for(int i=0; i<seed_num; i++)
      mag[i].free();
    delete [] mag;
    mag=NULL;
  };

  if (clusterLabel!=NULL) {
    for(int i=0; i<seed_num; i++)
      clusterLabel[i].free();
    delete [] clusterLabel;
    clusterLabel=NULL;
  };

  if (roiLabel!=NULL) {
    for(int i=0; i<seed_num; i++)
      roiLabel[i].free();
    delete [] roiLabel;
    roiLabel=NULL;
  };

  if (visibleLabel!=NULL) {
    for(int i=0; i<seed_num; i++)
      visibleLabel[i].free();
    delete [] visibleLabel;
    visibleLabel=NULL;
  };

  delete [] contourLabel;
  
  if (contourValues!=NULL) {
    for(int i=0; i<seed_num; i++)
      contourValues[i].free();
    delete [] contourValues;
    contourValues=NULL;
  };

  clusterLayer.free();


/*
  if (glyphWidth != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphWidth [i].free();
    delete [] glyphWidth;
    glyphWidth=NULL;
  }*/
  if (glyphColors != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphColors[i].free();
    delete [] glyphColors;
    glyphColors=NULL;
  };

  if(contourproperty.contourValues!=NULL)
  {
       for(int i=0;i<contourproperty.seed_num;i++)
      { 
          contourproperty.contourValues[i].free();
      }
      delete [] contourproperty.contourValues;
      contourproperty.contourValues = NULL;
  }

          contourproperty.isUpdate;

  kmeansproperty.clusterLayer.free();
  kmeansproperty.clusterWeight.free();
}


//TODO
void svGlyph::RemovePoint(svInt seedIndex)
{
}


void svGlyph::EnableLineWidth(svFloat minlw, svFloat maxlw)
{
   // clean up
   if(glyphWidth!=NULL) 
   {
     for(int i=0; i<seed_num; i++)
       glyphWidth[i].free();
     delete [] glyphWidth;
     glyphWidth = NULL;
   };
  
  lut.SetLineWidthRange(minlw,maxlw);
  lut.SetScalarRange(0.0007, 1.5);

  clean();
  cleanDisplayList(SV_IMMEDIATE);

  // regenerate glyphs
  ENABLE_LINEWIDTH = true;
}


void svGlyph::EnableColor()
{
  EnableColor(SV_LOCS,0.007,0.15);
}

void svGlyph::EnableColor(svInt colorModel, svFloat minValue, svFloat maxValue)
{
  // set glyph color
  if(glyphColors!=NULL) 
  {
     for(int i=0; i<seed_num; i++)
       glyphColors[i].free();
     delete [] glyphColors;
     glyphColors= NULL;
  };

  //lut.SetColorModel(colorModel, 0.0007, 1.5);
  lut.SetColorModel(colorModel, minValue, maxValue);

  clean();
  cleanDisplayList(SV_IMMEDIATE);

  ENABLE_COLOR = true;
}


void svGlyph::DisableColor()
{
  for(int i=0;i<seed_num;i++)
  {
	 // glyphColors[i].free();
	  for(int j=0;j<glyph[i].size();j++)
	  {
		  svVector4 color;
		  color[0] = 1;
		  color[1] = 1;
		  color[2] = 1;
		  color[3] = 1;
		  
		  glyphColors[i][j] = color;
	  }
  }

  ENABLE_COLOR = false;
}

void svGlyph::SetColorByCluster()
{
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
              int c = clusterLabel[i][j];
             glyphColors[i][j] = color->GetDiscreteColors(c);
         }
     }
     delete color;
}

void svGlyph::Generate()
{
  //BuildDisplayListFromStore();
}

void svGlyph::Render()
{
  //glColor3fv(render_property.color.getValue());
  if(display_mode == SV_IMMEDIATE) {
    Generate();
  }
  else if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    glCallList(display_list);
  }
  else if (display_mode == SV_STORE) 
  {
    //BuildDisplayListFromStore();
    /*
    if(ENABLE_COLOR==true || ENABLE_LINEWIDTH==true)
      RenderStoreDirect();
    else
      RenderStoreDirect();
      */
      //RenderStore();
  }
}

void svGlyph::RenderStoreDirect() const
{
}


void svGlyph::RenderStore() const
{
}

void svGlyph::SetKmeansProperty()
{
      kmeansproperty.isNormalize = true;
      kmeansproperty.dimension = 7;
      kmeansproperty.numCluster = 1;
      kmeansproperty.isWhole = false;
      for(int i=0;i<seed_num;i++)
      {
            kmeansproperty.clusterLayer.add(0);
      }
      for(int i = 0;i<kmeansproperty.dimension;i++)
      {
             kmeansproperty.clusterWeight.add(1);
      }
}

void svGlyph::SetContourProperty()
{
       contourproperty.seed_num = seed_num;
       contourproperty.contourValues = NULL;
//       contourproperty.isUpdate = NULL;
}

}
