


#include "svWidget.h"
#include <cmath>

namespace __svl_lib {


void svWidget::Init()
{
   layer.free();

  for(int i=0;i<level;i++)layer.add(0);

   boxside = 2;
   triangleside = 2;

   line[0][0]=0;line[0][1]=boxside/2.;
   line[1][0]= boxside * (level+1); line[1][1] = boxside/2.;

   box[0][0]= line[0][0] + 0.5 * boxside; box[0][1] = 0;
   box[1][0]= line[1][0] - 0.5 * boxside; box[1][1] = 0;
   box[2][0]= line[1][0] - 0.5 * boxside; box[2][1] = 0;

   showbox = false; //the unselected layers

   svScalar value1=sqrt(3);

//(1/2, -sqrt(3)/6)
//(-1/2, -sqrt(3)/6)
//(0, sqrt(3)/3)

   svVector3 pos;
   pos[0] = box[0][0];
   pos[1] = box[0][1] - triangleside * sqrt(3)/3;
   svVector3 t[3];
   t[0][0] = -triangleside/2;
   t[0][1] = -triangleside * value1/6.;
   t[1][0] = triangleside/2;
   t[1][1] = -triangleside * value1/6.;
   t[2][0] = 0;
   t[2][1] = triangleside * value1/3.;

   triangle[0][0][0] = t[0][0] + pos[0];
   triangle[0][0][1] = t[0][1] + pos[1];
   triangle[0][1][0] = t[1][0] + pos[0];
   triangle[0][1][1] = t[1][1] + pos[1];
   triangle[0][2][0] = t[2][0] + pos[0];
   triangle[0][2][1] = t[2][1] + pos[1];
 
  // cerr<<t[0][0]<<" "<<t[0][1]<<" "<<t[1][0]<<" "<<t[1][1]<<" "<<t[2][0]<<" "<<t[2][1]<<endl;

   //cerr<<triangle[0][0][0]<<" "<<triangle[0][0][1]<<" "
     // <<triangle[0][1][0]<<" "<<triangle[0][1][1]<<" "
      //<<triangle[0][2][0]<<" "<<triangle[0][2][1]<<endl;

   pos[0] = box[1][0];
   pos[1] = box[1][1] - triangleside *  sqrt(3)/3; 
   triangle[1][0][0] = t[0][0] + pos[0];
   triangle[1][0][1] = t[0][1] + pos[1] ;
   triangle[1][1][0] = t[1][0] + pos[0];
   triangle[1][1][1] = t[1][1] + pos[1];
   triangle[1][2][0] = t[2][0] + pos[0];
   triangle[1][2][1] = t[2][1] + pos[1];

   pos[0] = box[2][0];
   pos[1] = box[2][1] - triangleside *  sqrt(3)/3; 
   triangle[2][0][0] = t[0][0] + pos[0];
   triangle[2][0][1] = t[0][1] + pos[1] ;
   triangle[2][1][0] = t[1][0] + pos[0];
   triangle[2][1][1] = t[1][1] + pos[1];
   triangle[2][2][0] = t[2][0] + pos[0];
   triangle[2][2][1] = t[2][1] + pos[1];

   SetVisible();
}

void svWidget::Mouse(svScalar tranx, svScalar trany,
                   svScalar scalex, svScalar scaley,
                  int x, int y)
{
   mousex = x;
   mousey = y;
   for(int i=0;i<3;i++)
   {
      tselect[i] = false;
 //     bselect[i] = false;
   }
   bselect = false;
      if(x > box[0][0]*scalex + tranx
       && x < box[2][0]*scalex + tranx
       && y >box[0][1]*scaley+trany
       && y< (box[0][1]+boxside)*scaley+trany)
      {
          bselect=true;
      }
   if(!bselect) {
   int value = triangleside * sqrt(3);
   for(int i=0;i<3;i++)
   {
      if(x > triangle[i][0][0]*scalex+tranx 
      && x < (triangle[i][0][0]+triangleside)*scalex + tranx
       && y > triangle[i][0][1]*scaley+trany 
       && y< (triangle[i][0][1]+value)*scaley + trany)
      {
         if((i==2&&showbox) || i!=2)
         { //if(!(showbox && i==0))
           //{
               tselect[i]=true;break;
            //}
         }
      }
     }
     }
}
void svWidget::MoveRight()
{
   svScalar movement;
    
    int start = (box[0][0] - line[0][0] - boxside)/boxside;
    if(start <0) start = 0;
    else if(start < (box[0][0] - line[0][0] - boxside)/boxside) start = start + 1;

   movement = (line[0][0] + 0.5 * boxside + (start-1) * boxside) - box[0][0];

  if((box[1][0] + movement)> line[0][0] + 1.*boxside
   && (box[0][0]+movement)>line[0][0]
   && (box[2][0]+movement)<line[1][0])
  {
   for(int i=0;i<3;i++)
   {
         box[i][0] = box[i][0] + movement;
         for(int j=0;j<3;j++)
              triangle[i][j][0] =triangle[i][j][0]+movement;
   }
  }
  SetVisible();
}

void svWidget::MoveLeft()
{
   svScalar movement;

    int start = (box[0][0] - line[0][0] - boxside)/boxside;
    if(start <0) start = 0;
    else if(start < (box[0][0] - line[0][0] - boxside)/boxside) start = start + 1;

   movement = line[0][0] + 0.5 * boxside + (start+1) * boxside - box[0][0];    

  if((box[1][0] + movement)> line[0][0] + 1.*boxside
   && (box[0][0]+movement)>line[0][0]
   && (box[2][0]+movement)<line[1][0])
  {
   for(int i=0;i<3;i++)
   {
         box[i][0] = box[i][0] + movement;
         for(int j=0;j<3;j++)
              triangle[i][j][0] =triangle[i][j][0]+movement;
   }
  }
  SetVisible();
}
void svWidget::Move(int x, int y)
{
   svScalar movement;
  if(bselect)
  {
            movement = x - mousex;
            mousex = x; mousey=y;

  if((box[1][0] + movement)> line[0][0] + 1.*boxside
   && (box[0][0]+movement)>line[0][0]
   && (box[2][0]+movement)<line[1][0])
  {
   for(int i=0;i<3;i++)
   {
         box[i][0] = box[i][0] + movement;
         for(int j=0;j<3;j++)
              triangle[i][j][0] =triangle[i][j][0]+movement;
   }
  }
  }
  else
  {
   int index=-1;
   for(int i=0;i<3;i++)
   {
      if(tselect[i])
      {
            movement = x - mousex;
            mousex = x; mousey=y;
            index = i;break;
      }
   }
  if( index==0)
  {
       if((box[0][0]+movement)>line[0][0] && box[0][0] +movement <= box[1][0]-boxside)
       {
         box[index][0] = box[index][0] + movement;
         for(int j=0;j<3;j++)
         triangle[index][j][0] =triangle[index][j][0]+movement;
       }
  }
  else if(index == 1)
  {
      if(showbox)
      {
       if(box[1][0]+movement >= box[0][0]+boxside &&  box[1][0] +movement <= box[2][0]-boxside)
       {
         box[index][0] = box[index][0] + movement;
         for(int j=0;j<3;j++)
         triangle[index][j][0] =triangle[index][j][0]+movement;
       }
     }
     else
     {
       if(box[1][0]+movement >= box[0][0]+boxside &&  box[1][0] +movement < line[1][0])
       {
         box[index][0] = box[index][0] + movement;
         for(int j=0;j<3;j++)
         triangle[index][j][0] =triangle[index][j][0]+movement;
         if(!showbox)
         {
            box[index+1][0] = box[index+1][0] + movement;
            for(int j=0;j<3;j++)
            triangle[index+1][j][0] =triangle[index+1][j][0]+movement;
         }
       }
     }
  }
  else if(index == 2)
  {
       if((box[2][0]+movement)<line[1][0] && box[2][0]+movement>=box[1][0]+boxside)
       {
         box[index][0] = box[index][0] + movement;
         for(int j=0;j<3;j++)
         triangle[index][j][0] =triangle[index][j][0]+movement;
       }
  }

  }
 
  SetVisible();
}

void svWidget::Render()
{
  glEnable (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor3f(0,0,0);
  glBegin(GL_LINES);
  glVertex2f(line[0][0], line[0][1]);
  glVertex2f(line[1][0], line[1][1]);
  glEnd();

  if(bselect)
     glColor4f(253./255., 174./255., 97./255.,0.5);
  else
     glColor4f(0.5,0.5,0.5,0.5);
  glBegin(GL_QUADS);
  glVertex2f(box[0][0],box[0][1]);
  glVertex2f(box[1][0],box[1][1]);
  glVertex2f(box[1][0],box[1][1]+boxside);
  glVertex2f(box[0][0],box[0][1]+boxside);
  glEnd();    


  glColor4f(0.,0.,0.,0.5);

  for(int i=0;i<3;i++)
  {
     if(i==2 && !showbox)break;

      if(tselect[i])      glColor4f(253./255., 174./255., 97./255.,0.5); 
      else glColor4f(0.,0.,0.,0.5);

      glBegin(GL_TRIANGLES);
      for(int j=0;j<3;j++)
      {
           glVertex2f(triangle[i][j][0], triangle[i][j][1]);
      }
      glEnd();

       glColor3f(0,0,0);
      glBegin(GL_LINE_LOOP);
      for(int j=0;j<3;j++)
      {
           glVertex2f(triangle[i][j][0], triangle[i][j][1]);
      }
      glEnd();

  }

  glColor3f(0,0,0);
  glBegin(GL_LINE_STRIP);
  glVertex2f(box[1][0],box[1][1]);
  glVertex2f(box[2][0],box[2][1]);
  glVertex2f(box[2][0],box[2][1]+boxside);
  glVertex2f(box[1][0],box[1][1]+boxside);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex2f(box[0][0],box[0][1]);
  glVertex2f(box[1][0],box[1][1]);
  glVertex2f(box[1][0],box[1][1]+boxside);
  glVertex2f(box[0][0],box[0][1]+boxside);
  glEnd();

  glBegin(GL_LINES);
  glVertex2f(line[0][0],line[0][1]-boxside*0.5);
  glVertex2f(line[0][0],line[0][1]+boxside*0.5);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(line[1][0],line[1][1]-boxside*0.5);
  glVertex2f(line[1][0],line[1][1]+boxside*0.5);
  glEnd();

  glPointSize(4);
  glBegin(GL_POINTS);

  for(int i=0;i<level;i++)
  {
    if(layer[i]) glColor3f(165./255., 0, 38./255.);
    else glColor3f(0.5,0.5,0.5);    
     glVertex2f(line[0][0]+(svScalar)i * boxside + boxside, line[0][1]);
  }
  glEnd();
  glPointSize(1);

  svScalar x[3];
  bool flag[3];flag[0]=false;
  if(box[1][0]-box[0][0] > 15*boxside)
  { 
         flag[1]=false;
         x[0] = box[0][0];
         x[1] = box[1][0];
  }
  else 
  {
            x[0] = box[0][0];
            x[1] = x[0] + 15*boxside;
            flag[1]=true;
  }
  if(!showbox)
  { 
         x[2] = box[2][0];
         flag[2]=false;
  }
  else
  { 
       if(box[2][0]-x[1] < 15*boxside)
       {      x[2] = x[1] + 15 * boxside;flag[2]=true;}
       else
       {      x[2] = box[2][0];flag[2]=false;}
   }
 
  glColor3f(0,0,0); 
  char str[20];
  for(int i=0;i<3;i++)
  {
     if( (showbox &&i==2) || i<2)
     {
     sprintf(str, "(%0.2f, %0.2f, %0.2f)", values[i][0],values[i][1],values[i][2]);
     glRasterPos2f(x[i],box[i][1]-boxside*1.5);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]); 
     
    if(flag[i])
    {
     glBegin(GL_LINES);
     glVertex2f(box[i][0], box[i][1]);
     glVertex2f(x[i], box[i][1]-boxside*1.5);
     glEnd();
    }
   }
  }

  for(int i=0;i<level;i=i+10)
  {
     sprintf(str, "%d", i);
     glRasterPos2f(line[0][0]+boxside + boxside*(float)i,box[i][1]+boxside*1.2);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
     glBegin(GL_LINES);
     glVertex2f(line[0][0]+boxside + boxside*(float)i,box[i][1]+boxside*1.2);
     glVertex2f(line[0][0]+boxside + boxside*(float)i,line[0][1]);
     glEnd();
  }  
}

void svWidget::SetIndex( int zmin, int zmax, int notshowz)
{
     if(notshowz >= 0) showbox =true;
     else showbox= false;
//cerr<<zmin<<" "<<zmax<<endl;
   if(showbox)
   {
   box[0][0]= line[0][0] + 0.5 * boxside+zmin*boxside; box[0][1] = 0;
   box[1][0]= line[0][0] + 0.5 * boxside+(zmax+1)*boxside; box[1][1] = 0;
   svScalar b2 = line[0][0] + 0.5 * boxside+(notshowz+1)*boxside;
   if(b2<line[1][0])
        box[2][0]= b2; 
   else
        box[2][0] = line[1][0]-0.5*boxside;
   box[2][1] = 0;
   }
   else
   {
   box[0][0]= line[0][0] + 0.5 * boxside+zmin*boxside; box[0][1] = 0;
   box[1][0]= line[0][0] + 0.5 * boxside+(zmax+1)*boxside; box[1][1] = 0;
   box[2][0]= box[1][0]; box[2][1] = 0;
   }

   svScalar value1=sqrt(3);

   svVector3 pos;
   pos[0] = box[0][0];
   pos[1] = box[0][1] - triangleside *  sqrt(3)/3;
   svVector3 t[3];
   t[0][0] = -triangleside/2;
   t[0][1] = -triangleside * value1/6.;
   t[1][0] = triangleside/2;
   t[1][1] = -triangleside * value1/6.;
   t[2][0] = 0;
   t[2][1] = triangleside * value1/3.;

   triangle[0][0][0] = t[0][0] + pos[0];
   triangle[0][0][1] = t[0][1] + pos[1] ;
   triangle[0][1][0] = t[1][0] + pos[0];
   triangle[0][1][1] = t[1][1] + pos[1];
   triangle[0][2][0] = t[2][0] + pos[0];
   triangle[0][2][1] = t[2][1] + pos[1];

   pos[0] = box[1][0];
   pos[1] = box[1][1] - triangleside*sqrt(3)/3;
   triangle[1][0][0] = t[0][0] + pos[0];
   triangle[1][0][1] = t[0][1] + pos[1] ;
   triangle[1][1][0] = t[1][0] + pos[0];
   triangle[1][1][1] = t[1][1] + pos[1];
   triangle[1][2][0] = t[2][0] + pos[0];
   triangle[1][2][1] = t[2][1] + pos[1];

   pos[0] = box[2][0];
   pos[1] = box[2][1] - triangleside*sqrt(3)/3;
   triangle[2][0][0] = t[0][0] + pos[0];
   triangle[2][0][1] = t[0][1] + pos[1] ;
   triangle[2][1][0] = t[1][0] + pos[0];
   triangle[2][1][1] = t[1][1] + pos[1];
   triangle[2][2][0] = t[2][0] + pos[0];
   triangle[2][2][1] = t[2][1] + pos[1];

   SetVisible();     
}
void svWidget::Repeat(bool showbox)
{
   this->showbox = showbox;

   if(showbox)
   {
   //box[0][0]= line[0][0] + 0.5 * boxside; box[0][1] = 0;
   //box[1][0]= line[0][0] + 1.5 * boxside; box[1][1] = 0;
   if((box[1][0]+boxside)<line[1][0])
        box[2][0]= box[1][0] + boxside; 
   else
        box[2][0] = line[1][0]-0.5*boxside;
   box[2][1] = 0;
   }
   else
   {
   //box[0][0]= line[0][0] + 0.5 * boxside; box[0][1] = 0;
   //box[1][0]= line[1][0] - 0.5 * boxside; box[1][1] = 0;
   box[2][0]= box[1][0]; box[2][1] = 0;
   }

   svScalar value1=sqrt(3);

   svVector3 pos;
   pos[0] = box[0][0];
   pos[1] = box[0][1] - triangleside *  sqrt(3)/3;
   svVector3 t[3];
   t[0][0] = -triangleside/2;
   t[0][1] = -triangleside * value1/6.;
   t[1][0] = triangleside/2;
   t[1][1] = -triangleside * value1/6.;
   t[2][0] = 0;
   t[2][1] = triangleside * value1/3.;

   triangle[0][0][0] = t[0][0] + pos[0];
   triangle[0][0][1] = t[0][1] + pos[1] ;
   triangle[0][1][0] = t[1][0] + pos[0];
   triangle[0][1][1] = t[1][1] + pos[1];
   triangle[0][2][0] = t[2][0] + pos[0];
   triangle[0][2][1] = t[2][1] + pos[1];

   pos[0] = box[1][0];
   pos[1] = box[1][1] - triangleside*sqrt(3)/3;
   triangle[1][0][0] = t[0][0] + pos[0];
   triangle[1][0][1] = t[0][1] + pos[1] ;
   triangle[1][1][0] = t[1][0] + pos[0];
   triangle[1][1][1] = t[1][1] + pos[1];
   triangle[1][2][0] = t[2][0] + pos[0];
   triangle[1][2][1] = t[2][1] + pos[1];

   pos[0] = box[2][0];
   pos[1] = box[2][1] - triangleside*sqrt(3)/3;
   triangle[2][0][0] = t[0][0] + pos[0];
   triangle[2][0][1] = t[0][1] + pos[1] ;
   triangle[2][1][0] = t[1][0] + pos[0];
   triangle[2][1][1] = t[1][1] + pos[1];
   triangle[2][2][0] = t[2][0] + pos[0];
   triangle[2][2][1] = t[2][1] + pos[1];

   SetVisible();
}

void svWidget::SetVisible()
{
    for(int i=0;i<level;i++) layer[i]=0;

    int start = (box[0][0] - line[0][0] - boxside)/boxside;
    int end = (box[1][0] - line[0][0]-boxside)/boxside;

//    cerr<<start<<" "<<end<<" "<<(box[0][0] - line[0][0] - boxside)/boxside<<" "<<(box[1][0] - line[0][0]-boxside)/boxside<<endl;

    if(start <0) start = 0;
   // else  start = start + 1;

    else if(start < (box[0][0] - line[0][0] - boxside)/boxside) start = start + 1;

    //cerr<<start<<" "<<end<<endl;

    if(!showbox)
    {
          for(int i=start;i<=end;i++)layer[i] = 1;
          layerindex[0] = start;
          layerindex[1] = end;
          layerindex[2] = end;
    } 
    else
    {
        int end2 = (box[2][0] - line[0][0]-boxside)/boxside;
 
        layerindex[0]= start;
        layerindex[1] = end;
        layerindex[2] = end2;

        svIntArray l;
        for(int i=start;i<=end;i++)l.add(1);
        for(int i=end+1;i<=end2;i++)l.add(0);

        int size = end2 - start+1;
        int count = 0;
        for(int i=start-1;i>=0;i--)
        {
           int ii = size-1-count;
           layer[i] = l[ii]; 

           count++;
           if(count%size ==0 )count =0;
        }
        count = 0;
        for(int i=end2+1;i<level;i++)
        {
           int ii = count;
           layer[i] = l[ii];

           count++;
           if(count%size ==0 )count =0;
        }

        for(int i=start;i<=end;i++) layer[i] = 1;
        for(int i=end+1;i<=end2;i++)layer[i]=0;
    }
}

bool svWidget::isSelect()
{
    if(bselect) return true;

    bool flag = false;
    for(int i=0;i<3;i++)
      if(tselect[i]) {flag= true;break;}

   if(flag) return true;
   else return false;
}

void svWidget::Reset()
{
     for(int i=0;i<3;i++)tselect[i]=false;
     bselect = false;
}
}


