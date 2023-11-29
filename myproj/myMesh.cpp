#include "myMesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <GL/glew.h>
#include "myvector3d.h"
#include <vector>
#include <algorithm>
using namespace std;


myMesh::myMesh(void)
{
	/**** TODO ****/
}


myMesh::~myMesh(void)
{
	/**** TODO ****/
}

void myMesh::clear()
{
	for (unsigned int i = 0; i < vertices.size(); i++) if (vertices[i]) delete vertices[i];
	for (unsigned int i = 0; i < halfedges.size(); i++) if (halfedges[i]) delete halfedges[i];
	for (unsigned int i = 0; i < faces.size(); i++) if (faces[i]) delete faces[i];

	vector<myVertex*> empty_vertices;    vertices.swap(empty_vertices);
	vector<myHalfedge*> empty_halfedges; halfedges.swap(empty_halfedges);
	vector<myFace*> empty_faces;         faces.swap(empty_faces);
}

void myMesh::testVertexHalfedge(const std::vector<myVertex*> verts,const std::vector<myHalfedge*> halfs, const std::vector<myFace*> fs) {
	for (myVertex* v : verts) {
		v->check();
		//<myHalfedge*>::iterator it;
		auto itH = std::find_if(halfs.begin(), halfs.end(), [v](const myHalfedge* h) 
		{
				return v->originof == h;
		});
		if (itH == halfs.end()) {
			throw std::runtime_error("ERROR! Can't find orignal halfedge of Vertex in table of haldEdege\n");
		}
	}

	for (myHalfedge* h : halfs) {
		h->check();
		auto itV = std::find_if(verts.begin(), verts.end(), [h](const myVertex* ve)
		{
			return h->source == ve;
		});
		if (itV == verts.end()) {
			throw std::runtime_error("ERROR! Can't find souce of halfedge in table of vertex\n");
		}
		if (h->prev->twin != h->twin->next) {
			throw std::runtime_error("ERROR! h->prev->twin != h->twin->next\n");
		}
		/*if (h->next->twin != h->twin->prev) {
			throw std::runtime_error("ERROR! h->next->twin != h->twin->prev\n");
		}

		auto itF = std::find_if(fs.begin(), fs.end(), [h](const myFace* fa)
			{
				return h->adjacent_face == fa;
			});
		if (itF == fs.end()) {
			throw std::runtime_error("ERROR! Can't find adjacent face of halfedge in table of faces\n");
		}*/
	}
}


void myMesh::checkMesh()
{
	vector<myHalfedge*>::iterator it;
	for (it = halfedges.begin(); it != halfedges.end(); it++)
	{
		if ((*it)->twin == NULL)
			break;
	}
	if (it != halfedges.end())
		cout << "Error! Not all edges have their twins!\n";
	else cout << "Each edge has a twin!\n";

}


bool myMesh::readFile(std::string filename)
{
	string s, t, u;
	vector<int> faceids;
	myHalfedge** hedges;

	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Unable to open file!\n";
		return false;
	}
	name = filename;

	map<pair<int, int>, myHalfedge*> twin_map;
	map<pair<int, int>, myHalfedge*>::iterator it;
	//vector<myVertex*> vertexTab;
	vector<int> nbVerInFaceTab;
	int nbVerInFace = 0;

	while (getline(fin, s))
	{
		stringstream myline(s);
		myline >> t;
		if (t == "g") {}
		else if (t == "v")
		{
			float x, y, z;
			myline >> x >> y >> z;
			cout << "v " << x << " " << y << " " << z << endl;
			myVertex* vertex = new myVertex();
			vertex->point = new myPoint3D(x, y, z);
			vertices.push_back(vertex);

		}
		else if (t == "mtllib") {}
		else if (t == "usemtl") {}
		else if (t == "s") {}
		else if (t == "f")
		{
			cout << "f";
			cout << endl;
			myFace* f = new myFace(); // allocate the new face
			faceids.clear();

			while (myline >> u) {
				
				int faceID = atoi((u.substr(0, u.find("/"))).c_str()) -1;
			
				faceids.push_back(faceID);
					
				cout << " " << faceID << endl;
				
			}	
			if (faceids.size() < 3) {
				continue;
				// ignore degenerate faces
			}
			
			hedges = new myHalfedge * [faceids.size()]; // allocate the array for storing pointers to half-edges
			for (unsigned int i = 0; i < faceids.size(); i++) {
				hedges[i] = new myHalfedge(); // pre-allocate new half-edges
				hedges[i]->source = vertices[faceids[i]];
				
			}
			
			f->adjacent_halfedge = hedges[0]; // connect the face with incident edge

			for (unsigned int i = 0; i < faceids.size(); i++)
			{
				//next
				int iplusone = (i + 1) % faceids.size();
				//previous
				int iminusone = (i - 1 + faceids.size()) % faceids.size();

				// connect prevs, and next

				hedges[i]->next = hedges[iplusone];
				hedges[i]->prev = hedges[iminusone];
				hedges[i]->adjacent_face = f;
				hedges[i]->index = i;

				it = twin_map.find(make_pair(faceids[iplusone], faceids[i]));
				if (it == twin_map.end()) {
					twin_map[make_pair(faceids[i], faceids[iplusone])] = hedges[i];
				}
				else
				{
					hedges[i]->twin = it->second;
					it->second->twin = hedges[i];
				}
				// set originof
				
				vertices[faceids[i]]->originof = hedges[i];

				// push edges to halfedges in myMesh
				halfedges.push_back(hedges[i]);
			
			}
			// push face to faces in myMesh
			faces.push_back(f);
		}
			//nbVerInFaceTab.push_back(nbVerInFace);
			//cout << "nb: " << nbVerInFace << endl;
			//nbVerInFace = 0;
		}
	checkMesh();
	normalize();

	return true;
}

void myMesh::computeNormals()
{
	/**** TODO ****/
	for(int i=0;i<faces.size();i++)
	{
		faces[i]->computeNormal();
	}

	for (int i = 0;i < vertices.size();i++)
	{
		vertices[i]->computeNormal();
	}

}

void myMesh::normalize()
{
	if (vertices.size() < 1) return;

	int tmpxmin = 0, tmpymin = 0, tmpzmin = 0, tmpxmax = 0, tmpymax = 0, tmpzmax = 0;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (vertices[i]->point->X < vertices[tmpxmin]->point->X) tmpxmin = i;
		if (vertices[i]->point->X > vertices[tmpxmax]->point->X) tmpxmax = i;

		if (vertices[i]->point->Y < vertices[tmpymin]->point->Y) tmpymin = i;
		if (vertices[i]->point->Y > vertices[tmpymax]->point->Y) tmpymax = i;

		if (vertices[i]->point->Z < vertices[tmpzmin]->point->Z) tmpzmin = i;
		if (vertices[i]->point->Z > vertices[tmpzmax]->point->Z) tmpzmax = i;
	}

	double xmin = vertices[tmpxmin]->point->X, xmax = vertices[tmpxmax]->point->X,
		ymin = vertices[tmpymin]->point->Y, ymax = vertices[tmpymax]->point->Y,
		zmin = vertices[tmpzmin]->point->Z, zmax = vertices[tmpzmax]->point->Z;

	double scale = (xmax - xmin) > (ymax - ymin) ? (xmax - xmin) : (ymax - ymin);
	scale = scale > (zmax - zmin) ? scale : (zmax - zmin);

	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i]->point->X -= (xmax + xmin) / 2;
		vertices[i]->point->Y -= (ymax + ymin) / 2;
		vertices[i]->point->Z -= (zmax + zmin) / 2;

		vertices[i]->point->X /= scale;
		vertices[i]->point->Y /= scale;
		vertices[i]->point->Z /= scale;
	}
}

bool checkTriangulate(myFace* f) {
	myHalfedge* startEdge = f->adjacent_halfedge;
	myHalfedge* currentEdge = startEdge;
	int vertexCount = 0;

	// Count the number of vertices in the face.
	do {
		vertexCount++;
		currentEdge = currentEdge->next;
	} while (currentEdge != startEdge);

	if (vertexCount == 3) {
		return true;
	}
	return false;
}

myPoint3D* bestPosition(myPoint3D *p1, myPoint3D *p2) {
	//first thought, find the middle of v1 and v2
	//cout << "passe in best Position";
	return new myPoint3D((p1->X + p2->X) / 2.0, (p1->Y + p2->Y) / 2.0, (p1->Z + p2->Z) / 2.0);
}

double distance(myPoint3D* p1, myPoint3D* p2) {
	//first thought, find the middle of v1 and v2
	//cout << "passe distance";
	return  sqrt(pow(p2->X - p1->X, 2) + pow(p2->Y - p1->Y, 2) + pow(p2->Z - p1->Z, 2));
}



void myMesh::collapse(myHalfedge *e) {
	myVertex* newVertex = new myVertex();
	myVertex* v1 = new myVertex();
	myVertex* v2 = new myVertex();
	v1 = e->source;
	v2 = e->next->source;
	newVertex->point = bestPosition(v1->point, v2->point);
	vertices.push_back(newVertex);
	vector<myFace*> deleteFaces;
	vector<myHalfedge*>deleteHalfedge;

	for each(myHalfedge* he in halfedges)
	{
		//Find the two faces connecte with halfedge;
		if (he == e || he==e->twin) {
			if (checkTriangulate(he->adjacent_face)) {
				he->next->twin->twin = he->prev->twin;
				deleteFaces.push_back(he->adjacent_face);
				deleteHalfedge.push_back(he->next);
				deleteHalfedge.push_back(he->prev);
				deleteHalfedge.push_back(he);
			}
			else{
				he->next->prev = he->prev;
				he->prev->next = he->next;
				deleteHalfedge.push_back(he);
			}
		}
		if ( he->source == v1 || he->source == v2) {
			he->source = newVertex;
		}
	}

	for each (myFace * f in faces) {
		if (f->adjacent_halfedge == e) {
			f->adjacent_halfedge = e->next;
		}
	}

	//erase faces, halfedges, vertex
	vertices.erase(remove(vertices.begin(), vertices.end(), v1));
	vertices.erase(remove(vertices.begin(), vertices.end(), v2));

	for (int m = 0;m < deleteHalfedge.size();m++) {
		halfedges.erase(remove(halfedges.begin(), halfedges.end(), deleteHalfedge[m]));
	}
	if (deleteFaces.size() != 0) {
		for (int j = 0;j < deleteFaces.size();j++) {
			faces.erase(remove(faces.begin(), faces.end(), deleteFaces[j]));
		}
	}
	
}

myHalfedge* myMesh::findMinimalHalfedge() {
	//return halfedges.back();
	double min;
	myHalfedge* minEdges;
	min = distance(halfedges[0]->source->point, halfedges[0]->next->source->point);
	minEdges = halfedges[0];
	for each (myHalfedge * he in halfedges)
	{
		if (min > distance(he->source->point, he->next->source->point)) {
			min = distance(he->source->point, he->next->source->point);
			minEdges = he;
		}
		else {
			min = min;
			minEdges = minEdges;
		}
	}
	return minEdges;
}

double myMesh::findMinimalDistance() {
	double min;
	min = distance(halfedges[0]->source, halfedges[0]->next->source);
	for each (myHalfedge * he in halfedges)
	{	
		if (min > distance(he->source->point, he->next->source->point)) {
			min = distance(he->source->point, he->next->source->point);
		}
		else {
			min = min;
		}
	}
	return min;
}

void myMesh::allCollapse(double d) {
		for each (myHalfedge * he in halfedges)
		{
			if (distance(he->source->point, he->next->source->point) <= d) {
				collapse(he);
			}
		}
	
}

void myMesh::splitFaceTRIS(myFace *f, myPoint3D *p)
{
	/**** TODO ****/
}

void myMesh::splitEdge(myHalfedge *e1, myPoint3D *p)
{

	/**** TODO ****/
}

void myMesh::splitFaceQUADS(myFace *f, myPoint3D *p)
{
	/**** TODO ****/
}


void myMesh::subdivisionCatmullClark()
{
	/**** TODO ****/
}


void myMesh::triangulate()
{
	std::vector<myFace*>oldFaces;
	/**** TODO ****/
	for (int i = 0;i < faces.size();i++) {
		if (triangulate(faces[i])) {

			oldFaces.push_back(faces[i]);
		}
	}
	for (int j = 0;j < oldFaces.size();j++) {
		faces.erase(remove(faces.begin(), faces.end(), oldFaces[j]));
	}
	
}

//return false if already triangle, true othewise.
bool  myMesh::triangulate(myFace* f)
{
	/**** TODO ****/

	myHalfedge* startEdge = f->adjacent_halfedge;
	myHalfedge* currentEdge = startEdge;
	myPoint3D* center = new myPoint3D(0, 0, 0);
	vector<myVertex*> verticesFace;
	int vertexCount = 0;
	map<pair<myVertex*, myVertex*>,myHalfedge*> twinTracker;

	// Count the number of vertices in the face.
	do {
		vertexCount++;
		verticesFace.push_back(currentEdge->source);
		currentEdge = currentEdge->next;
		center = new myPoint3D(currentEdge->source->point->X + center->X, currentEdge->source->point->Y + center->Y, currentEdge->source->point->Z + center->Z);
	} while (currentEdge != startEdge);
	center = new myPoint3D(center->X / static_cast<double>(vertexCount), center->Y / static_cast<double>(vertexCount), center->Z / static_cast<double>(vertexCount));

	if (vertexCount == 3) {
		return false;  // Face is already a triangle, no need to triangulate.
	}
	if (vertexCount < 3) {
		// Handle degenerate cases here if needed.
		// For example, you might want to remove this face or handle it differently.
		return false;
	}
	myVertex* centerVextex = new myVertex();
	centerVextex->point = center;

	for (int i = 0;i < vertexCount;i++) {
		myFace* newFaceTrangulate = new myFace();
		myHalfedge** hedges;
		hedges = new myHalfedge * [3];
		// Allocate memory for the hedges
		for (int k = 0; k < 3; k++) {
			hedges[k] = new myHalfedge();
		}
		hedges[0]->source = centerVextex;
		
		hedges[0]->adjacent_face = newFaceTrangulate;

		//first face but 2 first points/vertex.
		hedges[1]->source = verticesFace[i];
		hedges[1]->adjacent_face = newFaceTrangulate;
		//next vertex
		hedges[2]->source = verticesFace[(i + 1) % vertexCount];
		hedges[2]->adjacent_face = newFaceTrangulate;

		newFaceTrangulate->adjacent_halfedge = hedges[0];

		for (int k = 0; k < 3; k++) {
			hedges[k]->next = hedges[(k + 1) % 3];
			hedges[k]->prev = hedges[(k + 2) % 3];	
		}

		//for (int k = 0; k < 3; k++) {
		//	auto twinKey = make_pair(hedges[k]->source, hedges[k]->prev->source);
		//	if (twinTracker.find(twinKey) != twinTracker.end()) {
		//		// Le jumeau existe déjà
		//		hedges[k]->twin = twinTracker[twinKey];
		//		twinTracker[twinKey]->twin = hedges[k];
		//	}
		//	else {
		//		// Le jumeau n'existe pas encore, donc stockez ce halfedge
		//		twinTracker[make_pair(hedges[k]->prev->source, hedges[k]->source)] = hedges[k];
		//	}
		//}

		centerVextex->originof = hedges[1];
		vertices.push_back(centerVextex);
		halfedges.push_back(hedges[1]);
		halfedges.push_back(hedges[2]);
		faces.push_back(newFaceTrangulate);
	}

	return true;  // Return true to indicate that the face was successfully triangulated.

}

