#pragma once
#include "polyhedron.h"
#include <list>
#include <vector>

struct POLYLINE
{
	std::list<icVector3> m_verticies;
	icVector3 m_rgb = icVector3(1.0, 0.0, 0.0);
	double m_weight = 1.0;
	double m_value;
	bool isNeighbor(const POLYLINE& line);
	void merge(const POLYLINE& line);
};

// void display_polyline(std::vector<POLYLINE>& polylines);
/*Vertex linearInterpolateByScalar(
	const Vertex& v0, 
	const Vertex& v1, 
	const double& thres);*/

	/*void lookUpTable(
	std::vector <Vertex>& r,
	const Vertex& v0,
	const Vertex& v1,
	const Vertex& v2,
	const Vertex& v3,
	const double& thres);*/
void marchingSquare(
	std::list<POLYLINE>& edges,
	const Polyhedron& poly,
	const double& thres,
	int saddle_q_index);

//Vertex linearInterpolateByScalar(const Vertex& v0, const Vertex& v1, const double& thres);

// void makePolylineFromEdges(
// 	std::vector<POLYLINE>&polylines,
// 	const std::list<POLYLINE>&edges);

struct CPOINT
{
	icVector3 m_pos = icVector3(0.0);
	icVector3 m_rgb = icVector3(0.0);
	double m_weight = 1.0;
	int type;
	//1 for min
	//2 for max
	//3 for saddle

};

void display_points(std::vector<CPOINT>& points);
//inline void getx1x2y1y2(Vertex& v0, Vertex& v2, double& x1, double& y1, double& x2, double& y2);
//inline void getx1x2y1y2(Vertex* v0, Vertex* v2, double& x1, double& y1, double& x2, double& y2);