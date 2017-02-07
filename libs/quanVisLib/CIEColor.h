#ifndef CIECOLOR_H
#define CIECOLOR_H

#include <math.h>
#include <string>
#include <iostream>
//#include "myColor3.h"
//#include <QDialog>
//#include <QColor>

#include <vector>
#include <set>
//#include "qtbat.h"

using namespace std;

//#include "AllBats.h"
//#include "OpenManager.h"


#define PI 3.1415926
namespace __svl_lib {
class CIEColor
{
public:    	
	static void XYZtoLuv(double X, double Y, double Z, double &L, double &u, double &v);
    static void LuvtoXYZ(double L, double u, double v, double &X, double &Y, double &Z);
	//static vector<double> getSpiralColor(double value, double H1, double H2);
	static vector<vector<double> > getP(double s, double b, double c, double h);//by Henan
	static double getp2(double l, double h);//by Henan

	static vector<double> getHueColor(double value,double H);
	static vector<double> getSpiralColor(double value, double s ,double bb, double c, double H1, double H2, double contrast=0.75);
	static vector<double> getSpiralColor(vector<vector<double> > MSCs, double value, double H1, double H2);
	static vector<double> SinglehueSequential(double t, double s, double b, double c, double h);
	static vector<double> SinglehueSequential2(double t, double s, double b, double c, double h);
	static vector<double> MultiplehueSequential(double t, double s, double b, double c, double &h1, double h2, double w=0.15); //by Henan
	static vector<double> MultiplehueSequential_LCH(double t, double s, double b, double c, double h1, double h2, double w=0.15); //by Henan
	static vector<double> QualitativeSequential(double t, double e, double r, double s, double b, double c, double h, double &ll);//by Henan
	double GetSmallt(double l, double s, double b, double c);

	//static vector<double> SinglehueSequential2(double t, double s, double b, double c, double h, double l);
	static vector<double> SinglehueSequential(vector<vector<double> > MSCs, double t, double s, double b, double c, double h);

	static vector<double> dividingLCHuvforRGB(double L, double C1, double C2, double H);
	static double dividingLCHuvforMSC_C(double L, double C1, double C2, double H);      
	static vector<double> dividingLCHuvforMSC_H(double L1, double L2, double H);
    static void closestLCHuvtoRGB(double L, double C, double H, double &r, double &g, double &b);
	static vector<double> CIEluvtoRGB(double Y, double u, double v);
	static vector<double> RGBtoCIEluv(double r, double g, double b);
	static void LCHuvtoLuv(double L, double C, double H, double &u, double &v);
	static void LuvtoLCHuv(double L, double u, double v, double &C, double &H);
    static vector<double> XYZtoRGB(double X, double Y, double Z);
    static vector<double> RGBtoXYZ(double r, double g, double b);
	static void LCHuvtoRGB(double L, double u, double v, double &r, double &g, double &b);
	static vector<double> MSC(float h);
	static vector<double> getSpiralTrace(double value, double H1, double H2);
	static vector<double> SinglehueSequential_LCH(double t, double s, double b, double c, double h);
	//static vector<double> SinglehueSequential_LCH2(double t, double s, double b, double c, double h, double l);
	static double RGBtoGrayScale(double r, double g, double b,int flag);
	//static vector<vector<double> > get3600MSC(QString fileName); //get most MSC, compute them if they are not stored
	
	static vector<double> getB(vector<double> b0, vector<double> b1, vector<double> b2, double t);
    static vector<double> getB_1(vector<double> b0, vector<double> b1, vector<double> b2, double v);
    static vector<double> getT(vector<double> p0, vector<double> p1, vector<double> p2, vector<double> q0, vector<double> q1, vector<double> q2, double l);
    static vector<double> getCseg(vector<double> p0, vector<double> p1, vector<double> p2, vector<double> q0, vector<double> q1, vector<double> q2, double t);

	//vector<double> getSpiralColor(double value, double H1, double H2, double contrast=0.75);
	
    vector<double> getApproximateMSC(double h);

};
}
#endif
