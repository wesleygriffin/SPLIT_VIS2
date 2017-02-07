#include "svSlice.h"
#include "svUtil.h"
#include <string.h>
#include <cmath>
#include <algorithm>


using namespace std;

namespace __svl_lib{

svSlice::svSlice(svVector3 l, svVector3 r)
{
	font_color[0] = 1;
	font_color[1] = 1;
	font_color[2] = 1;

	SetData(l, r);
}

svSlice::svSlice(svVector3 l, svVector3 r,
		vector<svVector3> pv, vector<svVector3> pn)
{
	font_color[0] = 1;
	font_color[1] = 1;
	font_color[2] = 1;

	SetData(l, r);
	SetPlane(pv, pn);	
}

void svSlice::SetData(svVector3 l, svVector3 r)
{
	lb = l;
	rb = r;

	widgetl = (rb[0] - lb[0])/2.;
}

void svSlice::SetPlane(vector<svVector3> w, vector<svVector3> v)
{
	plane_vector.clear();
	plane_normal.clear();
	widgetp.clear();
	NEAREST.clear();
	click_selection.clear();
	for(int i=0;i<intersectionp.size();i++)
		intersectionp[i].clear();
	intersectionp.clear();
	intersectionnum.clear();

//----------------------------------------------------------------------
	intersectionp.resize(w.size());
	intersectionnum.resize(w.size());
	click_selection.resize(w.size());
	SELECTED = GL_FALSE;
	for(int i=0;i<w.size();i++)
	{
		plane_vector.push_back(w[i]);	
		plane_normal.push_back(v[i]);
		plane_normal[i] = normalize(v[i]);

		svVector3 tw = plane_normal[i];// * (svScalar)widgetl
				//+ plane_vector[i];
		widgetp.push_back(w[i]); 

		intersectionp[i].resize(6);
		intersectionnum[i] = 0;

		NEAREST.push_back(false);
		click_selection[i] = false;
	
		/*svVector3 c;
		c[0] = 0;
		c[1] = 0;
		c[2] = 0;
		for(int j=0;j<intersectionnum[i];j++)
		{
			c[0] = c[0] + intersectionp[i][j][0];
			c[1] = c[1] + intersectionp[i][j][1];
			c[2] = c[2] + intersectionp[i][j][2];			
		}
		plane_vector[i][0] = c[0]/intersectionnum[i];
		plane_vector[i][1] = c[1]/intersectionnum[i];
		plane_vector[i][2] = c[2]/intersectionnum[i];
		widgetp[i] = plane_vector[i];*/
	}

	Intersection();
}

void svSlice::SetPlane(svVector3 w, svVector3 v, double d)
{
	plane_vector.clear();
	plane_normal.clear();
	widgetp.clear();
	NEAREST.clear();
	click_selection.clear();
	for(int i=0;i<intersectionp.size();i++)
		intersectionp[i].clear();
	intersectionp.clear();
	intersectionnum.clear();

//----------------------------------------------------------------------
	svVector3 min_p = w;
	svVector3 max_p = w;
	while(1)
	{
		min_p[0] = min_p[0] - v[0] * d;
		min_p[1] = min_p[1] - v[1] * d;
		min_p[2] = min_p[2] - v[2] * d;

		svVector3 A,B, dd1, dd2;
		bool flag = planeBox(lb, rb, min_p ,v);
		
		//cout<<min_p[0]<<" "<<min_p[1]<<" "<<min_p[2]<<" "<<plane_d<<" "<<flag<<endl;

		if(flag == false)
		{
			min_p[0] = min_p[0] + v[0] * d;
			min_p[1] = min_p[1] + v[1] * d;
			min_p[2] = min_p[2] + v[2] * d;
			break;
		}
	}	
	while(1)
	{
		max_p[0] = max_p[0] + v[0] * d;
		max_p[1] = max_p[1] + v[1] * d;
		max_p[2] = max_p[2] + v[2] * d;

		svVector3 A,B, dd1, dd2;
		bool flag = planeBox(lb, rb, max_p ,v);
		
		if(flag == false)
		{ 
			max_p[0] = max_p[0] - v[0] * d;
			max_p[1] = max_p[1] - v[1] * d;
			max_p[2] = max_p[2] - v[2] * d;
		break;}
	}
	int n = sqrt((max_p[0] - min_p[0]) * (max_p[0] - min_p[0])
		+ (max_p[1] - min_p[1]) * (max_p[1] - min_p[1])
		+ (max_p[2] - min_p[2]) * (max_p[2] - min_p[2]))/d + 1;	
	
	intersectionp.resize(n);
	intersectionnum.resize(n);
	click_selection.resize(n);
	SELECTED = GL_FALSE;
	for(int i=0;i<n;i++)
	{
		plane_vector.push_back(min_p);	
		plane_normal.push_back(v);
		plane_normal[i] = normalize(v);

		svVector3 tw = plane_normal[i];// * (svScalar)widgetl
				//+ plane_vector[i];
		widgetp.push_back(min_p); 

		intersectionp[i].resize(6);
		intersectionnum[i] = 0;

		NEAREST.push_back(false);
		click_selection[i] = false;

		if(fabs(min_p[0]-w[0])<1e-9
		&& fabs(min_p[1]-w[1])<1e-9
		&& fabs(min_p[2]-w[2])<1e-9)
		{
			current_plane = i;
		}

		min_p[0] = min_p[0] + v[0] * d;
		min_p[1] = min_p[1] + v[1] * d;
		min_p[2] = min_p[2] + v[2] * d;

	
		/*svVector3 c;
		c[0] = 0;
		c[1] = 0;
		c[2] = 0;
		for(int j=0;j<intersectionnum[i];j++)
		{
			c[0] = c[0] + intersectionp[i][j][0];
			c[1] = c[1] + intersectionp[i][j][1];
			c[2] = c[2] + intersectionp[i][j][2];			
		}
		plane_vector[i][0] = c[0]/intersectionnum[i];
		plane_vector[i][1] = c[1]/intersectionnum[i];
		plane_vector[i][2] = c[2]/intersectionnum[i];
		widgetp[i] = plane_vector[i];*/
	}

	click_selection[current_plane] = true;

	Intersection();
}
void svSlice::setSelected(vector<int> s, vector< vector<int> > s2)
{

	for(int i=0;i<plane_vector.size();i++)
	{
		click_selection[i] = false;
	}

	for(int i=0;i<s.size();i++)
	{
		click_selection[s[i]] = true;
	}

	for(int i=0;i<s2.size();i++)
	{
		for(int j=s2[i][0];j<=s2[i][1];j++)
		{
			if(j < plane_vector.size())
				click_selection[j] = true;
		}//cerr<<s2[i][0]<<" "<<s2[i][1]<<" "<<plane_vector.size()<<endl;
	}
	
	/*for(int i=0;i<plane_vector.size();i++)
	{
		cerr<<click_selection[i]<<" ";
	}cerr<<endl;
	*/
}
void svSlice::setCurrentPlane()
{
	for(int i=0;i<plane_vector.size();i++)
	{
		click_selection[i] = true;
	}
}
void svSlice::setCurrentPlane(float d)
{
	for(int i=0;i<plane_vector.size();i++)
	{
		click_selection[i] = false;
	}

	current_plane = d * plane_vector.size();
	click_selection[current_plane] = true;
}
void svSlice::RenderSlice()
{
    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glColor3f(font_color[0], font_color[1], font_color[2]);

	for(int ii=0;ii<plane_vector.size();ii++)
	{
		int i = ii;//plane_vector[ii];

		glBegin(GL_LINES);
		glVertex3f(widgetp[i][0],widgetp[i][1],widgetp[i][2]);
		glVertex3f(widgetp[i][0] + plane_normal[i][0] * widgetl,
				widgetp[i][1] + plane_normal[i][1] * widgetl,
				widgetp[i][2] + plane_normal[i][2] * widgetl);
		glEnd();

		if(click_selection[i])
			glColor4f(227./255.,74./255.,51./255.,0.5);
		else
			glColor4f(font_color[0], font_color[1], font_color[2],0.5);

		glBegin(GL_LINE_LOOP);
		for(int j=0;j<intersectionnum[i];j++)
			glVertex3f(intersectionp[i][j][0],intersectionp[i][j][1],intersectionp[i][j][2]);
		glEnd();

		if(ii%5 == 0 ||ii==plane_vector.size()-1)
		{
			char str[10];
			sprintf(str,"%d",i);
			glRasterPos3f(intersectionp[i][0][0]-0.5,intersectionp[i][0][1]-0.5,intersectionp[i][0][2]);
			for(int j=0;j<strlen(str);j++)
			    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		}
	}
}
void svSlice::RenderSlice(vector<int> plane)
{
    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glColor3f(font_color[0], font_color[1], font_color[2]);

	for(int ii=0;ii<plane.size();ii++)
	{
		int i = plane[ii];

		glBegin(GL_LINES);
		glVertex3f(widgetp[i][0],widgetp[i][1],widgetp[i][2]);
		glVertex3f(widgetp[i][0] + plane_normal[i][0] * widgetl,
				widgetp[i][1] + plane_normal[i][1] * widgetl,
				widgetp[i][2] + plane_normal[i][2] * widgetl);
		glEnd();

		if(click_selection[i])
			glColor3f(227./255.,74./255.,51./255.);
		else
			glColor3f(font_color[0], font_color[1], font_color[2]);

		glBegin(GL_LINE_LOOP);
		for(int j=0;j<intersectionnum[i];j++)
			glVertex3f(intersectionp[i][j][0],intersectionp[i][j][1],intersectionp[i][j][2]);
		glEnd();
	}
}
void svSlice::RenderSlice(bool two)
{


    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glColor3f(font_color[0], font_color[1], font_color[2]);

	for(int i=0;i<plane_vector.size();i++)
	{
		glBegin(GL_LINES);
		glVertex3f(widgetp[i][0],widgetp[i][1],widgetp[i][2]);
		glVertex3f(widgetp[i][0] + plane_normal[i][0] * widgetl,
				widgetp[i][1] + plane_normal[i][1] * widgetl,
				widgetp[i][2] + plane_normal[i][2] * widgetl);
		glEnd();


		glBegin(GL_LINE_LOOP);
		for(int j=0;j<intersectionnum[i];j++)
			glVertex3f(intersectionp[i][j][0],intersectionp[i][j][1],intersectionp[i][j][2]);
		glEnd();

		glColor4f(font_color[0], font_color[1], font_color[2], 0.25);
		if(SELECTED && ni == i && nj == 1)
		{
			glBegin(GL_POLYGON);
			for(int j=0;j<intersectionnum[i];j++)
				glVertex3f(intersectionp[i][j][0],intersectionp[i][j][1],intersectionp[i][j][2]);
			glEnd();			
		}
	}

//-----------spheres------------------
/*	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	
	glColor4f(font_color[0], font_color[1], font_color[2], 0.5);
	for(int i=0;i<plane_vector.size();i++)
	{
		glPushMatrix();
		if(SELECTED && ni == i && nj == 0)
		{
			glColor4f(1,1,0,0.5);			
		}
		else
			glColor4f(font_color[0], font_color[1], font_color[2], 0.5);
		glTranslatef(widgetp[i][0] + plane_normal[i][0] * widgetl,
				widgetp[i][1] + plane_normal[i][1] * widgetl,
				widgetp[i][2] + plane_normal[i][2] * widgetl);
		glutSolidSphere(1.,10.,10.);
		glPopMatrix();
	}
*/
}

//checkpoint

svVector3 incenter;
svVector3 indir;
bool compare(svVector3 lhs, svVector3 rhs)
{
	svVector3 u = lhs - incenter;
	svVector3 v = rhs - incenter;
        svVector3 c;

	c[0]=u[1]*v[2] - u[2]*v[1];
	c[1]=u[2]*v[0] - u[0]*v[2];
	c[2]=u[0]*v[1] - u[1]*v[0];

	c = normalize(c);

        float det;
	det = indir[0]*c[0]+indir[1]*c[1]+indir[2]*c[2];
	
	return det<0;
}
void svSlice::Intersection()
{
	svVector3 l0,l;
	svVector3 n,p0;
	float d;
	svVector3 p[12];
	svVector3 tmpp[6];
	int j=0;
	svVector3 out;
	out[0] = rb[0] + 10.;
	out[1] = rb[1] + 10.;
	out[2] = rb[2] + 10.;

	for(int t=0;t<plane_vector.size();t++)
	{
		for(int i=0;i<6;i++)
		{
			tmpp[i][0] = 0;
			tmpp[i][1] = 0;
			tmpp[i][2] = 0;
		}

		n = normalize(plane_normal[t]);
		p0 = widgetp[t];

		l[0]=0.;l[1]=0.;l[2]=1.;
		l0 = lb;		
		if(GetDot(l,n)!=0)
		{
			d = GetDot((p0 - l0),n)/GetDot(l,n);
			p[0] = d *l +l0;
		}
		else
			p[0] = out;

		l0[0] = lb[0];l0[1]=rb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[1] = d *l +l0;
		}
		else
			p[1] = out;

		l0[0] = rb[0];l0[1]=rb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[2] = d *l +l0;
		}
		else
			p[2] = out;

		l0[0] = rb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[3] = d *l +l0;
		}
		else
			p[3] = out;

		l[0]=0.;l[1]=1.;l[2]=0.;
		l0[0] = lb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[4] = d *l +l0;
		}
		else
			p[4] = out;

		l0[0] = rb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[5] = d *l +l0;
		}
		else
			p[5] = out;
		////cerr<<t<<" "<<d<<endl;
		//cerr<<t<<" "<<p[5][0]<<" "<<p[5][1]<<" "<<p[5][2]<<endl;

		l0[0] = rb[0];l0[1]=lb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[6] = d *l +l0;
		}
		else
			p[6] = out;

		l0[0] = lb[0];l0[1]=lb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[7] = d *l +l0;
		}
		else
			p[7] = out;

		l[0]=1.;l[1]=0.;l[2]=0.;
		l0[0] = lb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[8] = d *l +l0;
		}
		else
			p[8] = out;

		l0[0] = lb[0];l0[1]=rb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[9] = d *l +l0;
		}
		else
			p[9] = out;

		l0[0] = lb[0];l0[1]=rb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[10] = d *l +l0;
		}
		else
			p[10] = out;

		l0[0] = lb[0];l0[1]=lb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[11] = d *l +l0;
		}
		else
			p[11] = out;

		j = 0;
		//cerr<<"========="<<endl;
		for(int i=0;i<12;i++)
		{
			bool flag = false;
			if(p[i][0]>=lb[0]&&p[i][0]<=rb[0]
			&&p[i][1]>=lb[1]&&p[i][1]<=rb[1]
			&&p[i][2]>=lb[2]&&p[i][2]<=rb[2])
			{
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-lb[0])<0.0001
					&& fabs(p[i][1]-lb[1])<0.0001
					&& fabs(p[i][2]-lb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-rb[0])<0.0001
					&& fabs(p[i][1]-lb[1])<0.0001
					&& fabs(p[i][2]-lb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-lb[0])<0.0001
					&& fabs(p[i][1]-rb[1])<0.0001
					&& fabs(p[i][2]-lb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-rb[0])<0.0001
					&& fabs(p[i][1]-rb[1])<0.0001
					&& fabs(p[i][2]-lb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-lb[0])<0.0001
					&& fabs(p[i][1]-lb[1])<0.0001
					&& fabs(p[i][2]-rb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-rb[0])<0.0001
					&& fabs(p[i][1]-lb[1])<0.0001
					&& fabs(p[i][2]-rb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-lb[0])<0.0001
					&& fabs(p[i][1]-rb[1])<0.0001
					&& fabs(p[i][2]-rb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			else if(fabs(p[i][0]-rb[0])<0.0001
					&& fabs(p[i][1]-rb[1])<0.0001
					&& fabs(p[i][2]-rb[2])<0.0001)
			{					
					for(int kk=0;kk<j;kk++)
					{
						if(fabs(p[i][0]-tmpp[kk][0])<0.0001
						&& fabs(p[i][1]-tmpp[kk][1])<0.0001
						&& fabs(p[i][2]-tmpp[kk][2])<0.0001)
						{
							flag = true;break;
						}
					}				
					if(!flag)
					{	
						tmpp[j]=p[i];
						j++;
					}
			}
			incenter = tmpp[0];
			indir = n;
			std::sort(tmpp, tmpp+j, compare);

			intersectionnum[t] = j;
			j = 0;
			for(int i=0;i<intersectionnum[t];i++)
			{
				intersectionp[t][i] = tmpp[j];
				j ++;
			}

		}

		/*svVector3 c;
		c[0] = 0;
		c[1] = 0;
		c[2] = 0;
		for(int i=0;i<intersectionnum[t];i++)
		{
			c[0] = c[0] + intersectionp[t][i][0];
			c[1] = c[1] + intersectionp[t][i][1];
			c[2] = c[2] + intersectionp[t][i][2];			
		}
		plane_vector[t][0] = c[0]/intersectionnum[t];
		plane_vector[t][1] = c[1]/intersectionnum[t];
		plane_vector[t][2] = c[2]/intersectionnum[t];
		widgetp[t] = plane_vector[t];*/
	}
	
}
/*
void svSlice::SetRegularGrid()
{
	if(dir[0]>dir[1] && dir[0]>dir[2])
	{
			dir[0] = 1; dir[1] = 0; dir[2] = 0;
	}
	else if(dir[1]>dir[0] && dir[1]>dir[2])
	{
			dir[0] = 0; dir[1] = 1; dir[2] = 0;
	}
	else if(dir[2]>dir[1] && dir[2]>dir[0])
	{
			dir[0] = 0; dir[1] = 0; dir[2] = 1;
	}
	
	widgetp[0][0] = 0;
	widgetp[0][1] = 0;
	widgetp[0][2] = 0;

	widgetp[1][0] = widgetp[0][0]+dir[0]*widgetl;
	widgetp[1][1] = widgetp[0][1]+dir[1]*widgetl;
	widgetp[1][2] = widgetp[0][2]+dir[2]*widgetl;

    	Intersection();
}
*/
void svSlice::move_ctrlpoint(int x, int y, bool tilted, svScalar precision, int width, GLfloat *tm)
{
    GLdouble xx, yy, zz;
    GLdouble wld_x, wld_y, wld_z;       /* world coordinate */
    GLdouble tx, ty, tz;
    GLfloat invert_tb[16];

    int i, j, k;
	float deltal;
	float tmpl;

    xx = (GLdouble)x;
    yy = (GLdouble)width-(GLdouble)y;
    zz = wz;

    gluUnProject(xx, yy, zz,
                mvmatrix, projmatrix, viewport,
                &tx, &ty, &tz);

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        exit(0);
    }

    wld_x = tx * invert_tb[0] +
            ty * invert_tb[4] +
            tz * invert_tb[8] +
            invert_tb[12];

    wld_y = tx * invert_tb[1] +
            ty * invert_tb[5] +
            tz * invert_tb[9] +
            invert_tb[13];

    wld_z = tx * invert_tb[2] +
            ty * invert_tb[6] +
            tz * invert_tb[10] +
            invert_tb[14];

	if(nj == 0)
	{
		//plane_vector[ni] = widgetp[ni];
		svVector3 tmp_normal = plane_normal[ni];
		plane_normal[ni][0] = wld_x - widgetp[ni][0];
		plane_normal[ni][1] = wld_y - widgetp[ni][1];
		plane_normal[ni][2] = wld_y - widgetp[ni][2];
		plane_normal[ni] = normalize(plane_normal[ni]);
		if(!tilted)
		{
			if(plane_normal[ni][0]> plane_normal[ni][1]
			&& plane_normal[ni][0]> plane_normal[ni][2])
			{
				plane_normal[ni][0] = 1;
				plane_normal[ni][1] = 0;
				plane_normal[ni][2] = 0;
			}
			else if(plane_normal[ni][1]> plane_normal[ni][0]
			&& plane_normal[ni][1]> plane_normal[ni][2])
			{
				plane_normal[ni][0] = 0;
				plane_normal[ni][1] = 1;
				plane_normal[ni][2] = 0;
			}
			else 
			{
				plane_normal[ni][0] = 0;
				plane_normal[ni][1] = 0;
				plane_normal[ni][2] = 1;
			}
		}
		Intersection();
		if(intersectionnum[ni] == 0)
		{
			plane_normal[ni] = tmp_normal;
			Intersection();
		}
	}	
	else
	{
		svVector3 tmp_vector = plane_vector[ni];

		svVector3 wld;
		wld[0] = wld_x - rayInter[0];
		wld[1] = wld_y - rayInter[1];
		wld[2] = wld_z - rayInter[2];
		//cerr<<rayInter[0]<<" "<<rayInter[1]<<" "<<rayInter[2]<<endl;
		//cerr<<wld[0]<<" "<<wld[1]<<" "<<wld[2]<<" "<<GetDot(wld, plane_normal[ni])<<" "<<precision<<endl;
		svScalar d = GetDot(wld, plane_normal[ni])/ precision;
		//cerr<<d<<endl;
		int dd = (int)d;
		d = (svScalar)d * precision;
		//cerr<<plane_vector[ni][0]<<" "<<plane_vector[ni][1]<<" "<<plane_vector[ni][2]<<endl;
		plane_vector[ni] = start_plane_vector + d * plane_normal[ni];
		widgetp[ni] = plane_vector[ni];
		//rayInter = rayInter + d * plane_normal[ni];
		//cerr<<plane_vector[ni][0]<<" "<<plane_vector[ni][1]<<" "<<plane_vector[ni][2]<<endl;
		Intersection();

		if(intersectionnum[ni] == 0)
		{
			plane_vector[ni] = tmp_vector;
			widgetp[ni] = plane_vector[ni];
			Intersection();
		}

		/*svVector3 c;
		c[0] = 0;
		c[1] = 0;
		c[2] = 0;
		for(int j=0;j<intersectionnum[ni];j++)
		{
			c[0] = c[0] + intersectionp[ni][j][0];
			c[1] = c[1] + intersectionp[ni][j][1];
			c[2] = c[2] + intersectionp[ni][j][2];			
		}
		if(intersectionnum[i]>0)
		{
			plane_vector[ni][0] = c[0]/intersectionnum[ni];
			plane_vector[ni][1] = c[1]/intersectionnum[ni];
			plane_vector[ni][2] = c[2]/intersectionnum[ni];
		}
		widgetp[ni] = plane_vector[ni];*/
	}

}
void svSlice::Click(svVector3 rayPos, svVector3 rayDir, GLfloat *tm)
{
    svVector3 newRaypos;
    svVector3 newRaydir;
    GLfloat invert_tb[16];

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        exit(0);
    }

    newRaypos[0] = rayPos[0] * invert_tb[0] +
            rayPos[1] * invert_tb[4] +
            rayPos[2] * invert_tb[8] +
            invert_tb[12];
    newRaypos[1] = rayPos[0] * invert_tb[1] +
            rayPos[1] * invert_tb[5] +
            rayPos[2] * invert_tb[9] +
            invert_tb[13];
    newRaypos[2] = rayPos[0] * invert_tb[2] +
            rayPos[1] * invert_tb[6] +
            rayPos[2] * invert_tb[10] +
            invert_tb[14];

    newRaydir[0] = (rayPos[0]+rayDir[0]) * invert_tb[0] +
            (rayPos[1]+rayDir[1]) * invert_tb[4] +
            (rayPos[2]+rayDir[2]) * invert_tb[8] +
            invert_tb[12];
    newRaydir[1] = (rayPos[0]+rayDir[0]) * invert_tb[1] +
            (rayPos[1]+rayDir[1]) * invert_tb[5] +
            (rayPos[2]+rayDir[2]) * invert_tb[9] +
            invert_tb[13];
    newRaydir[2] = (rayPos[0]+rayDir[0])  * invert_tb[2] +
            (rayPos[1]+rayDir[1])  * invert_tb[6] +
            (rayPos[2]+rayDir[2])  * invert_tb[10] +
            invert_tb[14];
    newRaydir[0] = newRaydir[0] - newRaypos[0];
    newRaydir[1] = newRaydir[1] - newRaypos[1];
    newRaydir[2] = newRaydir[2] - newRaypos[2];
    newRaydir = normalize(newRaydir);

	//cerr<<"-----------------"<<endl;
    for(int i=0;i<plane_vector.size();i++)
    {
	svScalar tmpD;
	bool inside;
	//inside polygon
	bool in = false;
	svVector3 p;
	for(int j=1;j<intersectionnum[i]-1;j++)
	{
		inside = rayTriangleIntersect(newRaypos, newRaydir, 
			intersectionp[i][0],
			intersectionp[i][j],
			intersectionp[i][j+1],
			plane_normal[0], 
			tmpD, p);	
		if(inside)
		{
			in = true;
			break;
		}
	}
	if(in)
	{
		SELECTED = true;
		if(click_selection[i]) click_selection[i] = false;
		else click_selection[i] = true;
		break;
	}
    }
}

void svSlice::Find_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm)
{

    SELECTED = false;

    svVector3 newRaypos;
    svVector3 newRaydir;
    GLfloat invert_tb[16];

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        exit(0);
    }

    newRaypos[0] = rayPos[0] * invert_tb[0] +
            rayPos[1] * invert_tb[4] +
            rayPos[2] * invert_tb[8] +
            invert_tb[12];
    newRaypos[1] = rayPos[0] * invert_tb[1] +
            rayPos[1] * invert_tb[5] +
            rayPos[2] * invert_tb[9] +
            invert_tb[13];
    newRaypos[2] = rayPos[0] * invert_tb[2] +
            rayPos[1] * invert_tb[6] +
            rayPos[2] * invert_tb[10] +
            invert_tb[14];

    newRaydir[0] = (rayPos[0]+rayDir[0]) * invert_tb[0] +
            (rayPos[1]+rayDir[1]) * invert_tb[4] +
            (rayPos[2]+rayDir[2]) * invert_tb[8] +
            invert_tb[12];
    newRaydir[1] = (rayPos[0]+rayDir[0]) * invert_tb[1] +
            (rayPos[1]+rayDir[1]) * invert_tb[5] +
            (rayPos[2]+rayDir[2]) * invert_tb[9] +
            invert_tb[13];
    newRaydir[2] = (rayPos[0]+rayDir[0])  * invert_tb[2] +
            (rayPos[1]+rayDir[1])  * invert_tb[6] +
            (rayPos[2]+rayDir[2])  * invert_tb[10] +
            invert_tb[14];
    newRaydir[0] = newRaydir[0] - newRaypos[0];
    newRaydir[1] = newRaydir[1] - newRaypos[1];
    newRaydir[2] = newRaydir[2] - newRaypos[2];
    newRaydir = normalize(newRaydir);

	//cerr<<"-----------------"<<endl;
    for(int i=0;i<plane_vector.size();i++)
    {
	svScalar tmpD;
	bool inside;
	//inside sphere
	svVector3 spherePos = widgetp[i] + widgetl*plane_normal[i];
	inside = raySphereIntersect(newRaypos, newRaydir, 
				spherePos, 1,
				tmpD);
	if(inside)
	{
		ni = i;
		nj = 0;
		SELECTED = true;
		rayInter[0]  = spherePos[0] * tm[0] +
				    spherePos[1] * tm[4] +
				    spherePos[2] * tm[8] +
				    tm[12] + spherePos[0];
		rayInter[1]  = spherePos[0] * tm[1] +
		                    spherePos[1] * tm[5] +
		                    spherePos[2] * tm[9] +
		                    tm[13] + spherePos[1];
		rayInter[2]  = spherePos[0] * tm[2] +
		                    spherePos[1] * tm[6] +
		                    spherePos[2] * tm[10] +
		                    tm[14] + spherePos[2];
		gluProject((GLdouble)rayInter[0],
                        (GLdouble)rayInter[1],
                        (GLdouble)rayInter[2],
                        mvmatrix, projmatrix, viewport,
                        &(wx), &(wy), &(wz));
		break;
	}
	//inside polygon
	bool in = false;
	//cerr<<"================"<<endl;	
	svVector3 p;
	for(int j=1;j<intersectionnum[i]-1;j++)
	{
		inside = rayTriangleIntersect(newRaypos, newRaydir, 
			intersectionp[i][0],
			intersectionp[i][j],
			intersectionp[i][j+1],
			plane_normal[i], 
			tmpD, p);	
		//cerr<<intersectionp[i][0][0]<<" "<<intersectionp[i][0][1]<<" "<<intersectionp[i][0][2]
				//<<intersectionp[i][j][0]<<" "<<intersectionp[i][j][1]<<" "<<intersectionp[i][j][2]<<" "
				//<<intersectionp[i][j+1][0]<<" "<<intersectionp[i][j+1][1]<<" "<<intersectionp[i][j+1][2]<<endl;
			//cerr<<plane_normal[i][0]<<" "<<plane_normal[i][1]<<" "<<plane_normal[i][2]<<" ";
			//cerr<<tmpD<<endl;

		if(inside)
		{
			in = true;
			break;
		}
	}
	if(in)
	{
		ni = i;
		nj = 1;
		SELECTED = true;
		//svVector3 p;
		//LinetoPlane(plane_vector[i], plane_normal[i], rayPos, rayDir, p);
		start_plane_vector = plane_vector[i];
		rayInter[0]  = p[0] * tm[0] +
				    p[1] * tm[4] +
				    p[2] * tm[8] +
				    tm[12] + p[0];
		rayInter[1]  = p[0] * tm[1] +
		                    p[1] * tm[5] +
		                    p[2] * tm[9] +
		                    tm[13] + p[1];
		rayInter[2]  = p[0] * tm[2] +
		                    p[1] * tm[6] +
		                    p[2] * tm[10] +
		                    tm[14] + p[2];
		gluProject((GLdouble)rayInter[0],
		                (GLdouble)rayInter[1],
		                (GLdouble)rayInter[2],
		                mvmatrix, projmatrix, viewport,
		                &(wx), &(wy), &(wz));
		rayInter = p;
		break;
	}
    }
}

void svSlice::cleanup()
{
	widgetp.clear();
	plane_vector.clear();
	plane_normal.clear();
	for(int i=0;i<intersectionp.size();i++)
	{
		intersectionp[i].clear();
	}
	intersectionp.clear();

	intersectionnum.clear();
	click_selection.clear();
} 

}
