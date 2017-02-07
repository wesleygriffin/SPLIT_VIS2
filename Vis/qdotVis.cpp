#include "qdotGUI.h"

void initLighting()
{
  //setting of lightingcon
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .5};
  GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 0.5 };
  GLfloat mat_shininess[] = { 20.0 };
 // GLfloat light_position[] = { 0, 0, 60, 0.0 };
  GLfloat light_position[] = { 200.,200.,200, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
 // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}
void reshape(int width, int height)
{
	glViewport (0., 0., width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

        gluPerspective(view_info.view_angle,
                   view_info.aspect_ratio * width/height,
                   0.1,
                   1000);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
        gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
              0,0,0,
              0, 1, 0);

	glClearColor(0.5,0.5,0.5,1);	
//--------------------------------------------------------------------------------------------------
      glGetIntegerv (GL_VIEWPORT, viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX,vmat);
      glGetDoublev (GL_PROJECTION_MATRIX, pmat);

	glGetIntegerv (GL_VIEWPORT, qdotCutSlice[0]->viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, qdotCutSlice[0]->mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, qdotCutSlice[0]->projmatrix);

	glGetIntegerv (GL_VIEWPORT, qdotCutSlice[1]->viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, qdotCutSlice[1]->mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, qdotCutSlice[1]->projmatrix);

	svVector3 cameraLookat;
	cameraLookat[0] = 0;
	cameraLookat[1] = 0;
	cameraLookat[2] = 0;
	svVector3 cameraUp;
	cameraUp[0] = 0;
	cameraUp[1] = 1;
	cameraUp[2] = 0;
	svScalar near;

	qdotPicking[0]->initCamera(view_info.eye, cameraLookat, cameraUp,
				view_info.view_angle,
				width, height,
				0.1);
	qdotPicking[1]->initCamera(view_info.eye, cameraLookat, cameraUp,
				view_info.view_angle,
				width, height,
				0.1);

}


void main_reshape(int width, int height)
{
	glViewport (0., 0., width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

        gluPerspective(45,
                   (GLfloat)width/(GLfloat)height,
                   0.1,
                   1000);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
        gluLookAt(0,0,100,
              0,0,0,
              0, 1, 0);


	image_height = height;
	image_width = width;
	sub_height = height;
	sub_width = width/2;

    glutSetWindow(left_window);
    glutPositionWindow(0., 0.);
    glutReshapeWindow(sub_width, sub_height);
    glutReshapeFunc(reshape);

    glutSetWindow(right_window);
    glutPositionWindow(sub_width, 0.);
    glutReshapeWindow(width - sub_width, sub_height);
}

void main_display(void)
{
	glViewport (0., 0., image_width, image_height);
        glClearColor(0.5,0.5,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glutSwapBuffers();

}

void spatial_display(int d)
{
	trackball[d].getMatrix().getValue(m[d]);

	glPushMatrix();
	glMultMatrixf(m[d]);

	if(qdotRegion[d])
	{
		qdotMesh[d]->renderMesh(qdotField[d]->getCellPosVec(), 
								qdotField[d]->getCellVelVec(),
								qdotField[d]->getCellDenVec(),
								qdotField[d]->getCellIndex(),
								false, qdotOpacity);
	}

	if(qdotEncode[0] == 0)
	{
		qdotArrowSelection[0]->extendLength(qdotLength * 5e-11);
	}
	else if(qdotEncode[0] == 1)
	{
		qdotArrowSelection[0]->extendLength(qdotLength);
	}

	if(qdotOverview[d])
	{
		//if(qdotEncode[0] == 0)
		//{
			qdotDisc[d]->renderDisc(qdotLength * 5e-11);
		//}
		//else if(qdotEncode[0] == 1)
		//{
		//	qdotDisc[d]->renderDisc(qdotLength);
		//}
	}
	//#ifdef _VTK

	//#endif
	qdotArrowSelection[d]->CallList();
	qdotOutline[d]->DrawAxis(qdotField[d]->getLbbox(), qdotField[d]->getRbbox());
	qdotOutline[d]->DrawXYZ(qdotField[d]->getLbbox(), qdotField[d]->getRbbox());
	qdotOutline[d]->DrawAxis_scale(qdotField[d]->getLbbox(), qdotField[d]->getRbbox(), image_height);

	if(qdotCutVisible)
		qdotCutSlice[d]->RenderSlice();

	glPopMatrix();
}

void nonspatial_display(int d)
{
 	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glDisable(GL_LIGHTING);

		float ff;
		int f = sqrt((qdotDistanceSample[d] * qdotDistanceSample[d])/(twodzoom[d] * twodzoom[d]));
		ff = f;
		if(f==0)
			ff = 1.;
		else
			ff = f + 1.;

		glPushMatrix();

		glTranslatef(image_width/4.,image_height/2.,0);
		glScalef(twodzoom[d],twodzoom[d],twodzoom[d]);
		glTranslatef(twotranx[d],twotrany[d],0.);

		if(qdotDisplay[d] == 2)
		{
		  qdotMap[d]->Render(qdotOpacity, ff);
		}
		glPopMatrix();
	

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(pmat);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(vmat);
}
void left_display(void)
{
        glClearColor(qdotBack, qdotBack, qdotBack,1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(qdotDisplay[0] == 0 || qdotDisplay[0] == 1)
	{
		spatial_display(0);
	}
	else
	{
		nonspatial_display(0);
	}

//------------------------------------------------------------------------
	glDisable(GL_LIGHTING);	
	GLdouble vmat2[16], pmat2[16];
	GLint    viewport2[4];

      glGetIntegerv (GL_VIEWPORT, viewport2);
      glGetDoublev (GL_MODELVIEW_MATRIX,vmat2);
      glGetDoublev (GL_PROJECTION_MATRIX, pmat2);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport2[2], 0.0, (GLfloat) viewport2[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();

	glColor3f(qdotFont[0],qdotFont[1], qdotFont[2]);
	glBegin(GL_QUADS);
	glVertex2f(viewport2[2]-2,0);
	glVertex2f(viewport2[2],0);
	glVertex2f(viewport2[2],viewport2[3]);
	glVertex2f(viewport2[2]-2,viewport2[3]);
	glEnd();


	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat2);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat2);

        glutSwapBuffers();
}

void right_display(void)
{
        glClearColor(qdotBack, qdotBack, qdotBack,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(qdotDisplay[1] == 0|| qdotDisplay[1] == 1)
	{
		spatial_display(1);
	}
	else
	{
		nonspatial_display(1);
	}

//------------------------------------------------------------------------
	glDisable(GL_LIGHTING);	
	GLdouble vmat2[16], pmat2[16];
	GLint    viewport2[4];

      glGetIntegerv (GL_VIEWPORT, viewport2);
      glGetDoublev (GL_MODELVIEW_MATRIX,vmat2);
      glGetDoublev (GL_PROJECTION_MATRIX, pmat2);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport2[2], 0.0, (GLfloat) viewport2[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();

	glColor3f(qdotFont[0],qdotFont[1], qdotFont[2]);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(2,0);
	glVertex2f(2,viewport2[3]);
	glVertex2f(0,viewport2[3]);
	glEnd();

	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat2);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat2);

        glutSwapBuffers();
}

void spatial_mouse(int button, int state, int x, int y, int data_type)
{
	mouse_button = button;
	mouse_state = state;
	long s=0x00000000;
	mouse_y = y;

	s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
	s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
	s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

	int key_state = glutGetModifiers();
	s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0 ;
	s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0 ;
	s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0 ;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		qdotPicking[data_type]->getPickingray((svScalar)x,(svScalar)y);

		if(qdotClusterlayer[data_type] && !qdotCutSlice[data_type]->SELECTED)
		{
			svVector3 pn, pv;
			pn = qdotPicking[data_type]->getPickingdir();
			pv = qdotPicking[data_type]->getPickingpos();
			//qdotNewCutting[data_type] = false;
			//cerr<<pv[0]<<" "<<pv[1]<<" "<<pv[2]<<endl;
			//cerr<<pn[0]<<" "<<pn[1]<<" "<<pn[2]<<endl;
			//cerr<<pv[0]+100*pn[0]<<" "<<pv[1]+100*pn[1]<<" "<<pv[2]+100*pn[2]<<endl;
			qdotCutSlice[data_type]->Click(qdotPicking[data_type]->getPickingpos(),
							qdotPicking[data_type]->getPickingdir(), m[data_type]);
		}

		if(!qdotCutSlice[data_type]->SELECTED)
		{
				//cerr<<"trackball"<<endl;
			 //if (s & Trackball::BUTTON_DOWN){
				  trackball[data_type].mouseDown(s, x, y);
				//if(enable_sync)
				//{
					trackball[1-data_type] = trackball[data_type];
					trackball_legend[1-data_type] = trackball_legend[data_type];
				//}
			//}
		}
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
				  trackball[data_type].mouseDown(s, x, y);
				//if(enable_sync)
				//{
					trackball[1-data_type] = trackball[data_type];
					trackball_legend[1-data_type] = trackball_legend[data_type];
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if(qdotCutSlice[data_type]->SELECTED)
		{
			qdotNewCluster[data_type] = true;
			qdotNewContourCluster[data_type] = true;

			qdotCutSlice[data_type]->SELECTED = GL_FALSE;
		}
	}
	
		if (s & Trackball::BUTTON_UP)
		{
			 trackball[data_type].mouseUp(s, x, y);
			//if(enable_sync)
			//{
				trackball[1-data_type] = trackball[data_type];
				trackball_legend[1-data_type] = trackball_legend[data_type];
			//}
		 }
}

void
spatial_motion(int x, int y, int data_type)
{

	if(mouse_button == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN)
	{

			trackball[data_type].mouseMotion(x, y);
			//if(enable_legend_sync)
				trackball_legend[data_type].setRotateMatrix(trackball[data_type].getRotateMatrix());			
			//if(enable_sync)
			//{	
				trackball_legend[1-data_type]=trackball_legend[data_type];
				trackball[1-data_type] = trackball[data_type];			
			//}
	}
	else if(mouse_button == GLUT_RIGHT_BUTTON && mouse_state == GLUT_DOWN)
	{
			trackball[data_type].mouseMotion(x, y);
			//if(enable_legend_sync)
				trackball_legend[data_type].setRotateMatrix(trackball[data_type].getRotateMatrix());			
			//if(enable_sync)
			//{	
				trackball_legend[1-data_type]=trackball_legend[data_type];
				trackball[1-data_type] = trackball[data_type];			
			//}
	}

	mouse_x = x;
	mouse_y = y;

}
void nonspatial_mouse(int button, int state, int x, int y, int data_type)
{
	mouse_button = button;mouse_state = state;
	twod_mouse_state = state;
	
	long s=0x00000000;

		  s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
       		  s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
		  s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

		int key_state = glutGetModifiers();
		s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
		s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
		s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;


				  if (s & Trackball::BUTTON_DOWN)
				{
				    trackball_twod[data_type].mouseDown(s, x, y);
					trackball_twod[1-data_type] = trackball_twod[data_type];			
				  }

 				if (s & Trackball::BUTTON_UP)
				{
			   		 trackball_twod[data_type].mouseUp(s, x, y);
					//if(enable_sync)
					//{	
					trackball_twod[1-data_type] = trackball_twod[data_type];			

					//}
				}

			if(button == GLUT_LEFT_BUTTON
			&&state==GLUT_DOWN)
			{
				//twod_mouse_x = x;
				twod_mouse_y = y;

				float s_x;
				float s_y;
				s_x = (float)x -image_width/4.;
				s_y = image_height - y;
				s_y = (float)s_y - image_height/2.;
				s_x = (s_x/twodzoom[data_type]);///distance_scale[d];
				s_y = (s_y/twodzoom[data_type]);///distance_scale[d];
				s_x = s_x - twotranx[data_type];
				s_y = s_y - twotrany[data_type];

				float ff;
				int f = sqrt((qdotDistanceSample[data_type] * qdotDistanceSample[data_type])/(twodzoom[data_type] * twodzoom[data_type]));
				ff = f;
				if(f==0)
					ff = 1.;
				else
					ff = f + 1.;

				if(qdotDistancePoint[data_type])
					qdotMap[data_type]->SelectPoint(s_x, s_y, ff);
				else if(qdotDistanceZ[data_type])
				{
						int z = qdotMap[data_type]->SelectBox(x, image_height-y, 
											 image_width/4., image_height/2., 
											 twodzoom[data_type], 
								 twotranx[data_type], twotrany[data_type]);
						//refer_z[d] = qdotMap[d]->GetSelectedZ();	
						//if(enable_sync)
						//{
						if(qdotDistanceSync)
						{
						  int tmpz = qdotField[1-data_type]->getZlayer();
						  if(z <= tmpz-1)
						  {
						    qdotMap[1-data_type]->SetSelectedLayer(z);
							//refer_z[1-data_type] = qdotMap[1-data_type]->GetSelectedZ();	
						  }
							qdotNewDistance[1-data_type] = true;
						}
					qdotNewDistance[data_type] = true;
						//}
				}
				
			}
}

void 
nonspatial_motion(int x, int y, int data_type)
{
	if((mouse_button == GLUT_LEFT_BUTTON) && (mouse_state==GLUT_DOWN))
	    {
			trackball_twod[data_type].mouseMotion(x, y);	
			panintwod[data_type] = trackball_twod[data_type].getPanMatrix();	
			twotranx[data_type] = panintwod[data_type][0][3]*4.;
			twotrany[data_type] = panintwod[data_type][1][3]*4.;
			//if(enable_sync)
			//{
				//trackball_legend[1-data_type]=trackball_legend[data_type];
				trackball_twod[1-data_type]=trackball_twod[data_type];	
				panintwod[1-data_type] = trackball_twod[1-data_type].getPanMatrix();	
				twotranx[1-data_type] = panintwod[1-data_type] [0][3] * 4.;
				twotrany[1-data_type] = panintwod[1-data_type] [1][3] * 4.;			
			//}
			//cerr<<twotranx<<" "<<twotrany<<endl;
		
			
	   }
	   else
	   {

			trackball_twod[data_type].mouseMotion(x, y);
			zoomintwod[data_type] = trackball_twod[data_type].getZoomMatrix();
			twodzoom[data_type] = zoomintwod[data_type][2][3]/10.;
			if(twodzoom[data_type]<1) twodzoom[data_type]=1;
			//if(enable_sync)
			//{
				//trackball_legend[1-data_type]=trackball_legend[data_type];
				trackball_twod[1-data_type]=trackball_twod[data_type];
				zoomintwod[1-data_type] = trackball_twod[1-data_type].getZoomMatrix();
				twodzoom[1-data_type] = zoomintwod[1-data_type][2][3]/10.;
				if(twodzoom[1-data_type]<1) twodzoom[1-data_type]=1;
			//}

	    }
	//twod_mouse_y2 = y;
}
void left_mouse(int button, int state, int x, int y)
{
	if(qdotDisplay[0]==0 || qdotDisplay[0] == 1)
	{
		spatial_mouse(button, state, x, y, 0);
	}
	else
	{
		nonspatial_mouse(button, state, x, y, 0);
	}

 	redisplay_all();
}
void right_mouse(int button, int state, int x, int y)
{
	if(qdotDisplay[1]==0 || qdotDisplay[1] == 1)
	{
		spatial_mouse(button, state, x, y, 1);
	}
	else
	{
		nonspatial_mouse(button, state, x, y, 1);
	}

 	redisplay_all();
}
void left_motion(int x, int y)
{
	if(qdotDisplay[0]== 0 || qdotDisplay[0] == 1)
	{
		spatial_motion(x, y, 0);
	}
	else
	{
		nonspatial_motion(x, y, 0);
	}
 	redisplay_all();
}

void right_motion(int x, int y)
{
	if(qdotDisplay[1]==0 || qdotDisplay[1] == 1)
	{
		spatial_motion(x, y, 1);
	}
	else
	{
		nonspatial_motion(x, y, 1);
	}


 	redisplay_all();
}

void main_key(unsigned char key, int x, int y)
{
  switch (key)
  {
/*
  case 'z':
    	renderzmax = renderzmax -0.25;
	glutSetWindow(left_window);
	qdotArrowSelection[0]->RenderArrow(qdotOpacity, qdotClusterlayer, renderzmax);
	glutSetWindow(right_window);
	qdotArrowSelection[1]->RenderArrow(qdotOpacity, qdotClusterlayer, renderzmax);
	cout<<"zvalue "<<renderzmax<<endl;
	break;
  case 'a':
    	renderzmax = renderzmax + 0.25;
	glutSetWindow(left_window);
	qdotArrowSelection[0]->RenderArrow(qdotOpacity, qdotClusterlayer, renderzmax);
	glutSetWindow(right_window);
	qdotArrowSelection[1]->RenderArrow(qdotOpacity, qdotClusterlayer,renderzmax);
	cout<<"zvalue "<<renderzmax<<endl;
	break;
  case 'c':
	newcluster();
	break;
  case 's':
	increasescaling();
	break;
  case 'x':
	decreasescaling();
	break;
  case 'f':
	increasealpha();
	break;
  case 'v':
	decreasealpha();
	break;
  case 'g':
	increasearrowbase();
	break;
  case 'b':
	decreasearrowbase();
	break;
  case 'e':
	toggleencode();
	break;*/
  }	
	redisplay_all();
}
int main(int argc, char** argv)
{
	cerr<<"Start"<<endl;
	InitFile("log");

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInit(&argc, argv);
    glutInitWindowSize(2000,1000);    
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("SPLIT");



    init();
    initLighting();

    GLUI_Master.set_glutReshapeFunc(main_reshape);
    GLUI_Master.set_glutDisplayFunc(main_display);

    left_window = glutCreateSubWindow(window, 0, 0, 500, 1000);
    qdotArrowSelection[0]->svArrowGlyphRenderArrow(qdotOpacity);
    qdotMesh[0]->generateList(qdotField[0]->getCellPosVec(), 
							qdotField[0]->getCellVelVec(),
							qdotField[0]->getCellDenVec(),
							qdotField[0]->getCellIndex(),
							true, 3);
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutDisplayFunc(left_display);
    GLUI_Master.set_glutMouseFunc(left_mouse);
    glutMotionFunc(left_motion);
    glutKeyboardFunc(main_key);

    right_window = glutCreateSubWindow(window, 500, 0, 500, 1000);
    qdotArrowSelection[1]->svArrowGlyphRenderArrow(qdotOpacity);
    qdotMesh[1]->generateList(qdotField[1]->getCellPosVec(), 
							qdotField[1]->getCellVelVec(),
							qdotField[1]->getCellDenVec(),
							qdotField[1]->getCellIndex(),
							true, 4);
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutDisplayFunc(right_display);
    GLUI_Master.set_glutMouseFunc(right_mouse);
    glutMotionFunc(right_motion);
    glutKeyboardFunc(main_key);

    glui_function(0);
    glui_function(1);
	glui_visualization();
	glui_data_1();
	glui_data_2();
	glui_data_pop1->hide();
	glui_data_pop2->hide();

    //glui_lhc();
    //glui_rhc();

    redisplay_all();	
    glutMainLoop();


	return 0;
}
