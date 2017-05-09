

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

    virtual void Render();

    void SetValues(svVector3 *value){for(int i=0;i<3;i++)values[i]=value[i];}
    void SetVisible();
    svIntArray GetVisible(){return layer;}

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
};

}


#endif // __SV_WIDGET_H
