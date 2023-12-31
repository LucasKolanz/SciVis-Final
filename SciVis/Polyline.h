#pragma once
#include "polyhedron.h"
#include <utility>  
#include <list>
#include <vector>
class POLYLINE {
public:
	std::list<icVector3> m_vertices;
	icVector3 m_rgb = icVector3(1.0, 0.0, 0.0);
	double m_weight = 1;
	double m_value;
	bool isNeighbor(const POLYLINE&line);
	void merge(const POLYLINE&line);
};

struct CPOINT
{
	icVector3 m_pos = icVector3(0.0);
	icVector3 m_rgb = icVector3(0.0);
	double m_value = 0.0;
	double m_weight = 1.0;
	int type;
	//1 for min
	//2 for max
	//3 for saddle

};


///////////////////////////////////////////////////
// void marchingSquare(
// 	std::list<POLYLINE>& edges,
// 	const Polyhedron&poly, 
// 	const double&thres);
void marchingSquare(
	std::list<POLYLINE>& edges,
	const Polyhedron& poly,
	const double& thres,
	int saddle_q_index);
///////////////////////////////////////////////////
void makePolylineFromEdges(
	std::vector<POLYLINE>&polylines,
	const std::list<POLYLINE>& edges);
void display_polyline(std::vector<POLYLINE>&polylines);
