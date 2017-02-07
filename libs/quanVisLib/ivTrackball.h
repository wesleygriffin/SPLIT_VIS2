/**
 */

#ifndef __SVL_IV_TRACKBALL_H_
#define __SVL_IV_TRACKBALL_H_

#include "../jcUtil/MVectorMatrix.h"

namespace __svl_lib {

class Trackball {
public:
    Trackball();

    virtual void  mouseDown(int state, int x, int y);
	virtual void  mouseUp(int state, int x, int y);
	virtual void  mouseMotion(int x, int y);

	void attach(int behavior, int mouse_key_state);
    void reset();
    void setx();
    void sety();
    void setEye(const Vector3f & e);
	void setFocus(const Vector3f & f);

    long getBehavior() const
		{  return behavior;  }
    bool getRotateAction(){if(action==_ROTATE)return true;else return false;}

    void setBehavior(long flag)
		{  behavior = flag;  }

	Matrix4f getMatrix()
		{  return mouse_mat;  }

	Matrix4f getRotateMatrix()
		{  return rotate_mat;  }
	Matrix4f getSingleRotateMatrix() 
		{return rotate_single;}
	Matrix4f getPanZoomMatrix()
		{return pan_zoom_mat;}
	Matrix4f getPanMatrix()
		{return pan_mat2;}
	Matrix4f getZoomMatrix()
		{return zoom_mat;}

	void setMatrix(const Matrix4f & m)
		{  mouse_mat = m;  }

	void setRotateMatrix(const Matrix4f & m)
		{  mouse_mat = focus2world * m * world2focus * pan_zoom_mat;//mouse_mat ;  
			rotate_mat = m;}// * rotate_mat;}

	void setWindowSize(int w, int h)
		{  window_width = w;  window_height = h;  }

	void setRotateScale(Scalar s)
		{  rotate_scale = s;  }

	void setZoomScale(Scalar s)
		{  zoom_scale = s;  }

	void setPanScale(Scalar s)
		{  pan_scale = s;  }

	void setDollyScale(Scalar s)
		{  dolly_scale = s;  }
	double GetDollyScalar(){if(zoom_mat[2][3]==0)return 1; else return zoom_mat[2][3];}

	void print()
		{  mouse_mat.print();  }

	enum {
		SILENT         = 0x00000000,
		LBUTTON_DOWN   = 0x00000001,
		MBUTTON_DOWN   = 0x00000002,
		RBUTTON_DOWN   = 0x00000004,
        BUTTON_DOWN    = 0x00000007, 
		LBUTTON_UP     = 0x00000010,
		MBUTTON_UP     = 0x00000020,
		RBUTTON_UP     = 0x00000040,
        BUTTON_UP      = 0x00000070, 
		ALT_DOWN       = 0x00000100,
		CTRL_DOWN	   = 0x00000200,
		SHIFT_DOWN     = 0x00000400,
        KEY_DOWN       = 0x00000700, 
	};

	typedef enum {
		PAN         = 0x00000001,
		DOLLY       = 0x00000002,
		ROTATE      = 0x00000004,
		ZOOM        = 0x00000008,
		ALL         = 0x0000000F,
	} Behavior;

private:
	Vector3f trackballMapping(int x, int y);

protected:
	typedef enum {
        _NONE   = 0,
        _PAN    = 1,
        _ZOOM   = 2,
	_DOLLY  = 3 ,
        _ROTATE = 4,
    } Action;

    int behavior;           // the trackball behavior type
    Action  action;         // the active action
	Vector3f eye;           // eye pos in world
	Vector3f focus;         // objec rotation center
    Matrix4f mouse_mat;     // the mouse operation matrix
	Matrix4f pan_zoom_mat;
	Matrix4f zoom_mat;
	Matrix4f pan_mat2;
	Matrix4f rotate_mat;
	Matrix4f rotate_single; // the rotation matrix in every time by Henan
	Matrix4f world2focus;   // the translate from world to focus
	Matrix4f focus2world;   // the translate from focus to world
    Vector3f start;         // the mouse start pos on the great cicle
	int start_x, start_y;   // start point in screen space
//Matrix4f dolly_mat;
	Matrix4f dolly_all;
	Scalar rotate_scale;
	Scalar dolly_scale;
	Scalar zoom_scale;
	Scalar pan_scale;

	int window_width;
	int window_height;

	int action_tab[5];      // table to attach mouse/key action to action mode
};

}

#endif //__SVL_IV_TRACKBALL_H_
