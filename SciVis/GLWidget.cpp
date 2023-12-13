///////////////////////////////////////////////
#include "libraries/glew-2.1.0/include/GL/glew.h"
#include "libraries/freeglut-3.0.0/include/GL/freeglut.h"
// #include "gl/glew.h"
// #include "gl/freeglut.h"
///////////////////////////////////////////////
#include "GLWidget.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <vector>

#include "glError.h"
#include "ply.h"
#include "icVector.H"
#include "icMatrix.H"
#include "polyhedron.h"
#include "trackball.h"
#include "tmatrix.h"
#include "Polyline.h"
#include <iostream>
#include <qdebug.h>
#include <filesystem>

Polyhedron* poly = nullptr;
Polyhedron* ptCloud = nullptr;
Polyhedron* surface = nullptr;
std::vector<POLYLINE> polylines;
std::vector<CPOINT> points;

/*scene related variables*/
const float zoomspeed = 0.9;
const int win_width = 800;
const int win_height = 800;
const int view_mode = 0;		// 0 = othogonal, 1=perspective
const double radius_factor = 0.9;

/*
Use keys 1 to 0 to switch among different display modes.
Each display mode can be designed to show one type
visualization result.

Predefined ones:
display mode 1: solid rendering
display mode 2: show wireframes
display mode 3: render each quad with colors of vertices
*/
int display_mode = 1;

bool pointcloudOn = true;

/*User Interaction related variabes*/
float s_old, t_old;
float rotmat[4][4];
double zoom = 1.0;
double translation[2] = { 0, 0 };
int mouse_mode = -2;	// -1 = no action, 1 = tranlate y, 2 = rotate

/******************************************************************************
Forward declaration of functions
******************************************************************************/

void init(void);

/*functions for element picking*/
void display_vertices(GLenum mode, Polyhedron* poly);
void display_quads(GLenum mode, Polyhedron* poly);
void display_selected_vertex(Polyhedron* poly);
void display_selected_quad(Polyhedron* poly);

/*display vis results*/
void display_polyhedron(Polyhedron* poly);
void display_crit_points();


/******************************************************************************
Main program.
******************************************************************************/

GLWidget::GLWidget(QWidget *parent):
	QOpenGLWidget(parent), 
	mFullScreen(false),
	m_select_face(false),
	m_select_vertex(false){
	showNormal();
	setFixedSize(win_width, win_height);
}

GLWidget::~GLWidget(){
}

void GLWidget::openFile(const char* file) {

	if (file != nullptr && file[0] != '\0')
	{
		if (nullptr != poly) {
			poly->finalize();
			polylines.clear();
			poly = nullptr;
		}

	
		/*load mesh from ply file "./data/scalar_data/r14.ply" */
		FILE* this_file = fopen(file, "r");
		poly = new Polyhedron(this_file);
		//////////////////////////////////
		//This is already closed in Polyhedron constructor so this causes double free error:
		// fclose(this_file); 
		//////////////////////////////////

		/*initialize the mesh*/
		poly->initialize(); // initialize the mesh
		poly->write_info();
	}
}

void GLWidget::resizeGL(int width, int height){
/*	if (height == 0) { 
		height = 1;
	}

	glViewport(0, 0, width, height); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();*/
}



/******************************************************************************
Set projection mode
******************************************************************************/

void set_view(GLenum mode)
{
	GLfloat light_ambient0[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_diffuse0[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat light_specular0[] = { 0.0, 0.0, 0.0, 1.0 };

	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse1[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_specular1[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);


	glMatrixMode(GL_PROJECTION);
	if (mode == GL_RENDER)
		glLoadIdentity();

	if (view_mode == 0)
		glOrtho(-radius_factor * zoom, radius_factor * zoom, -radius_factor * zoom, radius_factor * zoom, 0.0, 40.0);
	else
		glFrustum(-radius_factor * zoom, radius_factor * zoom, -radius_factor, radius_factor, -1000, 1000);

	GLfloat light_position[3];
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	light_position[0] = 5.5;
	light_position[1] = 0.0;
	light_position[2] = 0.0;
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	light_position[0] = -0.1;
	light_position[1] = 0.0;
	light_position[2] = 0.0;
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
}

/******************************************************************************
Update the scene
******************************************************************************/

void set_scene(GLenum mode, Polyhedron* poly)
{
	glTranslatef(translation[0], translation[1], -3.0);

	/*multiply rotmat to current mat*/
	{
		int i, j, index = 0;

		GLfloat mat[16];

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				mat[index++] = rotmat[i][j];

		glMultMatrixf(mat);
	}

	glScalef(0.9 / poly->radius, 0.9 / poly->radius, 0.9 / poly->radius);
	glTranslatef(-poly->center.entry[0], -poly->center.entry[1], -poly->center.entry[2]);
}


/******************************************************************************
Init scene
******************************************************************************/

void init(void) {
	mat_ident(rotmat);

	/* select clearing color */
	glClearColor(0.0, 0.0, 0.0, 0.0);  // background
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT, GL_FILL);

	glDisable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//set pixel storage modes
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glEnable(GL_NORMALIZE);
	if (poly->orientation == 0)
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);
}


/******************************************************************************
Pick objects from the scene
******************************************************************************/

int processHits(GLint hits, GLuint buffer[])
{
	int i, j;
	GLuint names, *ptr;
	double smallest_depth = 1.0e+20, current_depth;
	int seed_id = -1;
	unsigned char need_to_update;

	ptr = (GLuint*)buffer;
	for (i = 0; i < hits; i++) {  /* for each hit  */
		need_to_update = 0;
		names = *ptr;
		ptr++;

		current_depth = (double)*ptr / 0x7fffffff;
		if (current_depth < smallest_depth) {
			smallest_depth = current_depth;
			need_to_update = 1;
		}
		ptr++;
		current_depth = (double)*ptr / 0x7fffffff;
		if (current_depth < smallest_depth) {
			smallest_depth = current_depth;
			need_to_update = 1;
		}
		ptr++;
		for (j = 0; j < names; j++) {  /* for each name */
			if (need_to_update == 1)
				seed_id = *ptr - 1;
			ptr++;
		}
	}
	return seed_id;
}

/******************************************************************************
Diaplay all quads for selection
******************************************************************************/

void display_quads(GLenum mode, Polyhedron* this_poly)
{
	unsigned int i, j;
	GLfloat mat_diffuse[4];

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);
	//glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	//glDisable(GL_LIGHTING);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (i = 0; i < this_poly->nquads; i++) {
		if (mode == GL_SELECT)
			glLoadName(i + 1);

		Quad* temp_q = this_poly->qlist[i];
		{
			mat_diffuse[0] = 1.0;
			mat_diffuse[1] = 1.0;
			mat_diffuse[2] = 0.0;
			mat_diffuse[3] = 1.0;
		}
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

		glBegin(GL_POLYGON);
		for (j = 0; j < 4; j++) {
			Vertex* temp_v = temp_q->verts[j];
			//glColor3f(0, 0, 0);
			glVertex3d(temp_v->x, temp_v->y, temp_v->z);
		}
		glEnd();
	}
}

/******************************************************************************
Diaplay all vertices for selection
******************************************************************************/

void display_vertices(GLenum mode, Polyhedron* this_poly)
{

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);
	//glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	for (int i = 0; i < this_poly->nverts; i++) {
		if (mode == GL_SELECT)
			glLoadName(i + 1);

		Vertex* temp_v = this_poly->vlist[i];
		{
			GLUquadric* quad = gluNewQuadric();

			glPushMatrix();
			glTranslatef(temp_v->x, temp_v->y, temp_v->z);
			glColor4f(0, 0, 1, 1.0);
			gluSphere(quad, this_poly->radius * 0.01, 50, 50);
			glPopMatrix();

			gluDeleteQuadric(quad);

		}
	}
}

/******************************************************************************
Diaplay selected quad
******************************************************************************/

void display_selected_quad(Polyhedron* this_poly)
{
	if (this_poly->selected_quad == -1)
	{
		return;
	}

	unsigned int i, j;
	GLfloat mat_diffuse[4];

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);

	Quad* temp_q = this_poly->qlist[this_poly->selected_quad];

	glBegin(GL_POLYGON);
	for (j = 0; j < 4; j++) {
		Vertex* temp_v = temp_q->verts[j];
		glColor3f(1.0, 0.0, 1.0);
		glVertex3d(temp_v->x, temp_v->y, 0.001);
	}
	glEnd();
}

// /******************************************************************************
// Display Critical points
// ******************************************************************************/
void display_crit_points()
{
	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	GLUquadric* quadric = gluNewQuadric();
	for (auto& sp : points)
	{
		glPushMatrix();
		glTranslated(sp.m_pos.x, sp.m_pos.y, sp.m_pos.z);
		icVector3 color(0.0, 0.0, 0.0);
		if (sp.type == 3)//saddle
		{
			color.x = 1.0;
			color.y = 0.0;
			color.z = 1.0;
		}
		else if (sp.type == 2) //max
		{
			color.x = 1.0;
			color.y = 0.0;
			color.z = 0.0;
		}
		else if (sp.type == 1) //min
		{
			color.x = 0.0;
			color.y = 0.0;
			color.z = 0.0;
		}
		glColor3f(color.x, color.y, color.z);
		gluSphere(quadric, sp.m_weight/10,16,16);
		glPopMatrix();
	}
	gluDeleteQuadric(quadric);

}

// /******************************************************************************
// Display Critical points
// ******************************************************************************/
void display_vert_points()
{
	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	// std::cerr<<"DISP VERT PTS"<<std::endl;

	GLUquadric* quadric = gluNewQuadric();
	int step = poly->nverts/100000;
	for (int i = 0; i < poly->nverts; i += step)
	{
		Vertex *v = poly->vlist[i];
		glPushMatrix();
		glTranslated(v->x, v->y, v->z);
		// icVector3 color(0.0, 0.0, 0.0);
		// if (sp.type == 3)//saddle
		// {
		// 	color.x = 1.0;
		// 	color.y = 0.0;
		// 	color.z = 1.0;
		// }
		// else if (sp.type == 2) //max
		// {
		// 	color.x = 1.0;
		// 	color.y = 0.0;
		// 	color.z = 0.0;
		// }
		// else if (sp.type == 1) //min
		// {
		// 	color.x = 0.0;
		// 	color.y = 0.0;
		// 	color.z = 0.0;
		// }
		glColor3f(v->R, v->G, v->B);
		// std::cerr<<v->x<<','<<v->y<<','<<v->z<<std::endl;
		gluSphere(quadric, 3,16,16);
		glPopMatrix();
	}
	gluDeleteQuadric(quadric);

}


/******************************************************************************
Diaplay selected vertex
******************************************************************************/

void display_selected_vertex(Polyhedron* this_poly)
{
	if (this_poly->selected_vertex == -1)
	{
		return;
	}

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);


	Vertex* temp_v = this_poly->vlist[this_poly->selected_vertex];

	GLfloat mat_diffuse[4];

	{
		mat_diffuse[0] = 1.0;
		mat_diffuse[1] = 0.0;
		mat_diffuse[2] = 0.0;
		mat_diffuse[3] = 1.0;
	}

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	{
		GLUquadric* quad = gluNewQuadric();
		glPushMatrix();
		glTranslatef(temp_v->x, temp_v->y, temp_v->z);
		gluSphere(quad, this_poly->radius * 0.01, 50, 50);
		glPopMatrix();
		gluDeleteQuadric(quad);
	}

}

/******************************************************************************
Diaplay the polygon with visualization results
******************************************************************************/

void display_polyhedron(Polyhedron* poly)
{
	unsigned int i, j;

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	for (i = 0; i < poly->nquads; i++) {

		Quad* temp_q = poly->qlist[i];

		switch (display_mode) {
		case 1:
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			GLfloat mat_diffuse[4] = { 1.0, 1.0, 0.0, 0.0 };
			GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

			glBegin(GL_POLYGON);
			for (j = 0; j < 4; j++) {
				Vertex* temp_v = temp_q->verts[j];
				glNormal3d(temp_v->normal.entry[0], temp_v->normal.entry[1], temp_v->normal.entry[2]);
				glVertex3d(temp_v->x, temp_v->y, temp_v->z);
			}
			glEnd();
			break;
		}
		case 2:
			glDisable(GL_LIGHTING);
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POLYGON);
			for (j = 0; j < 4; j++) {
				Vertex* temp_v = temp_q->verts[j];
				glNormal3d(temp_q->normal.entry[0], temp_q->normal.entry[1], temp_q->normal.entry[2]);
				glColor3f(0.0, 0.0, 0.0);
				glVertex3d(temp_v->x, temp_v->y, temp_v->z);
			}
			glEnd();

			glDisable(GL_BLEND);
			break;

		case 3:
			glDisable(GL_LIGHTING);

			glBegin(GL_POLYGON);
			for (j = 0; j < 4; j++) {
				Vertex* temp_v = temp_q->verts[j];
				glColor3f(temp_v->R, temp_v->G, temp_v->B);
				glVertex3d(temp_v->x, temp_v->y, temp_v->z);
			}
			glEnd();
			break;
		}
	}
}

/******************************************************************************
Process a keyboard action.  In particular, exit the program when an
"escape" is pressed in the window.
******************************************************************************/
void GLWidget::vertexColor() {
	double L = (poly->radius * 2) / 30;
	for (int i = 0; i < poly->nquads; i++) {
		Quad* temp_q = poly->qlist[i];
		for (int j = 0; j < 4; j++) {

			Vertex* temp_v = temp_q->verts[j];

			temp_v->R = int(temp_v->x / L) % 2 == 0 ? 1 : 0;
			temp_v->G = int(temp_v->y / L) % 2 == 0 ? 1 : 0;
			temp_v->B = 0.0;
		}
	}
}

// void GLWidget::trueColor() {

// 	for (int i = 0; i < poly->nquads; i++) {
// 		Quad* temp_q = poly->qlist[i];
// 		for (int j = 0; j < 4; j++) {

// 			Vertex* temp_v = temp_q->verts[j];

// 			temp_v->R = int(temp_v->x / L) % 2 == 0 ? 1 : 0;
// 			temp_v->G = int(temp_v->y / L) % 2 == 0 ? 1 : 0;
// 			temp_v->B = 0.0;
// 		}
// 	}
// }

void GLWidget::keyPressEvent(QKeyEvent *event) {
	int i;
	/* set escape key to exit */
	switch (event->key()) {
	case Qt::Key_Escape:
		poly->finalize();  // finalize_everything
		exit(0);
		break;

	case Qt::Key_1:
		display_mode = 1;
		update();
		break;

	case Qt::Key_2:
		display_mode = 2;
		update();
		break;

	case Qt::Key_3:
	{
		display_mode = 3;

		vertexColor();
		update();
	}
	break;
	case Qt::Key_4:
		display_mode = 4;
		{
			//codes to modify the color of each vertex

		}
		update();
		break;
	case Qt::Key_5: {
		POLYLINE line;
		line.m_rgb = icVector3(0.0, 1.0, 0.0);
		line.m_weight = 4;
		for (int i = 0; i < poly->nverts; i++) {
			if (std::abs(poly->vlist[i]->x) + std::abs(poly->vlist[i]->y) == 5) {
				poly->vlist[i]->scalar = 100;
				line.m_vertices.push_back(icVector3(
					poly->vlist[i]->x,
					poly->vlist[i]->y,
					poly->vlist[i]->z
				));
			}
		}
		polylines.push_back(line);
		/*
		for (int i = 0; i < poly->nverts; i++) {
			poly->vlist[i]->scalar = 200;
		}
		for (int i = 0; i < poly->nverts; i++) {
			if(std::abs(poly->vlist[i]->x) + std::abs(poly->vlist[i]->y) == 5){
			poly->vlist[i]->scalar = 100;
			}
		}
		for (int i = 1; i < 20; i++) {
			std::list<POLYLINE> edges0;
			marchingSquare(edges0, *poly, i*10);
			std::vector<POLYLINE> polylines0;
			makePolylineFromEdges(polylines0, edges0);
			for (auto&polyline : polylines0) {
				polyline.m_rgb = icVector3(i / 10.0, 0, 0);
				polylines.push_back(polyline);
			}
		}*/
		update(); 
	}break;
	case Qt::Key_6: {
		POLYLINE line;
		line.m_vertices.push_back(icVector3(-4, 0, 1));
		line.m_vertices.push_back(icVector3(-3, 0, 1));
		line.m_vertices.push_back(icVector3(-1, 0, 1));
		line.m_rgb = icVector3(1.0, 0.0, 0.0);
		//
		POLYLINE line2;
		line2.m_vertices.push_back(icVector3(-1, 0, 1));
		line2.m_vertices.push_back(icVector3(6, 0, 0));
		line2.m_vertices.push_back(icVector3(6, 2, 0));
		line2.m_rgb = icVector3(0.0, 1.0, 0.0);

		//polylines.push_back(line2);
		line.merge(line2);
		polylines.push_back(line);
		update();
	}break;
	case Qt::Key_R:
		mat_ident(rotmat);
		translation[0] = 0;
		translation[1] = 0;
		zoom = 1.0;
		update();
		break;
	}
}

/******************************************************************************
Callback function for dragging mouse
******************************************************************************/

void GLWidget::motion(int x, int y) {
	float r[4];
	float s, t;

	s = (2.0 * x - win_width) / win_width;
	t = (2.0 * (win_height - y) - win_height) / win_height;

	if ((s == s_old) && (t == t_old))
		return;

	switch (mouse_mode) {
	case 2:

		Quaternion rvec;

		mat_to_quat(rotmat, rvec);
		trackball(r, s_old, t_old, s, t);
		add_quats(r, rvec, rvec);
		quat_to_mat(rvec, rotmat);

		s_old = s;
		t_old = t;
		update();
		break;

	case 1:

		translation[0] += (s - s_old);
		translation[1] += (t - t_old);

		s_old = s;
		t_old = t;
		update();
		break;
	}
}

/******************************************************************************
Callback function for mouse clicks
******************************************************************************/
void GLWidget::mousePressEvent(QMouseEvent * event) {
	double x = event->x();
	double y = event->y();
	float xsize = (float)win_width;
	float ysize = (float)win_height;

	float s = (2.0 * x - win_width) / win_width;
	float t = (2.0 * (win_height - y) - win_height) / win_height;

	s_old = s;
	t_old = t;

	/*translate*/
	if (event->button() == Qt::LeftButton)
	{
		mouse_mode = 1;
	}

	/*rotate*/
	if (event->button() == Qt::RightButton)
	{
		mouse_mode = 2;
	}
}
void GLWidget::mouseReleaseEvent(QMouseEvent * event) {

	double x = event->position().x();
	double y = event->position().y();

	if (event->button() == Qt::LeftButton && m_select_face) {  // build up the selection feedback mode

		/*select face*/

		GLuint selectBuf[win_width];
		GLint hits;

		GLint viewport[4] = { 0, 0, this->width(), this->height() };
		//glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer(win_width, selectBuf);
		(void)glRenderMode(GL_SELECT);

		glInitNames();
		glPushName(0);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		/*create 5x5 pixel picking region near cursor location */
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0, 1.0, viewport);

		set_view(GL_SELECT);
		set_scene(GL_SELECT, poly);
		display_quads(GL_SELECT, poly);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glFlush();

		glMatrixMode(GL_MODELVIEW);

		hits = glRenderMode(GL_RENDER);
		qDebug() << glGetError();
		poly->selected_quad = processHits(hits, selectBuf);
		printf("Selected quad id = %d\n", poly->selected_quad);
		update();

	}else if (event->button() == Qt::LeftButton && m_select_vertex){
		/*select vertex*/

		GLuint selectBuf[win_width];
		GLint hits;
		GLint viewport[4] = {0, 0, this->width(), this->height()};

		//glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer(win_width, selectBuf);
		(void)glRenderMode(GL_SELECT);

		glInitNames();
		glPushName(0);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		/*  create 5x5 pixel picking region near cursor location */
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0, 1.0, viewport);

		set_view(GL_SELECT);
		set_scene(GL_SELECT, poly);
		display_vertices(GL_SELECT, poly);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glFlush();

		glMatrixMode(GL_MODELVIEW);

		hits = glRenderMode(GL_RENDER);
		poly->selected_vertex = processHits(hits, selectBuf);
		printf("Selected vert id = %d\n", poly->selected_vertex);
		update();

	}

	mouse_mode = -1;
}
void GLWidget::mouseDoubleClickEvent(QMouseEvent * event) {

}
void GLWidget::mouseMoveEvent(QMouseEvent * event) {
	motion(event->x(), event->y());
}
/******************************************************************************
Callback function for mouse wheel scroll
******************************************************************************/
void GLWidget::wheelEvent(QWheelEvent  *event) {
	if (event->angleDelta().y() > 0) {
		zoom *= zoomspeed;
		update();
	}
	else {
		zoom /= zoomspeed;
		update();
	}
}

void GLWidget::paintGL() {
	glClearColor(1.0, 1.0, 1.0, 1.0);  // background for rendering color coding and lighting

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	set_view(GL_RENDER);
	set_scene(GL_RENDER, poly);

	/*display the mesh*/
	display_polyhedron(poly);

	display_crit_points();
	if (pointcloudOn)
	{
		display_vert_points();
	}

	/*display selected elements*/
	display_selected_vertex(poly);
	display_selected_quad(poly);

	/*display polyline*/
	display_polyline(polylines);

/*	glFlush();
	//glutSwapBuffers();
	//glFinish();

	CHECK_GL_ERROR();*/
}

void GLWidget::initializeGL() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("%s", glewGetErrorString(err));
	}
	
	std::string NEON_file = "../SciVis/data/NEON_data/NEON_D16_ABBY_DP1_554000_5069000_test.ply"; 
	
	if (std::filesystem::exists(NEON_file))
	{
		openFile(NEON_file.c_str());
	}
	else if (std::filesystem::exists("../"+NEON_file))
	{
		NEON_file = "../"+NEON_file;
		openFile(NEON_file.c_str());
	}
	else
	{
		return;
	}
	init();
	togglePtcloud();

	// openFile("../SciVis/data/scalar_data/r12.ply");
	// std::cerr<<std::filesystem::current_path()<<std::endl;
	// getchar();

}

void GLWidget::togglePtcloud() {
	// plotVertexPoints();

	if (surface == nullptr)
	{
		ptCloud = poly;
		surface=ptCloud->ptcloud_to_quads(dx,dy);
	}

	if (pointcloudOn)
	{
		poly = surface;
	}
	else
	{
		poly = ptCloud;
	}
	pointcloudOn = !pointcloudOn;
}

void GLWidget::reapply()
{
	surface->finalize();
	free(surface);
	surface = nullptr;
	surface=ptCloud->ptcloud_to_quads(dx,dy);
	poly = surface;
	update();
}