#include <GL/glew.h>

#include "svArrowGlyph.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"




using namespace std;


namespace __svl_lib{

svArrowGlyph::svArrowGlyph(svVectorField *f):svGlyph()
{
  field = f;

  arrow_vertices = new VBOVertex[1];
  arrow_indices = new GLuint[1];
  arrow_base_vertices = new VBOVertex[1];
  arrow_base_indices = new GLuint[1];
  tube_vertices = new VBOVertex[1];
  legend_vertices = new VBOVertex1[1];
  tube_indices = new GLuint[1];
  tube_base_vertices = new VBOVertex[1];
  tube_base_indices = new GLuint[1];
  //arrow_indices_size = 0;
}
void svArrowGlyph::Clean()
{
     cleanup();
}
void svArrowGlyph::CleanData()
{
      delete [] arrow_vertices;
      delete [] arrow_base_vertices;
      delete [] legend_vertices;
      delete [] tube_vertices;
      delete [] tube_base_vertices;

}
void svArrowGlyph::CleanIndex()
{
     delete [] arrow_indices;
     delete [] tube_indices;
     delete [] arrow_base_indices;
     delete [] tube_base_indices;
}
void svArrowGlyph::Update()
{
     UpdateData();
     UpdateIndex();
}
void svArrowGlyph::UpdateData()
{
     //arrow_indices_size = dataSize*3*ARROWSLICE;
     //tube_indices_size = dataSize * 4 * CYLINDERSLICE; 

     CleanData();

    dataSize = 0;
    for(int i=0;i<seed_num;i++)
        for(int j=0;j<glyph[i].size();j++)
              dataSize++;

     arrow_vertices_size = dataSize*(ARROWSLICE+1);
     tube_vertices_size = dataSize*(CYLINDERSLICE+1)*2;
     arrow_base_vertices_size = dataSize*(ARROWSLICE+1);
     tube_base_vertices_size = dataSize*(CYLINDERSLICE+1+1);

     arrow_vertices = new VBOVertex[arrow_vertices_size];//svVector3Array[seed_num];
     tube_vertices = new VBOVertex[tube_vertices_size];//svVector3Array[seed_num];
     arrow_base_vertices = new VBOVertex[arrow_base_vertices_size];//svVector3Array[seed_num];
     tube_base_vertices = new VBOVertex[tube_base_vertices_size];//svVector3Array[seed_num];
     legend_vertices = new VBOVertex1[dataSize*2];//svVector3Array[seed_num];
     //arrow_indices = new GLuint[dataSize*3*ARROWSLICE];//svIntArray[seed_num];
     //tube_indices = new GLuint[dataSize*4*CYLINDERSLICE];//svIntArray[seed_num];
}

void svArrowGlyph::UpdateIndex()
{
     CleanIndex();

     int count = 0;
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
          {
                   if(visibleLabel[i][j])
                          count++;
          } 
     }
     //cerr<<count<<endl;
     arrow_indices_size = count*3*ARROWSLICE;
     tube_indices_size = count*4*CYLINDERSLICE;
     arrow_base_indices_size = count*3*ARROWSLICE;
     tube_base_indices_size = count*3*CYLINDERSLICE;
     arrow_indices = new GLuint[arrow_indices_size];
     tube_indices = new GLuint[tube_indices_size];
     arrow_base_indices = new GLuint[arrow_base_indices_size];
     tube_base_indices = new GLuint[tube_base_indices_size];
}


void svArrowGlyph::UpdateIndexVBO()
{
   if(glIsBuffer(ARROW_IVBO))
           glDeleteBuffers(1, &ARROW_IVBO);
   glGenBuffers(1, &ARROW_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_indices_size,
                arrow_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   if(glIsBuffer(ARROW_BASE_IVBO))
           glDeleteBuffers(1, &ARROW_BASE_IVBO);
   glGenBuffers(1, &ARROW_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_base_indices_size,
                arrow_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


   if(glIsBuffer(TUBE_IVBO))
           glDeleteBuffers(1, &TUBE_IVBO); 
   glGenBuffers(1, &TUBE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_indices_size,
                tube_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   if(glIsBuffer(TUBE_BASE_IVBO))
           glDeleteBuffers(1, &TUBE_BASE_IVBO);
   glGenBuffers(1, &TUBE_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_base_indices_size,
                tube_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

void svArrowGlyph::UpdateArrowVBO()
{
   if(glIsBuffer(ARROW_VBO))
           glDeleteBuffers(1, &ARROW_VBO);
   glGenBuffers(1, &ARROW_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_vertices_size,
                arrow_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   if(glIsBuffer(ARROW_BASE_VBO))
           glDeleteBuffers(1, &ARROW_BASE_VBO);
   glGenBuffers(1, &ARROW_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_base_vertices_size,
                arrow_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

void svArrowGlyph::UpdateTubeVBO()
{
    if(glIsBuffer(TUBE_VBO))
          glDeleteBuffers(1,&TUBE_VBO);
   glGenBuffers(1, &TUBE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_vertices_size,
                tube_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

    if(glIsBuffer(TUBE_BASE_VBO))
          glDeleteBuffers(1,&TUBE_BASE_VBO);
   glGenBuffers(1, &TUBE_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_base_vertices_size,
                tube_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

void svArrowGlyph::GenerateVBO()
{
//   arrow_indices_size = dataSize*3*ARROWSLICE;
//   int num =dataSize*(ARROWSLICE+1);

   glGenBuffers(1, &ARROW_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_vertices_size,
                arrow_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_indices_size,
                arrow_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_base_vertices_size,
                arrow_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_base_indices_size,
                arrow_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);




   glGenBuffers(1, &TUBE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_vertices_size,
                tube_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_indices_size,
                tube_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_base_vertices_size,
                tube_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_base_indices_size,
                tube_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void svArrowGlyph::RenderVBO()
{
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_TRIANGLES, arrow_indices_size, GL_UNSIGNED_INT,NULL);

//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//   glDisableClientState(GL_VERTEX_ARRAY);
//   glDisableClientState(GL_NORMAL_ARRAY);
//   glDisableClientState(GL_COLOR_ARRAY);

   glBindBuffer(GL_ARRAY_BUFFER, ARROW_BASE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_BASE_IVBO);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_TRIANGLES, arrow_base_indices_size, GL_UNSIGNED_INT,NULL);

//========================================

   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_QUADS, tube_indices_size, GL_UNSIGNED_INT,NULL);

   glBindBuffer(GL_ARRAY_BUFFER, TUBE_BASE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_BASE_IVBO);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_TRIANGLES, tube_base_indices_size, GL_UNSIGNED_INT,NULL);
//===============================

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);

}


void svArrowGlyph::Generate()
{

}

void svArrowGlyph::Render()
{
    
}

void svArrowGlyph::cleanup()
{
   if(glIsBuffer(ARROW_VBO))
   {
     glDeleteBuffers(1, &ARROW_VBO);
    glDeleteBuffers(1, &ARROW_IVBO);
    glDeleteBuffers(1, &TUBE_VBO);
    glDeleteBuffers(1, &TUBE_IVBO);
    glDeleteBuffers(1, &ARROW_BASE_VBO);
    glDeleteBuffers(1, &ARROW_BASE_IVBO);
    glDeleteBuffers(1, &TUBE_BASE_VBO);
    glDeleteBuffers(1, &TUBE_BASE_IVBO);
   }
    delete [] arrow_vertices;
      delete [] arrow_base_vertices;
      delete [] arrow_indices;
      delete [] arrow_base_indices;
      delete [] legend_vertices;
    delete [] tube_vertices;
      delete [] tube_base_vertices;
      delete [] tube_indices;
      delete [] tube_base_indices;

}

}
