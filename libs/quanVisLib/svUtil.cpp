
#include "svUtil.h"
#include <SOIL.h>

#define TEXTURE_LOAD_ERROR 0


namespace __svl_lib {

svVector3 svGetPerpendicularVector(const svVector3 & v)
{
	svVector3 r;
	if (fabs(v[2]) < 0.1){
		r[0] = v[1];
		r[1] = -v[0];
	}
	else if (fabs(v[1]) < 0.1){
		r[0] = v[2];
		r[2] = -v[0];
	}
	else{
		r[1] = v[2];
		r[2] = -v[1];
	}

	return normalize(cross(v, r));
}

svVector3 svGetRotatePoint(const svVector3& start, const svVector3& org, 
			const svVector3& axis, svScalar rotate_degree)
{
   svVector3 p, q, r;
   svScalar theta = svToRadian(rotate_degree);
   svScalar costheta,sintheta;

   // calculate the roatation axis
   r = normalize(axis);
   p = start - org;

   //cerr << "r: "; r.print();
   //cerr << "p: "; p.print();

   costheta = svScalar(cos(theta));
   sintheta = svScalar(sin(theta));

   q[0] += (costheta + (1 - costheta) * r[0] * r[0]) * p[0];
   q[0] += ((1 - costheta) * r[0] * r[1] - r[2] * sintheta) * p[1];
   q[0] += ((1 - costheta) * r[0] * r[2] + r[1] * sintheta) * p[2];

   q[1] += ((1 - costheta) * r[0] * r[1] + r[2] * sintheta) * p[0];
   q[1] += (costheta + (1 - costheta) * r[1] * r[1]) * p[1];
   q[1] += ((1 - costheta) * r[1] * r[2] - r[0] * sintheta) * p[2];

   q[2] += ((1 - costheta) * r[0] * r[2] - r[1] * sintheta) * p[0];
   q[2] += ((1 - costheta) * r[1] * r[2] + r[0] * sintheta) * p[1];
   q[2] += (costheta + (1 - costheta) * r[2] * r[2]) * p[2];
   //cerr << "q: "; q.print();

   q += org;
   //cerr << "org: "; org.print();
   //cerr << "q: "; q.print();
   return q;
}

// get the normal vector
//      v1-------------v0
//                    /
//                   /
//                  / v2
//
svVector3 svGetNorm(const svVector3& v1, const svVector3& v0,
                    const svVector3& v2)
{
  return normalize(cross((v1-v0), (v2-v0)));
}

svVector3 svAverage(const svVector3& v1, const svVector3& v2)
{
  return normalize(v1+v2);
}


svVector3 GetRotateVector(svVector3 v, GLfloat *m)
{
  svVector3 newv;
  svVector3 newp;
  newp[0] = m[12];
  newp[1] = m[13];
  newp[2] = m[14];
 
  newv[0]  = v[0] * m[0] +
             v[1] * m[4] +
             v[2] * m[8] +
             m[12] ;
  newv[1]  = v[0] * m[1] +
             v[1] * m[5] +
             v[2] * m[9] +
             m[13];
  newv[2]  = v[0] * m[2] +
             v[1] * m[6] +
             v[2] * m[10] +
             m[14] ;

  return normalize(newv-newp);
}

svVector3 GetNewVector(svVector3 v, GLfloat *m)
{
  svVector3 newv;

  newv[0]  = v[0] * m[0] +
             v[1] * m[4] +
             v[2] * m[8] +
             m[12] ;
  newv[1]  = v[0] * m[1] +
             v[1] * m[5] +
             v[2] * m[9] +
             m[13];
  newv[2]  = v[0] * m[2] +
             v[1] * m[6] +
             v[2] * m[10] +
             m[14] ;

  return newv;
}

void GetRotateAngle(svVector3 dir, double &angle_x,  double &angle_z)
{
            angle_x = acos(dir[2]);
                if(dir[1] > 0)
                {
                        angle_x = - angle_x;
                }
                double xy_project = dir[0] * dir[0] + dir[1] * dir[1];
                xy_project = sqrt(xy_project);
                 angle_z = acos(dir[1]/xy_project);
                if(angle_x < 0)
                {
                                if (dir[0] > 0)
                                 {
                                        angle_z =  -angle_z;
                                  }
                }
                else
                {
                                angle_z = 3.1415926 - angle_z;
                                if(dir[0] < 0)
                                        angle_z =  -angle_z;
                }

}

void GetRotateAngleX(svVector3 dir, double &angle_y,  double &angle_x)
{
            angle_y = acos(dir[0]);
                if(dir[2] > 0)
                {
                        angle_y = - angle_y;
                }
                double yz_project = dir[1] * dir[1] + dir[2] * dir[2];
                yz_project = sqrt(yz_project);
                angle_x = acos(dir[2]/yz_project);
                if(angle_y < 0)
                {
                                if (dir[1] > 0)
                                 {
                                        angle_x =  -angle_x;
                                  }
                }
                else
                {
                                angle_y = 3.1415926 - angle_y;
                                if(dir[1] < 0)
                                        angle_y =  -angle_y;
                }

}


void GetRotateFont(svVector3 dir, double &angle_x,  double &angle_z,
                   ViewProperty &property)
{
     svVector3 newdir = GetRotateVector(dir, property.tm);
     cerr<<newdir[0]<<" "<<newdir[1]<<" "<<newdir[2]<<endl;
     GetRotateAngle(newdir, angle_x, angle_z);
     angle_x = -angle_x;
     angle_z = -angle_z;
}
//----------------------------------------------------------
svVector3 GetVertical(svVector3 pos, svVector3 dir,  ViewProperty &property)
{
    GLfloat *invert_tb= new GLfloat[16];
    svVector3 p,p2;

//    svVector3 y;
    svVector3 v;
     v[0]  = dir[0] + pos[0];
     v[1] = dir[1] + pos[1];
     v[2] = dir[2] + pos[2];
 //   y[0]=0;y[1]=1;y[2]=0;
//    svVector3 x;
  //  x[0]=1;x[1]=0;x[2]=0;
//
                p2[0]  = pos[0] * property.tm[0] +
                            pos[1] * property.tm[4] +
                            pos[2] * property.tm[8] +
                            property.tm[12] ;
                p2[1]  = pos[0] * property.tm[1] +
                            pos[1] * property.tm[5] +
                            pos[2] * property.tm[9] +
                            property.tm[13];
                p2[2]  = pos[0] * property.tm[2] +
                            pos[1] * property.tm[6] +
                            pos[2] * property.tm[10] +
                            property.tm[14] ;


                p[0]  = v[0] * property.tm[0] +
                            v[1] * property.tm[4] +
                            v[2] * property.tm[8] +
                            property.tm[12] ;
                p[1]  = v[0] * property.tm[1] +
                            v[1] * property.tm[5] +
                            v[2] * property.tm[9] +
                            property.tm[13];
                p[2]  = v[0] * property.tm[2] +
                            v[1] * property.tm[6] +
                            v[2] * property.tm[10] +
                            property.tm[14] ;
//        p = normalize(p);


             GLdouble objx, objy, objz;
             objx = (GLdouble)p2[0];
             objy = (GLdouble)p2[1];
             objz = (GLdouble)p2[2];
             GLdouble winx1, winy1, winz1, winx2, winy2, winz2;
             gluProject(objx, objy, objz, property.mvmatrix, property.projmatrix, property.viewport,
                     &winx1, &winy1, &winz1);

             objx = (GLdouble)p[0];
             objy = (GLdouble)p[1];
             objz = (GLdouble)p[2];
             gluProject(objx, objy, objz, property.mvmatrix, property.projmatrix, property.viewport,
                     &winx2, &winy2, &winz2);
for(int ii=0;ii<4;ii++)
cerr<<property.viewport[ii]<<" ";
cerr<<endl;
cerr<<objx<<" "<<objy<<" "<<objz<<endl;
cerr<<winx1<<" "<<winy1<<" "<<winx2<<" "<<winy2<<endl;;
             GLdouble vx = winy1 - winy2;
             GLdouble vy = winx2 - winx1;
             double sum = vx*vx + vy*vy;
             sum = sqrt(sum);
             vx = vx/sum;
             vy = vy/sum;
cerr<<vx<<" "<<vy<<endl;
             vx = vx + winx1;
             vy = vy + winy1;
             glReadPixels((int)vx, (int)vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,&winz1); 
             gluUnProject( vx, vy, winz1, property.mvmatrix, property.projmatrix, property.viewport,
                        &objx, &objy, &objz);


    if(!invert_matrix(property.tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
   }

        svVector3 r2;
        r2[0]=objx;r2[1]=objy;r2[2]=objz;
        svVector3 r;

    r[0] = r2[0] * invert_tb[0] +
            r2[1] * invert_tb[4] +
            r2[2] * invert_tb[8] +
            invert_tb[12];

   r[1] = r2[0] * invert_tb[1] +
            r2[1] * invert_tb[5] +
            r2[2] * invert_tb[9] +
            invert_tb[13];

    r[2] = r2[0] * invert_tb[2] +
            r2[1] * invert_tb[6] +
            r2[2] * invert_tb[10] +
            invert_tb[14];

    r[0] = r[0] - pos[0];
    r[1] = r[1] - pos[1];
    r[2] = r[2] - pos[2];
    r = normalize(r);

        delete [] invert_tb;
        return r;

}
svVector3 GetVerticalByeye(svVector3 pos, svVector3 v, ViewProperty &property)
//svVector3 eye,
//                     svVector3 pos, 
//                     svVector3 v, 
//                     GLfloat *tm)
{
/*    GLfloat tm[16];
    for(int i=0;i<4;i++)
    {
          for(int j=0;j<4;j++)
          {
             tm[4*j+i] = property.tm[4*i+j];
          }
    }

    for(int i=0;i<4;i++)
    {
          for(int j=0;j<4;j++)
          {
            property. tm[4*i+j] = tm[4*i+j];
          }
    }
*/

    GLfloat *invert_tb= new GLfloat[16];
    svVector3 p,p2;


                p2[0]  = pos[0] * property.tm[0] +
                            pos[1] *property. tm[4] +
                            pos[2] *property. tm[8] +
                           property. tm[12] ;
                p2[1]  = pos[0] *property. tm[1] +
                            pos[1] * property.tm[5] +
                            pos[2] * property.tm[9] +
                            property.tm[13];
                p2[2]  = pos[0] * property.tm[2] +
                            pos[1] * property.tm[6] +
                            pos[2] * property.tm[10] +
                            property.tm[14] ;


                p[0]  = v[0] * property.tm[0] +
                            v[1] * property.tm[4] +
                            v[2] * property.tm[8] +
                            property.tm[12] ;
                p[1]  = v[0] * property.tm[1] +
                            v[1] * property.tm[5] +
                            v[2] * property.tm[9] +
                            property.tm[13];
                p[2]  = v[0] * property.tm[2] +
                            v[1] * property.tm[6] +
                            v[2] * property.tm[10] +
                            property.tm[14] ;

        p.normalize();
    property.eye[0] = p2[0];
    property.eye[1] = p2[1];
    svVector3 e = property.eye - p2;
    e.normalize();
//cerr<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
////cerr<<"e "<<e[0]<<" "<<e[1]<<" "<<e[2]<<endl;
        svVector3 v2 = cross(e, p);
         v2.normalize();
//cerr<<v2[0]<<" "<<v2[1]<<" "<<v2[2]<<endl;
    if(!invert_matrix(property.tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
   }

      v2 = v2 + p2;

        svVector3 r;

    r[0] = v2[0] * invert_tb[0] +
            v2[1] * invert_tb[4] +
            v2[2] * invert_tb[8] +
            invert_tb[12];

   r[1] = v2[0] * invert_tb[1] +
            v2[1] * invert_tb[5] +
            v2[2] * invert_tb[9] +
            invert_tb[13];

    r[2] = v2[0] * invert_tb[2] +
            v2[1] * invert_tb[6] +
            v2[2] * invert_tb[10] +
            invert_tb[14];
     r = r - pos;
     r.normalize();
/*
for(int i=0;i<16;i++)
    cerr<<invert_tb[i]<<" ";cerr<<endl;
for(int i=0;i<16;i++)
    cerr<<property.tm[i]<<" ";cerr<<endl;
//   r.normalize();
cerr<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
                r2[0]  = r[0] * property.tm[0] +
                            r[1] * property.tm[4] +
                            r[2] * property.tm[8] +
                            property.tm[12] ;
                r2[1]  = r[0] * property.tm[1] +
                            r[1] * property.tm[5] +
                            r[2] * property.tm[9] +
                            property.tm[13];
                r2[2]  = r[0] * property.tm[2] +
                            r[1] * property.tm[6] +
                            r[2] * property.tm[10] +
                            property.tm[14] ;
//r2.normalize();
cerr<<r2[0]<<" "<<r2[1]<<" "<<r2[2]<<endl;
*/
        delete [] invert_tb;
        return r;

}

double GetDot(svVector3 a, svVector3 b)
{
	double c;
	c = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	return c;
}

float PointtoLine(svVector3 p1, svVector3 p2, svVector3 p)
{
	//distance(x=a+tn, p) = ||(a-p)-((a-p)*n)n||
	//wiki http://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	svVector3 n;
	n = p1-p2;
	n = normalize(n);
	svVector3 a=p2;
	svVector3 ap=a - p;
	float apdot=GetDot(ap,n);
	svVector3 apn = apdot*n;
	svVector3 d = ap - apn;

	float distance = sqrt(d[0]*d[0]+d[1]*d[1]
				+d[2]*d[2]);	

	return distance;
}

float PointtoLine2(svVector3 p1, svVector3 n, svVector3 p)
{
	//distance(x=a+tn, p) = ||(a-p)-((a-p)*n)n||
	//wiki http://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	svVector3 a=p1;
	svVector3 ap=a - p;
	float apdot=GetDot(ap,n);
	svVector3 apn = apdot*n;
	svVector3 d = ap - apn;

	float distance = sqrt(d[0]*d[0]+d[1]*d[1]
				+d[2]*d[2]);	

	return distance;
}

bool LinetoPlane(svVector3 p0, svVector3 n, svVector3 l0, svVector3 l, svVector3 &newp)
{
	svVector3 p0l0 = p0 - l0;
	float tmpd = GetDot(p0l0, n);
	float t = GetDot(l, n);

	if(t != 0)
	{
		newp = (svScalar)(tmpd/t) * l + l0;
		return 1;
	}
	else
	{
		return 0;
	}
}
bool LessAngle(svVector3 p1, svVector3 p2, svVector3 p)
{
  svVector3 n1 = p - p1;
  svVector3 n2 = p2 - p1;
  
  double dot = n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2];
  
  if(dot >0)
    return true;
  else
    return false;
}
//https://sites.google.com/site/justinscsstuff/object-intersection
//
bool rayPlaneIntersect(svVector3 rPos, svVector3 rDir,
svVector3 pPos0, svVector3 pPos1, svVector3 pPos2,svVector3 pPos3,
svVector3 pDir, svScalar &distance, svVector3 &pos)
{
    float nDotD = GetDot(pDir, rDir);

    distance = 9e+9;

    if (nDotD == 0)
        return false;
    svVector3 x = rPos + rDir * (-GetDot(pDir, rPos - pPos0) / nDotD);
    svVector3 a = pPos0;
    svVector3 b = pPos1;
    svVector3 c = pPos2;
    svVector3 d = pPos3;

    if (GetDot(cross(b-a, x-a), pDir) < 0) return false;
    if (GetDot(cross(c-b, x-b), pDir) < 0) return false;
    if (GetDot(cross(d-c, x-c), pDir) < 0) return false;
    if (GetDot(cross(a-d, x-d), pDir) < 0) return false;    

    distance = sqrt((x[0] - rPos[0])*(x[0] - rPos[0])
                +  (x[1] - rPos[1])*(x[1] - rPos[1])
                +  (x[2] - rPos[2])*(x[2] - rPos[2]));
    pos = x;

    return true;
}
bool rayTriangleIntersect(svVector3 rPos, svVector3 rDir, 
			svVector3 pPos0, svVector3 pPos1, svVector3 pPos2,
			svVector3 pDir, svScalar &distance, svVector3 &pos) 
{
    float nDotD = GetDot(pDir, rDir);

    distance = 9e+9;

    if (nDotD == 0)
        return false;
    svVector3 x = rPos + rDir * (-GetDot(pDir, rPos - pPos0) / nDotD);
    svVector3 a = pPos0;
    svVector3 b = pPos1;
    svVector3 c = pPos2;


    if (GetDot(cross(b-a, x-a), pDir) > 0) return false;
    if (GetDot(cross(c-b, x-b), pDir) > 0) return false;
    if (GetDot(cross(a-c, x-c), pDir) > 0) return false;

    distance = sqrt((x[0] - rPos[0])*(x[0] - rPos[0])
		+  (x[1] - rPos[1])*(x[1] - rPos[1])
		+  (x[2] - rPos[2])*(x[2] - rPos[2]));
    pos = x;

    return true;
}
bool raySphereIntersect(svVector3 rPos, svVector3 rDir, 
				svVector3 sPos, svScalar sRadius,
				svScalar &distance) 
{
/* svVector3 pMinusC = rPos - sPos;
 float a = GetDot(rDir, rDir);
 float b = 2 * GetDot(rDir, pMinusC);
 float c = GetDot(pMinusC, pMinusC) - sRadius * sRadius;
 float disc = b * b - 4 * a * c;
 
 // no solution if discriminant is negative
 if (disc < 0)
 return false;
 
 svScalar t;
 if (disc == 0) {
        // one solution, barely touches side
 t = (svScalar )(-b + sqrt(disc)) / (2 * a);
 svVector3 x= rPos + rDir * t;

    distance = sqrt((x[0] - rPos[0])*(x[0] - rPos[0])
		+  (x[1] - rPos[1])*(x[1] - rPos[1])
		+  (x[2] - rPos[2])*(x[2] - rPos[2]));
    } 
 else {
 // two solutions: the smallest t is the closest intersection
        svScalar temp = sqrt(disc) / (2 * a);
 svScalar  t1 = -b + temp;
 svScalar t2 = -b - temp;
 t = min(t1, t2);
 svVector3 x= rPos + rDir * t;

    distance = sqrt((x[0] - rPos[0])*(x[0] - rPos[0])
		+  (x[1] - rPos[1])*(x[1] - rPos[1])
		+  (x[2] - rPos[2])*(x[2] - rPos[2]));*/

 svVector3 pMinusC = rPos - sPos;
 float a = pMinusC[0] * pMinusC[0] + pMinusC[1] * pMinusC[1] + pMinusC[2] * pMinusC[2];
 float b = GetDot(rDir, pMinusC);
 float disc = b * b - a + sRadius * sRadius;
 
 // no solution if discriminant is negative
 if (disc < 0)
 return false;
 
 svScalar t;
 if (disc == 0) {
        // one solution, barely touches side
 t = -b;//(svScalar )(-b + sqrt(disc)) / (2 * a);
 svVector3 x= rPos + rDir * t;

    distance = sqrt((x[0] - rPos[0])*(x[0] - rPos[0])
		+  (x[1] - rPos[1])*(x[1] - rPos[1])
		+  (x[2] - rPos[2])*(x[2] - rPos[2]));
    } 
 else {
 // two solutions: the smallest t is the closest intersection
        svScalar temp = sqrt(disc);
 svScalar t1 = -b + temp;//-b + temp;
 svScalar t2 = -b - temp;
 t = min(t1, t2);
 svVector3 x= rPos + rDir * t;

    distance = sqrt((x[0] - rPos[0])*(x[0] - rPos[0])
		+  (x[1] - rPos[1])*(x[1] - rPos[1])
		+  (x[2] - rPos[2])*(x[2] - rPos[2]));
 }
 


 return true;
}

double pointtoLine(svVector3 a, svVector3 p, svVector3 n)
{
	double d;

	svVector3 t;
	t[0] = a[0] - p[0];
	t[1] = a[1] - p[1];
	t[2] = a[2] - p[2];

	d = fabs(t[0] * n[0] + t[1] * n[1] + t[2] * n[2]);

	return d;
}

svVector3  CrossProduct(svVector3 u, svVector3 v)
{
	svVector3 c;

	c[0]=u[1]*v[2] - u[2]*v[1];
	c[1]=u[2]*v[0] - u[0]*v[2];
	c[2]=u[0]*v[1] - u[1]*v[0];

	c = normalize(c);

	return c;
}
GLboolean invert_matrix( const GLfloat *m, GLfloat *out )
{
/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

//for(int i=0;i<16;i++)cerr<<m[i]<<" ";cerr<<endl;

 GLfloat wtmp[4][8];
 GLfloat m0, m1, m2, m3, s;
 GLfloat *r0, *r1, *r2, *r3;

 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

 r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
 r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
 r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

 r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
 r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
 r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

 r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
 r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
 r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

 r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
 r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
 r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

 /* choose pivot - or die */
 if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
 if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
 if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
//cerr<<r0[0]<<endl;
 if (fabs(0.0 - r0[0])<1e-10)  return GL_FALSE;
//cerr<<"done"<<endl;
 /* eliminate first variable     */
 m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
 s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
 s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
 s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r0[5];
 if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r0[6];
 if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r0[7];
 if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
 if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
 if (0.0 == r1[1])  return GL_FALSE;

 /* eliminate second variable */
 m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
 r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
 s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
 if (0.0 == r2[2])  return GL_FALSE;

 /* eliminate third variable */
 m3 = r3[2]/r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
 r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
 r3[7] -= m3 * r2[7];
 /* last check */
 if (0.0 == r3[3]) return GL_FALSE;

 s = 1.0/r3[3];              /* now back substitute row 3 */
 r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

 m2 = r2[3];                 /* now back substitute row 2 */
 s  = 1.0/r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
 r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
 r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
 r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

 m1 = r1[2];                 /* now back substitute row 1 */
 s  = 1.0/r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
 r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
 r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

 m0 = r0[1];                 /* now back substitute row 0 */
 s  = 1.0/r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
 r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

 MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
 MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
 MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
 MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
 MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
 MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
 MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
 MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

 return GL_TRUE;

#undef MAT
#undef SWAP_ROWS
}
double pointtoPlane(svVector3 a, svVector3 p, svVector3 n)
{
	double d;

	svVector3 t(3);
	t[0] = a[0] - p[0];
	t[1] = a[1] - p[1];
	t[2] = a[2] - p[2];

	d = fabs(t[0] * n[0] + t[1] * n[1] + t[2] * n[2]);

	return d;
}
bool linePlane(svVector3 a, svVector3 b, svVector3 p, svVector3 n)
{
	bool flag = false;
	svVector3 AP = p - a;
	svVector3 BP = p - b;	
	double dd1 = GetDot(AP, n);
	double dd2 = GetDot(BP, n);
	if(dd1 * dd2 <= 0 ) flag = true;

	return flag;	
}
bool planeBox(svVector3 lb, svVector3 rb, svVector3 p, svVector3 n)
{
	bool flag = false;

	svVector3 a,b;
	a[0] = lb[0];a[1] = lb[1]; a[2] = lb[2];
	b[0] = lb[0];b[1] = lb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	

	a[0] = rb[0];a[1] = lb[1]; a[2] = lb[2];
	b[0] = rb[0];b[1] = lb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	

	a[0] = rb[0];a[1] = rb[1]; a[2] = lb[2];
	b[0] = rb[0];b[1] = rb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	

	a[0] = lb[0];a[1] = rb[1]; a[2] = lb[2];
	b[0] = lb[0];b[1] = rb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	

	a[0] = lb[0];a[1] = lb[1]; a[2] = lb[2];
	b[0] = rb[0];b[1] = lb[1]; b[2] = lb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	
	a[0] = rb[0];a[1] = lb[1]; a[2] = lb[2];
	b[0] = rb[0];b[1] = rb[1]; b[2] = lb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	

	a[0] = rb[0];a[1] = rb[1]; a[2] = lb[2];
	b[0] = lb[0];b[1] = rb[1]; b[2] = lb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	

	a[0] = lb[0];a[1] = rb[1]; a[2] = lb[2];
	b[0] = lb[0];b[1] = lb[1]; b[2] = lb[2];
	flag = linePlane(a, b, p, n);

	if(flag) 	return flag;	
	a[0] = lb[0];a[1] = lb[1]; a[2] = rb[2];
	b[0] = rb[0];b[1] = lb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);
	if(flag) 	return flag;	
	a[0] = rb[0];a[1] = lb[1]; a[2] = rb[2];
	b[0] = rb[0];b[1] = rb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);
	if(flag) 	return flag;	
	a[0] = rb[0];a[1] = rb[1]; a[2] = rb[2];
	b[0] = lb[0];b[1] = rb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);
	if(flag) 	return flag;	
	a[0] = lb[0];a[1] = rb[1]; a[2] = rb[2];
	b[0] = lb[0];b[1] = lb[1]; b[2] = rb[2];
	flag = linePlane(a, b, p, n);

	return flag;	
}

svInt  getNumOfIntegerDigits(svScalar num)
{
          int multiplier = 0;

          double numCopy = num;

          if(numCopy<1&&numCopy!=0)
          {
            while (numCopy<1)//(fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
                {
                       multiplier--;
                       numCopy *=10;
                };
          }
          else if(numCopy==0)
                multiplier=0;
          else if(numCopy>=1)
          {
            while(1)
                {
                  if(numCopy>=0 && numCopy <10)
                  {
                    break;
                  };
                  multiplier++;
                  numCopy /= 10.;
                };
           };
          return multiplier;
}

void RenderSphere(svVector3 glyph, svScalar radius, svVector3 dir,
                   int segment1, int segment2)
{
   svVector3 sphere_seg_pos[segment1+1][segment2+1];
   svVector3 sphere_seg_norm[segment1+1][segment2+1];
   svVector3 center;
  
   center[0]=0;center[1]=0;center[2]=0;
   svVector3 pos;pos[0]=0;pos[1]=0;pos[2]=-1;
   svVector3 head;
   svVector3 x;x[0]=1;x[1]=0;x[2]=0;
   svVector3 y;y[0]=0;y[1]=1;y[2]=0;
   svVector3 z;z[0]=0;z[1]=0;z[2]=1;
   for(int i=0;i<segment1+1;i++)//0-180
   {
        head = svGetRotatePoint(pos, center, x, (svScalar)i * (svScalar)(180./segment1));
        svVector3 c;
        c[0]=0;c[1]=0;c[2]=head[2];
        for(int j=0;j<segment2+1;j++)//0-360
        {
             sphere_seg_pos[i][j]=svGetRotatePoint(head, c, z, (svScalar)j * (svScalar)(360./segment2)); 
        }
   }

   for(int i=0;i<segment1+1;i++)//0-180
   {
        for(int j=0;j<segment2+1;j++)//0-360
        {
             sphere_seg_pos[i][j]=sphere_seg_pos[i][j] * radius
                            + glyph + radius * dir;
             sphere_seg_norm[i][j]=sphere_seg_pos[i][j]-(glyph + radius * dir);
             sphere_seg_norm[i][j]=normalize(sphere_seg_norm[i][j]);
        }
   } 

   for(int i=0;i<segment1;i++)//0-180
   {
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<segment2+1;j++)//0-360
        {
            glNormal3f(sphere_seg_norm[i][j][0],sphere_seg_norm[i][j][1],sphere_seg_norm[i][j][2]);
            glVertex3f(sphere_seg_pos[i][j][0],sphere_seg_pos[i][j][1],sphere_seg_pos[i][j][2]);
            glNormal3f(sphere_seg_norm[i+1][j][0],sphere_seg_norm[i+1][j][1],sphere_seg_norm[i+1][j][2]);
            glVertex3f(sphere_seg_pos[i+1][j][0],sphere_seg_pos[i+1][j][1],sphere_seg_pos[i+1][j][2]);
        }
        glEnd(); 
   } 
}
void RenderCylinderTexture(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,svVector4 col,
                svScalar ratio, int texture_num, int segment)
{
        int num = texture_num;
        svScalar whole_degree = 360./num;
        int bar_segment, white_segment;

        svScalar bar_degree;
        bar_degree = whole_degree * ratio;
        svScalar white_degree;
        white_degree = whole_degree - bar_degree;

        bar_segment = segment * ratio;// (segment/num) * ratio;
//        if(bar_segment == 0)
//                bar_segment = 1;
        white_segment = segment - bar_segment;//segment/num - bar_segment;

        svVector3 v;
        svVector3 rotate_base, rotate_top, rotate_normal;
        v = svGetPerpendicularVector(dir);
        svVector3 head = glyph + radius * v;
        svVector3 top = glyph + height* dir;

        vector<svVector3> base;
        for(int i=0;i<num;i++)
        {
                svVector3 tmphead = glyph + radius * v;
                head = svGetRotatePoint(tmphead, glyph, dir, i * whole_degree);

                glColor4f(1,1,1,col[3]);
                glBegin(GL_QUAD_STRIP);
                for(int j=0;j<=white_segment;j++)
                {
                        rotate_base = svGetRotatePoint(head, glyph,dir, 
                               (svScalar)j * white_degree/(svScalar)white_segment);
                        rotate_top = rotate_base + height*dir;
                        rotate_normal = normalize(rotate_base - glyph);

                        glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
                        glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
                        glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
                        glVertex3f(rotate_top[0], rotate_top[1], rotate_top[2]);
                        base.push_back(rotate_base);
                }
                glEnd();

                glColor4f(col[0],col[1],col[2],col[3]);
                glBegin(GL_QUAD_STRIP);
                for(int j=0;j<=bar_segment;j++)
                {
                        rotate_base = svGetRotatePoint(head, glyph,dir, 
                       (svScalar)j * bar_degree/(svScalar)bar_segment + white_degree);

                        rotate_normal = normalize(rotate_base - glyph);
                        rotate_top = rotate_base + height*dir;

                        glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
                        glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
                        glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
                        glVertex3f(rotate_top[0], rotate_top[1], rotate_top[2]);
                        base.push_back(rotate_base);
                }
                glEnd();

        }

        glColor4f(col[0], col[1], col[2], col[3]);
        glBegin(GL_POLYGON);
        glNormal3f(-dir[0],-dir[1],-dir[2]);
        for(int i=0;i<base.size();i++)
        {
          glVertex3f(base[i][0],base[i][1],base[i][2]);
        }
        glEnd();

       base.clear();
}
void RenderCylinder(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment)
{

    svVector3 cylinder_seg_norm[(segment+1)*2];
    svVector3 cylinder_seg_pos[(segment+1)*2];
    float interval_degree  = 360./(float)segment;
    for(int i=0;i<=segment;i++) {
        if(i == 0)
        {
           svVector3 ori =  svGetPerpendicularVector(dir);
           cylinder_seg_pos[0] = glyph + radius * ori;
           cylinder_seg_pos[1] = cylinder_seg_pos[0] + height * dir;
           cylinder_seg_norm[0] = normalize(cylinder_seg_pos[0] - glyph);
           cylinder_seg_norm[1] = normalize(cylinder_seg_pos[0] - glyph);
         }
        else
        {
           cylinder_seg_pos[i*2] = svGetRotatePoint(cylinder_seg_pos[0],
                                         glyph,
                                         dir,
                                         i * interval_degree);
           cylinder_seg_pos[i*2+1] = cylinder_seg_pos[i*2] + height * dir;
           cylinder_seg_norm[i*2] = normalize(cylinder_seg_pos[i*2] - glyph);
           cylinder_seg_norm[i*2+1] = normalize(cylinder_seg_pos[i*2] - glyph);
        }
    }
 
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=segment;i++)
    {
          glNormal3f(cylinder_seg_norm[2*i+1][0], 
                    cylinder_seg_norm[2*i+1][1],
                    cylinder_seg_norm[2*i+1][2]);
          glVertex3f(cylinder_seg_pos[2*i+1][0],
                    cylinder_seg_pos[2*i+1][1],
                    cylinder_seg_pos[2*i+1][2]);

          glNormal3f(cylinder_seg_norm[2*i][0],
                    cylinder_seg_norm[2*i][1],
                    cylinder_seg_norm[2*i][2]);
          glVertex3f(cylinder_seg_pos[2*i][0],
                    cylinder_seg_pos[2*i][1],
                    cylinder_seg_pos[2*i][2]);

    } 
    glEnd();

  /*
   glDisable(GL_LIGHTING);

   glBegin(GL_LINES);  
   for(int i=0;i<=segment;i++)
    {
          glVertex3f(cylinder_seg_pos[2*i+1][0],
                    cylinder_seg_pos[2*i+1][1],
                    cylinder_seg_pos[2*i+1][2]);
          glVertex3f(cylinder_seg_pos[2*i+1][0]+cylinder_seg_norm[2*i+1][0],
                    cylinder_seg_pos[2*i+1][1]+cylinder_seg_norm[2*i+1][1],
                    cylinder_seg_pos[2*i+1][2]+cylinder_seg_norm[2*i+1][2]);
    }

   glEnd();

   glEnable(GL_LIGHTING);
*/
 

    if(height >1e-3)
    {
/*    glBegin(GL_POLYGON);
    glNormal3f(dir[0],dir[1],dir[2]);
    for(int i=0;i<=segment;i++)
    {
          glVertex3f(cylinder_seg_pos[2*i+1][0],
                    cylinder_seg_pos[2*i+1][1],
                    cylinder_seg_pos[2*i+1][2]);
    }
    glEnd();
*/
    glBegin(GL_POLYGON);
    glNormal3f(-dir[0],-dir[1],-dir[2]);
    for(int i=0;i<=segment;i++)
    {
          glVertex3f(cylinder_seg_pos[2*i][0],
                    cylinder_seg_pos[2*i][1],
                    cylinder_seg_pos[2*i][2]);
    }
    glEnd();
    }
    else
    {

//    glNormal3f(dir[0],dir[1],dir[2]);
    glNormal3f(-dir[0],-dir[1],-dir[2]);
    for(int i=0;i<=segment;i++)
    {
          glVertex3f(cylinder_seg_pos[2*i+1][0],
                    cylinder_seg_pos[2*i+1][1],
                    cylinder_seg_pos[2*i+1][2]);
    }
    glEnd();
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    }


}
void GetCylinder(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment,
                svVector3 *cylinder_seg_norm,
                 svVector3 *cylinder_seg_pos)
{
//    svVector3 cylinder_seg_norm[(segment+1)*2];
//    svVector3 cylinder_seg_pos[(segment+1)*2];
    float interval_degree  = 360./(float)segment;
    for(int i=0;i<=segment;i++) {
        if(i == 0)
        {
           svVector3 ori =  svGetPerpendicularVector(dir);
           cylinder_seg_pos[0] = glyph + radius * ori;
           cylinder_seg_pos[1] = cylinder_seg_pos[0] + height * dir;
           cylinder_seg_norm[0] = normalize(cylinder_seg_pos[0] - glyph);
           cylinder_seg_norm[1] = normalize(cylinder_seg_pos[0] - glyph);
         }
        else
        {
           cylinder_seg_pos[i*2] = svGetRotatePoint(cylinder_seg_pos[0],
                                         glyph,
                                         dir,
                                         360-i * interval_degree);
           cylinder_seg_pos[i*2+1] = cylinder_seg_pos[i*2] + height * dir;
           cylinder_seg_norm[i*2] = normalize(cylinder_seg_pos[i*2] - glyph);
           cylinder_seg_norm[i*2+1] = normalize(cylinder_seg_pos[i*2] - glyph);
        }
    }
}
void GetCone(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment,
              svVector3 *cone_seg_norm, svVector3 *cone_seg_pos)
{
//    svVector3 cone_seg_norm[segment+1];
//    svVector3 cone_seg_pos[segment];
//    cone_seg_norm[0]= normalize(dir);
    float interval_degree  = 360./(float)segment;
    for(int i=0;i<segment;i++) {
        if(i == 0)
        {
           svVector3 ori =  normalize(svGetPerpendicularVector(dir));
           cone_seg_pos[0] = glyph + radius * ori;
         }
        else
        {
           cone_seg_pos[i] = svGetRotatePoint(cone_seg_pos[0],
                                         glyph,
                                         dir,
                                         i * interval_degree);
        }
    }

      glyph = glyph + height * dir;

    for(int i=1;i<=segment;i++){
        if(i==1)
        {
           cone_seg_norm[0] = svAverage(
                                  svGetNorm(
                                            glyph,
                                            cone_seg_pos[0],
                                            cone_seg_pos[segment-1]
                                           ),
                                  svGetNorm(glyph,
                                            cone_seg_pos[1],
                                            cone_seg_pos[0]
                                            )
                                        );
          }
          else if(i == segment){
            cone_seg_norm[i-1] = svAverage(
                                   svGetNorm(glyph,
                                        cone_seg_pos[i-1],
                                        cone_seg_pos[i-2]),
                                   svGetNorm(glyph,
                                             cone_seg_pos[0],
                                             cone_seg_pos[i-1]
                                             )
                                        );
          }
          else
          {
            cone_seg_norm[i-1] = svAverage(
                                   svGetNorm(glyph,
                                        cone_seg_pos[i-1],
                                        cone_seg_pos[i-2]),
                                   svGetNorm(glyph,
                                             cone_seg_pos[i],
                                             cone_seg_pos[i-1]
                                             )
                                        );
          }
        }
}

void RenderCone(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment)
{

    svVector3 cone_seg_norm[segment+1];
    svVector3 cone_seg_pos[segment];
    cone_seg_norm[0]= normalize(dir);
    float interval_degree  = 360./(float)segment;
    for(int i=0;i<segment;i++) {
        if(i == 0)
        {
           svVector3 ori =  normalize(svGetPerpendicularVector(cone_seg_norm[0]));
           cone_seg_pos[0] = glyph + radius * ori;
         }
        else
        {
           cone_seg_pos[i] = svGetRotatePoint(cone_seg_pos[0],
                                         glyph,
                                         cone_seg_norm[0],
                                         i * interval_degree);
        }
    }

      glyph = glyph + height * dir;

    for(int i=1;i<=segment;i++){
        if(i==1)
        {
           cone_seg_norm[1] = svAverage(
                                  svGetNorm(
                                            glyph,
                                            cone_seg_pos[0],
                                            cone_seg_pos[segment-1]
                                           ),
                                  svGetNorm(glyph,
                                            cone_seg_pos[1],
                                            cone_seg_pos[0]
                                            )
                                        );
          }
          else if(i == segment){
            cone_seg_norm[i] = svAverage(
                                   svGetNorm(glyph,
                                        cone_seg_pos[i-1],
                                        cone_seg_pos[i-2]),
                                   svGetNorm(glyph,
                                             cone_seg_pos[0],
                                             cone_seg_pos[i-1]
                                             )
                                        );
          }
          else
          {
            cone_seg_norm[i] = svAverage(
                                   svGetNorm(glyph,
                                        cone_seg_pos[i-1],
                                        cone_seg_pos[i-2]),
                                   svGetNorm(glyph,
                                             cone_seg_pos[i],
                                             cone_seg_pos[i-1]
                                             )
                                        );
          }
        }
      for(int i=0;i<segment-1;i++)
      {//cerr<<cone_seg_norm[i+1][0]<<" "<<cone_seg_norm[i+2][0]<<" "<<cone_seg_norm[i+1][1]<<" "<<cone_seg_norm[i+2][1]<<" "<<cone_seg_norm[i+1][2]<<" "<<cone_seg_norm[i+2][2]<<endl;
          glBegin(GL_TRIANGLES);
              glNormal3f(cone_seg_norm[0][0],cone_seg_norm[0][1],cone_seg_norm[0][2]);
              glVertex3f(glyph[0],glyph[1],glyph[2]);

              glNormal3f(cone_seg_norm[i+1][0],cone_seg_norm[i+1][1],cone_seg_norm[i+1][2]);
              glVertex3f(cone_seg_pos[i][0],cone_seg_pos[i][1],cone_seg_pos[i][2]);

              glNormal3f(cone_seg_norm[i+2][0],cone_seg_norm[i+2][1],cone_seg_norm[i+2][2]);
              glVertex3f(cone_seg_pos[i+1][0],cone_seg_pos[i+1][1],cone_seg_pos[i+1][2]);
            glEnd();

       }
          glBegin(GL_TRIANGLES);
              glNormal3f(cone_seg_norm[0][0],cone_seg_norm[0][1],cone_seg_norm[0][2]);
              glVertex3f(glyph[0],glyph[1],glyph[2]);

              glNormal3f(cone_seg_norm[segment][0],cone_seg_norm[segment][1],cone_seg_norm[segment][2]);
              glVertex3f(cone_seg_pos[segment-1][0],cone_seg_pos[segment-1][1],cone_seg_pos[segment-1][2]);

              glNormal3f(cone_seg_norm[1][0],cone_seg_norm[1][1],cone_seg_norm[1][2]);
              glVertex3f(cone_seg_pos[0][0],cone_seg_pos[0][1],cone_seg_pos[0][2]);
            glEnd();

        glBegin(GL_POLYGON);
        glNormal3f(-dir[0],-dir[1],-dir[2]);
        for(int i=0;i<segment;i++)
        {
              glVertex3f(cone_seg_pos[i][0],cone_seg_pos[i][1],cone_seg_pos[i][2]);
        }
        glEnd();
}

void RenderButtonUp(svScalar width, svScalar height,
                 int segment, svScalar radius)
{
    svVector3 boundary_pos[segment+1];

    for(int i=0;i<=segment;i++)
    {
         double degree = (i * 2*SV_PI)/segment;
         boundary_pos[i][0] = cos(degree) * radius;
         boundary_pos[i][1] = sin(degree) * radius;
         boundary_pos[i][2] = 0;      
    } 
}

void RenderButtonDown(svScalar width, svScalar height,
                 int segment)
{

}
/*
GLuint loadTexture(GLuint texture, const string filename, int &width, int &height) 
{
   png_byte header[8];
   FILE *fp = fopen(filename.c_str(), "rb");
   if (!fp) {
     return TEXTURE_LOAD_ERROR;
   }
   fread(header, 1, 8, fp);
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }

   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }

   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }

   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }

   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }

   png_init_io(png_ptr, fp);

   png_read_info(png_ptr, info_ptr);

   int bit_depth, color_type;
   png_uint_32 twidth, theight;
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
   width = twidth;
   height = theight;
   png_read_update_info(png_ptr, info_ptr);

   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

   png_byte *image_data = new png_byte[rowbytes * height];
   if (!image_data) {

     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }

   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers) {

     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     delete[] image_data;
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }

  for (int i = 0; i < height; ++i)
     row_pointers[height - 1 - i] = image_data + i * rowbytes;

   png_read_image(png_ptr, row_pointers);

   GLuint texture1;
   glGenTextures(1, &texture1);
   glBindTexture(GL_TEXTURE_2D, texture1);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
       GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   delete[] image_data;
   delete[] row_pointers;
   fclose(fp);
 
   return texture1;
}

*/
/*
GLuint loadBMP(const char * filename)
{
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int width, height;
  unsigned int imageSize;   // = width*height*3
  unsigned char * data;

  FILE * file = fopen(filename,"rb");
  if (!file){printf("Image could not be opened\n"); return 0;}

  if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return false;
  }

  if ( header[0]!='B' || header[1]!='M' ){
    printf("Not a correct BMP file\n");
    return 0;
  }

  dataPos    = *(int*)&(header[0x0A]);
  imageSize  = *(int*)&(header[0x22]);
  width      = *(int*)&(header[0x12]);
  height     = *(int*)&(header[0x16]);

  if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos==0)      dataPos=54; // The BMP header is done that way

  data = new unsigned char [imageSize];
  fread(data,1,imageSize,file);
  fclose(file);

  GLuint texName;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
                height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                data);


  return texName;
}
*/
/*
GLuint loadBMP2( const char * filename )
{

  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 256;
  height = 256;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

 for(int i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];

   data[index] = R;
   data[index+2] = B;

}

  GLuint texName;

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
                height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                data);


  return texName;
}*/
/*#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
static GLuint texName;

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage);
}
*/
void SOILTexture(const char *filename, GLuint & texture)
{
     int width, height;
     unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

//   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
              GL_UNSIGNED_BYTE, image);


    free(image);
}
}
