#include "svColors.h"
#include <algorithm>
namespace __svl_lib {

void svColors::RGB2LAB(double R, double G, double B, double &l, double &a, double &b)
{

        if(fabs(R)<0.00001
                && fabs(G)<0.00001
                && fabs(B) <0.00001)
        {
                l = 0;
                a = 0;
                b = 0;
        }
        else
        {
                double var_R = R;
                double var_G = G;
                double var_B = B;

                if ( var_R > 0.04045 ) var_R = pow( ( ( var_R + 0.055 ) / 1.055 ) , 2.4);
                else                   var_R = var_R / 12.92;
                if ( var_G > 0.04045 ) var_G = pow(( ( var_G + 0.055 ) / 1.055 ) , 2.4);
                else                   var_G = var_G / 12.92;
                if ( var_B > 0.04045 ) var_B = pow(( ( var_B + 0.055 ) / 1.055 ) , 2.4);
                else                   var_B = var_B / 12.92;

                var_R = var_R * 100;
                var_G = var_G * 100;
                var_B = var_B * 100;
                double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
                double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
                double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;     

                double ref_X =  95.047  ;
                double ref_Y = 100.000;
                double ref_Z = 108.883;

                double var_X = X / ref_X;          //ref_X =  95.047   Observer= 2°, Illuminant= D65
                double var_Y = Y / ref_Y;         //ref_Y = 100.000
                double var_Z = Z / ref_Z;          //ref_Z = 108.883

                if ( var_X > 0.008856 ) var_X = pow(var_X , ( 1./3. ));
                else                    var_X = ( 7.787 * var_X ) + ( 16. / 116. );
                if ( var_Y > 0.008856 ) var_Y = pow(var_Y , ( 1./3. ));
                else                    var_Y = ( 7.787 * var_Y ) + ( 16. / 116. );
                if ( var_Z > 0.008856 ) var_Z = pow(var_Z , ( 1./3. ));
                else                    var_Z = ( 7.787 * var_Z ) + ( 16. / 116. );

                l = ( 116 * var_Y ) - 16;
                a = 500 * ( var_X - var_Y );
                b = 200 * ( var_Y - var_Z );

        }

}
void svColors::LAB2RGB(double l, double a, double b, double &R, double &G, double &B)
{
        double var_Y = ( l + 16 ) / 116;
        double var_X = a / 500 + var_Y;
        double var_Z = var_Y - b / 200;

        if ( pow(var_Y,3.) > 0.008856 ) var_Y = pow(var_Y,3.);
        else                      var_Y = ( var_Y - 16. / 116. ) / 7.787;
        if ( pow(var_X,3.) > 0.008856 ) var_X = pow(var_X, 3.);
        else                      var_X = ( var_X - 16. / 116. ) / 7.787;
        if ( pow(var_Z,3.) > 0.008856 ) var_Z = pow(var_Z,3.);
        else                      var_Z = ( var_Z - 16. / 116. ) / 7.787;

        double ref_X =  95.047;
        double ref_Y = 100.000;
        double ref_Z = 108.883;
        double X = ref_X * var_X;     //ref_X =  95.047     Observer= 2°, Illuminant= D65
        double Y = ref_Y * var_Y;     //ref_Y = 100.000
        double Z = ref_Z * var_Z;     //ref_Z = 108.883



        var_X = X / 100;        //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
        var_Y = Y / 100;        //Y from 0 to 100.000
        var_Z = Z / 100;       //Z from 0 to 108.883

        double var_R = var_X *  3.2406 + var_Y * (-1.5372) + var_Z * (-0.4986);
        double var_G = var_X * (-0.9689) + var_Y *  1.8758 + var_Z *  0.0415;
        double var_B = var_X *  0.0557 + var_Y * (-0.2040) + var_Z *  1.0570;
        if ( var_R > 0.0031308 ) var_R = 1.055 * pow( var_R , ( 1. / 2.4 ) ) - 0.055;
        else                     var_R = 12.92 * var_R;
        if ( var_G > 0.0031308 ) var_G = 1.055 * pow( var_G , ( 1. / 2.4 ) ) - 0.055;
        else                     var_G = 12.92 * var_G;
        if ( var_B > 0.0031308 ) var_B = 1.055 * pow( var_B , ( 1. / 2.4 ) ) - 0.055;
        else                     var_B = 12.92 * var_B;

        R = var_R;
        G = var_G;
        B = var_B;
}


void svColors::ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2)
{
        double colorl, colora, colorb;

        RGB2LAB(r,g,b,colorl,colora,colorb);


        double phimaxl = 3.1415926/4.;
        double phimaxr = 3.1415926/4.;

        double phi = 0;
        double phi_max;
        double gama;
        double tmpa = 0;
        double tmpb = 0;
        double theta;

        tmpa = colora;
        tmpb = colorb;

        if(tmpa >=0 && tmpb >=0)
        {
                gama = 0.5;
                phi_max = 3.1415926/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta = cos(tmpa/c);
        }
        else if(tmpa >0 && tmpb <0)
        {
                gama = 2.;
                phi_max = 3.1415926/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta =  -cos(tmpa/c);
        }
        else if(tmpa <0 && tmpb >0)
        {
                gama = 0.5;
                phi_max = (3.1415926*3.)/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta =  (3.1415926/2. - cos(fabs(tmpa)/c)) + 3.1415926/2.;
        }
        else if(tmpa <0 && tmpb <0)
        {
                gama = 2.;
                phi_max = (3.1415926*3.)/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta =  cos(fabs(tmpa)/c) + 3.1415926;
        }
        phi = phi_max * (1 - pow(fabs(theta)/(3.1415926/2),gama));
        colora = tmpa * cos(phi) - tmpb * sin(phi);
        colorb = tmpa * sin(phi) + tmpb * cos(phi);


        LAB2RGB(colorl, colora, colorb, r2,g2,b2);

        if(r2<0) r2=0;
        if(g2<0) g2=0;
        if(b2<0) b2=0;
        if(r2>1) r2=1;
        if(g2>1) g2=1;
        if(b2>1) b2=1;

}

svColors::svColors()
{
        double SPIRALCOLORS[60][3]=
{
{0.36846,0.316005,0.00451253},
{0.373397,0.332967,0.00484021},
{0.377324,0.34994,0.00509973},
{0.380115,0.36694,0.00538434},
{0.381625,0.383986,0.00562467},
{0.381669,0.401097,0.006112},
{0.38002,0.418299,0.00655623},
{0.376378,0.435619,0.00706493},
{0.370351,0.45309,0.00762841},
{0.361395,0.470753,0.00815003},
{0.348746,0.488655,0.00866171},
{0.331269,0.50685,0.00933107},
{0.307107,0.525415,0.009838},
{0.272985,0.544431,0.0104863},
{0.221717,0.564012,0.0111416},
{0.128634,0.584297,0.0117929},
{0.00553399,0.600134,0.122408},
{0.00564613,0.610625,0.23058},
{0.00572829,0.621163,0.297438},
{0.00576353,0.631702,0.348974},
{0.00581427,0.642204,0.392175},
{0.00577209,0.652645,0.430111},
{0.00578988,0.662999,0.464453},
{0.00581281,0.673252,0.496212},
{0.00592395,0.683387,0.52606},
{0.0059052,0.693397,0.554462},
{0.00586102,0.703269,0.581767},
{0.00583275,0.712992,0.608239},
{0.00576822,0.72256,0.634091},
{0.00590317,0.731959,0.659498},
{0.00553141,0.741192,0.684613},
{0.00582762,0.750233,0.709566},
{0.00572919,0.759087,0.734491},
{0.00555819,0.767738,0.759508},
{0.00523947,0.776176,0.784741},
{0.00501687,0.784383,0.810318},
{0.00414087,0.792351,0.836389},
{0.00372027,0.800046,0.863091},
{0.00304048,0.807451,0.890615},
{0.0024615,0.81453,0.91916},
{0.00168178,0.821247,0.948982},
{0.00045956,0.827551,0.980395},
{0.207832,0.82921,0.999999},
{0.376605,0.825669,0.999999},
{0.473762,0.823447,0.999998},
{0.544384,0.822324,0.999997},
{0.600201,0.822124,0.999998},
{0.646382,0.822704,0.999998},
{0.685731,0.823948,0.999999},
{0.719968,0.825759,1},
{0.750226,0.828056,1},
{0.777459,0.830778,0.999598},
{0.802006,0.834182,0.996809},
{0.825607,0.838455,0.986675},
{0.84509,0.843106,0.980732},
{0.862054,0.847425,0.981713},
{0.878132,0.851504,0.985237},
{0.893858,0.855364,0.989893},
{0.909006,0.859374,0.993318},
{0.92349,0.863528,0.995855}
};    


for(int i=0;i<59;i++)
{
       svVector4 color;
       color[0] =  SPIRALCOLORS[i][0];
       color[1] =  SPIRALCOLORS[i][1];
       color[2] =  SPIRALCOLORS[i][2];
       color[3] = 1;

  double R,G,B, r,g,b;
                R = color[0];
                G = color[1];
                B = color[2];
                ColorBlindSafe(R,G,B,r,g,b);

                color[0] = r;
                color[1] = g;
                color[2] = b;

       spiralColor.add(color);
}
	colorModel = new svVector4Array[2];
	
	double D3COLORS1[25][3]={
	  {0.121569,0.466667,0.705882},
	  {0.682353,0.780392,0.909804},
	  {1,0.498039,0.054902},
	  {1,0.733333,0.470588},
	  {0.172549,0.627451,0.172549},
	  {0.596078,0.87451,0.541176},
	  {0.839216,0.152941,0.156863},
	  {1,0.596078,0.588235},
	  {0.580392,0.403922,0.741176},
	  {0.772549,0.690196,0.835294},
	  {0.54902,0.337255,0.294118},
	  {0.768627,0.611765,0.580392},
	  {0.890196,0.466667,0.760784},
	  {0.968627,0.713725,0.823529},
	  {0.737255,0.741176,0.133333},
	  {0.858824,0.858824,0.552941},
	  {0.0901961,0.745098,0.811765},
	  {0.619608,0.854902,0.898039},
	  {0.223529,0.231373,0.47451},
	  {0.321569,0.329412,0.639216},
	  {0.419608,0.431373,0.811765},
	  {0.611765,0.619608,0.870588},
	  {0.388235,0.47451,0.223529},
	  {0.54902,0.635294,0.321569},
	  {0.709804,0.811765,0.419608}
	};
	
	for(int i=0;i<25;i++)
	{
			svVector4 color;
			color[0] = D3COLORS1[i][0];
			color[1] = D3COLORS1[i][1];
			color[2] = D3COLORS1[i][2];
			color[3] = 1;
   double R,G,B, r,g,b;
                R = color[0];
                G = color[1];
                B = color[2];
                ColorBlindSafe(R,G,B,r,g,b);

                color[0] = r;
                color[1] = g;
                color[2] = b;

			colorModel[0].add(color);
	}
	
	double D3COLORS2[25][3]={
	  {0.807843,0.858824,0.611765},
	  {0.54902,0.427451,0.192157},
	  {0.741176,0.619608,0.223529},
	  {0.905882,0.729412,0.321569},
	  {0.905882,0.796078,0.580392},
	  {0.517647,0.235294,0.223529},
	  {0.678431,0.286275,0.290196},
	  {0.839216,0.380392,0.419608},
	  {0.905882,0.588235,0.611765},
	  {0.482353,0.254902,0.45098},
	  {0.647059,0.317647,0.580392},
	  {0.807843,0.427451,0.741176},
	  {0.870588,0.619608,0.839216},
	  {0.192157,0.509804,0.741176},
	  {0.419608,0.682353,0.839216},
	  {0.619608,0.792157,0.882353},
	  {0.776471,0.858824,0.937255},
	  {0.901961,0.333333,0.0509804},
	  {0.992157,0.552941,0.235294},
	  {0.992157,0.682353,0.419608},
	  {0.992157,0.815686,0.635294},
	  {0.192157,0.639216,0.329412},
	  {0.454902,0.768627,0.462745},
	  {0.631373,0.85098,0.607843},
	  {0.780392,0.913725,0.752941}
	};
	
	for(int i=0;i<25;i++)
	{
			svVector4 color;
			color[0] = D3COLORS2[i][0];
			color[1] = D3COLORS2[i][1];
			color[2] = D3COLORS2[i][2];
			color[3] = 1;
//			colorModel[1].add(color);

   double R,G,B, r,g,b;
		R = color[0];
		G = color[1];
		B = color[2];
		ColorBlindSafe(R,G,B,r,g,b);

		color[0] = r;
		color[1] = g;
		color[2] = b;
                        colorModel[1].add(color);

	}
}

svVector4 svColors::GetDiscreteColors(int value)
{
        svVector4 color;
        color[0]=1;color[1]=1;color[2]=1;color[3]=1;
        if(value<0)return color;
return colorModel[0][value%25];//hard code
}

svVector4 svColors::GetDiscreteColors(int color_model, int value)
{
        svVector4 color;
        color[0]=1;color[1]=1;color[2]=1;color[3]=1;
        if(value<0)return color;
	return colorModel[color_model][value];
}
svVector4Array svColors::GetContinuousColors(svScalarArray &value)
{
       int s = value.size();
       
       svVector4Array color;

       vector<svScalar> tmp_value;
       tmp_value.resize(s);
       for(int i=0;i<s;i++)
       {
            tmp_value[i] = value[i];
            color.add(spiralColor[i]);
       }
       sort(tmp_value.begin(), tmp_value.end());

       int count = 0;
       int step = 59/s;
       for(int i=0;i<s;i++)
       {
           int index;
           for(int j=0;j<s;j++)
           {
               if(tmp_value[i] == value[j])
               {
                    index = j;break;
               }
           }   
           color[index] = spiralColor[count];
           count = count+step;
       }

       tmp_value.clear();       

       return color;
}
svColors::~svColors()
{
        spiralColor.free();
	for(int i=0;i<2;i++)
	{
		colorModel[i].free();
	}
	delete [] colorModel;
	colorModel = NULL;
}

}
