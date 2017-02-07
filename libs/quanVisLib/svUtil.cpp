
#include "svUtil.h"

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

	//cerr<<x[0]<<" "<<x[1]<<" "<<x[2]<<endl;

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
 if (0.0 == r0[0])  return GL_FALSE;

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
            while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
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
}
