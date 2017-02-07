/**
 *  base class ,svPrimitive, 
 */

#ifndef __SV_PRIMITIVE_H
#define __SV_PRIMITIVE_H

#include "svVectorField.h"
#include "svParticle.h"
#include "svArray.h"
#include "svLut.h"

//#include "svColor.h"

#define SV_BOTH_DIR   2      // both directions
#define SV_FORWARD    1      // forward dirction of particel advection
#define SV_BACKWARD (-1)     // backward

#define SV_STEP_BREAK  0  // advance one timestep - should stop 
#define SV_STEP_OK     1

#define DEFAULT_MAX_ADVECTION_TIME   100.0 
#define DEFAULT_SEGMENT_FREQ         6
#define DEFAULT_SEGMENT_LENGTH       0.01
// minimum length of a primitive segment

#define SV_IMMEDIATE     0   // no sotre, render in immediate mode
#define SV_DISPLAYLIST   1   // no store, generate and render in display list
#define SV_STORE         2   // store first and then render in immediate mode
#define SV_HYBRID        3   // store and then render in display list
#define SV_VERTEXARRAY   4   // store and render using OpenGL vertex array
#define SV_VERTEXSHADER  5   // store and render using programmable verterx shader

namespace __svl_lib {

typedef struct _RenderProperty {
    svColor4 color;
    svFloat  line_width;
} RenderProperty;

class svPrimitive
{
 public:
  svPrimitive(){};
  svPrimitive(svInt _seed_num);
  svPrimitive(svVectorField* f, svParticle* sd, 
              svInt sd_num, svUint mode);
	svPrimitive(svVectorField *f){field = f;}
  virtual ~svPrimitive();

  //------------------------------------------------------------
  virtual void Render()=0;
  virtual void DrawOutline( svBool set_color_flag = true );

  //------------------------------------------------------------
  virtual void Generate()=0;
  virtual void GenerateFromFile(){};

  //------------------------------------------------------------
  virtual svInt gotoNextPosition(svParticle &p, svInt &dir);
  virtual svInt gotoNextPosition(svParticle &p, svInt &dir, 
                        svFloat time_interval, svBool adv);
  virtual void cleanDisplayList(svUint newMode);

  //------------------------------------------------------------
  svDouble TwoPointsSqLength(svVector3 pa, svVector3 pb) const
  {
    return ((pa[0]-pb[0])* (pa[0]-pb[0])+
            (pa[1]-pb[1])* (pa[1]-pb[1])+
            (pa[2]-pb[2])* (pa[2]-pb[2]));
  }
  virtual void SetBBox(svVector3 &new_lbbox, svVector3 &new_rbbox)
  {
    lbbox = new_lbbox;
    rbbox = new_rbbox;
  }

  //------------------------------------------------------------
  virtual svVector4 CalcLineColor(const svVector3 &pos) {
    //assert(field!=NULL);
    svVector3 v = field->GetVector(pos,0);
    svScalar  vlen = v.length();
    //return lut.GetColor(vlen, true);
    return lut.GetColor(vlen, false);
  };
  
  //------------------------------------------------------------
  virtual svScalar CalcLineWidth(const svVector3 &pos) {
    //assert(field!=NULL);
    svVector3 v = field->GetVector(pos,0);
    svScalar  vlen = v.length();
    svScalar  lw = lut.GetScalar(vlen, true);
    return lw;
  }

  svVectorField* GetField() const
	{  return field;  }

  void SetField(svVectorField* f)
	{  field = f;  }

  svParticle* GetSeed() const
	{  return seed;  };

  void SetSeed(svParticle* s)
	{  seed = s;  }

  virtual svLut  GetLut() 
        {return lut; }
  // 2007

  svInt GetSeedNum() const
	{  return seed_num;  }

  void SetSeedNum(svInt num)
	{  seed_num = num;  }

  svScalar GetMaxAdvectTime() const
	{  return max_advection_time;  }

  void SetMaxAdvectTime (svScalar t)
	{  max_advection_time = t;  }

  svFloat GetSegmentLength() const
      { return segment_len;  }

  void SetSegmentLength(svFloat l)
        { segment_len = l;  }

  svInt GetSegmentFreq() const
	{  return segment_freq;  }

  void SetSegmentFreq(svInt freq)
	{  segment_freq = freq;  }

  svInt GetAdvectDirection() const
	{  return direction;  }

  void SetAdvectDirection(svInt dir)
	{  direction = dir;  }

  svUint GetDisplayMode() const
	{  return display_mode;  }

  void SetDisplayList(svInt list)
       { display_list = list;}

  void SetDisplayMode(svUint mode)
	{  display_mode = mode;  };

  RenderProperty GetRenderProperty()
	{  return render_property;  }

  void SetRenderProperty(const RenderProperty & propery)
	{ render_property = propery;  }

  // Set color models
  //void  SetColorModel(svColor * mycolor)
  //{
    //if(mycolor== NULL)  ENABLE_COLOR=false;
    //else {
      //_colors       = mycolor;
      //ENABLE_COLOR = true;
    //}
  //}
  //svColor*  GetColorModel() const
  //{
    //return _colors;
  //}

  virtual void  EnableColor( )
  {ENABLE_COLOR=true;}
  virtual void  EnableColor(svInt color_model, 
                     svScalar min_value, 
                     svScalar max_value)
  {
     _color_model = color_model;
     _min_value   = min_value;
     _max_value   = max_value;
     ENABLE_COLOR = true;

     lut.SetColorModel(_color_model, min_value, max_value);
  }

 protected:
  svVectorField* field;// the field to advect
  svParticle* seed;// the seed particles from which to advect
  svInt seed_num;// the number of seeds


  svScalar max_advection_time;// maximum elapse time to advect (in seconds)
  svInt    segment_freq;  // how many advection steps to draw a point
  svFloat  segment_len;   // how far to draw a point
  svInt    direction;// SV_FORWARD or SV_BACKWARD or SV_BOTH_DIR

  svUint display_mode; // display mode
  svUint display_list; // OpenGL display list


  svVector3 lbbox, rbbox; // bounding box 

  svUint  STILL_UPDATE;
  svBool  ENABLE_SEGMENT_FREQ; // if true: add a new point 
  svBool  ENABLE_LINEWIDTH;   // if true: lineWidth varies (default:false)

//-----------------------------------------------------------
  RenderProperty render_property;

  svLut lut;
  //svColor *_colors; // color
  svScalar _min_value, _max_value;
  svInt    _color_model;
  svBool   ENABLE_COLOR;       // if true: different color (default:false);
//-----------------------------------------------------------
};
}
#endif // __SV_PRIMITIVE_H
