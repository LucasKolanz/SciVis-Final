#include "MarchingSquare.h"
//////////////////////////////////////////////////////
#include "libraries/glew-2.1.0/include/GL/glew.h"
// #include "GL/glew.h"
//////////////////////////////////////////////////////
#include <iostream>
#include "Project.h"
#include "Polyline.h"


#define EPSILON 1.0e-5

// bool POLYLINE::isNeighbor(const POLYLINE& line)
// {
// 	if ((m_verticies.front() - line.m_verticies.front()).length() < EPSILON ||
// 		(m_verticies.front() - line.m_verticies.back()).length() < EPSILON || 
// 		(m_verticies.back() - line.m_verticies.front()).length() < EPSILON ||
// 		(m_verticies.back() - line.m_verticies.back()).length() < EPSILON)
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }

// void POLYLINE::merge(const POLYLINE& line)
// {
// 	if ((m_verticies.front() - line.m_verticies.front()).length() < EPSILON)
// 	{
// 		auto line_ = line;
// 		line_.m_verticies.pop_front();
// 		for (auto i = line_.m_verticies.begin(); i != line_.m_verticies.end(); i++)
// 		{
// 			m_verticies.push_front(*i);
// 		}
// 	}
// 	else if ((m_verticies.front() - line.m_verticies.back()).length() < EPSILON)
// 	{
// 		auto reverseLine = line;
// 		reverseLine.m_verticies.pop_back();
// 		reverseLine.m_verticies.reverse();
// 		for (auto i = reverseLine.m_verticies.begin(); i != reverseLine.m_verticies.end(); i++)
// 		{
// 			m_verticies.push_front(*i);
// 		}
// 	}
// 	else if ((m_verticies.back() - line.m_verticies.front()).length() < EPSILON)
// 	{
// 		auto line_ = line;
// 		line_.m_verticies.pop_front();
// 		for (auto i = line_.m_verticies.begin(); i != line_.m_verticies.end(); i++)
// 		{
// 			m_verticies.push_back(*i);
// 		}
// 	}
// 	else if ((m_verticies.back() - line.m_verticies.back()).length() < EPSILON)
// 	{
// 		auto reverseLine = line;
// 		reverseLine.m_verticies.pop_back();
// 		reverseLine.m_verticies.reverse();
// 		for (auto i = reverseLine.m_verticies.begin(); i != reverseLine.m_verticies.end(); i++)
// 		{
// 			m_verticies.push_back(*i);
// 		}
// 	}
// }

// void makePolylineFromEdges(
// 	std::vector<POLYLINE>& polylines,
// 	const std::list<POLYLINE>& edges)
// {
// 	polylines.reserve(edges.size());
// 	auto edges_temp(edges);
// 	while (edges_temp.size() > 0)
// 	{
// 		polylines.push_back(edges_temp.front());
// 		edges_temp.erase(edges_temp.begin());
// 		int init_size = 0;
// 		while (init_size != edges_temp.size())
// 		{
// 			init_size = edges_temp.size();
// 			for (auto i = edges_temp.begin(); i != edges_temp.end();)
// 			{
// 				if (polylines.back().isNeighbor(*i))
// 				{
// 					polylines.back().merge(*i);
// 					i = edges_temp.erase(i);
// 				}
// 				else
// 				{
// 					i++;
// 				}
// 			}
// 		}
// 	}
// }

// void display_polyline(std::vector<POLYLINE>& polylines)
// {
// 	glDisable(GL_LIGHTING);
// 	glEnable(GL_LINE_SMOOTH);
// 	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
// 	for (auto& polyline : polylines)
// 	{
// 		glLineWidth(polyline.m_weight);
// 		glColor3f(
// 			polyline.m_rgb.entry[0],
// 			polyline.m_rgb.entry[1],
// 			polyline.m_rgb.entry[2]);
// 		glBegin(GL_LINE_STRIP);

// 		for (auto it = polyline.m_verticies.begin(); it != polyline.m_verticies.end(); it++)
// 		{
// 			glVertex3d(it->entry[0], it->entry[1], it->entry[2]);
// 		}
// 		glEnd();
// 	}

// 	glDisable(GL_BLEND);
// }

// Vertex linearInterpolateByScalar(const Vertex& v0, const Vertex& v1, const double& thres)
// {
// 	double f0_f1 = v0.scalar - v1.scalar;
// 	Vertex r(0.0, 0.0, 0.0);
// 	if (std::abs(f0_f1) < EPSILON)
// 	{
// 		r.x = (v0.x + v1.x) / 2;
// 		r.y = (v0.y + v1.y) / 2;
// 		r.z = (v0.z + v1.z) / 2;
// 	}
// 	else
// 	{
// 		double t = std::abs((v0.scalar - thres) / ((v0.scalar - thres) - (v1.scalar - thres)));
// 		r.x = v0.x + t * (v1.x - v0.x);
// 		r.y = v0.y + t * (v1.y - v0.y);
// 		r.z = v0.z + t * (v1.z - v0.z);
// 	}
// 	return r;
// }



inline void lookUpTable(
	std::vector <Vertex>& r,
	const Vertex& v0,
	const Vertex& v1,
	const Vertex& v2,
	const Vertex& v3,
	const double &thres,
	bool saddle,
	int quad_ind)
{
	Vertex vert0, vert1, vert2, vert3;
	Vertex vs[4] = { v0,v1,v2,v3 };
	double x1, x2, y1, y2;
	/*x1 = v0.x >= v1.x ? v1.x : v0.x;
	x1 = x1 >= v2.x ? v2.x : x1;
	x2 = v0.x < v1.x ? v1.x : v0.x;
	x2 = x2 < v2.x ? v2.x : x2;

	y1 = v0.y >= v1.y ? v1.y : v0.y;
	y1 = y1 >= v2.y ? v2.y : y1;
	y2 = v0.y < v1.y ? v1.y : v0.y;
	y2 = y2 < v2.y ? v2.y : y2;*/

	x2 = v0.x;
	y2 = v0.y;
	x1 = v2.x;
	y1 = v2.y;

	for (int i = 0; i < 4; i++)
	{
		if (vs[i].x == x2 && vs[i].y == y2)
		{
			vert0 = vs[i];
			//std::cout <<"vert0 = " << i << std::endl;
		}
		else if (vs[i].x == x1 && vs[i].y == y2)
		{
			vert1 = vs[i];
			//std::cout << "vert1 = " << i << std::endl;

			//std::cout << f21 << " f21 i is " << i << std::endl;
		}
		else if (vs[i].x == x1 && vs[i].y == y1)
		{
			vert2 = vs[i];
			//std::cout << "vert2 = " << i << std::endl;

			//std::cout << f22 << " f22 i is " << i << std::endl;
		}
		else if (vs[i].x == x2 && vs[i].y == y1)
		{
			vert3 = vs[i];
			//std::cout << "vert3 = " << i << std::endl;

			//std::cout << f12 << " f12 i is " << i << std::endl;
		}
	}


	r.reserve(2);
	int id = 0;
	if (vert0.scalar <= thres + EPSILON)
	{
		id += 1;
	}
	if (vert1.scalar <= thres + EPSILON)
	{
		id += 2;
	}
	if (vert2.scalar <= thres + EPSILON)
	{
		id += 4;
	}
	if (vert3.scalar <= thres + EPSILON)
	{
		id += 8;
	}
	double center = 0.0;
	/*if (quad_ind == 96 && !saddle)
	{
		std::cout << "=============================================================" << std::endl;
		id == 5;
	}*/
	switch (id)
	{
	case 0:
		break;
	case 1:
		r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
		r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
		break;
	case 2:
		r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
		r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
		break;
	case 3:
		r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
		r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
		break;
	case 4:
		r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
		r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
		break;
	case 5:
		std::cout << "CASE 5" << std::endl;
		center = vert0.scalar + vert1.scalar + vert2.scalar + vert3.scalar;
		center = center / 4;
		if (saddle)
		{
			if (center <= thres)
			{
				std::cout << "BELOW" << std::endl;
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v3, v0, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));*/
			}
			else
			{
				std::cout << "ABOVE" << std::endl;

				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert3, vert0, thres));
				//r.push_back(linearInterpolateByScalar(v0, v1, thres));
				//r.push_back(linearInterpolateByScalar(v2, v3, thres));
				//r.push_back(linearInterpolateByScalar(v3, v0, thres));
				//r.push_back(linearInterpolateByScalar(v1, v2, thres));
			}
		}
		else
		{
			if (center <= thres)
			{
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
			}
			else
			{
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
			}
		}
		break;
	case 6:
		r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
		r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
		break;
	case 7:  ///////////////////////////////////////////////////////////////case 7 and 8 are the same
		r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
		r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
		break;
	case 8:
		r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
		r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
		break;
	case 9:
		r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
		r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
		break;
	case 10:
		// std::cout << "CASE 10 " << thres << std::endl;
		// std::cout << "vert0: (" << vert0.x << ',' << vert0.y << ',' << vert0.z << ") " << vert0.scalar << std::endl;
		// //std::cout << "v0: (" << v0.x << ',' << v0.y << ',' << v0.z << ") " << v0.scalar << std::endl;
		// std::cout << "vert1: (" << vert1.x << ',' << vert1.y << ',' << vert1.z << ")" << vert1.scalar << std::endl;
		// std::cout << "vert2: (" << vert2.x << ',' << vert2.y << ',' << vert2.z << ")" << vert2.scalar << std::endl;
		// std::cout << "vert3: (" << vert3.x << ',' << vert3.y << ',' << vert3.z << ")" << vert3.scalar << std::endl;
		center = vert0.scalar + vert1.scalar + vert2.scalar + vert3.scalar;
		center = center / 4;
		if (saddle)
		{
			if (center <= thres)//correct
			{
				/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));*/
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));

			}
			else//correct
			{
				/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));*/
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert3, vert0, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
			}
		}
		else
		{
			if (center <= thres)
			{
				std::cout << "less than THRES" << std::endl;
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));

			}
			else
			{
				std::cout << "GREATER THAN THRES" << std::endl;
				r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
				/*r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
				r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
				r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
				r.push_back(linearInterpolateByScalar(vert2, vert3, thres));*/
			}
		}
		break;
	case 11:
		r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
		r.push_back(linearInterpolateByScalar(vert2, vert3, thres));
		break;
	case 12:
		r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
		r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
		break;
	case 13:
		r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
		r.push_back(linearInterpolateByScalar(vert1, vert2, thres));
		break;
	case 14:
		r.push_back(linearInterpolateByScalar(vert0, vert1, thres));
		r.push_back(linearInterpolateByScalar(vert0, vert3, thres));
		break;
	case 15:
		/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v2, v3, thres));*/
		break;
	}


}

void marchingSquare(
	std::list<POLYLINE>& edges, 
	const Polyhedron& poly, 
	const double& thres,
	int saddle_q_index)
{
	bool saddle;
	for (auto i = 0; i < poly.nquads; i++)
	{
		//std::cout << i << std::endl;
		if (i == saddle_q_index)
		{
			saddle = true;
			//std::cout << "SADDLE AT QUAD: " << i << std::endl;
		}
		else
		{
			//std::cout << "not a saddle at quad: " << i << std::endl;
			saddle = false;
		}
		std::vector<Vertex> r;

		lookUpTable(r,
			*poly.qlist[i]->verts[0],
			*poly.qlist[i]->verts[1],
			*poly.qlist[i]->verts[2],
			*poly.qlist[i]->verts[3],
			thres,saddle,i);
		if (r.size() > 0)
		{
			/*if (r.size() == 4)
			{
				std::cout<< "POSSIBLE SADDLE"<<std::endl; 
			}*/
			for (auto r_i = 0; r_i < r.size() / 2; r_i++)
			{
				POLYLINE line;
				auto v0 = icVector3(r[r_i * 2].x, r[r_i * 2].y, r[r_i * 2].z);
				auto v1 = icVector3(r[r_i*2+1].x, r[r_i * 2 + 1].y, r[r_i * 2 + 1].z);
				line.m_verticies.push_back(v0);
				line.m_verticies.push_back(v1);
				line.m_value = thres;
				edges.push_back(line);
			}
		}
	}
}

void display_points(std::vector<CPOINT>& points)
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