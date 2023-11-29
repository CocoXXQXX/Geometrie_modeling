#include "myVertex.h"
#include "myvector3d.h"
#include "myHalfedge.h"
#include "myFace.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <iostream>

myVertex::myVertex(void)
{
	point = NULL;
	originof = NULL;
	normal = new myVector3D(1.0,1.0,1.0);
} 

myVertex::~myVertex(void)
{
	if (normal) delete normal;
}

void myVertex::computeNormal()
{
	///**** TODO ****/
    myVector3D SommeNormal(0.0, 0.0, 0.0);
    int nbFace = 0;

    myHalfedge* startEdge = this->originof;
    myHalfedge* currentEdge = startEdge;

    do {
        if (currentEdge->adjacent_face != nullptr && currentEdge->adjacent_face->normal) {
            SommeNormal += * currentEdge->adjacent_face->normal;
            nbFace++;
        }
        currentEdge = currentEdge->twin->next; // Passe à l'arête suivante autour du sommet
    } while (currentEdge != startEdge && currentEdge != nullptr);

    if (nbFace > 0) {
        SommeNormal = SommeNormal / static_cast<float>(nbFace); // Calcule la moyenne
    }
	SommeNormal.normalize(); // Normalisation
	*this->normal = SommeNormal; // Affectation de la normale calculée
}

void myVertex::check() {
	if (this == nullptr) {
		throw std::runtime_error("Invalide vertex: vertex is nullptr");
	}
	if (this->point == nullptr) {
		throw std::runtime_error("Invalide vertex: vextex point is nullptr");
	}
	if (this->originof == nullptr) {
		throw std::runtime_error("Invalide vertex: vextex original haldedges is nullptr");
	}
}
