

#ifndef __SV_WIDGET_H
#define __SV_WIDGET_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string.h>
#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"

namespace __svl_lib {

class svWidget {
  public:
    svWidget(){};
    ~svWidget(){layer.free();}

    void SetLevel(int l){level = l;}
  
    void Repeat(bool showbox); 
    void Init(); 
    void Mouse(svScalar tranx, svScalar trany,
                   svScalar scalex, svScalar scaley,
                  int x, int y);
    //void Movement(int x, int y);
    void Move(int x, int y);
    void SetHistoValues(svScalarArray *histo)
    {
          tophisto = -1;
          for(int i=0;i<4;i++)
                 histovalues[i].free();
          for(int i=0;i<4;i++)
          {
              for(int j=0;j<histo[i].size();j++)
              {
                 histovalues[i].add(histo[i][j]);
                 
              }
          }
          for(int j=0;j<histo[0].size();j++)
          {
               svScalar count = histo[0][j] + histo[1][j]
                              + histo[2][j] + histo[3][j];
               if(tophisto< count) tophisto = count;
          }
    }
    void SetValues(svScalarArray v, bool ifmin)  
    {
      topmax = -1;
      topvalues.free();
      for(int i=0;i<v.size();i++)
      {
           if(v[i] > topmax) topmax = v[i];
      }
      for(int i=0;i<v.size();i++)
      {
           topvalues.add(v[i]);
      }
    } 
   
    void MoveLeft();
    void MoveRight();

    virtual void Render(svScalar maglevel);

    void SetValues(svVector3 *value){for(int i=0;i<3;i++)values[i]=value[i];}
    void SetVisible();
    svIntArray GetVisible(){return layer;}
    void SetIndex(int zmin, int zmax, int notshowz);
    void GetIndex(svInt *index){for(int i=0;i<3;i++)index[i]= layerindex[i];}
    void Reset();
    bool isSelect();

  private:
     svScalar boxside;
     svVector3 box[3];
     svVector3 line[2];
     bool showbox;
     svScalar triangleside;
     svVector3 triangle[3][3];
     svBool tselect[3];
     svBool bselect;
     svInt level;
     int mousex;
     int mousey;
     svIntArray layer;
     svInt layerindex[3];
     svVector3 values[3];

     svScalar topmax;
     svScalarArray topvalues;
     svScalarArray *histovalues;
     svScalar tophisto;
};

}


#endif // __SV_WIDGET_H
