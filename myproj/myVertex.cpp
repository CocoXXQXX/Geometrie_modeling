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
	/**** TODO ****/
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
