#include "CIEColor.h"
//#include <QFile>
//#include <QTextStream>

namespace __svl_lib {
/***
Keqin's note
uv(red) = 0.64, 0.33
uv(green) = 0.3, 0.6
uv(blue) = 0.15, 0.06

L: 0-100, H:0-360, C:0-200

white point: what I find online: u = 0.2009, v = 0.4610
 //a most complete and helpful web http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Luv.html

note that here u, v, Y is between 0-1
some CIEluv model may move white point to 0, 0, and Y is 0=100

The XYZ coordinates of the CIE Standard Illuminant D65 reference white.
    double[] D65 = {0.9505d, 1d, 1.0890d};

*//////
 
//******************************************************************************
// 2-dimensional vector.
//******************************************************************************
struct tVec2 { double x, y; };
  
//******************************************************************************
// 3-dimensional vector.
//******************************************************************************
struct tVec3 { double x, y, z; };
  
//******************************************************************************
// 3x3 matrix
//******************************************************************************
struct tMat3x3 { double m[3][3]; };

//******************************************************************************
// Set an indexed matrix column to a given vector.
//******************************************************************************
void Mat_SetCol(tMat3x3 * pMat, int colIdx, const tVec3 & vec)
{
    pMat->m[0][colIdx] = vec.x;
    pMat->m[1][colIdx] = vec.y;
    pMat->m[2][colIdx] = vec.z;
}
  
//******************************************************************************
// Calculate the inverse of a 3x3 matrix. Return false if it is non-invertible.
//******************************************************************************
bool Mat_Invert(tMat3x3 * pOutMat, const tMat3x3 & inMat)
{
    // calculate the minors for the first row
    double minor00 = inMat.m[1][1]*inMat.m[2][2] - inMat.m[1][2]*inMat.m[2][1];
    double minor01 = inMat.m[1][2]*inMat.m[2][0] - inMat.m[1][0]*inMat.m[2][2];
    double minor02 = inMat.m[1][0]*inMat.m[2][1] - inMat.m[1][1]*inMat.m[2][0];
  
    // calculate the determinant
    double determinant =   inMat.m[0][0] * minor00
                        + inMat.m[0][1] * minor01
                        + inMat.m[0][2] * minor02;
  
    // check if the input is a singular matrix (non-invertable)
    // (note that the epsilon here was arbitrarily chosen)
    if( determinant > -0.000001f && determinant < 0.000001f )
        return false;
  
    // the inverse of inMat is (1 / determinant) * adjoint(inMat)
    double invDet = 1.0f / determinant;
    pOutMat->m[0][0] = invDet * minor00;
    pOutMat->m[0][1] = invDet * (inMat.m[2][1]*inMat.m[0][2] - inMat.m[2][2]*inMat.m[0][1]);
    pOutMat->m[0][2] = invDet * (inMat.m[0][1]*inMat.m[1][2] - inMat.m[0][2]*inMat.m[1][1]);
  
    pOutMat->m[1][0] = invDet * minor01;
    pOutMat->m[1][1] = invDet * (inMat.m[2][2]*inMat.m[0][0] - inMat.m[2][0]*inMat.m[0][2]);
    pOutMat->m[1][2] = invDet * (inMat.m[0][2]*inMat.m[1][0] - inMat.m[0][0]*inMat.m[1][2]);
  
    pOutMat->m[2][0] = invDet * minor02;
    pOutMat->m[2][1] = invDet * (inMat.m[2][0]*inMat.m[0][1] - inMat.m[2][1]*inMat.m[0][0]);
    pOutMat->m[2][2] = invDet * (inMat.m[0][0]*inMat.m[1][1] - inMat.m[0][1]*inMat.m[1][0]);
  
    return true;
}
  
//******************************************************************************
// Multiply a column vector on the right of a 3x3 matrix.
//******************************************************************************
void Mat_MulVec( tVec3 * pOutVec, const tMat3x3 & mat, const tVec3 inVec )
{
    pOutVec->x = mat.m[0][0]*inVec.x + mat.m[0][1]*inVec.y + mat.m[0][2]*inVec.z;
    pOutVec->y = mat.m[1][0]*inVec.x + mat.m[1][1]*inVec.y + mat.m[1][2]*inVec.z;
    pOutVec->z = mat.m[2][0]*inVec.x + mat.m[2][1]*inVec.y + mat.m[2][2]*inVec.z;
}
 

//******************************************************************************
// Convert an sRGB color channel to a linear sRGB color channel.
//******************************************************************************
double GammaExpand_sRGB(double nonlinear)
{    
    return   ( nonlinear <= 0.04045f )
           ? ( nonlinear / 12.92f )
           : ( powf( (nonlinear+0.055f)/1.055f, 2.4f ) );
}
  
//******************************************************************************
// Convert a linear sRGB color channel to a sRGB color channel.
//******************************************************************************
double GammaCompress_sRGB(double linear)
{    
    return   ( linear <= 0.0031308f )
           ? ( 12.92f * linear )
           : ( 1.055f * powf( linear, 1.0f/2.4f ) - 0.055f );
}
  
//******************************************************************************
// Convert an sRGB color to a linear sRGB color.
//******************************************************************************
void GammaExpand_sRGB(tVec3 * pColor)
{
    pColor->x = GammaExpand_sRGB( pColor->x );
    pColor->y = GammaExpand_sRGB( pColor->y );
    pColor->z = GammaExpand_sRGB( pColor->z );
}
  
//******************************************************************************
// Convert a linear sRGB color to an sRGB color.
//******************************************************************************
void GammaCompress_sRGB(tVec3 * pColor)
{
    pColor->x = GammaCompress_sRGB( pColor->x );
    pColor->y = GammaCompress_sRGB( pColor->y );
    pColor->z = GammaCompress_sRGB( pColor->z );
}
 
//******************************************************************************
// Convert a linear sRGB color to an sRGB color 
//******************************************************************************
void CalcColorSpaceConversion_RGB_to_XYZ
(
    tMat3x3 *     pOutput,  // conversion matrix
    const tVec2 & red_xy,   // xy chromaticity coordinates of the red primary
    const tVec2 & green_xy, // xy chromaticity coordinates of the green primary
    const tVec2 & blue_xy,  // xy chromaticity coordinates of the blue primary
    const tVec2 & white_xy  // xy chromaticity coordinates of the white point
)
{
    // generate xyz chromaticity coordinates (x + y + z = 1) from xy coordinates
    tVec3 r = { red_xy.x,   red_xy.y,   1.0f - (red_xy.x + red_xy.y) };
    tVec3 g = { green_xy.x, green_xy.y, 1.0f - (green_xy.x + green_xy.y) };
    tVec3 b = { blue_xy.x,  blue_xy.y,  1.0f - (blue_xy.x + blue_xy.y) };
    tVec3 w = { white_xy.x, white_xy.y, 1.0f - (white_xy.x + white_xy.y) };
  
    // Convert white xyz coordinate to XYZ coordinate by letting that the white
    // point have and XYZ relative luminance of 1.0. Relative luminance is the Y
    // component of and XYZ color.
    //   XYZ = xyz * (Y / y) 
    w.x /= white_xy.y;
    w.y /= white_xy.y;
    w.z /= white_xy.y;
  
    // Solve for the transformation matrix 'M' from RGB to XYZ
    // * We know that the columns of M are equal to the unknown XYZ values of r, g and b.
    // * We know that the XYZ values of r, g and b are each a scaled version of the known
    //   corresponding xyz chromaticity values.
    // * We know the XYZ value of white based on its xyz value and the assigned relative
    //   luminance of 1.0.
    // * We know the RGB value of white is (1,1,1).
    //                  
    //   white_XYZ = M * white_RGB
    //
    //       [r.x g.x b.x]
    //   N = [r.y g.y b.y]
    //       [r.z g.z b.z]
    //
    //       [sR 0  0 ]
    //   S = [0  sG 0 ]
    //       [0  0  sB]
    //
    //   M = N * S
    //   white_XYZ = N * S * white_RGB
    //   N^-1 * white_XYZ = S * white_RGB = (sR,sG,sB)
    //
    // We now have an equation for the components of the scale matrix 'S' and
    // can compute 'M' from 'N' and 'S'
  
    Mat_SetCol( pOutput, 0, r );
    Mat_SetCol( pOutput, 1, g );
    Mat_SetCol( pOutput, 2, b );
  
    tMat3x3 invMat;
    Mat_Invert( &invMat, *pOutput );
  
    tVec3 scale;
    Mat_MulVec( &scale, invMat, w );
  
    pOutput->m[0][0] *= scale.x;
    pOutput->m[1][0] *= scale.x;
    pOutput->m[2][0] *= scale.x;
  
    pOutput->m[0][1] *= scale.y;
    pOutput->m[1][1] *= scale.y;
    pOutput->m[2][1] *= scale.y;
  
    pOutput->m[0][2] *= scale.z;
    pOutput->m[1][2] *= scale.z;
    pOutput->m[2][2] *= scale.z;


	
}
 
void CalcColorSpaceConversion_RGB_to_XYZ( tMat3x3 *     pOutput)
{
	pOutput->m[0][0] = 0.4124564;
    pOutput->m[0][1] = 0.3575761;
    pOutput->m[0][2] = 0.1804375;
  
    pOutput->m[1][0] = 0.2126729;
    pOutput->m[1][1] = 0.7151522;
    pOutput->m[1][2] = 0.0721750;

    pOutput->m[2][0] = 0.0193339;
    pOutput->m[2][1] = 0.1191920;
    pOutput->m[2][2] = 0.9503041;

}

void CalcColorSpaceConversion_XYZ_to_RGB( tMat3x3 *     pOutput)
{
	pOutput->m[0][0] = 3.2404542;
    pOutput->m[0][1] = -1.5371385;
    pOutput->m[0][2] = -0.4985314;
  
    pOutput->m[1][0] = -0.9692660;
    pOutput->m[1][1] = 1.8760108;
    pOutput->m[1][2] = 0.0415560;
  
    pOutput->m[2][0] = 0.0556434;
    pOutput->m[2][1] = -0.2040259;
    pOutput->m[2][2] = 1.0572252;       
}

//******************************************************************************
// Example of using the color space conversion functions
//******************************************************************************
void ExampleColorSpaceConversion()
{
    // define chromaticity coordinates for sRGB space
    tVec2 sRGB_red_xy   = { 0.64f, 0.33f };
    tVec2 sRGB_green_xy = { 0.30f, 0.60f };
    tVec2 sRGB_blue_xy  = { 0.15f, 0.06f };
    tVec2 sRGB_white_xy = { 0.3127f, 0.3290f };
  
    // generate conversion matrix from linear sRGB space to XYZ space
    tMat3x3 convert_sRGB_to_XYZ;
    CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ,
                                         sRGB_red_xy,
                                         sRGB_green_xy,
                                         sRGB_blue_xy,
                                         sRGB_white_xy );
  
    // generate conversion matrix from XYZ space to linear sRGB space
    tMat3x3 convert_XYZ_to_sRGB;
    Mat_Invert( &convert_XYZ_to_sRGB, convert_sRGB_to_XYZ );
  
    // define a color in sRGB space
    tVec3 myColor = { 0.2f, 0.5f, 0.8f };
  
    // convert form sRGB to XYZ
    {
        // convert from gamma-corrected sRGB to linear sRGB
        GammaExpand_sRGB( &myColor );
  
        // convert from linear sRGB to XYZ
        Mat_MulVec( &myColor, convert_sRGB_to_XYZ, myColor );
    }
  
    // convert form XYZ back to sRGB
    {
        // convert from XYZ to linear sRGB
        Mat_MulVec( &myColor, convert_XYZ_to_sRGB, myColor );
  
        // convert from linear sRGB to gamma-corrected sRGB
        GammaCompress_sRGB( &myColor );
    }
}



 
vector<double> CIEColor::RGBtoXYZ(double r, double g, double b)
 {//r, g, b: value span 0-1
    vector<double> result;
	/* tVec2 sRGB_red_xy   = { 0.64f, 0.33f };
    tVec2 sRGB_green_xy = { 0.30f, 0.60f };
    tVec2 sRGB_blue_xy  = { 0.15f, 0.06f };
    tVec2 sRGB_white_xy = { 0.3127f, 0.3290f };
  
    // generate conversion matrix from linear sRGB space to XYZ space
    tMat3x3 convert_sRGB_to_XYZ;
    CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ,
                                         sRGB_red_xy,
                                         sRGB_green_xy,
                                         sRGB_blue_xy,
                                         sRGB_white_xy );*/
  
    // generate conversion matrix from XYZ space to linear sRGB space
    //Mat_Invert( &convert_XYZ_to_sRGB, convert_sRGB_to_XYZ );
	
	tMat3x3 convert_sRGB_to_XYZ;
	CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ);
    tMat3x3 convert_XYZ_to_sRGB;
	CalcColorSpaceConversion_XYZ_to_RGB( &convert_XYZ_to_sRGB);	
  
    // define a color in sRGB space
    tVec3 myColor = {r, g, b };
	  
    // convert form sRGB to XYZ
    {
        // convert from gamma-corrected sRGB to linear sRGB
        GammaExpand_sRGB( &myColor );
  
        // convert from linear sRGB to XYZ
        Mat_MulVec( &myColor, convert_sRGB_to_XYZ, myColor );
    }
	result.resize(3);

	result[0]=myColor.x;
	result[1]=myColor.y;
	result[2]=myColor.z;

    return result;
 }


vector<double> CIEColor::XYZtoRGB(double X, double Y, double Z)
 {
    vector<double> result;
	/* tVec2 sRGB_red_xy   = { 0.64f, 0.33f };
    tVec2 sRGB_green_xy = { 0.30f, 0.60f };
    tVec2 sRGB_blue_xy  = { 0.15f, 0.06f };
    tVec2 sRGB_white_xy = { 0.3127f, 0.3290f };
  
    // generate conversion matrix from linear sRGB space to XYZ space
    tMat3x3 convert_sRGB_to_XYZ;
    CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ,
                                         sRGB_red_xy,
                                         sRGB_green_xy,
                                         sRGB_blue_xy,
                                         sRGB_white_xy );
  
    // generate conversion matrix from XYZ space to linear sRGB space
    tMat3x3 convert_XYZ_to_sRGB;
    Mat_Invert( &convert_XYZ_to_sRGB, convert_sRGB_to_XYZ );*/

		tMat3x3 convert_sRGB_to_XYZ;
	CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ);
    tMat3x3 convert_XYZ_to_sRGB;
	CalcColorSpaceConversion_XYZ_to_RGB( &convert_XYZ_to_sRGB);
  
    // define a color in sRGB space
    tVec3 myColor = {X, Y, Z };
    {
        // convert from XYZ to linear sRGB
        Mat_MulVec( &myColor, convert_XYZ_to_sRGB, myColor );
  
        // convert from linear sRGB to gamma-corrected sRGB
        GammaCompress_sRGB( &myColor );
    }

	result.resize(3);

	result[0]=myColor.x;
	result[1]=myColor.y;
	result[2]=myColor.z;

    return result;
 }

vector<double> CIEColor::RGBtoCIEluv(double r, double g, double b)
{ //r, g, b: 0-1
	tVec3 myColor;// = {r, g, b};
    double X, Y, Z, x, y, L, u, v;
	/*tVec2 sRGB_red_xy   = { 0.64f, 0.33f };
    tVec2 sRGB_green_xy = { 0.30f, 0.60f };
    tVec2 sRGB_blue_xy  = { 0.15f, 0.06f };
    tVec2 sRGB_white_xy = { 0.3127f, 0.3290f };
  
    // generate conversion matrix from linear sRGB space to XYZ space
    tMat3x3 convert_sRGB_to_XYZ;
    CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ,
                                         sRGB_red_xy,
                                         sRGB_green_xy,
                                         sRGB_blue_xy,
                                         sRGB_white_xy );
  
    // generate conversion matrix from XYZ space to linear sRGB space
    tMat3x3 convert_XYZ_to_sRGB;
    Mat_Invert( &convert_XYZ_to_sRGB, convert_sRGB_to_XYZ );*/

		tMat3x3 convert_sRGB_to_XYZ;
	CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ);
    tMat3x3 convert_XYZ_to_sRGB;
	CalcColorSpaceConversion_XYZ_to_RGB( &convert_XYZ_to_sRGB);

	//vector<double> test = RGBtoXYZ(r, g, b);	
	myColor.x = r;
	myColor.y = g;
	myColor.z = b;

	// convert from gamma-corrected sRGB to linear sRGB
	GammaExpand_sRGB( &myColor );
  
	// convert from linear sRGB to XYZ
	Mat_MulVec( &myColor, convert_sRGB_to_XYZ, myColor );
		
	X = myColor.x;  Y = myColor.y;  Z = myColor.z;
	/*x = X/(X+Y+Z);
	y = Y/(X+Y+Z);

	u = 4*x / (-2*x + 12*y + 3);
	v = 9*y / (-2*x + 12*y + 3);*/

	XYZtoLuv(X, Y, Z, L, u, v);
		
	vector<double> result;
	result.resize(3);

	result[0]= L, result[1]=u, result[2]=v;

	return result;
}

void CIEColor::LuvtoXYZ(double L, double u, double v, double &X, double &Y, double &Z)
{
	//L=L*100.0;
	double a, b, c, d, e, k, Xr, Yr, Zr, u0,v0;
	Xr=0.9505, Yr=1, Zr=1.0890;
	e=216.0/24389;
	k=24389.0/27;;

	u0= 4 * Xr / (Xr + 15*Yr + 3*Zr);
	v0= 9 * Yr / (Xr + 15*Yr + 3*Zr);

	Y=L>k*e?  pow( (L+16)/116.0, double(3.0)) : (L/k);

	d= Y * ( 39 * L /(v+13*L*v0) - 5);
	c = -1.0/3.0;
	b = -5*Y;
	a = (52*L/(u+13*L*u0)-1)/3.0;
	X= (d-b)/(a-c);
	Z= X*a+b;

}

void CIEColor::XYZtoLuv(double X, double Y, double Z, double &L, double &u, double &v)
{
	double yr,u1,v1,vr1, ur1, e, k,Xr,Yr,Zr;
	//double[] D65 = {0.9505d, 1d, 1.0890d};//CIE Standard Illuminant D65 reference white
	Xr=0.9505, Yr=1, Zr=1.0890;
	e=216.0/24389;
	k=24389.0/27;

	u1=4*X/(X+15*Y+3*Z);
	v1=9*Y/(X+15*Y+3*Z);
	ur1=4*Xr/(Xr+15*Yr+3*Zr);
	vr1=9*Yr/(Xr+15*Yr+3*Zr);
	yr=Y/Yr;
	L= yr>e? (116* pow(yr,double(1.0/3.0)) - 16) : k*yr;
	u=13*L*(u1-ur1);
	v=13*L*(v1-vr1);
	//L=L/100.0;
}

vector<double> CIEColor::CIEluvtoRGB(double L, double u, double v)
{

	

    vector<double> result;
	
    /*tVec2 sRGB_red_xy   = { 0.64f, 0.33f };
    tVec2 sRGB_green_xy = { 0.30f, 0.60f };
    tVec2 sRGB_blue_xy  = { 0.15f, 0.06f };
    tVec2 sRGB_white_xy = { 0.3127f, 0.3290f };
  
    // generate conversion matrix from linear sRGB space to XYZ space
    tMat3x3 convert_sRGB_to_XYZ;
    CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ,
                                         sRGB_red_xy,
                                         sRGB_green_xy,
                                         sRGB_blue_xy,
                                         sRGB_white_xy );



	//0.4124564  0.3575761  0.1804375 //from http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Luv.html
     //0.2126729  0.7151522  0.0721750
    //0.0193339  0.1191920  0.9503041
  
    // generate conversion matrix from XYZ space to linear sRGB space
    tMat3x3 convert_XYZ_to_sRGB;
    Mat_Invert( &convert_XYZ_to_sRGB, convert_sRGB_to_XYZ );
  
	//	 3.2404542 -1.5371385 -0.4985314 ////from http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Luv.html
    //-0.9692660  1.8760108  0.0415560
    //0.0556434 -0.2040259  1.0572252

    // define a color in sRGB space

	
	/*LuvtoXYZ(79.6138,  -0.8423,  0.0675,  X,  Y,  Z);
	XYZtoLuv( 0.53,  0.56,  0.61,  L,  u,  v);*/


		tMat3x3 convert_sRGB_to_XYZ;
	CalcColorSpaceConversion_RGB_to_XYZ( &convert_sRGB_to_XYZ);
    tMat3x3 convert_XYZ_to_sRGB;
	CalcColorSpaceConversion_XYZ_to_RGB( &convert_XYZ_to_sRGB);

	//ExampleColorSpaceConversion();

	// u v to x, y, z
	/*x = 9*u / (6*u - 16*v + 12);
    y = 4*v / (6*u - 16*v + 12);
	
	//x, y, Y
	X = x/y * Y;
    Z = (1-x-y)/y * Y;*/

		double x,y, X,Y,Z; //Y luminance
	LuvtoXYZ(L, u, v, X, Y, Z);

    tVec3 myColor = {X, Y, Z };
    
    // convert from XYZ to linear sRGB
    Mat_MulVec( &myColor, convert_XYZ_to_sRGB, myColor );
  
    // convert from linear sRGB to gamma-corrected sRGB
    GammaCompress_sRGB( &myColor );    

	result.resize(3);

	result[0]=myColor.x;
	result[1]=myColor.y;
	result[2]=myColor.z;

	//////////
	//test
	/*vector<double> test;
	if( result[0]>0 && result[1]>0 && result[2]>0)
	{
		test = RGBtoCIEluv(result[0], result[1], result[2]);
	}*/
	//test
	
	//result.resize(3);

	//result[0]=myColor.x;
	//result[1]=myColor.y;
	//result[2]=myColor.z;
    return result;
 }

void CIEColor::LCHuvtoRGB(double L, double C, double H, double &r, double &g, double &b)
{   
	double u, v;
	LCHuvtoLuv(L, C, H, u, v);
	//if(L>0.21 || L<0.19)
	//	L=L;

	//u = u + 0.19783001, v = v + 0.46831998;
	vector<double> RGB = CIEluvtoRGB(L, u, v);
	r=RGB[0], g=RGB[1], b=RGB[2];	
}

void CIEColor::closestLCHuvtoRGB(double L, double C, double H, double &r, double &g, double &b)
{   
	double u, v;
	LCHuvtoLuv(L, C, H, u, v);
	//u = u + 0.19783001, v = v + 0.46831998;
	vector<double> RGB = CIEluvtoRGB(L, u, v);
	r=RGB[0], g=RGB[1], b=RGB[2];
	if( (r<0 || r>1) || (g<0 || g>1) || ( b<0 || b>1) )
	{	
	   RGB=dividingLCHuvforRGB(L, 0, C, H);	
	   r=RGB[0], g=RGB[1], b=RGB[2];
	}
}
/*
vector<double> CIEColor::getApproximateMSC(double )h
{
	
	closestLCHuvtoRGB

}

void CIEColor::SinglehueSequential(double s, double b, double c, double h)
{
	int N=10;
	vector<double> Pseg;
	vector<float> p0(2,0),p1(2,0), p2(2,0), q0(2,0), q1(2,0), q2(2,0);
	//p2 = LCHuv(100, 0, h)
	//p0 = LCHuv(0, 0, h)
*/

vector<vector<double> > CIEColor::getP(double s, double b, double c, double h)
{
	vector<vector<double> > p_triple;
	p_triple.resize(3);

	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	p1=dividingLCHuvforMSC_H(0,100,h);
	p0[0]=0, p0[1]=0, p0[2]=h;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	p_triple[0].push_back(p0[0]);
	p_triple[0].push_back(p1[0]);
	p_triple[0].push_back(p2[0]);
	p_triple[1].push_back(p0[1]);
	p_triple[1].push_back(p1[1]);
	p_triple[1].push_back(p2[1]);
	p_triple[2].push_back(p0[2]);
	p_triple[2].push_back(p1[2]);
	p_triple[2].push_back(p2[2]);

	return p_triple;
}

vector<double> CIEColor::SinglehueSequential_LCH(double t, double s, double b, double c, double h)
{

	int N=10;
	vector<double> Pseg;
	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	p1=dividingLCHuvforMSC_H(0,100,h);
	p0[0]=0, p0[1]=0, p0[2]=h;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	//cerr<<125-125*pow(0.2,(1-c)*b+t*c)<<endl;

	vector<double> T = getT(p0, p1, p2, q0, q1, q2, 125-125*pow(0.2,(1-c)*b+t*c)); 
	Pseg = getCseg(p0, p1, p2, q0, q1, q2, T[0]);
	//cerr<<Pseg[0]<<" "<<Pseg[1]<<endl;
	Pseg[2]=h;

	return Pseg;
}

double CIEColor::RGBtoGrayScale(double r, double g, double b,int flag)
{ 
	if(flag==0)
		return  r*0.3 + g*0.59 + b*0.11;
	else if(flag==1)
	{
	   vector<double> luv = RGBtoCIEluv(r, g, b);	   
	   vector<double> RGB(3,0);
	   closestLCHuvtoRGB(luv[0], 0, 0, RGB[0], RGB[1], RGB[2]);
	   return RGB[0];	
	}
	else
	{
	   vector<double> luv = RGBtoCIEluv(r, g, b);	 
	   return luv[0];		
	}
}

vector<double> CIEColor::SinglehueSequential(vector<vector<double> > MSCs, double t, double s, double b, double c, double h)
{
	int N=10;
	vector<double> Pseg;
	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	//p1=dividingLCHuvforMSC_H(0,100,h);
	int mid1, mid2;
	mid1 = int(h * 100);  
	float ML, MC, dl;
	dl = h*100 - mid1;
	mid1 = mid1%3600;
	mid2 = (mid1+1)%3600;;
	ML = MSCs[mid1][0]*(1-dl) + MSCs[mid2][0]*dl;
	MC = MSCs[mid1][1]*(1-dl) + MSCs[mid2][1]*dl;	

	p1[0]= ML, p1[1]= MC, p1[2]= h;
	p0[0]=0, p0[1]=0, p0[2]=h;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	vector<double> T = getT(p0, p1, p2, q0, q1, q2, 125-125*pow(0.2,(1-c)*b+t*c)); 
	Pseg = getCseg(p0, p1, p2, q0, q1, q2, T[0]);
	
	Pseg[2]=h;
	double r, g, k;
	//LCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	closestLCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	Pseg[0]= r, Pseg[1]=g, Pseg[2]=k;
	return Pseg;
}

vector<double> CIEColor::SinglehueSequential(double t, double s, double b, double c, double h)
{
	int N=10;
	vector<double> Pseg;
	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	p1=dividingLCHuvforMSC_H(0,100,h);
	p0[0]=0, p0[1]=0, p0[2]=h;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	vector<double> T = getT(p0, p1, p2, q0, q1, q2, 125-125*pow(0.2,(1-c)*b+t*c)); 
	Pseg = getCseg(p0, p1, p2, q0, q1, q2, T[0]);
	//Pseg[2]=h;
	double r, g, k;
	//LCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	closestLCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	Pseg[0]= r, Pseg[1]=g, Pseg[2]=k;
	return Pseg;
}

vector<double> CIEColor::SinglehueSequential2(double t, double s, double b, double c, double h)
{
	int N=10;
	vector<double> Pseg;
	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	p1=dividingLCHuvforMSC_H(0,100,h);
	p0[0]=0, p0[1]=0, p0[2]=h;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	vector<double> T = getT(p0, p1, p2, q0, q1, q2, t); 
	Pseg = getCseg(p0, p1, p2, q0, q1, q2, T[0]);
	//Pseg[2]=h;
	double r, g, k;
	//LCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	closestLCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	Pseg[0]= r, Pseg[1]=g, Pseg[2]=k;
	return Pseg;
}

vector<double> CIEColor::MultiplehueSequential(double t, double s, double b, double c, double &h1, double h, double w)
{
	vector<double> Pseg;
	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	p1=dividingLCHuvforMSC_H(0,100,h);
	p0[0]=0, p0[1]=0, p0[2]=h;

	vector<double> pbright(3,0);
	vector<double> luv = RGBtoCIEluv(1,1,0);
	pbright[0] = luv[0];
	LuvtoLCHuv(luv[0], luv[1], luv[2], pbright[1], pbright[2]);	

	//cerr<<pbright[0]<<" "<<pbright[1]<<" "<<pbright[2]<<endl;
	p2[0] = 100 *(1- w) + w *pbright[0];

	//cerr<<pbright[0]<<" "<<pbright[1]<<" "<<pbright[2]<<" "<<h<<endl;
	double M = fmod((180 + pbright[2] - h),360) - 180;//cerr<<pbright[2]<<" "<<h<<" "<<M<<" "<<w * M<<endl;
	p2[2] = fmod((h + w * M),360);

	vector<double> pmid(3,0);
	pmid = 	dividingLCHuvforMSC_H(0,100,p2[2]);
	vector<double> pend(3,0);

	if(p2[0]<=pmid[0])
	{
		pend[0] = 0;pend[1] = 0;pend[2] = p2[2];
	}
	else
	{
		pend[0] = 100;pend[1] = 0;pend[2] = p2[2];
	}

	double alpha = (pend[0] - p2[0]) / (pend[0] - pmid[0]);
	double Sm = alpha * (pmid[1] - pend[1]) + pend[1];

	//cerr<<Sm<<" "<<w * s * pbright[1]<<endl;;

	if(Sm< w * s * pbright[1])
		p2[1] = Sm;
	else
		p2[1] = w * s * pbright[1];

	//cerr<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<" "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<" "<<p0[0]<<" "<<p0[1]<<" "<<p0[2]<<endl;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	//cerr<<q1[0]<<" "<<q2[0]<<" "<<p2[0]<<" "<<p1[0]<<" "<<s<<endl; 

	double cc = c;
	c = log10((125 - p2[0])/125)/log10(0.2) - b;
	c = c / (1-b);//cerr<<c<<endl;
	if(c>cc)
		c= cc;

	vector<double> T = getT(p0, p1, p2, q0, q1, q2, 125-125*pow(0.2,(1-c)*b+t*c)); 
	//cerr<<125-125*pow(0.2,(1-c)*b+t*c)<<endl;
	Pseg = getCseg(p0, p1, p2, q0, q1, q2, T[0]);
	h1 = Pseg[2];
	/*double uu,vv;

	LCHuvtoLuv(Pseg[0], Pseg[1], Pseg[2],uu,vv);
	cerr<<Pseg[0]<<" "<<uu<<" "<<vv<<endl;*/
	//cerr<<Pseg[0]<<" "<<" "<<Pseg[1]<<" "<<Pseg[2]<<endl;
	//Pseg[2]=h;
	double r, g, k;
	//LCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);
	closestLCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);

	Pseg[0]= r, Pseg[1]=g, Pseg[2]=k;

	//cerr<<r<<" "<<g<<" "<<k<<endl;
	return Pseg;	
}

vector<double> CIEColor::QualitativeSequential(double t, double e, double r, double s, double b, double c, double h, double &ll)
{
	vector<double> pbright(3,0);
	vector<double> luv = RGBtoCIEluv(1,1,0);
	pbright[0] = luv[0];
	LuvtoLCHuv(luv[0], luv[1], luv[2], pbright[1], pbright[2]);

	vector<double> cy(3,0);
	cy[0] = pbright[0];//L
	cy[1] = pbright[1];//C
	cy[2] = pbright[2];//H

	double alpha;
	double hprime0, hprime1;

	vector<double> Pseg(3,0);
	Pseg[2] = fmod(360 * (e + t * r ), 360);


	hprime0 = fmod(Pseg[2],360.);
	hprime1 = fmod(cy[2],360.);

	if(fabs(hprime1 - hprime0)<180)
	{
		alpha = fabs(hprime1 - hprime0);
	}
	else
	{
		alpha = 360 - fabs(hprime1 - hprime0);
	}

	alpha = alpha / 180;

	Pseg[0] = (1. - alpha) * b * cy[0] + alpha * (1-c) * b * cy[0];
	
	vector<double> smax(3,0);
	luv = RGBtoCIEluv(1,0,0);
	smax[0] = luv[0];
	LuvtoLCHuv(luv[0], luv[1], luv[2], smax[1], smax[2]);

	//---Smax----
	vector<double> pmid(3,0);
	pmid = 	dividingLCHuvforMSC_H(0,100,Pseg[2]);
	vector<double> pend(3,0);

	if(Pseg[0]<=pmid[0])
	{
		pend[0] = 0;pend[1] = 0;pend[2] = Pseg[0];
	}
	else
	{
		pend[0] = 100;pend[1] = 0;pend[2] = Pseg[0];
	}

	double a = (pend[0] - Pseg[0]) / (pend[0] - pmid[0]);
	double Sm = a * (pmid[1] - pend[1]) + pend[1];
	//----------

	if(Sm < s * smax[1])
	{
		Pseg[1] = Sm;
	}
	else
	{
		Pseg[1] = s * smax[1];
	}
	//cerr<<Pseg[0]<<" "<<Pseg[1]<<" "<<Pseg[2]<<endl;

	double r0, g0, k0;
	//LCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r, g, k);

	ll = Pseg[0];

	closestLCHuvtoRGB(Pseg[0], Pseg[1], Pseg[2], r0, g0, k0);

	Pseg[0]= r0, Pseg[1]=g0, Pseg[2]=k0;
	//cerr<<Pseg[0]<<" "<<Pseg[1]<<" "<<Pseg[2]<<endl;

	return Pseg;
}

double CIEColor::GetSmallt(double l, double s, double b, double c)
{

	double t;
	double A = log10((125-l)/l) / log10(0.2);
	t = (A - (1-c)*b)/c;
	
	return t;
}

vector<double> CIEColor::MultiplehueSequential_LCH(double t, double s, double b, double c, double h1, double h, double w)
{
	vector<double> Pseg;
	vector<double> p0(3,0),p1(3,0), p2(3,0), q0(3,0), q1(3,0), q2(3,0);
		
	p2[0]=100, p2[1]=0, p2[2]=h;
	p1=dividingLCHuvforMSC_H(0,100,h);
	p0[0]=0, p0[1]=0, p0[2]=h;

	vector<double> pbright(3,0);
	vector<double> luv = RGBtoCIEluv(1,1,0);
	pbright[0] = luv[0];
	LuvtoLCHuv(luv[0], luv[1], luv[2], pbright[1], pbright[2]);


	p2[0] = 100 *(1- w) + w *pbright[0];

	double M = fmod((180 + pbright[2] - h),360) - 180;
	p2[2] = fmod((h + w * M),360);

	vector<double> pmid(3,0);
	pmid = 	dividingLCHuvforMSC_H(0,100,p2[2]);
	vector<double> pend(3,0);

	if(p2[0]<=pmid[0])
	{
		pend[0] = 0;pend[1] = 0;pend[2] = p2[2];
	}
	else
	{
		pend[0] = 100;pend[1] = 0;pend[2] = p2[2];
	}

	double alpha = (pend[0] - p2[0]) / (pend[0] - pmid[0]);
	double Sm = alpha * (pmid[1] - pend[1]) + pend[1];

	//cerr<<Sm<<" "<<w * s * pbright[1];

	if(Sm< w * s * pbright[1])
		p2[1] = Sm;
	else
		p2[1] = w * s * pbright[1];

	//cerr<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<endl;

	q0[0] = (1-s) * p0[0] + s *p1[0];
	q0[1] = (1-s) * p0[1] + s *p1[1];
	q0[2] = (1-s) * p0[2] + s *p1[2];

	q2[0] = (1-s) * p2[0] + s *p1[0];
	q2[1] = (1-s) * p2[1] + s *p1[1];
	q2[2] = (1-s) * p2[2] + s *p1[2];

	q1[0] = (q0[0] + q2[0])/2;
	q1[1] = (q0[1] + q2[1])/2;
	q1[2] = (q0[2] + q2[2])/2;

	vector<double> T = getT(p0, p1, p2, q0, q1, q2, 125-125*pow(0.2,(1-c)*b+t*c)); 
	Pseg = getCseg(p0, p1, p2, q0, q1, q2, T[0]);
	Pseg[2]=h;

	return Pseg;	
}
vector<double> CIEColor::getB(vector<double> b0, vector<double> b1, vector<double> b2, double t)
{
	vector<double> output;
	output.resize(3);
	output[0] = ((1-t) * (1-t) * b0[0] + 2 * (1-t) * t * b1[0] + t * t * b2[0]);	
	output[1] = ((1-t) * (1-t) * b0[1] + 2 * (1-t) * t * b1[1] + t * t * b2[1]);	
	output[2] = ((1-t) * (1-t) * b0[2] + 2 * (1-t) * t * b1[2] + t * t * b2[2]);	
	return output;
}

vector<double> CIEColor::getB_1(vector<double> b0, vector<double> b1, vector<double> b2, double v)
{
	vector<double> output;
	output.resize(3);
	output[0] = (b0[0]-b1[0]+sqrt(b1[0]*b1[0]-b0[0]*b2[0]+(b0[0]-2*b1[0]+b2[0])*v))/(b0[0]-2*b1[0]+b2[0]);
	//cerr<<b0[0]<<" "<<b1[0]<<" "<<b2[0]<<endl;
	//cerr<<b1[0]*b1[0]-b0[0]*b2[0]+(b0[0]-2*b1[0]+b2[0])*v<<endl;
	output[1] = (b0[1]-b1[1]+sqrt(b1[1]*b1[1]-b0[1]*b2[1]+(b0[1]-2*b1[1]+b2[1])*v))/(b0[1]-2*b1[1]+b2[1]);
	//cerr<<b0[1]-2*b1[1]+b2[1]<<endl;
	output[2] = (b0[2]-b1[2]+sqrt(b1[2]*b1[2]-b0[2]*b2[2]+(b0[2]-2*b1[2]+b2[2])*v))/(b0[2]-2*b1[2]+b2[2]);
	//cerr<<b0[2]-2*b1[2]+b2[2]<<endl;
	return output;
}

vector<double> CIEColor::getT(vector<double> p0, vector<double> p1, vector<double> p2, vector<double> q0, vector<double> q1, vector<double> q2, double l)
{
	vector<double> output;
	

	if(l<=q1[0])
	{
        output = getB_1( p0, q0, q1, l);	
		output[0] = output[0]*0.5; 
		output[1] = output[1]*0.5; 
		output[2] = output[2]*0.5;
	}
	else
    {
		output = getB_1( q1, q2, p2, l);//cerr<<output[0]<<endl;	
		output[0] = output[0]*0.5 + 0.5; 
		output[1] = output[1]*0.5 + 0.5; 
		output[2] = output[2]*0.5 + 0.5;
		//cerr<<output[0]<<endl;
	}
	return output;	
}

vector<double> CIEColor::getCseg(vector<double> p0, vector<double> p1, vector<double> p2, vector<double> q0, vector<double> q1, vector<double> q2, double t)
{
	vector<double> output;
	if(t<=0.5)
	{
		output = getB( p0, q0, q1, 2*t);	
	}
	else
	{
	    output = getB( q1, q2, p2, 2*(t-0.5));	
	}	
	return output;
}


vector<double> CIEColor::getHueColor(double value,double H)
{
	vector<double> RGB;
	RGB = CIEColor::SinglehueSequential(value, 0.75, 0.0, 1.00, H);
	//cerr<<RGB[0]<<" ";

	return RGB;
}
vector<double> CIEColor::getSpiralColor(double value, double s, double bb, double c,double H1, double H2, double contrast)
{
	if( H1>H2 )
		H2 = H2+360;

	double H = value * (H2-H1) + H1;	
	double r, g, b;	
	vector<double> RGB;
	if(contrast>1.0)
	{
		 closestLCHuvtoRGB(value*100, 200, H, r, g, b);
	     RGB.push_back(r);
		 RGB.push_back(g); 
		 RGB.push_back(b);
	}
	else
	{
	     RGB = CIEColor::SinglehueSequential(value, s, bb, c, H);	     
	}
	return RGB;
}


vector<double> CIEColor::getSpiralColor(vector<vector<double> > MSCs, double value, double H1, double H2)
{
	if( H1>H2 )
		H2 = H2+360;

	double H = value * (H2-H1) + H1;	
	double r, g, b;	
	//closestLCHuvtoRGB(value*100, 200, H, r, g, b);
	vector<double> RGB = CIEColor::SinglehueSequential(MSCs, value, 0.75, 0.0, 1.00, H);
	//vector<double> RGB;	RGB.push_back(r); RGB.push_back(g); RGB.push_back(b);
	return RGB;
}

vector<double> CIEColor::getSpiralTrace(double value, double H1, double H2)
{
	if( H1>H2 )
		H2 = H2+360;

	double H = value * (H2-H1) + H1;	
	double r, g, b;	
	//closestLCHuvtoRGB(value*100, 200, H, r, g, b);
	vector<double> LCH = CIEColor::SinglehueSequential_LCH(value, 0.75, 0., 1., H);
	//vector<double> RGB;	RGB.push_back(r); RGB.push_back(g); RGB.push_back(b);
	return LCH;
}

vector<double> CIEColor::dividingLCHuvforRGB(double L, double C1, double C2, double H)
{   
	double u, v;
	vector<double> RGB;

	if(C2-C1<0.001)
	{
		LCHuvtoLuv(L, C1, H, u, v);
	    u = u + 0.19783001, v = v + 0.46831998;
	    RGB = CIEluvtoRGB(L, u, v);
		return RGB;
	}	

	double C= (C1 + C2)/2;
	LCHuvtoLuv(L, C, H, u, v);
	u = u + 0.19783001, v = v + 0.46831998;
	RGB = CIEluvtoRGB(L, u, v);		
	double r=RGB[0], g=RGB[1], b=RGB[2];
	if( (r<0 || r>1) || (g<0 || g>1) || ( b<0 || b>1) )
	{
		return dividingLCHuvforRGB(L, C1, C, H);	
	}
	else
	{
		return dividingLCHuvforRGB(L, C, C2, H);	
	}
}

double CIEColor::dividingLCHuvforMSC_C(double L, double C1, double C2, double H)
{   
	double u, v;
	vector<double> RGB;
	if(L==100 || L==0)
		return 0;
	if(C2-C1<0.01)
	{
		return (C2 + C1)/2;
	}	

	double C= (C1 + C2)/2;
	LCHuvtoLuv(L, C, H, u, v);
	u = u + 0.19783001, v = v + 0.46831998;
	RGB = CIEluvtoRGB(L, u, v);		
	double r=RGB[0], g=RGB[1], b=RGB[2];
	if( (r<0 || r>1) || (g<0 || g>1) || ( b<0 || b>1) )
	{
		return dividingLCHuvforMSC_C(L, C1, C, H);	
	}
	else
	{
		return dividingLCHuvforMSC_C(L, C, C2, H);	
	}
}

/*
vector<vector<double>> CIEColor::get3600MSC(QString fileName) //get most MSC, compute them if they are not stored
{
    //search all files
    //read cross talk symbol file
	vector<vector<double>> MSCs;
    QFile inFile(fileName);
	QFile outFile(fileName);
	bool flag=true;
	//vector<QString> orthologySymbol;	
	if(inFile.open(QIODevice::ReadOnly))
	{
		QTextStream in(&inFile);
		while ( !in.atEnd() )
		{
			QString line = in.readLine();    						  			  				
		    QStringList sname=line.split(' ');	
			if(sname.size()!=3)
			{
				MSCs.clear();
				flag=false;
				break;
			}
			vector<double> temp;
			for(int i=0; i<sname.size(); i++)
		    {				
				temp.push_back(sname[i].toDouble());							
			}
			MSCs.push_back(temp);	
		}		   
	}	
	inFile.close();
	if(MSCs.size()!=3600)
	{
	    MSCs.clear();
	    flag=false;
	}
	if(!flag)
	{
		if(outFile.open(QIODevice::WriteOnly))
		{
			double H=0;
			QTextStream out(&outFile);	
			for(int i=0; i<3600; i++)
			{
				H=0.1*i;			
				vector<double> LCH = dividingLCHuvforMSC_H(0, 100, H);				
				out << LCH[0] << " " << LCH[1] << " " << LCH[2] << "\n";						
			}
			outFile.close();
		}	
	}
	return MSCs;
}
*/

vector<double> CIEColor::dividingLCHuvforMSC_H(double L1, double L2, double H)
{	
	if( L1>L2 )
	{
	   swap(L1,L2);
	}
	if(L2-L1<0.1)
	{
		vector<double> LC;
		double L=(L2+L1)/2;
		LC.push_back(L); LC.push_back(dividingLCHuvforMSC_C(L, 0, 200, H)); LC.push_back(H); 
		return LC;
	}
	vector<double> C;	
	int slice = 10;
	C.resize(slice+1);
	float L, dl = (L2-L1)/slice;
	for(int i=0; i<slice+1; i++)
	{
	    L = L1 + dl*i;
		C[i] = dividingLCHuvforMSC_C(L, 0, 200, H);
	}
	double max=-100000, maxi, mc;
	set<int> maxiSet;
	for(int i=1; i<10; i++)
	{
		if(C[i-1] <= C[i] && C[i-1] <= C[i+1])
	        mc = C[i] + C[i+1];
		else if (C[i+1] <= C[i] && C[i+1] <= C[i-1])
			mc = C[i-1]+ C[i];
		else 
			mc = C[i-1]+ C[i+1];
		if(mc > max)
		{
			maxiSet.clear();
			max=mc, maxi=i;		
			maxiSet.insert(i);
		}
		else if(mc == max)
		{
			maxiSet.insert(i);
		}
	}
	if(maxiSet.size()>3)
    {
		vector<double> LC;
		double L=(L2+L1)/2;
		LC.push_back(L); LC.push_back(dividingLCHuvforMSC_C(L, 0, 200, H)); LC.push_back(H); 
		return LC;
	}

	L=L1;
	L1 = L+dl*(*maxiSet.begin()-1); L2 = L+dl*(*maxiSet.rbegin()+1);
	return dividingLCHuvforMSC_H(L1, L2, H);

}
// from julia in github
//it may be wrong
//MSC - Most Saturated Color for given hue h
// ---------------------
// Calculates the most saturated color for any given hue by
// finding the corresponding corner in LCHuv space

vector<double> CIEColor::MSC(float h)
{//h: 0-360;
    //Corners of RGB cube
	const double H[6] = {12.173988685914473, 85.872748860776770, 127.72355046632740, 192.17397321802082, 265.87273498040290, 307.72354567594960}; //convert(LCHuv,RGB(1,0,0)).h
    //const double h1 = 85.872748860776770; //convert(LCHuv,RGB(1,1,0)).h
    //const double h2 = 127.72355046632740; //convert(LCHuv,RGB(0,1,0)).h
    //const double h3 = 192.17397321802082; //convert(LCHuv,RGB(0,1,1)).h
    //const double h4 = 265.87273498040290; //convert(LCHuv,RGB(0,0,1)).h
    //const double h5 = 307.72354567594960; //convert(LCHuv,RGB(1,0,1)).h

    //Wrap h to [0, 360] range]
    //mod(h, 360);
	if(h>=0)
	{
		h = h - 360*int(h/360);
	}
	else
	{
		h = h - 360*(int(h/360)+1);
	}
	
    int p=0; //variable
    int o=0; //min
    int t=0; //max

    //Selecting edge of RGB cube; R=1 G=2 B=3
    //if h0 <= h < h1
	if ( H[0] <= h && h < H[1])
    {
		p=1; o=2; t=0;
	}
    else if ( H[1] <= h && h < H[2]) //h1 <= h < h2
    {
		p=0; o=2; t=1;
	}
    else if ( H[2] <= h && h < H[3])
    {
		p=2; o=0; t=1;
	}
    else if ( H[3] <= h && h < H[4])
    {
		p=1; o=0; t=2;
	}
    else if ( H[4] <= h && h < H[5])
    {
		p=0; o=1; t=2;
	}
    else if (H[5] <= h || h < H[0])
    {
		p=2; o=1; t=0;
	}
    // col=zeros(3)
	double col[3]={0,0,0};

    //check if we are directly on the edge of the RGB cube (within some tolerance)
    for(int edge=0; edge<6; edge++)// in [h0, h1, h2, h3, h4, h5]
    {   
		if(H[edge] - 200* (2.220446049250313e-16) < h && h < H[edge] + 200* (2.220446049250313e-16))
        {
			col[p] = ( edge == 0 || edge == 2 || edge ==4)? 0.0 : 1.0;
            col[o] = 0.0;
            col[t] = 1.0;
            //return convert(LCHuv, RGB(col[1],col[2],col[3]))
			vector<double> luv = RGBtoCIEluv(col[0],col[1],col[2]);
			double C_out, H_out;
			vector<double> out(3,0);
			out[0]=luv[0];
			LuvtoLCHuv(luv[0], luv[1], luv[2], out[1], out[2]);
			return out;
	    }
	}
        
    double alpha=-sin(h/180.0*3.1415926);
    double beta=cos(h/180.0*3.1415926);

    //un &vn are calculated based on reference white (D65)
    //X=0.95047; Y=1.0000; Z=1.08883
    const double un=0.19783982482140777; //4.0X/(X+15.0Y+3.0Z)
    const double vn=0.46833630293240970; //9.0Y/(X+15.0Y+3.0Z)

    //sRGB matrix
	const double M[3][3] = { 
		                     {0.4124564, 0.3575761, 0.1804375},
	                         {0.2126729,  0.7151522,  0.0721750},
				             {0.0193339,  0.1191920,  0.9503041}
	                       };
    const double g=2.4;

    double m_tx=M[t][0];
    double m_ty=M[t][1];
    double m_tz=M[t][2];
    double m_px=M[p][0];
    double m_py=M[p][1];
    double m_pz=M[p][2];

    double f1=(4*alpha*m_px+9*beta*m_py);
    double a1=(4*alpha*m_tx+9*beta*m_ty);
    double f2=(m_px+15*m_py+3*m_pz);
    double a2=(m_tx+15*m_ty+3*m_tz);

    double cp=((alpha*un+beta*vn)*a2-a1)/(f1-(alpha*un+beta*vn)*f2);

    //gamma inversion
    cp = cp <= 0.003 ? 12.92*cp : 1.055*pow(cp,(1.0/g))-0.05;
    //cp = 1.055cp^(1.0/g)-0.05
	
    //col[p]=clamp(cp,0.0,1.0);
	//clamp(x, lo, hi)
    //Return x if lo <= x <= hi. If x < lo, return lo. If x > hi, return hi
	if( 0.0 <= cp && cp <=1.0)
		col[p]=cp;
	else if(cp < 0.0)
		col[p]=0.0;
	else 
		col[p]=1.0;

    col[o]=0.0;
    col[t]=1.0;
	

	/*tVec3 myColor;
	myColor.x = col[0], myColor.y = col[1], myColor.z = col[2];
    
    // convert from linear sRGB to gamma-corrected sRGB
	GammaCompress_sRGB( &myColor );    

	col[0] = myColor.x, col[1]= myColor.y, col[2]== myColor.z;
	*/

	vector<double> luv = RGBtoCIEluv(col[0],col[1],col[2]);
	double C_out, H_out;
	vector<double> out(3,0);

	//luv[1] = luv[1] - 0.19783001, luv[2] = luv[2] - 0.46831998;
		
	out[0]=luv[0];
	LuvtoLCHuv(luv[0], luv[1], luv[2], out[1], out[2]);
	return out;
    //return convert(LCHuv, RGB(col[1],col[2],col[3]));
}


// Maximum saturation for given lightness and hue
// ----------------------
// Maximally saturated color for a specific hue and lightness
// is found by looking for the edge of LCHuv space.

/*function MSC(h,l)
    pmid=MSC(h)

    if l <= pmid.l
        pend=LCHuv(0,0,0)
    elseif l > pmid.l
        pend=LCHuv(100,0,0)
    end

    a=(pend.l-l)/(pend.l-pmid.l)
    a*(pmid.c-pend.c)+pend.c
end
}*/

/*double CIEColor::dividingLCHuvforC(double L, double C1, double C2, double H)
{   
	double u, v;
	vector<double> RGB;

	if(C2-C1<0.001)
	{
		LCHuvtoLuv(L, C1, H, u, v);
	    u = u + 0.19783001, v = v + 0.46831998;
	    // RGB = CIEluvtoRGB(L, u, v);
		double C= (C1 + C2)/2;
		return C;
	}	

	double C= (C1 + C2)/2;
	LCHuvtoLuv(L, C, H, u, v);
	u = u + 0.19783001, v = v + 0.46831998;
	RGB = CIEluvtoRGB(L, u, v);		
	double r=RGB[0], g=RGB[1], b=RGB[2];
	if( (r<0 || r>1) || (g<0 || g>1) || ( b<0 || b>1) )
	{
		return dividingLCHuvforRGB(L, C1, C, H);	
	}
	else
	{
		return dividingLCHuvforRGB(L, C, C2, H);	
	}
}


vector<double> CIEColor::dividingLCHuvforL(double L1, double L2, double H)
{   //for finding largest C
	double u, v, L, C;
	vector<double> RGB;
	vector<double> LC(2,0);

	if(L2-L1<0.001)
	{	
		L= (L1 + L2)/2;
		C = dividingLCHuvforC(L, 0, 200, H);
		LC[0]=L, LC[1]=C;
		return LC;
	}	

	L= (L1 + L2)/2;
	C = dividingLCHuvforC(L, 0, 200, H);
	LC[0]=L, LC[1]=C;
	u = u + 0.19783001, v = v + 0.46831998;
	RGB = CIEluvtoRGB(L, u, v);		
	double r=RGB[0], g=RGB[1], b=RGB[2];
	if( (r<0 || r>1) || (g<0 || g>1) || ( b<0 || b>1) )
	{
		return dividingLCHuvforRGB(L, C1, C, H);	
	}
	else
	{
		return dividingLCHuvforRGB(L, C, C2, H);	
	}
}*/


void CIEColor::LuvtoLCHuv(double L, double u, double v, double &C, double &H)
{
	L = L;
	C = sqrt(u*u+v*v);
	H = atan2(v, u) * 180 / 3.1415926535897;
	//H: 0 - 360
}

void CIEColor::LCHuvtoLuv(double L, double C, double H, double &u, double &v)
{
	H = H/180*3.14159269897;
	L = L;
	u = C * cos (H);
	v = C * sin (H);
}

}
