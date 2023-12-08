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
extern std::vector<CPOINT> points;
extern int display_mode;
// extern void display_crit_points();


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
	if (points.size() > 0)
	{
		points.clear();
		GLWidget::algorithm6();
	}
	update();
}
void GLWidget::algorithm3() 
{
	if (display_mode == 3)
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
}
void GLWidget::algorithm4() 
{
	if (display_mode == 3)
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
}
void GLWidget::algorithm5() {
	if (display_mode == 3)
	{
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
}
void GLWidget::algorithm6() {
	if (points.size() > 0)
	{
		points.clear();
	}
	else
	{
		plotCriticalPoints();
	}
	update();
}
void GLWidget::algorithm7() {
	// plotVertexPoints();
	poly->ptcloud_to_quads(10,10);
	update();
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
	criticalContours();
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

double findCriticalPoint(Quad* quad, double& x0, double& y0, bool& test, bool &saddle)
{
	//Find x and y values of quad
	double x1, x2, y1, y2,f11,f21,f22,f12;
	x1 = quad->verts[0]->x >= quad->verts[1]->x ? quad->verts[1]->x : quad->verts[0]->x;
	x1 = x1 >= quad->verts[2]->x ? quad->verts[2]->x : x1;
	x2 = quad->verts[0]->x < quad->verts[1]->x ? quad->verts[1]->x : quad->verts[0]->x;
	x2 = x2 < quad->verts[2]->x ? quad->verts[2]->x : x2;

	y1 = quad->verts[0]->y >= quad->verts[1]->y ? quad->verts[1]->y : quad->verts[0]->y;
	y1 = y1 >= quad->verts[2]->y ? quad->verts[2]->y : y1;
	y2 = quad->verts[0]->y < quad->verts[1]->y ? quad->verts[1]->y : quad->verts[0]->y;
	y2 = y2 < quad->verts[2]->y ? quad->verts[2]->y : y2;

	for (int i = 0; i <4; i++)
	{
		if (quad->verts[i]->x == x1 && quad->verts[i]->y == y1)
		{
			f11 = quad->verts[i]->scalar;
			//std::cout <<f11<< " f11 i is " << i << std::endl;
		}
		else if (quad->verts[i]->x == x2 && quad->verts[i]->y == y1)
		{
			f21 = quad->verts[i]->scalar;
			//std::cout << f21 << " f21 i is " << i << std::endl;
		}
		else if (quad->verts[i]->x == x2 && quad->verts[i]->y == y2)
		{
			f22 = quad->verts[i]->scalar;
			//std::cout << f22 << " f22 i is " << i << std::endl;
		}
		else if (quad->verts[i]->x == x1 && quad->verts[i]->y == y2)
		{
			f12 = quad->verts[i]->scalar;
			//std::cout << f12 << " f12 i is " << i << std::endl;
		}
	}
	
	x0 = (x2 * f11 - x1 * f21 - x2 * f12 + x1 * f22) / (f11 - f21 - f12 + f22);
	y0 = (y2 * f11 - y2 * f21 - y1 * f12 + y1 * f22) / (f11 - f21 - f12 + f22);

	/*std::cout << x1 << ',' << x0 << ',' << x2 << std::endl;
	std::cout << y1<< ',' <<y0 << ',' <<y2 << std::endl;*/
	

	saddle = false;
	if (x1 <= x0 && x2 >= x0 && y1 <= y0 && y2 >= y0)
	{

		//std::cout << "THIS ONE: " << fxy(x0, y0, x1, y1, x2, y2, f11, f21, f12, f22) << std::endl;
		//drawContourLine(fxy(x0,y0,x1,y1,x2,y2,f11,f21,f12,f22), icVector3(1.0, 0.0, 0.0));
		double fxy0, fyx0,fxx0,fyy0;
		fxy0 = fxy(x0, y0, x1, y1, x2, y2, f11, f21, f12, f22);
		fyx0 = fyx(x0, y0, x1, y1, x2, y2, f11, f21, f12, f22);
		fxx0 = fxx(x0, y0, x1, y1, x2, y2, f11, f21, f12, f22);
		fyy0 = fyy(x0,y0,x1,y1,x2,y2,f11,f21,f12,f22);

		double det_hessian = fxx0 * fyy0 - fxy0 * fyx0;

		if (det_hessian < 0) //Saddle pt
		{
			saddle = true;
		}
		else
		{
			if (fxx0 > 0)
			{
				std::cout << "found an additional local min" << std::endl;
			}
			else if (fxx0 < 0)
			{
				std::cout << "found an additional local MAX" << std::endl;
			}
			saddle = false;
		}

		test = true;
	}
	else
	{
		test = false;
	}
	

	return fofxy(x0, y0, x1, y1, x2, y2, f11, f21, f12, f22);
}

double fofxy(double x, double y, double x1, double y1,
	double x2, double y2,double f11, double f21, double f12, double f22)
{
	double t1, t2, t3, t4,d,denomrecip;

	denomrecip = 1 / ((x2-x1) * (y2-y1));

	t1 = (x2-x)*(y2-y) * f11;
	t2 = (x - x1) * (y2 - y) * f21;
	t3 = (x2 - x) * (y - y1) * f12;
	t4 = (x - x1) * (y - y1) * f22;
	return (t1 + t2 + t3 + t4)*denomrecip;
}

double fxx(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22)
{
	double t1, t2, t3, t4, d, denomrecip;

	denomrecip = 1 / ((x2 - x1) * (y2 - y1));

	t1 = (-1) * (y2 - y) * f11;
	t2 = (1) * (y2 - y) * f21;
	t3 = (-1) * (y - y1) * f12;
	t4 = (1) * (y - y1) * f22;
	return (t1 + t2 + t3 + t4)*denomrecip;
}


double fyy(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22)
{
	double t1, t2, t3, t4, d, denomrecip;

	denomrecip = 1 / ((x2 - x1) * (y2 - y1));

	t1 = (x2 - x) * (-1) * f11;
	t2 = (x - x1) * (-1) * f21;
	t3 = (x2 - x) * (1) * f12;
	t4 = (x - x1) * (1) * f22;
	return (t1 + t2 + t3 + t4)*denomrecip;
}

double fxy(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22)
{
	/*double t1, t2, t3, t4, d, denomrecip;

	denomrecip = 1 / ((x2 - x1) * (y2 - y1));

	t1 = (-1) * (y2 - y) * f11;
	t2 = (1) * (y2 - y) * f21;
	t3 = (-1) * (y - y1) * f12;
	t4 = (1) * (y - y1) * f22;
	return (t1 + t2 + t3 + t4) * denomrecip;*/
	return fyx(x, y, x1, y1, x2, y2, f11, f21, f12, f22);
}

double fyx(double x, double y, double x1, double y1,
	double x2, double y2, double f11, double f21, double f12, double f22)
{
	double t1, t2, t3, t4, d, denomrecip;

	denomrecip = 1 / ((x2 - x1) * (y2 - y1));

	t1 = (-1) * (-1) * f11;
	t2 = (1) * (-1) * f21;
	t3 = (-1) * (1) * f12;
	t4 = (1) * (1) * f22;
	return (t1 + t2 + t3 + t4) * denomrecip;
}

void cpheight(std::vector<POLYLINE> polylines)
{
	double max, min,i;
	i = 0;
	findMinMax(min, max);
	for (auto& polyline : polylines)
	{
		i += 1;
		for (auto& vert : polyline.m_vertices)
		{
			vert.z = 10* (polyline.m_value - min) / (max - min);
		}
		
	}
}

void criticalContours()
{
	double x0, y0;
	x0 = 0.0;
	y0 = 0.0;
	int j = 0;
	for (int i = 0; i < poly->nquads; i++)
	{
		bool test, saddle;
		double value = findCriticalPoint(poly->qlist[i], x0, y0, test, saddle);
		if (test)
		{
			/*CPOINT point;
			point.m_rgb.x = 1.0;
			point.m_rgb.z = 1.0;
			point.m_pos.x = x0;
			point.m_pos.y = y0;
			point.m_pos.z = 0;
			if (saddle)
			{
				point.type = 3;
			}
			points.push_back(point);*/
			//std::cout << "HEWRE: " << i<<','<<x0<<','<<y0 << std::endl;
			if (saddle)
			{

				// if (j == 1)
				// {
				// 	std::cout << "START PROBLEM CONTOUR" << std::endl;
				// }
				std::cerr<<"j, value: "<<j<<", "<<value<<std::endl;
				drawContourLine(value, icVector3(0.0, 0.0, 0.0), i);

				// if (j == 1)
				// {
				// 	std::cout << "END PROBLEM CONTOUR" << std::endl;
				// }

			}
			j += 1;

		}
	}
}



void plotCriticalPoints()
{
	//local min
	for (int i = 0; i < poly->nverts; i++)
	{
		auto v = poly->vlist[i]->scalar;
		int num = poly->vlist[i]->nquads;
		if (num < 4)
		{
			continue;
		}

		//if (i == 100)
		//{
		//	v = -100;
		//}

		bool flag_rej = false;
		for (int q = 0; q < 4; q++)
		{
			auto quad = poly->vlist[i]->quads[q];
			for (int j = 0; j < 4; j++)
			{
				if (quad->verts[j]->scalar<v)
				{
					flag_rej = true;
					break;
				}
			}
			if (flag_rej)
				break;
		}
		if (!flag_rej)
		{
			//This is local  min
			CPOINT point;
			point.m_rgb.x = 1.0;
			point.m_pos.x = poly->vlist[i]->x;
			point.m_pos.y = poly->vlist[i]->y;
			point.m_pos.z = poly->vlist[i]->z;

			point.type = 1;
			points.push_back(point);
			// std::cout << "MIN FOUND AT " << i << std::endl;
		}
	}

	//local max
	for (int i = 0; i < poly->nverts; i++)
	{
		auto v = poly->vlist[i]->scalar;
		int num = poly->vlist[i]->nquads;
		if (num < 4)
		{
			continue;
		}

		//if (i == 200)
		//{
		//	v = 100;
		//}

		bool flag_rej = false;
		int queue;
		for (int q = 0; q < 4; q++)
		{
			auto quad = poly->vlist[i]->quads[q];
			for (int j = 0; j < 4; j++)
			{
				if (quad->verts[j]->scalar > v)
				{
					flag_rej = true;
					break;
				}
			}
			if (flag_rej)
				break;
		}
		if (!flag_rej)
		{
			//This is local  max
			CPOINT point;
			point.m_rgb.x = 1.0;
			point.m_pos.x = poly->vlist[i]->x;
			point.m_pos.y = poly->vlist[i]->y;
			point.m_pos.z = poly->vlist[i]->z;
			point.type = 2;
			points.push_back(point);
			// std::cout << "max FOUND AT " << i<<',' << std::endl;
		}
	}

	//saddles
	double x0, y0;
	double max, min, range, value;
	findMinMax(min, max);
	range = max - min;
	for (int i = 0; i < poly->nquads; i++)
	{
		bool test, saddle;

		double value = findCriticalPoint(poly->qlist[i], x0, y0, test, saddle);
		if (test)
		{
			CPOINT point;
			point.m_rgb.x = 1.0;
			point.m_rgb.z = 1.0;
			point.m_pos.x = x0;
			point.m_pos.y = y0;
			if (fieldHeightOn)
			{
				point.m_pos.z = ((value - min) / range) * 10;
			}
			else
			{
				point.m_pos.z = 0;
			}
			if (saddle)
			{
				point.type = 3;
			}
			points.push_back(point);
		}
	}
	// display_crit_points();
}


