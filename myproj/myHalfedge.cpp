#include "myHalfedge.h"
#include <stdexcept>

myHalfedge::myHalfedge(void)
{
	source = NULL; 
	adjacent_face = NULL; 
	next = NULL;  
	prev = NULL;  
	twin = NULL;  
}

void myHalfedge::copy(myHalfedge *ie)
{
/**** TODO ****/
}

myHalfedge::~myHalfedge(void)
{
}

void myHalfedge::check() {
	if (this == nullptr) {
		throw std::runtime_error("Invalide Halfedge: Halfedge is nullptr");
	}
	if (this->source == nullptr) {
		throw std::runtime_error("Invalide Halfedge: Halfedge source is nullptr");
	}
	if (this->adjacent_face == nullptr) {
		throw std::runtime_error("Invalide Halfedge: Haldedge adjacent face is nullptr");
	}
	if (this->next == nullptr) {
		throw std::runtime_error("Invalide Halfedge: Haldedge next is nullptr");
	}
	if (this->prev == nullptr) {
		throw std::runtime_error("Invalide Halfedge: Haldedge prev is nullptr");
	}
	if (this->twin == nullptr) {
		throw std::runtime_error("Invalide Halfedge: Haldedge twin is nullptr");
	}
	if (this->twin->twin != this) {
		throw std::runtime_error("twin twin is not original haldedges");
	}
	
	
}
