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
svGlyph::svGlyph()
{
      //SetKmeansProperty();
      //SetContourProperty();

  glyphScale = DEFAULT_GLYPH_SCALE;
  glyphRadius = DEFAULT_GLYPH_RADIUS;
  display_list = DEFAULT_DISPLAYLIST;

      glyph=NULL;
      dir = NULL;
      mag = NULL;
     
      clusterLabel = NULL;
      roiLabel = NULL;
      visibleLabel = NULL;

      glyphColors = NULL;
      glyphWidth = NULL;

      glyphSize = 0;
}
/*
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
*/


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

void svGlyph::New(svVectorField* f, int numPlane)
//svChar *indir, svChar *inf1, svChar *inf2,          
{
      clean();

      seed_num = numPlane;

      field = f;

      //fileDir = strdup(indir);
      //char *str1 = strdup(inf1);
      //char *str2 = strdup(inf2);
      //fileName1 = new char[400];
      //sprintf(fileName1, "%s/%s", indir, str1);
      //fileName2 = new char[400];
      //sprintf(fileName2, "%s/%s", indir, str2);     

     //cerr<<fileDir<<" "<<fileName1<<" "<<fileName2<<endl;

     glyph = new svVector3Array[seed_num];
     dir = new svVector3Array[seed_num];
     mag = new svScalarArray[seed_num];
 
     clusterLabel = new svIntArray[seed_num];
     roiLabel = new svIntArray[seed_num];
     visibleLabel = new svIntArray[seed_num];
     
     glyphColors = new svVector4Array[seed_num];
     glyphWidth = new svScalarArray[seed_num];

     maxClusterLabel = 0;
     glyphSize = 0;

     //SetKmeansProperty();
     //SetContourProperty();
}

void svGlyph::SetData(char *infName, int seed)
{
//cerr<<infName<<endl;
        int layer = seed;
        ifstream infile(infName);
        int num;
        infile>>num;
        for(int i=0;i<num;i++)
        {//` cerr<<i<<" "<<num<<endl;
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

        //glyphSize = glyphSize + glyph[layer].size();
  //cerr<<"setdata"<<endl; 
        infile.close();

       // DisableColor();
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
// glyphSize = glyphSize - glyph[layer].size();

	glyph[layer].free();
	dir[layer].free();
	mag[layer].free();
	
	clusterLabel[layer].free();
	roiLabel[layer].free();
	visibleLabel[layer].free();
	
	glyphColors[layer].free();
}

void svGlyph::ResetCluster()
{
        maxClusterLabel = 0;
	for(int i=0;i<seed_num;i++)
	{
		for(int j=0;j<glyph[i].size();j++)
		{
			clusterLabel[i][j] = -1;
		}
	}
}

/*
void svGlyph::SetContourProperty(ContourProperty & property)
{
         if(contourproperty.contourValues == NULL)
         {
             contourproperty.seed_num = property.seed_num;
            // cerr<<property.seed_num<<endl;
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
*/

void svGlyph::GenerateContours(ContourProperty &property) //const
{
    for(int j=0;j<seed_num;j++)
    {
       if(property.isUpdate[j] == 1)
       {
        for(int i=0;i<property.contourValues[j].size();i++)
        {
          //   cerr<<i<<" "<<j<<" "<<property.contourValues[j][i]<<" "<<property.vtkdir<<endl;
             GenerateContour(property.outputfile, property.vtkdir,
				              j, property.contourValues[j][i]);
         }
        }
     }
                                  
}

/*
void svGlyph::SetKmeansProperty(KmeansProperty & property)
{
    kmeansproperty.isNormalize = property.isNormalize;
    kmeansproperty.dimension = property.dimension;
    kmeansproperty.numCluster = property.numCluster;
    int num=property.clusterLayer.size();
    kmeansproperty.clusterLayer.free();
    for(int i=0;i<num;i++)
    {
        kmeansproperty.clusterLayer.add(property.clusterLayer[i]);
       // cerr<<kmeansproperty.clusterLayer[0]<<endl;
    }
    kmeansproperty.clusterWeight.free();
    for(int i=0;i<property.dimension;i++)
    {
        kmeansproperty.clusterWeight.add(property.clusterWeight[i]);
   }
}
*/
void svGlyph::GenerateClusters(svIntArray *cluster) 
{
     maxClusterLabel = 0;
     for(int i=0;i<seed_num;i++)
     {
         for(int j=0;j<glyph[i].size();j++)
         {
             clusterLabel[i][j] = cluster[i][j];
             if(maxClusterLabel < cluster[i][j])
             {
                maxClusterLabel = cluster[i][j]; 
             }             
         }
     }
}
void svGlyph::GenerateClusters(KmeansProperty & property)// const
{
      int max = -1;
      for(int i=0;i<property.clusterLayer.size();i++)
      {
         if(property.clusterLayer[i] > max) 
                     max = property.clusterLayer[i];
      }
      for(int i=0;i<=max;i++)
      {
       //  maxClusterLabel = maxClusterLabel + property.numCluster;
         SaveToFile(property.file1, property.clusterLayer,
		            property.dimension, i);
         GenerateCluster(property.file1, property.file2,
		                 property.isNormalize, 
						 property.dimension,
						 property.clusterWeight,
						 property.numCluster);
         SetClusterLabel(property.file1, property.clusterLayer,i);
      }
      /*for(int i=0;i<seed_num;i++)
      {
         if(property.clusterLayer[i] < 0 )
         {
              int s = glyph[i].size();
              for(int j=0;j<s;j++)
              {
                  clusterLabel[i][j] = -1;
              }
         }
      }*/
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

}

void svGlyph::clean()
{
  glyphSize = 0;

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

  


 //delete [] fileDir;
 //delete [] fileName1;
 //delete [] fileName2;
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
/*
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
  kmeansproperty.clusterWeight.free();*/
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
/*
void svGlyph::SetKmeansProperty()
{
      kmeansproperty.isNormalize = true;
      kmeansproperty.dimension = 7;
      kmeansproperty.numCluster = 1;
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
*/

void svGlyph::SaveToFile(char *outputfile, svIntArray clusterLayer, 
                     int dimension, int clusterIndex)
{
  glyphSize = 0;

  ofstream outf(outputfile);
  
  for(int i=0;i<seed_num;i++)
  {
      if(clusterLayer[i] == clusterIndex)
      {
          //cerr<<clusterIndex<<" "<<i<<endl;
          int layer = i;
          int s = glyph[layer].size();
          for(int j=0; j<s; j++)
          {
                if(roiLabel[layer][j])
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
		//	cerr<<glyph[layer][j][0]<<endl;
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
                    glyphSize++;
                }
          }
    }
  }

  outf.close();
}
void svGlyph::GenerateContour(char *contourfile, char *vtkdir, int layer, float contourValue)
{
                char *vtkName = new char[200];
                sprintf(vtkName, "%s/%d.vtk", vtkdir, layer);

               // cerr<<vtkName<<" "<<contourValue<<endl;
                svContour *contourField = new svContour(field);
                contourField->ComputeContours(vtkName, contourfile, contourValue);

                delete contourField;
                delete [] vtkName;
//cerr<<"setdata"<<endl;
                SetData(contourfile, layer);
}

void svGlyph::GenerateCluster(char *clusterfile, char *immefile, bool isNormalize,
                                 int dimension, svScalarArray clusterWeight,
								  int numCluster)
{
     ifstream pFile(clusterfile);
     if(pFile.peek() == std::ifstream::traits_type::eof())
     {
       pFile.close();
     }
     else
     {
        pFile.close();
        svKmeans *clusterField = new svKmeans();

        if(isNormalize)
        {
            clusterField->Normalization(clusterfile, immefile,dimension,
                                          glyphSize, clusterWeight);
            clusterField->ComputeClusters(immefile, clusterfile,dimension,
                                          glyphSize, numCluster);
        }
       else
        {
            std::ifstream src(clusterfile, std::ios::binary);
            std::ofstream dst(immefile, std::ios::binary);
            dst << src.rdbuf();
            clusterField->ComputeClusters(immefile, clusterfile,dimension,
                                          glyphSize, numCluster);
        }

        delete clusterField;
     }
}

void svGlyph::SetClusterLabel(char *clusterfile, svIntArray clusterLayer, int clusterIndex)
{
        int max = maxClusterLabel;
        ifstream infile(clusterfile);
        for(int i=0;i<seed_num;i++)
        {
             if(clusterLayer[i] == clusterIndex)
             {
                int s = glyph[i].size();
                for(int j=0;j<s;j++)
                {
                    if(roiLabel[i][j])
                    {
                         int cluster;
                         infile>>cluster;
                         clusterLabel[i][j] = cluster+maxClusterLabel;
                         if(clusterLabel[i][j] > max) max = clusterLabel[i][j];
                    }
                   /* else
                    {
                         clusterLabel[i][j] = -1;
                    }*/
                }
             }
        }
        infile.close();
        maxClusterLabel = max;
}


}
