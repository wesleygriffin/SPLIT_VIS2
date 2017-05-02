/*  svGlyph
 */
#include "svGlyph.h"
#include <algorithm>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string.h>
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
  seed_num=0;
  glyphScale = DEFAULT_GLYPH_SCALE;
  glyphRadius = DEFAULT_GLYPH_RADIUS;
  display_list = DEFAULT_DISPLAYLIST;

  dataSize = 0;

      glyph=NULL;
	dir = NULL;
      mag = NULL;
      exp = NULL;
      coe = NULL;

     glyphFormat = NULL;

      contourLabel = NULL;
      clusterLabel = NULL;
      roiLabel = NULL;
      visibleLabel = NULL;

      glyphColors = NULL;
      glyphWidth = NULL;

      glyphSize = 0;
 
      symmetrylist[0] = new svList(); 
      symmetrylist[1] = new svList();
      symmetrylist[2] = new svList();
      symmetrylist[3] = new svList();
      symmetrylist[4] = new svList();
      symmetrylist[5] = new svList();
  // peeling = new svPeeling();

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
     if(seed_num > 0)
      clean();

      dataSize =0 ;

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
     exp = new svScalarArray[seed_num]; 
     coe = new svScalarArray[seed_num];


     glyphFormat = new svIntArray[seed_num];

     contourLabel = new svIntArray[seed_num]; 
     clusterLabel = new svIntArray[seed_num];
     roiLabel = new svIntArray[seed_num];
     visibleLabel = new svIntArray[seed_num];

     glyphColors = new svVector4Array[seed_num];
     glyphWidth = new svScalarArray[seed_num];

     maxClusterLabel = -1;
     glyphSize = 0;

     //SetKmeansProperty();
     //SetContourProperty();
}

void svGlyph::SetBBox()
{
  lbbox=svVector3(9e+5,9e+5,9e+5);
  rbbox=svVector3(-9e+5,-9e+5,-9e+5);

  for(int i=0;i<seed_num;i++)
  {
    
       for(int j=0;j<glyph[i].size();j++)
      {//cerr<<j<<" "<<i<<endl;
           if(glyph[i][j][0] > rbbox[0]) rbbox[0] = glyph[i][j][0];
           if(glyph[i][j][1] > rbbox[1]) rbbox[1] = glyph[i][j][1];
           if(glyph[i][j][2] > rbbox[2]) rbbox[2] = glyph[i][j][2];
           if(glyph[i][j][0] < lbbox[0]) lbbox[0] = glyph[i][j][0];
           if(glyph[i][j][1] < lbbox[1]) lbbox[1] = glyph[i][j][1];
           if(glyph[i][j][2] < lbbox[2]) lbbox[2] = glyph[i][j][2];
      }
  }

}
void svGlyph::SetData(char *infName, int seed)
{
//cerr<<infName<<endl;
        int layer = seed;
        ifstream infile(infName);
        int num;
        infile>>num;
        for(int i=0;i<num;i++)
        {// cerr<<i<<" "<<num<<endl;
                svVector3 pos, vec;
                svScalar den;

                infile>>pos[0]>>pos[1]>>pos[2]
                                >>vec[0]>>vec[1]>>vec[2]
                                >>den;
                vec.normalize();
                glyph[layer].add(pos);
                dir[layer].add(vec);
                mag[layer].add(den);

                svScalar tmpexp = getNumOfIntegerDigits(den);
                svScalar tmpcoe;
                tmpcoe = den/pow(10.,tmpexp);
                exp[layer].add(tmpexp);
                coe[layer].add(tmpcoe);

                clusterLabel[layer].add(-1);
                roiLabel[layer].add(true);
                visibleLabel[layer].add(true);

               svVector4 color;
               color[0]=1;color[1]=1;color[2]=1;color[3]=1;
               glyphColors[layer].add(color);
               dataSize ++;
        }

        //glyphSize = glyphSize + glyph[layer].size();
  //cerr<<"setdata"<<endl; 
        infile.close();
/*
        svSymmetry *symmetry = new svSymmetry(field);
        svVector3 pdir1, pdir2, ppos1, ppos2;
        pdir1[0]=1;pdir1[1]=1;pdir1[2]=0;
        pdir2[0]=-1;pdir2[1]=1;pdir2[2]=0;
        ppos1[0]=0;ppos1[1]=0;ppos1[2]=0;
        ppos2[0]=0;ppos2[1]=0;ppos2[2]=0;
        SymmetryProperty pro;
        pro.dir.add(normalize(pdir1));pro.dir.add(normalize(pdir2));
        pro.pos.add(ppos1);pro.pos.add(ppos2);
        pro.datafile=strdup(infName);
        pro.outputfile=strdup("tmpfile");
        symmetry->ComputeSymmetry(pro);
        delete symmetry;
*/
       // DisableColor();
}

void svGlyph::SetFormat(char *infname, int seed)
{
     ifstream infile (infname);

     int n;infile>>n;

     for(int i=0;i<n;i++)
     {
           int a,b;
           infile>>a>>b;
           glyphFormat[seed].add(a);
           glyphFormat[seed].add(b);
     }

     infile.close();
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
        coe[layer].free();
        exp[layer].free();	

	clusterLabel[layer].free();
	roiLabel[layer].free();
	visibleLabel[layer].free();

	glyphColors[layer].free();
}

void svGlyph::ResetCluster()
{
        maxClusterLabel = -1;
	for(int i=0;i<seed_num;i++)
	{
		for(int j=0;j<glyph[i].size();j++)
		{
			clusterLabel[i][j] = -1;
                        roiLabel[i][j] = true;
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

    contourList.free();
//    omp_set_num_threads(10);
    for(int i=0;i<property.contourValues[0].size();i++)
             contourList.add(0);
    dataSize = 0;

    #pragma omp parallel for
    for(int j=0;j<seed_num;j++)
    {
       if(property.isUpdate[j] == 1)
       {
        for(int i=0;i<property.contourValues[j].size();i++)
        {
//             cerr<<i<<" "<<j<<" "<<property.contourValues[j][i]<<" "<<property.vtkdir<<endl;
                char *contourfile = new char[200];
                sprintf(contourfile,"%s/contour%d%d.txt", property.vtkdir,j,i);
            int index = glyph[j].size();
           GenerateContour(contourfile, property.vtkdir,
				              j, property.contourValues[j][i]);
//property.outputfile,
             SetContourLabel(index,j, i);
                delete [] contourfile;
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

//sampling each cluster
void svGlyph::SetSampleData(int size)
{
   int count[maxClusterLabel+1];
   for(int i=0;i<maxClusterLabel+1;i++)
        count[i] = 0;

   sampleLabel.free();
   for(int i=0;i<seed_num;i++)
   {
         for(int j=0;j<glyph[i].size();j++)
         {
             if(visibleLabel[i][j])
             {
              int c = clusterLabel[i][j];
              if(count[c] < size)
              {
                    sampleLabel.add(i);
                    sampleLabel.add(j);
                    count[c]++;
              }
             }              
         }
   }
}

void svGlyph::GenerateClusters(svChar *inf)
{
     ifstream infile(inf);
   
     maxClusterLabel = -1;
     for(int i=0;i<seed_num;i++)
     {
         for(int j=0;j<glyph[i].size();j++)
         { 
             int cluster;
             infile>>cluster;
             clusterLabel[i][j] = cluster;
             if(maxClusterLabel < cluster)
             {
                maxClusterLabel = cluster;
             }
         }
     }

    infile.close();
}

void svGlyph::GenerateClusters(svIntArray *cluster) 
{
     maxClusterLabel = -1;
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
cerr<<maxClusterLabel<<endl;
}
void svGlyph::GenerateClustersBySymmetry(KmeansProperty & property, 
                      SymmetryProperty & sproperty,
                      vector<int> symmetrytype)// const
{
      int max = -1;
      for(int i=0;i<property.clusterLayer.size();i++)
      {
         if(property.clusterLayer[i] > max)
                     max = property.clusterLayer[i];
      }

   if(symmetrytype.size()>0)
   {
    int count = 0;
    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
            if(roiLabel[i][j] == true)
            {
              bool flag;
              if(dot(glyph[i][j]-sproperty.pos, 
                          dir[i][j])>=0) flag = true;
              else flag = false;
              for(int t=0;t<symmetrytype.size();t++)
              {
                   int size = symmetrylist[symmetrytype[t]]->getSize(count);
                   if(size == 1) continue;
                   else
                   {
                      int data1[size];
                      int data2[size];

                      symmetrylist[symmetrytype[t]]->getData(count, 
                                        data1, 
                                        data2);
                     roiLabel[data1[1]][data2[1]]=1-flag;
                     roiLabel[data1[0]][data2[0]]=flag;
                   }
              }
             }
            count++;
         }
    }cerr<<"symmetrytype"<<endl;
   }

      for(int i=0;i<=max;i++)
      {
         SaveToFile(property.file1, property.clusterLayer,
                            property.dimension, i);
         GenerateCluster(property.file1, property.file2,
                                 property.isNormalize,
                                                 property.dimension,
                                                 property.clusterWeight,
                                                 property.numCluster);
         SetClusterLabelBySymmetry(property.file1, property.clusterLayer,i,symmetrytype);
      }
}
void svGlyph::SetSampling(svInt frequency)
{
   int count=0;
     for(int i=0;i<seed_num;i++)
   {
       if(glyph[i].size()>0)
       {
        for(int j=0;j<glyph[i].size();j++)
        {
           if(count%frequency == 0 && visibleLabel[i][j])
                 visibleLabel[i][j] = 1;
           else
                 visibleLabel[i][j] = 0;
           count++;
        }

       }
   }
}

void svGlyph::SetSampling(vector<int> symmetrytype, svInt frequency)
{
    for(int i=0;i<seed_num;i++)
      for(int j=0;j<glyph[i].size();j++)
         visibleLabel[i][j] = false;

    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
                   int x = glyphFormat[i][j*2];
                   int y = glyphFormat[i][j*2+1];
                   if(x%frequency ==0 && y%frequency ==0 && i%frequency==0)
                   {
                       visibleLabel[i][j] = true;
                   }
         }
    }

   if(symmetrytype.size()>0)
   {
    int count = 0;
    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
            if(visibleLabel[i][j] == true)
            {
              for(int t=0;t<symmetrytype.size();t++)
              {
                   int size = symmetrylist[symmetrytype[t]]->getSize(count);
                   int data1[size];
                   int data2[size];
 
                   symmetrylist[symmetrytype[t]]->getData(count, data1, data2);

                    for(int m=0;m<size;m++)
                    {
                          visibleLabel[data1[m]][data2[m]] = true;
                    }
              }
             }
            count++;
         }
    }
   }
}

void svGlyph::GenerateSymmetry(SymmetryProperty &property)
{
   svSymmetry *symmetry = new svSymmetry(field);

//   for(int i=0;i<4;i++)
//   {
 /*       cerr<<i<<"symmetry"<<endl;
        char *symmetrystr = new char[200];
        for(int j=0;j<200;j++) symmetrystr[j] = '\0';
        for(int j=0;j<property.dir.size();j++)
        {
            sprintf(symmetrystr, "%s(%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f)", symmetrystr,
                  property.pos[j][0], property.pos[j][1], property.pos[j][2],
                  property.dir[j][0], property.dir[j][1], property.dir[j][2]);
        }
        switch(i)
        {
          case 0:  sprintf(property.outputfile,"%s/antisymmetry%s.txt", 
                   property.datafile, symmetrystr);
                   break;
          case 1:  sprintf(property.outputfile,"%s/positivesymmetry%s.txt",
                   property.datafile, symmetrystr);
                   break;
          case 2:  sprintf(property.outputfile,"%s/negativenearsymmetry%s.txt",
                   property.datafile, symmetrystr);
                   break;
          case 3:  sprintf(property.outputfile,"%s/negativefarsymmetry%s.txt",
                   property.datafile, symmetrystr);
                   break;
          //cerr<<property.outputfile<<endl;
        }
        delete [] symmetrystr;
*/
        sprintf(property.outputdir, "%s", property.datafile);
/*
        ifstream infile(property.outputfile);
        if(!infile.is_open())
        {
               infile.close();
              char *str = new char[200];
               sprintf(property.inputfile, "%s/layerall.txt",property.datafile);
              // porropertt.inputfile = strdup(str);
             delete [] str;

             switch(i)
             {
               case 0:
                      symmetry->ComputeAntiSymmetry(property);break;
               case 1:
                      symmetry->ComputePositiveSymmetry(property);break;
               case 2:
                      symmetry->ComputeNegativeNearSymmetry(property);break;
               case 3:
                      symmetry->ComputeNegativeFarSymmetry(property);break;
             }
        }
        else 
           infile.close();
*/
         sprintf(property.inputfile, "%s/layerall.txt",property.datafile);
        symmetry->ComputeSymmetry(property);
//=================save to list=======================

  
        for(int i=0;i<SYMMETRY_TYPE;i++)
        {
          symmetrylist[i]->destroy();
          symmetrylist[i]->new_list(dataSize);

          char *symmetrystr = new char[200];
          for(int j=0;j<200;j++) symmetrystr[j] = '\0';
          for(int j=0;j<property.dir.size();j++)
          {
            sprintf(symmetrystr, "%s(%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f)", symmetrystr,
                  property.pos[0], property.pos[1], property.pos[2],
                  property.dir[0], property.dir[1], property.dir[2]);

           }

           char *str = new char[400];
           switch(i)
           {
               case 0:  sprintf(str,"%s/xsym%s.txt",
                     property.outputdir, symmetrystr);break;
               case 1:  sprintf(str,"%s/xanti%s.txt",
                     property.outputdir, symmetrystr);break;
               case 2:  sprintf(str,"%s/ysym%s.txt",
                     property.outputdir, symmetrystr);break;
               case 3:  sprintf(str,"%s/yanti%s.txt",
                     property.outputdir, symmetrystr);break;
               case 4:  sprintf(str,"%s/zsym%s.txt",
                     property.outputdir, symmetrystr);break;
               case 5:  sprintf(str,"%s/zanti%s.txt",
                     property.outputdir, symmetrystr);break;
            }

            ifstream infile(str);
            int n;
            infile>>n;
        
             int count = 0;
             for(int ii=0;ii<seed_num;ii++)
             {
                 for(int j=0;j<glyph[ii].size();j++)
                {
                int m;
                 infile>>m;
                 vector<int> value; value.resize(m*2);
                 for(int t=0;t<m*2;t++)
                 {
                    infile>>value[t];
                 }     
             
                 symmetrylist[i]->add_next(count, value);
                 count++;
                 }   
             }
             infile.close();
             delete [] symmetrystr;
             delete [] str;
       }
//        symmetrylist[i]->display_list();
  // }

   delete symmetry;
}

void svGlyph::ResetVisible()
{
    for(int i=0;i<seed_num;i++)
    {
          for(int j=0;j<glyph[i].size();j++)
          {
                 visibleLabel[i][j] = true;
          }
    }
}

void svGlyph::SetSymmetryVisible(vector<int> type)
{
//    cerr<<"type "<<type.size()<<endl;

if(type.size()>0)
{
    int count = 0;
    for(int i=0;i<seed_num;i++)
    {
          for(int j=0;j<glyph[i].size();j++)
          {
             bool flag = false;
             for(int t=0;t<type.size();t++)
             {
               int size = symmetrylist[type[t]]->getSize(count);
               if(size>1 && visibleLabel[i][j])
               {
                     visibleLabel[i][j] = true;
                     flag = true; break;
               }
              /* else
               {
                     visibleLabel[i][j] = false;
               }*/
             }
              if(!flag) visibleLabel[i][j] = false;
         //     if(visibleLabel[i][j])cerr<<i<<" "<<j<<endl;
               count++;         
          }
    }
}
}
void svGlyph::SetVisible(int contour)
{
    for(int i=0;i<seed_num;i++)
    {
          for(int j=0;j<glyph[i].size();j++)
          {
                 if(visibleLabel[i][j] && contour == contourLabel[i][j])
                 {
                      visibleLabel[i][j] = true;//cerr<<contourLabel[i][j]<<" ";
                 }
                 else
                 {
                      visibleLabel[i][j] = false;
                 }
          }
    }
}

void svGlyph::SetVisible(svScalar z1, svScalar z2)
{
        for(int i =0;i<seed_num;i++)
        {
                for(int j=0;j<glyph[i].size();j++)
                {
                        if(visibleLabel[i][j] && i>=z1 && i<=z2)
                        {
                                visibleLabel[i][j] = true;
                        }
                        else
                        {
                                visibleLabel[i][j] = false;
                        }
                }
        }
}

void svGlyph::SetVisible(int contour, svScalar z1, svScalar z2, int frequency)
{
        for(int i =0;i<seed_num;i++)
        {
             if(i>=z1 && i<=z2)
             {
                for(int j=0;j<glyph[i].size();j++)
                {
                        if( j%frequency==0 && contour == contourLabel[i][j])
                        {
                                visibleLabel[i][j] = true;
                        }
                        else
                        {
                                visibleLabel[i][j] = false;
                        }
                }
            }
            else
             {
                for(int j=0;j<glyph[i].size();j++)
                {
                       visibleLabel[i][j] = false;
                }
            }

        }
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
               if(mag[i][j] >= mag1 && mag[i][j] <= mag2)
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
  if (coe!=NULL) {
    for(int i=0; i<seed_num; i++)
     coe[i].free();
    delete [] coe;
    coe=NULL;
  };
  if (exp!=NULL) {
    for(int i=0; i<seed_num; i++)
     exp[i].free();
    delete [] exp;
    exp=NULL;
  };
  if (glyphFormat!=NULL) {
    for(int i=0; i<seed_num; i++)
      glyphFormat[i].free();
    delete [] glyphFormat;
    glyphFormat=NULL;
  };
  if (clusterLabel!=NULL) {
    for(int i=0; i<seed_num; i++)
      clusterLabel[i].free();
    delete [] clusterLabel;
    clusterLabel=NULL;
  };
  clusterLabelbymag.free();

  if (contourLabel!=NULL) {
    for(int i=0; i<seed_num; i++)
      contourLabel[i].free();
    delete [] contourLabel;
    contourLabel=NULL;
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


       sampleLabel.free();
  


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

void svGlyph::SetColor(svVector4 color)
{
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
             glyphColors[i][j] = color;
         }
     }

}

void svGlyph::SetColor(int index1, int index2, svVector4 color1, svVector4 color2)
{
     SetColor(color1);
     for(int i=0;i<seed_num;i++)
     {
             glyphColors[i][index1] = color2;
             glyphColors[i][index2] = color2;

     }

}

void svGlyph::SetColorByCluster(svIntArray index, svVector4 c)
{
     SetColor(c);
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
              int c = clusterLabel[i][j];
              bool flag = false;
              for(int t=0;t<index.size();t++)
              {
                  if(c == index[t])
                  {
                     flag = true;
                     break;
                  }
              }             
            if(flag)
               glyphColors[i][j] = color->GetDiscreteColors(c);
         }
     }
     delete color;
}
void svGlyph::SetColorByPower()
{
     svColors *color = new svColors();
     svScalarArray value;

     for(int i=numPower-1;i>=0;i--)
     {
          value.add((svScalar)i);
     }
//cerr<<value.size()<<endl;
     svVector4Array cc = color->GetContinuousColors(value);
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
            // if(exp[i][j]>0)
            // {
                  svInt e = numPower-(exp[i][j]+scaling)-1; 
                if(e>=0)
                  glyphColors[i][j] = cc[e]; 
                else
                 {
                       glyphColors[i][j][0]=1;
                       glyphColors[i][j][1]=1;
                       glyphColors[i][j][2]=1;
                 }
                 glyphColors[i][j][3] = 0.5;
           // }
         }
     }
     value.free();
     delete color;
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
void svGlyph::SetColorByCluster(svIntArray cluster)
{
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
              bool flag = false;
              int c = clusterLabel[i][j];
              for(int t=0;t<cluster.size();t++)
              {
                   if(c == cluster[t])
                   {
                           flag = true;break;
                   }
              }
             if(flag)
             glyphColors[i][j] = color->GetDiscreteColors(c);
         }
     }
     delete color;
}

void svGlyph::SetColorByClusterMag()
{
     svScalarArray clustermag;
     int count[maxClusterLabel+1];
     for(int i=0;i<maxClusterLabel+1;i++)
     {
         count[i] = 0;
         clustermag.add(0);
     }
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
             int c=clusterLabel[i][j];
             if(c>=0)
             {
                clustermag[c] = clustermag[c]+mag[i][j];
                count[c]++;
             }
         }
     }

    for(int i=0;i<maxClusterLabel+1;i++)
    {
        if(count[i]>0)
           clustermag[i] = clustermag[i]/count[i];
    }

    vector<svScalar> cmag;
    for(int i=0;i<maxClusterLabel+1;i++)
    {
          cmag.push_back(clustermag[i]);
    }

    svIntArray order;
    sort(cmag.begin(), cmag.end());
    for(int i=0;i<maxClusterLabel+1;i++)
    {
         double td = 9e+9;double dd;
         int in=-1;
         for(int j=0;j<maxClusterLabel+1;j++)
         {
               dd = fabs(cmag[j] - clustermag[i]);
               if(dd < td){
                   bool flag = false;
                   for(int t=0;t<order.size();t++)
                   {
                        if(order[t] == maxClusterLabel-j){flag = true; break;} 
                   } 
                   if(!flag){td = dd; in = j;}
               }
         }
         order.add(maxClusterLabel-in);
//         cerr<<clustermag[i]<<" "<<maxClusterLabel-in<<endl;
    }

     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
         {
              int c = clusterLabel[i][j];
             if(c==-1)
                 glyphColors[i][j] = color->GetDiscreteColors(c);
             else
             {
                  clusterLabel[i][j] = order[c];
                  glyphColors[i][j] = color->GetDiscreteColors(order[c]);
             }
         }
     }
     
     order.free();
     delete color; 
}
void svGlyph::Generate()
{
  //BuildDisplayListFromStore();
}

void svGlyph::DrawGrid(svVector3 startPos, svVector3 dir1, svVector3 dir2,
                       svScalar stepD1, svScalar stepD2,
                       svInt stepN1, svInt stepN2)
{
     glDisable(GL_LIGHTING);
     glDisable(GL_LIGHT0);
       
     svVector3 pos = startPos;
     for(int i=0;i<=stepN1;i++)
     {
         glBegin(GL_LINES);
         glVertex3f(pos[0],pos[1],pos[2]);
         svVector3 end = pos + stepN2*stepD2*dir2;
         glVertex3f(end[0], end[1], end[2]);
         glEnd();
         pos =  pos + stepD1 * dir1;
     }

     pos = startPos;
     for(int i=0;i<=stepN2;i++)
     {
         glBegin(GL_LINES);
         glVertex3f(pos[0],pos[1],pos[2]);
         svVector3 end = pos + stepN1*stepD1*dir1;
         glVertex3f(end[0], end[1], end[2]);
         glEnd();
         pos =  pos + stepD2 * dir2;
     }

}

void svGlyph::DrawSilkPlane(svVector3 planeDir)
{
  glLineWidth(1.0);
 // if(field!=NULL)
  //     field->GetBoundingBox(&lbbox,&rbbox);

   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1.0, 1.0); 
    for(int i=0;i<seed_num;i++)
    {
        if(glyph[i].size()>0)
        {
             glColor4f(render_property.color[0],
                    render_property.color[1],
                    render_property.color[2],
                  render_property.color[3]);
             glBegin(GL_QUADS);
             if(planeDir[2] == 1.)
             {
                 glVertex3f(lbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],rbbox[1],glyph[i][0][2]);
                 glVertex3f(lbbox[0],rbbox[1],glyph[i][0][2]);
             }     
             glEnd();
             glColor4f(1-render_property.color[0],
                    1-render_property.color[1],
                    1-render_property.color[2],
                  render_property.color[3]);
             glBegin(GL_LINE_LOOP);
             if(planeDir[2] == 1.)
             {
                 glVertex3f(lbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],rbbox[1],glyph[i][0][2]);
                 glVertex3f(lbbox[0],rbbox[1],glyph[i][0][2]);
             }
             glEnd();
        }
    }
   glDisable(GL_POLYGON_OFFSET_FILL);

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

void svGlyph::SetContourLabel()
{
   for(int i=0;i<seed_num;i++)
   {
       for(int j=0;j<glyph[i].size();j++)
       {
           contourLabel[i].add(0);
       }
   }
}

void svGlyph::SetContourLabel(int index, int layer, int in)
{
   for(int i=index;i<glyph[layer].size();i++)
   {
	contourLabel[layer].add(in);
   }

}


void svGlyph::GenerateContour(char *contourfile, char *vtkdir, int layer, float contourValue)
{
                char *vtkName = new char[200];
                sprintf(vtkName, "%s/%d.vtk", vtkdir, layer);

              //  cerr<<vtkName<<" "<<contourValue<<endl;
                svContour *contourField = new svContour(field);
                contourField->ComputeContours(vtkName, contourfile, contourValue);

                delete contourField;
                delete [] vtkName;
                field-> ProcessContour(contourfile, vtkdir, layer);

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
                         clusterLabel[i][j] = cluster+maxClusterLabel+1;
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

void svGlyph::SetClusterLabelBySymmetry(char *clusterfile, svIntArray clusterLayer, int clusterIndex,  vector<int> symmetrytype)
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
                         clusterLabel[i][j] = cluster+maxClusterLabel+1;
                         if(clusterLabel[i][j] > max) max = clusterLabel[i][j];
                    }
                }
             }
       }
       infile.close();
       maxClusterLabel = max;

       int count = 0;
       for(int i=0;i<seed_num;i++)
       {
         for(int j=0;j<glyph[i].size();j++)
         {
              for(int t=0;t<symmetrytype.size();t++)
              {
                      int size = symmetrylist[symmetrytype[t]]->getSize(count);
                      if(size == 1) continue;

                      int data1[size];
                      int data2[size];

                      symmetrylist[symmetrytype[t]]->getData(count,
                                        data1,
                                        data2);
                     if(clusterLabel[data1[0]][data2[0]]>-1) 
                             clusterLabel[data1[1]][data2[1]] = clusterLabel[data1[0]][data2[0]];
                    else if (clusterLabel[data1[1]][data2[1]]>-1) 
                             clusterLabel[data1[0]][data2[0]] = clusterLabel[data1[1]][data2[1]];
              }
              count++;
         }
       }

}

svVector3 svGlyph::GetCenter(int index)
{
   svVector3 c;
   c[0] = 0;
   c[1] = 0;
   c[2] = 0;

   for(int i=0;i<glyph[index].size();i++)
   {
      c[0] = glyph[index][i][0]+c[0];
      c[1] = glyph[index][i][1]+c[1];
      c[2] = glyph[index][i][2]+c[2];
   }

   c[0] = c[0]/glyph[index].size();
   c[1] = c[1]/glyph[index].size();
   c[2] = c[2]/glyph[index].size();

   return c;
}

void svGlyph::GetBoundary(int index, svVector3 &l, svVector3 &r)
{
   l[0] = 9e+9;
   l[1] = 9e+9;
   l[2] = 9e+9;
   r[0] = -9e+9;
   r[1] = -9e+9;
   r[2] = -9e+9;

   for(int i=0;i<glyph[index].size();i++)
   {
       if(glyph[index][i][0] < l[0])l[0] = glyph[index][i][0];
       if(glyph[index][i][1] < l[1])l[1] = glyph[index][i][1];
       if(glyph[index][i][2] < l[2])l[2] = glyph[index][i][2];
       if(glyph[index][i][0] > r[0])r[0] = glyph[index][i][0];
       if(glyph[index][i][1] > r[1])r[1] = glyph[index][i][1];
       if(glyph[index][i][2] > r[2])r[2] = glyph[index][i][2];
   }
}

}
