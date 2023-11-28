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

void GLWidget::algorithm1() 
{
	
}
void GLWidget::algorithm2() 
{
	if (polylines.size() > 0)
	{
		polylines.clear();
	}
	else
	{
		int tot_lines = 100;
		double max, min, range, value;
		findMinMax(min, max);
		range = max - min;
		range = max - min;
		for (int i = 0; i < tot_lines; ++i)
		{
			value = (i * 1.0) * (range / tot_lines) + min;
			//std::cout << value << std::endl;
			drawContourLine(value, icVector3(0.0,0.0,0.0), false);
		}
	}
	update();
}
void GLWidget::algorithm3() {

}
void GLWidget::algorithm4() {
	// contHeight();
	if (polylines.size() > 0)
	{
		GLWidget::algorithm2();
	}
	fieldHeight();
	update();
}
void GLWidget::algorithm5() {
	qDebug() << 5;
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

// void drawAllLines()

void contHeight()
{	
	double max, min;
	findMinMax(min, max);
	for (auto& polyline : polylines) 
	{
		for (auto& vertex : polyline.m_vertices)
		{
			// auto& vertex = polylines->vlist[i];
			if (contHeightOn)
			{
				// polyline.m_vertex->z = 0; 
				vertex.z = 0;
			}
			else
			{
				double s_v = 10;//vertex.scalar;

				double l = (s_v - min) / (max - min);
				// polyline.vertex->z = l*10; 
				vertex.z = l*10;
			}
		}
	}
	// for (auto i = 0; i < polylines->nverts; i++)
	// {
		
	// }
	contHeightOn = !contHeightOn;
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