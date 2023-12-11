#include "Polyline.h"
///////////////////////////////////////////////////////////
// #include "gl/glew.h"
#include "libraries/glew-2.1.0/include/GL/glew.h"
#include <iostream>
///////////////////////////////////////////////////////////

#define EPSILON 1.0e-6
/*
In computer graphics, a polyline is a continuous line 
that is composed of one or more connected straight line segments, 
which, together, make up a shape. 
You can create a polyline by specifying the endpoints of each segment. 
In draw programs, you can treat a polyline as a single object or 
divide it into  segments.
*/
bool POLYLINE::isNeighbor(const POLYLINE&line) {

	if ((m_vertices.front() - line.m_vertices.front()).length() < EPSILON ||
		(m_vertices.front() - line.m_vertices.back()).length() < EPSILON ||
		(m_vertices.back() - line.m_vertices.front()).length() < EPSILON ||
		(m_vertices.back() - line.m_vertices.back()).length() < EPSILON) {
		return true;
	}
	else
		return false;

}
void POLYLINE::merge(const POLYLINE&line) {
	if ((m_vertices.front() - line.m_vertices.front()).length() < EPSILON) {
		POLYLINE line_ = line;
		line_.m_vertices.pop_front();
		for (auto i = line_.m_vertices.begin(); i != line_.m_vertices.end(); i++) {
			m_vertices.push_front(*i);
		}
	}
	else if ((m_vertices.front() - line.m_vertices.back()).length() < EPSILON) {
		POLYLINE reverseLine = line;	
		reverseLine.m_vertices.pop_back();
		reverseLine.m_vertices.reverse();
		for (auto i = reverseLine.m_vertices.begin(); i != reverseLine.m_vertices.end(); i++) {
			m_vertices.push_front(*i);
		}
	}
	else if ((m_vertices.back() - line.m_vertices.front()).length() < EPSILON) {
		POLYLINE line_ = line;
		line_.m_vertices.pop_front();
		for (auto i = line_.m_vertices.begin(); i != line_.m_vertices.end(); i++) {
			m_vertices.push_back(*i);
		}
	}
	else if ((m_vertices.back() - line.m_vertices.back()).length() < EPSILON) {
		POLYLINE reverseLine = line;
		reverseLine.m_vertices.pop_back();
		reverseLine.m_vertices.reverse();
		for (auto i = reverseLine.m_vertices.begin(); i != reverseLine.m_vertices.end(); i++) {
			m_vertices.push_back(*i);
		}
	}
}

Vertex linearInterpolateByScalar(const Vertex&v0, const Vertex&v1, const double&thres) {
	double f0_f1 = v0.scalar - v1.scalar;
	Vertex r(0.0, 0.0, 0.0);
	if (std::abs(f0_f1) < 1.0e-5) {
		r.x = (v0.x + v1.x) / 2;
		r.y = (v0.y + v1.y) / 2;
		r.z = (v0.z + v1.z) / 2;
	}
	else {
		double t = std::abs((v0.scalar - thres) / ((v0.scalar - thres) - (v1.scalar- thres)));
		r.x = v0.x + t * (v1.x - v0.x);
		r.y = v0.y + t * (v1.y - v0.y);
		r.z = v0.z + t * (v1.z - v0.z);
	}
	return r;
}

void lookUpTable(
	std::vector<Vertex> &r,
	const Vertex&v0, 
	const Vertex&v1, 
	const Vertex&v2,
	const Vertex&v3,
	const double&thres,
	bool saddle) {
	r.reserve(2);
	int id = 0;
	if (v0.scalar <= thres + EPSILON) {
		id += 1;
	}
	if (v1.scalar <= thres + EPSILON) {
		id += 2;
	}
	if (v2.scalar <= thres + EPSILON) {
		id += 4;
	}
	if (v3.scalar <= thres + EPSILON) {
		id += 8;
	}
	double center = 0;
	switch (id)
	{
	case 0:
		break;
	case 1:
		// v0 - v1 //v0 - v3
		r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v0, v3, thres));
		break;
	case 2:
		// v0 - v1 //v1 - v2
		r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v1, v2, thres));
		break;
	case 3:
		// v1 - v2 //v0 - v3
		r.push_back(linearInterpolateByScalar(v1, v2, thres));
		r.push_back(linearInterpolateByScalar(v0, v3, thres));
		break;
	case 4:
		// v1 - v2 //v2 - v3
		r.push_back(linearInterpolateByScalar(v1, v2, thres));
		r.push_back(linearInterpolateByScalar(v2, v3, thres));
		break;
	case 5:
		// v0 - v1 //v1 - v2
		// v2 - v3 //v0 - v3
		center = v0.scalar + v1.scalar + v2.scalar + v3.scalar;
		center /= 4;
		
		if (saddle)
		{
			if (center <= thres)
			{
				// std::cout << "BELOW" << std::endl;
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v3, v0, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));*/
			}
			else
			{
				// std::cout << "ABOVE" << std::endl;

				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v3, v0, thres));
				//r.push_back(linearInterpolateByScalar(v0, v1, thres));
				//r.push_back(linearInterpolateByScalar(v2, v3, thres));
				//r.push_back(linearInterpolateByScalar(v3, v0, thres));
				//r.push_back(linearInterpolateByScalar(v1, v2, thres));
			}
		}
		else
		{
			if (center <= thres) {
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				// std::cerr<<"case 5 center<=thres"<<std::endl;
			}
			else {
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				// std::cerr<<"case 5 center>thres"<<std::endl;
			}
		}
		break;
	case 6:
		// v0 - v1 //v2 - v3
		r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v2, v3, thres));
		break;
	case 7:
		// v2 - v3 //v0- v3
		r.push_back(linearInterpolateByScalar(v2, v3, thres));
		r.push_back(linearInterpolateByScalar(v0, v3, thres));
		break;
	case 8:
		// v2 - v3 //v0- v3
		r.push_back(linearInterpolateByScalar(v2, v3, thres));
		r.push_back(linearInterpolateByScalar(v0, v3, thres));
		break;
	case 9:
		// v0 - v1 //v2- v3
		r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v2, v3, thres));
		break;
	case 10:
		// v0 - v1 //v0- v3
		// v1 - v2 //v2- v3
		center = v0.scalar + v1.scalar + v2.scalar + v3.scalar;
		center /= 4;
		
		if (saddle)
		{
			if (center <= thres)//correct
			{
				/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));*/
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));

			}
			else//correct
			{
				/*r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));*/
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v3, v0, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
			}
		}
		else
		{
			// std::cerr<<"center, thres: "<<center<<", "<<thres<<std::endl
			if (center <= thres) {
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				// r.push_back(linearInterpolateByScalar(v0, v1, thres));
				// r.push_back(linearInterpolateByScalar(v0, v3, thres));
				// r.push_back(linearInterpolateByScalar(v1, v2, thres));
				// r.push_back(linearInterpolateByScalar(v2, v3, thres));
				// std::cerr<<"case 10 center<=thres"<<std::endl;
			}
			else {
				// r.push_back(linearInterpolateByScalar(v0, v3, thres));
				// r.push_back(linearInterpolateByScalar(v0, v1, thres));
				// r.push_back(linearInterpolateByScalar(v1, v2, thres));
				// r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v1, thres));
				r.push_back(linearInterpolateByScalar(v1, v2, thres));
				r.push_back(linearInterpolateByScalar(v2, v3, thres));
				r.push_back(linearInterpolateByScalar(v0, v3, thres));
				// std::cerr<<"case 10 center>thres"<<std::endl;
			}
		}
		
		break;
	case 11:
		// v1 - v2 //v2- v3
		r.push_back(linearInterpolateByScalar(v1, v2, thres));
		r.push_back(linearInterpolateByScalar(v2, v3, thres));
		break;
	case 12:
		// v1 - v2 //v0- v3
		r.push_back(linearInterpolateByScalar(v1, v2, thres));
		r.push_back(linearInterpolateByScalar(v0, v3, thres));
		break;
	case 13:	
		// v0 - v1 //v1- v2
		r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v1, v2, thres));
		break;
	case 14:
		// v0 - v1 //v0 - v3
		r.push_back(linearInterpolateByScalar(v0, v1, thres));
		r.push_back(linearInterpolateByScalar(v0, v3, thres));
		break;
	case 15:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


		////////////////////////////////////////////////////////
		lookUpTable(r,
			*poly.qlist[i]->verts[0],
			*poly.qlist[i]->verts[1],
			*poly.qlist[i]->verts[2],
			*poly.qlist[i]->verts[3],
			thres,saddle);
		// lookUpTable(r,
		// 	*poly.qlist[i]->verts[0],
		// 	*poly.qlist[i]->verts[1],
		// 	*poly.qlist[i]->verts[2],
		// 	*poly.qlist[i]->verts[3],
		// 	thres,saddle,i);
		////////////////////////////////////////////////////////
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
				line.m_vertices.push_back(v0);
				line.m_vertices.push_back(v1);
				// line.m_value = thres;
				edges.push_back(line);
			}
		}
	}
}


// void marchingSquare(
// 	std::list<POLYLINE>& edges,
// 	const Polyhedron&poly, 
// 	const double&thres) {
// 	for (int i = 0; i < poly.nquads; i++) {
// 		std::vector<Vertex> r;
// 		lookUpTable(r,
// 			*poly.qlist[i]->verts[0],
// 			*poly.qlist[i]->verts[1],
// 			*poly.qlist[i]->verts[2],
// 			*poly.qlist[i]->verts[3],
// 			thres);
// 		if (r.size() > 0) {
// 			for (int r_i = 0; r_i < r.size() / 2; r_i++) {
// 				POLYLINE line;
// 				auto v0 = icVector3(
// 					r[r_i * 2].x,
// 					r[r_i * 2].y,
// 					r[r_i * 2].z);
// 				auto v1 = icVector3(
// 					r[r_i * 2 + 1].x,
// 					r[r_i * 2 + 1].y,
// 					r[r_i * 2 + 1].z);
// 				/*if((v0 - v1).length() < EPSILON)
// 					line.m_vertices.push_back(v0);
// 				else {
// 					line.m_vertices.push_back(v0);
// 					line.m_vertices.push_back(v1);
// 				}*/
// 				line.m_vertices.push_back(v0);
// 				line.m_vertices.push_back(v1);
// 				edges.push_back(line);
// 			}
// 		}
// 	}
// }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void makePolylineFromEdges(
	std::vector<POLYLINE>&polylines,
	const std::list<POLYLINE>& edges) {
	polylines.reserve(edges.size());
	std::list<POLYLINE> edges_temp(edges);
	while (edges_temp.size() > 0){
		polylines.push_back(edges_temp.front());
		edges_temp.erase(edges_temp.begin());
		int init_size = 0;
		while (init_size != edges_temp.size()){
			init_size = edges_temp.size();
			for (auto i = edges_temp.begin(); i != edges_temp.end();) {
				if (polylines.back().isNeighbor(*i)) {
					polylines.back().merge(*i);
					i = edges_temp.erase(i);
				}
				else
					i++;
			}
		}
	}
	
}
void  display_polyline(std::vector<POLYLINE>&polylines) {
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (auto&polyline : polylines) {
		glLineWidth(polyline.m_weight);
		glColor3f(polyline.m_rgb.entry[0], polyline.m_rgb.entry[1], polyline.m_rgb.entry[2]);
		glBegin(GL_LINE_STRIP);
		for (auto it = polyline.m_vertices.begin(); it != polyline.m_vertices.end(); it++) {
			glVertex3d(it->entry[0], it->entry[1], it->entry[2]);
		}
		glEnd();
	}
	glDisable(GL_BLEND);
}