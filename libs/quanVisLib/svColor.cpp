#include "svColor.h"

namespace __svl_lib{

double D3COLORS[58][3]={
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
  {0.709804,0.811765,0.419608},
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
  {0.780392,0.913725,0.752941},
  {0.458824,0.419608,0.694118},
  {0.619608,0.603922,0.784314},
  {0.737255,0.741176,0.862745},
  {0.854902,0.854902,0.921569},
  {0.388235,0.388235,0.388235},
  {0.588235,0.588235,0.588235},
  {0.741176,0.741176,0.741176},
  {0.85098,0.85098,0.85098 }}; 
  
void svColor::RGB2LAB(double R, double G, double B, double &l, double &a, double &b)
{

	if(fabs(R)<0.00001
		&& fabs(G)<0.00001
		&& fabs(B) <0.00001)
	{
		//cerr<<"00"<<endl;
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

		//Observer. = 2°, Illuminant = D65
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

void svColor::LAB2RGB(double l, double a, double b, double &R, double &G, double &B)
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


void svColor::ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2)
{
	double colorl, colora, colorb;

	RGB2LAB(r,g,b,colorl,colora,colorb);

	//if(fabs(0.168741-r)<0.001)
	//	cerr<<r<<" "<<g<<" "<<b<<" "<<colorl<<" "<<colora<<" "<<colorb<<" ";
		
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

	//phi = phi_maxl * (1 - abs(atan(tmpb/tmpa))/(3.1415926/2));
	phi = phi_max * (1 - pow(abs((float)theta)/(3.1415926/2),gama));
	colora = tmpa * cos(phi) - tmpb * sin(phi);
	colorb = tmpa * sin(phi) + tmpb * cos(phi);	


	LAB2RGB(colorl, colora, colorb, r2,g2,b2);

	if(r2<0) r2=0;
	if(g2<0) g2=0;
	if(b2<0) b2=0;
	if(r2>1) r2=1;
	if(g2>1) g2=1;
	if(b2>1) b2=1;

	//if(fabs(0.168741-r)<0.001)
	//	cerr<<phi<<" "<<colorl<<" "<<colora<<" "<<colorb<<" "<<r2<<" "<<g2<<" "<<b2<<endl;

}


svColor::svColor()
{
  dataSize = 0;
  spiral_level = 0;
  d3_level = 0;
  dataVel = NULL;
  range[0] = NULL;
  range[1] = NULL;
  spiralcolor = NULL;
  d3color = NULL;
  
  font_color[0] = 1;
  font_color[1] = 1;
  font_color[2] = 1;
}

void svColor::setHC(vector< vector<double> > mag)
{
	for(int i=0;i<hc_magrange.size();i++)
	{	
		hc_magrange[i].clear();
	}
	if(hc_magrange.size()>0)
	{
		hc_magrange.clear();
	}

	int n = mag.size();
	hc_magrange.resize(n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<mag[i].size();j++)
		{
			hc_magrange[i].push_back(mag[i][j]);
		}
		hc_index.push_back(false);
	}
}

svVector4 * svColor::getColors(bool toggleBlind, bool toggle, 
				int level, int *cluster, int size,
				svVector3 font_color)
{
	svVector4 *color = new svVector4[size];
	if(toggle)
	{
		if(!toggleBlind)
			for(int i=0;i<size;i++)
			{
				//cerr<<i<<" "<<size<<" "<<cluster[i]<<endl;
				if(cluster[i]>=0)
					color[i] = d3color[cluster[i]%level];
				else
				{
					color[i][0] = font_color[0];
					color[i][1] = font_color[1];
					color[i][2] = font_color[2];
				}
			}
		else
			for(int i=0;i<size;i++)
			{
				if(cluster[i]>=0)
					color[i] = d3color_blindsafe[cluster[i]%level];
				else
				{
					color[i][0] = font_color[0];
					color[i][1] = font_color[1];
					color[i][2] = font_color[2];
				}
			}
	}
	else
	{
		if(!toggleBlind)
			for(int i=0;i<size;i++)
			{	
				if(cluster[i]>=0)
					color[i] = spiralcolor[cluster[i] * (1000/(level-1))];
				else
				{
					color[i][0] = font_color[0];
					color[i][1] = font_color[1];
					color[i][2] = font_color[2];
				}
			}
		else
			for(int i=0;i<size;i++)
			{	
				if(cluster[i]>=0)
					color[i] = spiralcolor_blindsafe[cluster[i] * (1000/(level-1))];
				else
				{
					color[i][0] = font_color[0];
					color[i][1] = font_color[1];
					color[i][2] = font_color[2];
				}
			}
	}
	return color;
}

svVector4 * svColor::getColors(bool toggleBlind, int *cluster, svVector3 font_color, int size)
{
	svVector4 *color = new svVector4[size];

		if(!toggleBlind)
		{
			for(int i=0;i<size;i++)
			{
				//cerr<<i<<" "<<size<<" "<<cluster[i]<<endl;
				if(cluster[i]>=0)
					color[i] = d3color[cluster[i]];
				else
				{
					color[i][0] = font_color[0];
					color[i][1] = font_color[1];
					color[i][2] = font_color[2];
				}
			}
		}
		else
		{	for(int i=0;i<size;i++)
			{
				if(cluster[i]>=0)
					color[i] = d3color_blindsafe[cluster[i]];
				else
				{
					color[i][0] = font_color[0];
					color[i][1] = font_color[1];
					color[i][2] = font_color[2];
				}
			}
		}

	return color;
}

svVector4 * svColor::getColors(bool toggleBlind,vector< vector<int> > cluster, svVector3 font_color, int size)
{
	svVector4 *color = new svVector4[size];

	int count = 0;
		if(!toggleBlind)
		{
			for(int i=0;i<cluster.size();i++)
			{
				for(int j=0;j<cluster[i].size();j++)
				{
					//cerr<<i<<" "<<size<<" "<<cluster[i]<<endl;
					if(cluster[i][j]>=0)
						color[count] = d3color[cluster[i][j]];
					else
					{
						color[count][0] = font_color[0];
						color[count][1] = font_color[1];
						color[count][2] = font_color[2];
					}
					count++;
				}
			}
		}
		else
		{	for(int i=0;i<cluster.size();i++)
			{
				for(int j=0;j<cluster[i].size();j++)
				{
					if(cluster[i][j]>=0)
						color[count] = d3color_blindsafe[cluster[i][j]];
					else
					{
						color[count][0] = font_color[0];
						color[count][1] = font_color[1];
						color[count][2] = font_color[2];
					}
					count++;
				}
			}
		}

	return color;
}
svVector4 * svColor::getColors(bool toggleBlind, bool toggle, int size)
{
	svVector4 *color = new svVector4[size];
	if(toggle)
	{
		if(!toggleBlind)
			for(int i=0;i<size;i++)
			{
				color[i] = d3color[i];
				//cerr<<i<<" "<<color[i][0]<<" "<<color[i][1]<<" "<<color[i][2]<<endl;
			}
		else
			for(int i=0;i<size;i++)
			{
				color[i] = d3color_blindsafe[i];
			}
	}
	else
	{
		if(!toggleBlind)
			for(int i=0;i<size;i++)
			{	
				color[i] = spiralcolor[i*(1000/(size-2))];
			}
		else
			for(int i=0;i<size;i++)
			{	
				color[i] = spiralcolor_blindsafe[i*(1000/(size-2))];
			}
	}
	return color;
}
void svColor::SetSpiralColor()
{ 

 spiral_level = 1001;
 spiralcolor = new svVector4[spiral_level];
 spiralcolor_blindsafe = new svVector4[spiral_level];
 
 CIEColor *c;
 
 double t; 
 double s = 1.;
 double b = 0.88;
 double cc = 0.5;
 double h1;
 double h2;
 double w = 0.15;
 
 for(int i=0;i<spiral_level;i++)
 {
  if(spiral_level>1)
  {
   t = (float)i/(float)(spiral_level -1);
   h2 = (float)i/(spiral_level-1) * 360;//M_PI * 2;
  }
  else
  {
	t = 0;
	h2 = 0;
  }
	//cerr<<t<<" "<<h2<<endl;
   
   vector<double> rgb(3);
   rgb = c->MultiplehueSequential(t, s, b, cc, h1, h2, w);
   
   spiralcolor [i][0] = rgb[0];
   spiralcolor [i][1] = rgb[1];
   spiralcolor [i][2] = rgb[2];
   spiralcolor [i][3] = 255.;

	//cerr<<rgb[0]<<" "<<rgb[1]<<" "<<rgb[2]<<endl;
 }
 
 //delete c;
 //cerr<<"---"<<endl;
 for(int i=0;i<spiral_level;i++)
 {
   double R,G,B, r,g,b;
   R = spiralcolor[i][0];
   G = spiralcolor[i][1];
   B = spiralcolor[i][2];
   
   ColorBlindSafe(R,G,B,r,g,b);
   
   spiralcolor_blindsafe[i][0]= r;
   spiralcolor_blindsafe[i][1]= g;
   spiralcolor_blindsafe[i][2]= b;
   spiralcolor_blindsafe[i][3]= 255;   

	//cerr<<r<<" "<<g<<" "<<b<<endl;
 }


 //delete c;
}
void svColor::SetSpiralColor(int level, double *range_min, double *range_max)
{ 
//cerr<<"Set Spiral Color"<<endl;
  if(spiral_level >0)
  {
    delete [] range[0];
    delete [] range[1];
    delete [] spiralcolor;
    delete [] spiralcolor_blindsafe;
  }
  //cerr<<"Set Spiral Color"<<endl;
 spiral_level = level;
 range[0] = new svScalar[spiral_level];
 range[1] = new svScalar[spiral_level];
 spiralcolor = new svVector4[spiral_level];
 spiralcolor_blindsafe = new svVector4[spiral_level];
 
 for(int i=0;i<spiral_level;i++)
 {
   if(i==0)
   {
    range[0][i] = range_min[i];
    range[1][i] = range_max[i];
   }
   else
   {
     range[0][i] = range_max[i-1];
     range[1][i] = range_max[i];
   }
  //cerr<<range[0][i]<<" "<<range[1][i]<<endl;
 }
 
 CIEColor *c;
 
 double t; 
 double s = 1.;
 double b = 0.88;
 double cc = 0.5;
 double h1;
 double h2;
 double w = 0.15;
 
 for(int i=0;i<spiral_level;i++)
 {
  if(spiral_level>1)
  {
   t = (float)i/(float)(spiral_level -1);
   h2 = (float)i/(spiral_level-1) * 360;//M_PI * 2;
  }
  else
  {
	t = 0;
	h2 = 0;
  }
	//cerr<<t<<" "<<h2<<endl;
   
   vector<double> rgb(3);
   rgb = c->MultiplehueSequential(t, s, b, cc, h1, h2, w);
   
   spiralcolor [i][0] = rgb[0];
   spiralcolor [i][1] = rgb[1];
   spiralcolor [i][2] = rgb[2];
   spiralcolor [i][3] = 255.;

	//cerr<<rgb[0]<<" "<<rgb[1]<<" "<<rgb[2]<<endl;
 }
 
 //delete c;
 //cerr<<"---"<<endl;
 for(int i=0;i<spiral_level;i++)
 {
   double R,G,B, r,g,b;
   R = spiralcolor[i][0];
   G = spiralcolor[i][1];
   B = spiralcolor[i][2];
   
   ColorBlindSafe(R,G,B,r,g,b);
   
   spiralcolor_blindsafe[i][0]= r;
   spiralcolor_blindsafe[i][1]= g;
   spiralcolor_blindsafe[i][2]= b;
   spiralcolor_blindsafe[i][3]= 255;   

	//cerr<<r<<" "<<g<<" "<<b<<endl;
 }


 //delete c;
}
void svColor::SetD3Color()
{
  if(d3_level > 0)
  {
    delete [] d3color;
    delete [] d3color_blindsafe;
  }

  d3_level = 58;
  d3color = new svVector4[d3_level];
  d3color_blindsafe = new svVector4[d3_level];
  
  for(int i=0;i<d3_level;i++)
  {
    d3color[i][0] = D3COLORS[i][0];
    d3color[i][1] = D3COLORS[i][1];
    d3color[i][2] = D3COLORS[i][2];
  }
  
  for(int i=0;i<d3_level;i++)
 {
   double R,G,B, r,g,b;
   R = d3color[i][0];
   G = d3color[i][1];
   B = d3color[i][2];
   
   ColorBlindSafe(R,G,B,r,g,b);
   
   d3color_blindsafe[i][0]= r;
   d3color_blindsafe[i][1]= g;
   d3color_blindsafe[i][2]= b;
 } 

}
void svColor::SetD3Color(int *cluster, svVector3 *vel, int level, int num)
{
  if(dataSize > 0)
  {
   delete [] dataVel;
   delete [] dataCol;
  }

  if(d3_level > 0)
  {
    delete [] d3color;
    delete [] d3color_blindsafe;
  }
   
	
  dataSize = num;
  dataVel = new svVector3[dataSize];
  dataCol = new svInt[dataSize];
 
  d3_level = level;
  d3color = new svVector4[d3_level];
  d3color_blindsafe = new svVector4[d3_level];
  
  for(int i=0;i<d3_level;i++)
  {
    d3color[i][0] = D3COLORS[i][0];
    d3color[i][1] = D3COLORS[i][1];
    d3color[i][2] = D3COLORS[i][2];
  }
  
  for(int i=0;i<d3_level;i++)
 {
   double R,G,B, r,g,b;
   R = d3color[i][0];
   G = d3color[i][1];
   B = d3color[i][2];
   
   ColorBlindSafe(R,G,B,r,g,b);
   
   d3color_blindsafe[i][0]= r;
   d3color_blindsafe[i][1]= g;
   d3color_blindsafe[i][2]= b;
 } 


    for(int i=0;i<num;i++)
    {
      dataCol[i] = cluster[i];
      dataVel[i] = vel[i];
    }
 
  
}


void svColor::GetSpiralColor(int *cluster, svVector4 * color, int num, bool blindsafe)
{
  if(blindsafe)
  {
    for(int i=0;i<num;i++)
    {
	//cerr<<i<<" "<<cluster[i]<<endl;
	//cerr<<i<<" "<<spiralcolor_blindsafe[cluster[i]][0]<<endl;
      color[i] = spiralcolor_blindsafe[cluster[i]];
	//cerr<<num<<" "<<i<<" "<<color[i][0]<<endl;
    }
  }
  else
  {
    for(int i=0;i<num;i++)
    {
      color[i] = spiralcolor[cluster[i]];
    }
  }   
	//cerr<<"=============="<<endl; 
}

void svColor::GetD3Color(int *cluster, svVector4 *color, int num, bool blindsafe)
{


  
  if(blindsafe)
  {
    for(int i=0;i<num;i++)
    {
      color[i] = d3color_blindsafe[cluster[i]];
    }
  }
  else
  {
    for(int i=0;i<num;i++)
    {
      color[i] = d3color[cluster[i]];

    }
  }    
}

void svColor::RenderSpiralColorLegend(bool blindsafe, float scalar)
{
	glPushMatrix();

	char str[50];
	float x = 0;
	float y = 0;
	float size = 1;	
	float m_length_in = 1;

	glColor3f(font_color[0],font_color[1],font_color[2]);

	double index;

	for(int i=0;i<spiral_level;i++)
	{
			if(blindsafe)
			{
			  glColor3f(spiralcolor_blindsafe[i][0],
				    spiralcolor_blindsafe[i][1],
				    spiralcolor_blindsafe[i][2]);
			}
			else
			{
			  glColor3f(spiralcolor[i][0],
				    spiralcolor[i][1],
				    spiralcolor[i][2]);
			}

			glBegin(GL_QUADS);
			glVertex2f(x,y);
			glVertex2f(x+size,y);
			glVertex2f(x+size,y+size);
			glVertex2f(x,y+size);
			glEnd();

			glColor3f(font_color[0],font_color[1],font_color[2]);
	
			sprintf(str,"%6.2e - %6.2e",range[0][i], range[1][i]);
			glRasterPos2f(x + size*1.1, y );
			for(int j=0;j<strlen(str);j++)
			     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

			sprintf(str,"%d",spiral_level-1-i);
			glRasterPos2f(x - size*1.3, y);
			for(int j=0;j<strlen(str);j++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

			y = y + size;
	
	}

	glPopMatrix();
	//}
}

void svColor::RenderHCLegend(bool blindsafe, bool enable_color)
{
	glPushMatrix();

	char str[50];
	float x = 0;
	float y = 0;
	float size = 1;	
	float m_length_in = 1;

	for(int i=0;i<hc_index.size();i++)
	{
			if(blindsafe)
			{
			  glColor3f(d3color_blindsafe[i][0],
				    d3color_blindsafe[i][1],
				    d3color_blindsafe[i][2]);
			}
			else
			{
			  glColor3f(d3color[i][0],
				    d3color[i][1],
				    d3color[i][2]);
			}

			if(enable_color)
			{
				glBegin(GL_QUADS);
				glVertex2f(x,y);
				glVertex2f(x+size,y);
				glVertex2f(x+size,y+size);
				glVertex2f(x,y+size);
				glEnd();
			}

			glColor3f(font_color[0],font_color[1],font_color[2]);
			
			if(hc_index[i])
			{
				glBegin(GL_QUADS);
				glVertex2f(x-2*size,y);
				glVertex2f(x,y);
				glVertex2f(x,y+size);
				glVertex2f(x-2*size,y+size);
				glEnd();
			}
			else
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(x-2*size,y);
				glVertex2f(x,y);
				glVertex2f(x,y+size);
				glVertex2f(x-2*size,y+size);
				glEnd();
			}

			sprintf(str,"%6.2e - %6.2e",hc_magrange[i][0],hc_magrange[i][1]);
			glRasterPos2f(x + size*1.1, y );
			for(int j=0;j<strlen(str);j++)
			     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
			y = y + size;
	}

	glPopMatrix();
}

void svColor::RenderD3ColorLegend(bool blindsafe, int selected_p)
{
	glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);
	
	glColor3f(font_color[0],font_color[1],font_color[2]);

        glBegin(GL_LINES);
        glVertex3f(-1.2,0,0);
        glVertex3f(1.2,0,0);
        glEnd();
        glRasterPos3f(1.2,0,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

	glPushMatrix();
	glTranslatef(1.2,0.,0.);
	glRotatef(90,0.,1.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,-1.2,0);
        glVertex3f(0,1.2,0);
        glEnd();
        glRasterPos3f(0,1.2,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');

	glPushMatrix();
	glTranslatef(0.,1.2,0.);
	glRotatef(-90,1.,0.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,0,-1.2);
        glVertex3f(0,0,1.2);
        glEnd();
        glRasterPos3f(0,0,1.2);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');

	glPushMatrix();
	glTranslatef(0.,0.,1.2);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

	//glScalef(20.,20.,20.);
	glPointSize(2.);
	char str[10];
	//int dataSize = qdotField->GetDataSize();

	for(int i=0;i<dataSize;i++)
	{
		if(blindsafe)
		{
		  glColor3f(d3color_blindsafe[dataCol[i]][0],
			  d3color_blindsafe[dataCol[i]][1],
			    d3color_blindsafe[dataCol[i]][2]);
		}
		else
		{
		  glColor3f(d3color[dataCol[i]][0],
			  d3color[dataCol[i]][1],
			    d3color[dataCol[i]][2]);
		}

		glBegin(GL_POINTS);
		glVertex3f(dataVel[i][0],dataVel[i][1],dataVel[i][2]);
		glEnd();
		
	}

	if(selected_p>=0 && selected_p<dataSize)
	{
		glColor3f(0,0,0);
		glBegin(GL_LINES);
		glVertex3f(dataVel[selected_p][0], dataVel[selected_p][1], dataVel[selected_p][2]);
		glVertex3f(dataVel[selected_p][0]*1.5, dataVel[selected_p][1]*1.5, dataVel[selected_p][2]*1.5);	
		glEnd();
	}

	glPointSize(1.);  
}
void svColor::RenderD3ColorLegend(svVector3 *vel, int *cluster, int size, bool blindsafe)
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);
	
	glColor3f(font_color[0],font_color[1],font_color[2]);

        glBegin(GL_LINES);
        glVertex3f(-1.2,0,0);
        glVertex3f(1.2,0,0);
        glEnd();
        glRasterPos3f(1.2,0,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

	glPushMatrix();
	glTranslatef(1.2,0.,0.);
	glRotatef(90,0.,1.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,-1.2,0);
        glVertex3f(0,1.2,0);
        glEnd();
        glRasterPos3f(0,1.2,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');

	glPushMatrix();
	glTranslatef(0.,1.2,0.);
	glRotatef(-90,1.,0.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,0,-1.2);
        glVertex3f(0,0,1.2);
        glEnd();
        glRasterPos3f(0,0,1.2);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');

	glPushMatrix();
	glTranslatef(0.,0.,1.2);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

	//glScalef(20.,20.,20.);
	glPointSize(2.);
	char str[10];
	//int dataSize = qdotField->GetDataSize();

	for(int i=0;i<size;i++)
	{//cerr<<i<<" "<<size<<" "<<cluster[i]<<endl;
		if(blindsafe)
		{
		  glColor3f(d3color_blindsafe[cluster[i]][0],
			  d3color_blindsafe[cluster[i]][1],
			    d3color_blindsafe[cluster[i]][2]);
		}
		else
		{
		  glColor3f(d3color[cluster[i]][0],
			  d3color[cluster[i]][1],
			    d3color[cluster[i]][2]);
		}

		glBegin(GL_POINTS);
		glVertex3f(vel[i][0],vel[i][1],vel[i][2]);
		glEnd();
		
	}


	glPointSize(1.);  
}
void svColor::Mouse(int mx, int my)
{
	float x = 0;
	float y = 0;
	float size = 1;	

	for(int i=0;i<hc_index.size();i++)
	{
		if(mx > x -2*size && x < 0
		&& my >y && my < y + size)
		{
			if(hc_index[i])
			{
				hc_index[i] = false;
			}
			else
			{
				hc_index[i] = true;
			}
		}
		y = y + size;
	}
}
void svColor::cleanup()
{
  delete [] dataVel;
  delete [] dataCol;
  delete [] range[0];
  delete [] range[1];
  delete [] spiralcolor;
  delete [] spiralcolor_blindsafe;
  delete [] d3color;
  delete [] d3color_blindsafe;
  

	for(int i=0;i<hc_magrange.size();i++)
	{	
		hc_magrange[i].clear();
	}
	if(hc_magrange.size()>0)
	{
		hc_magrange.clear();
	}
	hc_index.clear();
  dataSize = 0;
}
  

}
