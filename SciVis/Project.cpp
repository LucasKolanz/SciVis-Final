#include "GLWidget.h"
#include "Project.h"
#include "icVector.H"
#include "icMatrix.H"
#include "polyhedron.h"
#include "Polyline.h"

#include <QDebug>
#include <iostream>

extern Polyhedron* poly;
extern std::vector<POLYLINE> polylines;
// std::vector<CPOINT> points;

void GLWidget::algorithm1() //toggle 100 evenly spaced contour lines, critical pt contour lines
{
	if (polylines.size() > 0)
	{
		polylines.clear();
	}
	else
	{
		drawAllLines();
	}
	update();
}
void GLWidget::algorithm2() //toggle field/contour line height
{
	fieldHeight();
	if (polylines.size() > 0)
	{
		polylines.clear();
		GLWidget::algorithm1();
	}
	update();
}
void GLWidget::algorithm3() 
{
	if (rainbowMapOn)
	{
		vertexColor();
	}
	else
	{
		rainbowMap();
	}
	toggleMapBool(rainbow);
	update();
}
void GLWidget::algorithm4() 
{
	if (bicolorMapOn)
	{
		vertexColor();
	}
	else
	{
		bicolorMap();
	}
	toggleMapBool(bicolor);
	update();
}
void GLWidget::algorithm5() {
	if (greyscaleMapOn)
	{
		vertexColor();
	}
	else
	{
		greyscaleMap();
	}
	toggleMapBool(greyscale);
	update();
}
void GLWidget::algorithm6() {
	qDebug() << 6;
}
void GLWidget::algorithm7() {
	qDebug() << 7;
}
void GLWidget::algorithm8() {
	qDebug() << 8;
}

///////////////////////////////////////////////////////////////
////////Start Project 1 functions and helpers
///////////////////////////////////////////////////////////////

void toggleMapBool(int map)
{
	int arrlen = sizeof(colorMapBools)/sizeof(colorMapBools[0]);
	for (int i = 0; i < arrlen; ++i)
	{
		if (i == map)
		{
			*colorMapBools[i] = !*colorMapBools[i];
		}
		else
		{
			*colorMapBools[i] = false;
		}
	}
}

void fieldHeight()
{
	double max, min;
	findMinMax(min, max);
	for (auto i = 0; i < poly->nverts; i++)
	{
		
		auto& vertex = poly->vlist[i];
		if (fieldHeightOn)
		{
			vertex->z = 0;
		}
		else
		{
			double s_v = vertex->scalar;
			double l = (s_v - min) / (max - min);
			vertex->z = l*10;
		}
	}
	fieldHeightOn = !fieldHeightOn;
}

void findMinMax(double& min, double& max)
{
	min = INFINITY;
	max = -min;
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		if (vertex->scalar < min)
		{
			min = vertex->scalar;
		}
		if (vertex->scalar > max)
		{
			max = vertex->scalar;
		}
	}
}

void rainbowMap()
{
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;

		icVector3 c = rainbowColorFromScalar(s_v);

		vertex->R = c.x;
		vertex->G = c.y;
		vertex->B = c.z;
	}
}

void bicolorMap()
{
	double max, min;
	findMinMax(min, max);
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;
		icVector3 c1(0.0, 1.0, 0.0);
		icVector3 c2(1.0, 0.0, 1.0);

		double l = (s_v - min) / (max - min);
		double r = (max - s_v) / (max - min);

		icVector3 c = c1 * l + c2 * r;

		vertex->R = c.x;
		vertex->G = c.y;
		vertex->B = c.z;

	}
}

void greyscaleMap()
{
	double max, min;
	//TEST ME
	findMinMax(min, max);

	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;
		double gray_ = (s_v - min) / (max - min);

		vertex->R = vertex->G = vertex->B = gray_;
	}
}

icVector3 rainbowColorFromScalar(double scalar)
{
	icVector3 HSV, c;
	double max, min;
	findMinMax(min, max);
	double delta = max - min;
	//This value needs to be linearly scaled from 0-300 from the delta value
	//Don't go all the way to 300 because we want red to blue and 300-360 is magenta
	HSV.x = (scalar - min) * 300 / delta;
	HSV.y = 1;
	HSV.z = 1;

	//NEED TO IMPLEMENT
	HSVtoRGB(HSV, c);
	return c;
}


void RGBtoHSV(icVector3& rgb, icVector3& hsv)
{
	double cmax,cmin,delta,R,G,B,H,S,V;

	R = rgb.x;
	G = rgb.y;
	B = rgb.z;
	H = 0;
	S = 0;
	V = 0;
	
	cmax = max(R, G, B);
	cmin = min(R, G, B);

	delta = cmax - cmin;

	if (delta == 0)
	{
		H = 0;
	}
	else if (cmax == R)
	{
		H = fmod((((G - B) / delta) + 360) * 60, 360);
	}
	else if (cmax == G)
	{
		H = fmod((((B - R) / delta) + 120) * 60, 360);
	}
	else if (cmax == B)
	{
		H = fmod((((R - G) / delta) + 240) * 60, 360);
	}
	
	if (cmax == 0)
	{
		S = 0;
	}
	else
	{
		S = delta / cmax;
	}

	V = cmax;

	hsv.x = H;
	hsv.y = S;
	hsv.z = V;

	return;
}

void HSVtoRGB(icVector3& hsv, icVector3& rgb)
{
	double H, S, V,c,x,m,r,g,b;

	H = hsv.x;
	S = hsv.y;
	V = hsv.z;

	c = V * S;
	x = c * (1.0 - fabs(fmod(H / 60.0, 2.0) - 1.0));
	m = V - c;



	if (H >= 0.0 && H < 60.0)
	{
		r = c;
		g = x;
		b = 0;
	}
	else if (H >= 60.0 && H < 120.0)
	{
		r = x;
		g = c;
		b = 0;
	}
	else if (H >= 120.0 && H < 180.0)
	{
		r = 0;
		g = c;
		b = x;
	}
	else if (H >= 180.0 && H < 240.0)
	{
		r = 0;
		g = x;
		b = c;
	}
	else if (H >= 240.0 && H < 300.0)
	{
		r = x;
		g = 0;
		b = c;
	}
	else if (H >= 300.0 && H < 360.0)
	{
		r = c;
		g = 0;
		b = x;
	}
	else
	{
		std::cerr << "ERROR: H needs to be between 0 and 360 but is "<<H<<'.' << std::endl;
		r = -1;
		g = -1;
		b = -1;
	}

	rgb.x = r + m;
	rgb.y = g + m;
	rgb.z = b + m;

	return;
}

double max(double m1,double m2,double m3)
{
	return std::max(m1, std::max(m2, m3));
}

double min(double m1, double m2, double m3)
{
	return std::min(m1, std::min(m2, m3));
}

///////////////////////////////////////////////////////////////
////////End Project 1 functions and helpers
///////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////
////////Start Project 2 functions and helpers
///////////////////////////////////////////////////////////////

void drawAllLines()
{
	int tot_lines = 100;
	double max, min, range, value;
	findMinMax(min, max);
	range = max - min;
	range = max - min;
	for (int i = 0; i < tot_lines; ++i)
	{
		value = (i * 1.0) * (range / tot_lines) + min;
		drawContourLine(value, icVector3(0.0,0.0,0.0), false);
	}
}

void drawContourLine(double v, icVector3 rgb,int saddle_index)
{
	std::list<POLYLINE> edges0;
	marchingSquare(edges0, *poly, v,saddle_index);
	std::vector<POLYLINE> polylines0;
	makePolylineFromEdges(polylines0, edges0);
	for (auto& polyline : polylines0) {
		polyline.m_rgb = icVector3(rgb.x, rgb.y, rgb.z);
		polylines.push_back(polyline);
	}
}