/*

Functions for learnply

Eugene Zhang, 2005

The polygon data structure
*/

#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "ply.h"
#include "icVector.H"
#include "icMatrix.H"
#include "polyhedron.h"
#include "ply_io.h"

static PlyFile* in_ply;


/******************************************************************************
Read in a polyhedron from a file.
******************************************************************************/
Polyhedron::Polyhedron(FILE *file)
{
	int i, j;
	int elem_count;
	char *elem_name;

	if (file != NULL)
	{
		/*** Read in the original PLY object ***/
		in_ply = read_ply(file);
		
		for (i = 0; i < in_ply->num_elem_types; i++) {

			/* prepare to read the i'th list of elements */
			elem_name = setup_element_read_ply(in_ply, i, &elem_count);

			if (equal_strings("vertex", elem_name)) {

				/* create a vertex list to hold all the vertices */
				nverts = max_verts = elem_count;
				vlist = new Vertex *[nverts];

				/* set up for getting vertex elements */

				setup_property_ply(in_ply, &vert_props[0]);
				setup_property_ply(in_ply, &vert_props[1]);
				setup_property_ply(in_ply, &vert_props[2]);
				setup_property_ply(in_ply, &vert_props[3]);
				setup_property_ply(in_ply, &vert_props[4]);
				setup_property_ply(in_ply, &vert_props[5]);
				setup_property_ply(in_ply, &vert_props[6]);
				setup_property_ply(in_ply, &vert_props[7]);
				setup_property_ply(in_ply, &vert_props[8]);
				setup_property_ply(in_ply, &vert_props[9]);

				vert_other = get_other_properties_ply(in_ply,
					offsetof(Vertex_io, other_props));

				/* grab all the vertex elements */
				for (j = 0; j < nverts; j++) {
					Vertex_io vert;
					get_element_ply(in_ply, (void *)&vert);

					/* copy info from the "vert" structure */
					vlist[j] = new Vertex(vert.x, vert.y, vert.z);
					vlist[j]->vx = vert.vx;
					vlist[j]->vy = vert.vy;
					vlist[j]->vz = vert.vz;


					vlist[j]->R = vert.R*1.0/255.0;
					vlist[j]->G = vert.G*1.0/255.0;
					vlist[j]->B = vert.B*1.0/255.0;

					vlist[j]->scalar = vert.s;

					vlist[j]->other_props = vert.other_props;
				}
			}
			else if (equal_strings("face", elem_name)) {

				/* create a list to hold all the face elements */
				nquads = max_quads = elem_count;
				qlist = new Quad *[nquads];

				/* set up for getting face elements */
				setup_property_ply(in_ply, &face_props[0]);
				face_other = get_other_properties_ply(in_ply, offsetof(Face_io, other_props));

				/* grab all the face elements */
				for (j = 0; j < elem_count; j++) {
					Face_io face;
					get_element_ply(in_ply, (void *)&face);

					if (face.nverts != 4) {
						fprintf(stderr, "Face has %d vertices (should be four).\n",
							face.nverts);
						exit(-1);
					}

					/* copy info from the "face" structure */
					qlist[j] = new Quad;
					// qlist[j]->verts[0] = face.verts[0];
					qlist[j]->verts[0] = (Vertex *)face.verts[0];
					qlist[j]->verts[1] = (Vertex *)face.verts[1];
					qlist[j]->verts[2] = (Vertex *)face.verts[2];
					qlist[j]->verts[3] = (Vertex *)face.verts[3];
					qlist[j]->other_props = face.other_props;
				}
			}
			else
				get_other_element_ply(in_ply);
		}

		/* close the file */
		close_ply(in_ply);

		//////////////////////////////////////////////////////////////////////
		/* fix up vertex pointers in quads */
		// for (i = 0; i < nquads; i++) {
		// 	qlist[i]->verts[0] = vlist[(int)qlist[i]->verts[0]];
		// 	qlist[i]->verts[1] = vlist[(int)qlist[i]->verts[1]];
		// 	qlist[i]->verts[2] = vlist[(int)qlist[i]->verts[2]];
		// 	qlist[i]->verts[3] = vlist[(int)qlist[i]->verts[3]];
		// }
		for (i = 0; i < nquads; i++) {
			qlist[i]->verts[0] = vlist[(size_t)qlist[i]->verts[0]];
			qlist[i]->verts[1] = vlist[(size_t)qlist[i]->verts[1]];
			qlist[i]->verts[2] = vlist[(size_t)qlist[i]->verts[2]];
			qlist[i]->verts[3] = vlist[(size_t)qlist[i]->verts[3]];
		}
		//////////////////////////////////////////////////////////////////////

		/* get rid of quads that use the same vertex more than once */

		scrub_quads();
	}
}

Polyhedron::Polyhedron(int numverts,int numedges,int numquads)
{
	nverts = max_verts = numverts;
	nedges = numedges;
	nquads = max_quads = numquads;

	vlist = new Vertex * [max_verts];
	qlist = new Quad * [max_quads];
	
	init_verts();
	// init_quads();
	// scrub_quads();
}

Polyhedron::Polyhedron()
{
	nverts = nedges = nquads = 0;
	max_verts = max_quads = 50;

	vlist = new Vertex * [max_verts];
	qlist = new Quad * [max_quads];

}

void Polyhedron::scrub_quads()
{
	for (int i = nquads - 1; i >= 0; i--) 
	{
		Quad *quad = qlist[i];
		Vertex *v0 = quad->verts[0];
		Vertex *v1 = quad->verts[1];
		Vertex *v2 = quad->verts[2];
		Vertex *v3 = quad->verts[3];

		if (v0 == v1 || v1 == v2 || v2 == v3 || v3 == v0) {
			free(qlist[i]);
			nquads--;
			qlist[i] = qlist[nquads];
		}
	}
}

void Polyhedron::init_verts()
{
	for (int j = 0; j < nverts; j++) 
	{
		vlist[j] = new Vertex(0, 0, 0);
		// vlist[j]->vx = 0;
		// vlist[j]->vy = 0;
		// vlist[j]->vz = 0;

		// vlist[j]->R = 0;
		// vlist[j]->G = 0;
		// vlist[j]->B = 0;

		// vlist[j]->scalar = 0;

		// vlist[j]->other_props = vert.other_props;
	}
}

void Polyhedron::init_quads()
{
	// for (j = 0; j < elem_count; j++) 
	// {

	// 	/* copy info from the "face" structure */
	// 	qlist[j] = new Quad;
	// 	// qlist[j]->verts[0] = face.verts[0];
	// 	qlist[j]->verts[0] = (Vertex *)face.verts[0];
	// 	qlist[j]->verts[1] = (Vertex *)face.verts[1];
	// 	qlist[j]->verts[2] = (Vertex *)face.verts[2];
	// 	qlist[j]->verts[3] = (Vertex *)face.verts[3];
	// 	qlist[j]->other_props = face.other_props;
	// }
}

void Polyhedron::write_info()
{
	printf("#verts: %d\n", nverts);
	printf("#edges: %d\n", nedges);
	printf("#faces: %d\n", nquads);
}

/******************************************************************************
Write out a polyhedron to a file.
******************************************************************************/
void Polyhedron::write_file(FILE *file)
{
	int i;
	PlyFile *ply;
	char **elist;
	int num_elem_types;

	/*** Write out the transformed PLY object ***/

	elist = get_element_list_ply(in_ply, &num_elem_types);
	ply = write_ply(file, num_elem_types, elist, in_ply->file_type);

	/* describe what properties go into the vertex elements */

	describe_element_ply(ply, "vertex", nverts);
	describe_property_ply(ply, &vert_props[0]);
	describe_property_ply(ply, &vert_props[1]);
	describe_property_ply(ply, &vert_props[2]);
	//  describe_other_properties_ply (ply, vert_other, offsetof(Vertex_io,other_props));

	describe_element_ply(ply, "face", nquads);
	describe_property_ply(ply, &face_props[0]);

	//  describe_other_properties_ply (ply, face_other,
	//                                offsetof(Face_io,other_props));

	//  describe_other_elements_ply (ply, in_ply->other_elems);

	copy_comments_ply(ply, in_ply);
	char mm[1024];
	sprintf(mm, "modified by learnply");
	//  append_comment_ply (ply, "modified by simvizply %f");
	append_comment_ply(ply, mm);
	copy_obj_info_ply(ply, in_ply);

	header_complete_ply(ply);

	/* set up and write the vertex elements */
	put_element_setup_ply(ply, "vertex");
	for (i = 0; i < nverts; i++) {
		Vertex_io vert;

		/* copy info to the "vert" structure */
		vert.x = vlist[i]->x;
		vert.y = vlist[i]->y;
		vert.z = vlist[i]->z;
		vert.other_props = vlist[i]->other_props;

		put_element_ply(ply, (void *)&vert);
	}

	/* index all the vertices */
	for (i = 0; i < nverts; i++)
		vlist[i]->index = i;

	/* set up and write the face elements */
	put_element_setup_ply(ply, "face");

	Face_io face;
	////////////////////////////////////////////////////
	// face.verts = new size_t[4];
	face.verts = new int[4];
	////////////////////////////////////////////////////

	for (i = 0; i < nquads; i++) {

		/* copy info to the "face" structure */
		face.nverts = 4;
		face.verts[0] = qlist[i]->verts[0]->index;
		face.verts[1] = qlist[i]->verts[1]->index;
		face.verts[2] = qlist[i]->verts[2]->index;
		face.verts[3] = qlist[i]->verts[3]->index;
		face.other_props = qlist[i]->other_props;

		put_element_ply(ply, (void *)&face);
	}
	put_other_elements_ply(ply);

	close_ply(ply);
	free_ply(ply);
}

void Polyhedron::initialize()
{
	selected_quad = -1;
	selected_vertex = -1;

	create_pointers();
	calc_edge_length();
	calc_bounding_sphere();
	calc_face_normals_and_area();
	average_normals();
}

void Polyhedron::finalize() {

	for (int i = 0; i < nquads; i++) {
		//free(qlist[i]->other_props);
		free(qlist[i]);
	}
	for (int i = 0; i < nedges; i++) {
		free(elist[i]->quads);
		free(elist[i]);
	}
	for (int i = 0; i < nverts; i++) {
		free(vlist[i]->quads);
		//free(vlist[i]->other_props);
		free(vlist[i]);
	}

	free(qlist);
	free(elist);
	free(vlist);
	if (!vert_other)
		free(vert_other);
	if (!face_other)
		free(face_other);
}

/******************************************************************************
Find out if there is another face that shares an edge with a given face.

Entry:
  f1    - face that we're looking to share with
  v1,v2 - two vertices of f1 that define edge

Exit:
  return the matching face, or NULL if there is no such face
******************************************************************************/

Quad *Polyhedron::find_common_edge(Quad *f1, Vertex *v1, Vertex *v2)
{
	int i, j;
	Quad *f2;
	Quad *adjacent = NULL;

	/* look through all faces of the first vertex */

	for (i = 0; i < v1->nquads; i++) {
		f2 = v1->quads[i];
		if (f2 == f1)
			continue;
		/* examine the vertices of the face for a match with the second vertex */
		for (j = 0; j < 4; j++) {

			/* look for a match */
			if (f2->verts[j] == v2) {

#if 0
				/* watch out for triple edges */

				if (adjacent != NULL) {

					fprintf(stderr, "model has triple edges\n");

					fprintf(stderr, "face 1: ");
					for (k = 0; k < f1->nverts; k++)
						fprintf(stderr, "%d ", f1->iverts[k]);
					fprintf(stderr, "\nface 2: ");
					for (k = 0; k < f2->nverts; k++)
						fprintf(stderr, "%d ", f2->iverts[k]);
					fprintf(stderr, "\nface 3: ");
					for (k = 0; k < adjacent->nverts; k++)
						fprintf(stderr, "%d ", adjacent->iverts[k]);
					fprintf(stderr, "\n");

				}

				/* if we've got a match, remember this face */
				adjacent = f2;
#endif

#if 1
				/* if we've got a match, return this face */
				return (f2);
#endif

			}
		}
	}

	return (adjacent);
}


/******************************************************************************
Create an edge.

Entry:
  v1,v2 - two vertices of f1 that define edge
******************************************************************************/

void Polyhedron::create_edge(Vertex *v1, Vertex *v2)
{
	int i, j;
	Quad *f;

	/* make sure there is enough room for a new edge */

	if (nedges >= max_edges) {

		max_edges += 100;
		Edge **list = new Edge *[max_edges];

		/* copy the old list to the new one */
		for (i = 0; i < nedges; i++)
			list[i] = elist[i];

		/* replace list */
		free(elist);
		elist = list;
	}

	/* create the edge */

	elist[nedges] = new Edge;
	Edge *e = elist[nedges];
	e->index = nedges;
	e->verts[0] = v1;
	e->verts[1] = v2;
	nedges++;

	/* count all quads that will share the edge, and do this */
	/* by looking through all faces of the first vertex */

	e->nquads = 0;

	for (i = 0; i < v1->nquads; i++) {
		f = v1->quads[i];
		/* examine the vertices of the face for a match with the second vertex */
		for (j = 0; j < 4; j++) {
			/* look for a match */
			if (f->verts[j] == v2) {
				e->nquads++;
				break;
			}
		}
	}

	/* make room for the face pointers (at least two) */
	if (e->nquads < 2)
		e->quads = new Quad *[2];
	else
		e->quads = new Quad *[e->nquads];

	/* create pointers from edges to faces and vice-versa */

	e->nquads = 0; /* start this out at zero again for creating ptrs to quads */

	for (i = 0; i < v1->nquads; i++) {

		f = v1->quads[i];

		/* examine the vertices of the face for a match with the second vertex */
		for (j = 0; j < 4; j++)
			if (f->verts[j] == v2) {

				e->quads[e->nquads] = f;
				e->nquads++;

				if (f->verts[(j + 1) % 4] == v1)
					f->edges[j] = e;
				else if (f->verts[(j + 2) % 4] == v1)
					f->edges[(j + 2) % 4] = e;
				else if (f->verts[(j + 3) % 4] == v1)
					f->edges[(j + 3) % 4] = e;
				else {
					fprintf(stderr, "Non-recoverable inconsistancy in create_edge()\n");
					exit(-1);
				}

				break;  /* we'll only find one instance of v2 */
			}

	}
}


/******************************************************************************
Create edges.
******************************************************************************/

void Polyhedron::create_edges()
{
	int i, j;
	Quad *f;
	Vertex *v1, *v2;
	double count = 0;

	/* count up how many edges we may require */

	for (i = 0; i < nquads; i++) {
		f = qlist[i];
		for (j = 0; j < 4; j++) {
			v1 = f->verts[j];
			v2 = f->verts[(j + 1) % 4];
			Quad *result = find_common_edge(f, v1, v2);
			if (result)
				count += 0.5;
			else
				count += 1;
		}
	}

	/*
	printf ("counted %f edges\n", count);
	*/

	/* create space for edge list */

	max_edges = (int)(count + 10);  /* leave some room for expansion */
	elist = new Edge *[max_edges];
	nedges = 0;

	/* zero out all the pointers from faces to edges */

	for (i = 0; i < nquads; i++)
		for (j = 0; j < 4; j++)
			qlist[i]->edges[j] = NULL;

	/* create all the edges by examining all the quads */

	for (i = 0; i < nquads; i++) {
		f = qlist[i];
		for (j = 0; j < 4; j++) {
			/* skip over edges that we've already created */
			if (f->edges[j])
				continue;
			v1 = f->verts[j];
			v2 = f->verts[(j + 1) % 4];
			create_edge(v1, v2);
		}
	}
}


/******************************************************************************
Create pointers from vertices to faces.
******************************************************************************/

void Polyhedron::vertex_to_quad_ptrs()
{
	int i, j;
	Quad *f;
	Vertex *v;

	/* zero the count of number of pointers to faces */

	for (i = 0; i < nverts; i++)
		vlist[i]->max_quads = 0;

	/* first just count all the face pointers needed for each vertex */

	for (i = 0; i < nquads; i++) {
		f = qlist[i];
		for (j = 0; j < 4; j++)
			f->verts[j]->max_quads++;
	}

	/* allocate memory for face pointers of vertices */

	for (i = 0; i < nverts; i++) {
		vlist[i]->quads = (Quad **)
			malloc(sizeof(Quad *) * vlist[i]->max_quads);
		vlist[i]->nquads = 0;
	}

	/* now actually create the face pointers */

	for (i = 0; i < nquads; i++) {
		f = qlist[i];
		for (j = 0; j < 4; j++) {
			v = f->verts[j];
			v->quads[v->nquads] = f;
			v->nquads++;
		}
	}
}

/******************************************************************************
Create pointers from vertices to edges.
******************************************************************************/

void Polyhedron::vertex_to_edge_ptrs()
{
	Edge *e;
	Vertex *v;

	/* zero the count of number of pointers to faces */

	for (int i = 0; i < nverts; i++)
		vlist[i]->max_edges = 0;

	/* first just count all the edge pointers needed for each vertex */

	for (int i = 0; i < nedges; i++) {
		e = elist[i];
		for (int j = 0; j < 2; j++)
			e->verts[j]->max_edges++;
	}

	/* allocate memory for edge pointers of vertices */

	for (int i = 0; i < nverts; i++) {
		vlist[i]->edges = (Edge **)
			malloc(sizeof(Edge *) * vlist[i]->max_edges);
		vlist[i]->nedges = 0;
	}

	/* now actually create the edge pointers */

	for (int i = 0; i < nedges; i++) {
		e = elist[i];
		for (int j = 0; j < 2; j++) {
			v = e->verts[j];
			v->edges[v->nedges] = e;
			v->nedges++;
		}
	}
}


/******************************************************************************
Find the other quad that is incident on an edge, or NULL if there is
no other.
******************************************************************************/

Quad *Polyhedron::other_quad(Edge *edge, Quad *quad)
{
	/* search for any other quad */
	for (int i = 0; i < edge->nquads; i++)
		if (edge->quads[i] != quad)
			return (edge->quads[i]);

	/* there is no such other quad if we get here */
	return (NULL);
}


/******************************************************************************
Order the pointers to faces that are around a given vertex.

Entry:
  v - vertex whose face list is to be ordered
******************************************************************************/

void Polyhedron::order_vertex_to_quad_ptrs(Vertex *v)
{
	int i, j;
	Quad *f;
	Quad *fnext;
	int nf;
	int vindex;
	int boundary;
	int count;

	nf = v->nquads;
	f = v->quads[0];

	/* go backwards (clockwise) around faces that surround a vertex */
	/* to find out if we reach a boundary */

	boundary = 0;

	for (i = 1; i <= nf; i++) {

		/* find reference to v in f */
		vindex = -1;
		for (j = 0; j < 4; j++)
			if (f->verts[j] == v) {
				vindex = j;
				break;
			}

		/* error check */
		if (vindex == -1) {
			fprintf(stderr, "can't find vertex #1\n");
			exit(-1);
		}

		/* corresponding face is the previous one around v */
		fnext = other_quad(f->edges[vindex], f);

		/* see if we've reached a boundary, and if so then place the */
		/* current face in the first position of the vertice's face list */

		if (fnext == NULL) {
			/* find reference to f in v */
			for (j = 0; j < v->nquads; j++)
				if (v->quads[j] == f) {
					v->quads[j] = v->quads[0];
					v->quads[0] = f;
					break;
				}
			boundary = 1;
			break;
		}

		f = fnext;
	}

	/* now walk around the faces in the forward direction and place */
	/* them in order */

	f = v->quads[0];
	count = 0;

	for (i = 1; i < nf; i++) {

		/* find reference to vertex in f */
		vindex = -1;
		for (j = 0; j < 4; j++)
			if (f->verts[(j + 1) % 4] == v) {
				vindex = j;
				break;
			}

		/* error check */
		if (vindex == -1) {
			fprintf(stderr, "can't find vertex #2\n");
			exit(-1);
		}

		/* corresponding face is next one around v */
		fnext = other_quad(f->edges[vindex], f);

		/* break out of loop if we've reached a boundary */
		count = i;
		if (fnext == NULL) {
			break;
		}

		/* swap the next face into its proper place in the face list */
		for (j = 0; j < v->nquads; j++)
			if (v->quads[j] == fnext) {
				v->quads[j] = v->quads[i];
				v->quads[i] = fnext;
				break;
			}

		f = fnext;
	}
}


/******************************************************************************
Find the index to a given vertex in the list of vertices of a given face.

Entry:
  f - face whose vertex list is to be searched
  v - vertex to return reference to

Exit:
  returns index in face's list, or -1 if vertex not found
******************************************************************************/
int Polyhedron::face_to_vertex_ref(Quad *f, Vertex *v)
{
	int j;
	int vindex = -1;

	for (j = 0; j < 4; j++)
		if (f->verts[j] == v) {
			vindex = j;
			break;
		}

	return (vindex);
}

/******************************************************************************
Create various face and vertex pointers.
******************************************************************************/
void Polyhedron::create_pointers()
{
	int i;

	/* index the vertices and quads */

	for (i = 0; i < nverts; i++)
		vlist[i]->index = i;

	for (i = 0; i < nquads; i++)
		qlist[i]->index = i;

	/* create pointers from vertices to quads */
	vertex_to_quad_ptrs();

	/* make edges */
	create_edges();

	vertex_to_edge_ptrs();

	/* order the pointers from vertices to faces */
	for (i = 0; i < nverts; i++) {
		//		if (i %1000 == 0)
		//			fprintf(stderr, "ordering %d of %d vertices\n", i, nverts);
		order_vertex_to_quad_ptrs(vlist[i]);

	}
	/* index the edges */

	for (i = 0; i < nedges; i++) {
		//		if (i %1000 == 0)
		//			fprintf(stderr, "indexing %d of %d edges\n", i, nedges);
		elist[i]->index = i;
	}

}

void Polyhedron::calc_bounding_sphere()
{
	unsigned int i;
	icVector3 min, max;

	for (i = 0; i < nverts; i++) {
		if (i == 0) {
			min.set(vlist[i]->x, vlist[i]->y, vlist[i]->z);
			max.set(vlist[i]->x, vlist[i]->y, vlist[i]->z);
		}
		else {
			if (vlist[i]->x < min.entry[0])
				min.entry[0] = vlist[i]->x;
			if (vlist[i]->x > max.entry[0])
				max.entry[0] = vlist[i]->x;
			if (vlist[i]->y < min.entry[1])
				min.entry[1] = vlist[i]->y;
			if (vlist[i]->y > max.entry[1])
				max.entry[1] = vlist[i]->y;
			if (vlist[i]->z < min.entry[2])
				min.entry[2] = vlist[i]->z;
			if (vlist[i]->z > max.entry[2])
				max.entry[2] = vlist[i]->z;
		}
	}
	center = (min + max) * 0.5;
	radius = length(center - min);
}

void Polyhedron::calc_edge_length()
{
	int i;
	icVector3 v1, v2;

	for (i = 0; i < nedges; i++) {
		v1.set(elist[i]->verts[0]->x, elist[i]->verts[0]->y, elist[i]->verts[0]->z);
		v2.set(elist[i]->verts[1]->x, elist[i]->verts[1]->y, elist[i]->verts[1]->z);
		elist[i]->length = length(v1 - v2);
	}
}

void Polyhedron::calc_face_normals_and_area()
{
	unsigned int i, j;
	icVector3 v0, v1, v2, v3;
	Quad *temp_q;
	double edge_length[4];

	area = 0.0;
	for (i = 0; i < nquads; i++) {
		for (j = 0; j < 4; j++)
			edge_length[j] = qlist[i]->edges[j]->length;

		icVector3 d1, d2;
		d1.set(qlist[i]->verts[0]->x, qlist[i]->verts[0]->y, qlist[i]->verts[0]->z);
		d2.set(qlist[i]->verts[2]->x, qlist[i]->verts[2]->y, qlist[i]->verts[2]->z);
		double dia_length = length(d1 - d2);

		double temp_s1 = (edge_length[0] + edge_length[1] + dia_length) / 2.0;
		double temp_s2 = (edge_length[2] + edge_length[3] + dia_length) / 2.0;
		qlist[i]->area = sqrt(temp_s1*(temp_s1 - edge_length[0])*(temp_s1 - edge_length[1])*(temp_s1 - dia_length)) +
			sqrt(temp_s2*(temp_s2 - edge_length[2])*(temp_s2 - edge_length[3])*(temp_s2 - dia_length));

		area += qlist[i]->area;
		temp_q = qlist[i];
		v1.set(vlist[qlist[i]->verts[0]->index]->x, vlist[qlist[i]->verts[0]->index]->y, vlist[qlist[i]->verts[0]->index]->z);
		v2.set(vlist[qlist[i]->verts[1]->index]->x, vlist[qlist[i]->verts[1]->index]->y, vlist[qlist[i]->verts[1]->index]->z);
		v0.set(vlist[qlist[i]->verts[2]->index]->x, vlist[qlist[i]->verts[2]->index]->y, vlist[qlist[i]->verts[2]->index]->z);
		qlist[i]->normal = cross(v0 - v1, v2 - v1);
		normalize(qlist[i]->normal);
	}

	double signedvolume = 0.0;
	icVector3 test = center;
	for (i = 0; i < nquads; i++) {
		icVector3 cent(vlist[qlist[i]->verts[0]->index]->x, vlist[qlist[i]->verts[0]->index]->y, vlist[qlist[i]->verts[0]->index]->z);
		signedvolume += dot(test - cent, qlist[i]->normal)*qlist[i]->area;
	}
	signedvolume /= area;
	if (signedvolume < 0)
		orientation = 0;
	else {
		orientation = 1;
		for (i = 0; i < nquads; i++)
			qlist[i]->normal *= -1.0;
	}
}

void Polyhedron::average_normals()
{
	int i, j;

	for (i = 0; i < nverts; i++) {
		vlist[i]->normal = icVector3(0.0);
		for (j = 0; j < vlist[i]->nquads; j++)
			vlist[i]->normal += vlist[i]->quads[j]->normal;
		normalize(vlist[i]->normal);
	}
}

void Polyhedron::maxmins(double &maxx,double &maxy,double &maxz,double &minx,double &miny,double &minz)
{
	minx = std::numeric_limits<double>::max();
	miny = minz = minx;
	maxx = maxy = maxz = -minx;

	#pragma omp parallel for default(none) reduction(max:maxx,maxy,maxz) reduction(min:minx,miny,minz) shared(vlist)
	for (int i = 0; i < nverts; ++i)
	{
		Vertex *v = vlist[i];
		if (v->x > maxx)
		{
			maxx = v->x;
		}
		else if (v->x < minx)
		{
			minx = v->x;
		}
		if (v->y > maxy)
		{
			maxy = v->y;
		}
		else if (v->y < miny)
		{
			miny = v->y;
		}
		if (v->z > maxz)
		{
			maxz = v->z;
		}
		else if (v->z < minz)
		{
			minz = v->z;
		}
	}
}

int Polyhedron::pos_to_index(double x, double y,\
	double maxx,double maxy,double minx,\
	double miny, int nx,int ny)
{
	// Calculate the width and height of each grid cell
    double gridWidth = (maxx - minx) / nx;
    double gridHeight = (maxy - miny) / ny;

    // Calculate the position's relative location within the grid
    double relX = x - minx;
    double relY = y - miny;

    // Calculate grid indices
    int i = relX / gridWidth;
    int j = relY / gridHeight;

    // Clamp the values to grid boundaries
    if (i >= nx) i = nx - 1;
    if (j >= ny) j = ny - 1;
    if (i < 0) i = 0;
    if (j < 0) j = 0;
 
	return ny*j + i;
}

void Polyhedron::index_to_pos(int gridnx,int gridny, double minx, \
	double miny, double gridw, double gridh,\
	double &x, double &y)
{
	x = minx+gridnx*gridw;
	y = miny+gridny*gridh;
}

Polyhedron* Polyhedron::ptcloud_to_quads(double dx,double dy)
{
	double maxx,maxy,maxz,minx,miny,minz;
	maxmins(maxx,maxy,maxz,minx,miny,minz);

	int nx,ny;

	nx = ((maxx-minx)/dx);
	ny = ((maxy-miny)/dy);

	// std::cerr<<maxx<<','<<maxy<<','<<maxz<<std::endl;
	// std::cerr<<minx<<','<<miny<<','<<minz<<std::endl;

	double* gridz = new double[nx*ny];
	icVector3* gridrgb = new icVector3[nx*ny];
	int* ngrid = new int[nx*ny];

	// std::vector<std::vector<double>> gridz(nx,std::vector<double>(ny));
	// std::vector<std::vector<icVector3>> gridrgb(nx,std::vector<icVector3>(ny));

	#pragma omp parallel for default(none) shared(gridz,gridrgb,ngrid)
	for (int i = 0; i < nx*ny; ++i)
	{
		gridz[i] = 0;
		gridrgb[i] = (0.0,0.0,0.0);
		ngrid[i] = 0;
	}


	for (int k = 0; k < nverts; k++)
	{
		int index = pos_to_index(vlist[k]->x,vlist[k]->y,maxx,maxy,minx,miny,nx,ny);
		gridz[index] += vlist[k]->z;
		gridrgb[index].x += vlist[k]->R;
		gridrgb[index].y +=	vlist[k]->G;
		gridrgb[index].z +=	vlist[k]->B;

		ngrid[index] ++;
	}

	double z_shift = (maxz+minz)/2;
	#pragma omp parallel for default(none) shared(gridz,gridrgb,ngrid)
	for (int i = 0; i < nx*ny; ++i)
	{
		double g = ngrid[i];
		if (g != 0)
		{
			gridz[i] /= g;
			// gridz[i] -= z_shift;
			gridrgb[i].x /= g;
			gridrgb[i].y /= g;
			gridrgb[i].z /= g;
		}
	}


	#pragma omp parallel for default(none) shared(gridz,gridrgb,ngrid)
	for (int i = 0; i < nx*ny; ++i)
	{
		if (gridz[i] == 0)
		{
			//If there is no points in this grid space just take average of some two 
			//adjacent vertices.

			//test if this spot is on an edge
			//if it is, take vertices on top and bottom
			//if not, take vertices to either side
			int m,n; //two grid indices for averaging
		
			if (i == 0)//check if i is bottom left corner
			{
				m = i+1;
				n = i+nx;
			}
			else if (i == nx-1) //check if i is bottom right corner
			{
				m = i-1;
				n = i+nx;
			}
			else if (i == (nx-1)*ny-1)//check if i is top left
			{
				m = i+1;
				n = i-nx;
			}
			else if (i == nx*ny-1) //check if i is top right corner
			{
				m = i-1;
				n = i-nx;
			}
			else if (i % (nx-1) == 0) //i is top or bottom fow
			{
				m = i+nx+1;
				n = i+nx-1;
			}
			else
			{
				m = i+1;
				n = i-1;
			}

			gridz[i] = (gridz[m]+gridz[n])/2;
			gridrgb[i].x = (gridrgb[m].x+gridrgb[n].x)/2;
			gridrgb[i].y = (gridrgb[m].y+gridrgb[n].y)/2;
			gridrgb[i].z = (gridrgb[m].z+gridrgb[n].z)/2;
			
		}
	}	



	//MAKE NEW pOLY or something
	int numverts,numedges,numquads;	
	numverts = nx*ny;
	numedges = 2*ny*nx-ny-nx;
	numquads = (ny-1)*(nx-1);
	Polyhedron* new_poly = new Polyhedron(numverts,numedges,numquads);

	//Put in vertices
	// #pragma omp parallel for default(none) shared(new_poly)

	for (int i = 0; i < numverts; ++i)
	{
		// if (gridz[i] > 0)
		// {
			double x,y;
			int gridy = std::floor(i/nx);
			int gridx = i%nx;
			index_to_pos(gridx,gridy, minx,miny,dx,dy,x,y);
			Vertex* v = new_poly->vlist[i];
			v->x = x;
			v->y = y;
			v->z = gridz[i];


			// std::cerr<<gridrgb[i].x<<','<<gridrgb[i].y<<','<<gridrgb[i].z<<std::endl;

			if (maxx < x || minx > x)
			{
				std::cerr<<"i: "<<i<<std::endl;
				std::cerr<<x<<std::endl;
				std::cerr<<y<<std::endl;
				std::cerr<<gridz[i]<<std::endl;
			}
			else if (maxy < y || miny > y)
			{				
				std::cerr<<"i: "<<i<<std::endl;
				std::cerr<<x<<std::endl;
				std::cerr<<y<<std::endl;
				std::cerr<<gridz[i]<<std::endl;
			}
			else if (maxz < gridz[i] || minz > gridz[i])
			{				
				std::cerr<<"i: "<<i<<std::endl;
				std::cerr<<x<<std::endl;
				std::cerr<<y<<std::endl;
				std::cerr<<gridz[i]<<std::endl;
			}

			// std::cerr<<x<<','<<y<<','<<gridz[i]<<std::endl;


			v->R = gridrgb[i].x;
			v->G = gridrgb[i].y;
			v->B = gridrgb[i].z;		
			v->scalar = gridz[i];
		// }
	}

	// std::cerr<<numquads<<std::endl;
	// std::cerr<<numverts<<std::endl;
	// std::cerr<<numquads-1+nx-1<<", "<<numquads-1+nx-1+1<<", "<<numquads-1+nx+nx-1<<", "<<numquads-1+nx+nx-1+1<<std::endl;
	//Put in quads
	// #pragma omp parallel for default(none) shared(new_poly,nx)
	for (int i = 0; i < numquads; ++i)
	{
		new_poly->qlist[i] = new Quad;

		int quady = std::floor(i/(nx-1));
		int quadx = i%(nx-1);

		// if (i < 20)
			// std::cerr<<i+quady<<", "<<i+quady+1<<", "<<i+nx+quady<<", "<<i+nx+quady+1<<std::endl;

		new_poly->qlist[i]->verts[0] = new_poly->vlist[i+quady];
		new_poly->qlist[i]->verts[1] = new_poly->vlist[i+1+quady];
		new_poly->qlist[i]->verts[2] = new_poly->vlist[i+nx+quady+1];
		new_poly->qlist[i]->verts[3] = new_poly->vlist[i+nx+quady];
	}

	new_poly->scrub_quads();
	new_poly->initialize(); // initialize the mesh
	new_poly->write_info();

	// for (int i = 0; i < )
	return new_poly;

}

// void index_to_pos(int gridnx,int gridny, double minx, \
// 	double miny, double gridw, double gridh)