#include "myFace.h"
#include "myvector3d.h"
#include "myHalfedge.h"
#include "myVertex.h"
#include <GL/glew.h>
#include <iostream>

myFace::myFace(void)
{
	adjacent_halfedge = NULL;
	normal = new myVector3D(1.0, 1.0, 1.0);
}

myFace::~myFace(void)
{
	if (normal) delete normal;
}

void myFace::computeNormal()
{

    //std::cout << "normal face : " << normal->dX << std::endl;
    //std::cout << "normal face : " << normal->dX << std::endl;
/**** TODO ****/
     // Obtenez les trois sommets de la face (pour un maillage triangulaire)
    if (!adjacent_halfedge) return;
    myVertex* v1 = adjacent_halfedge->source;
    myVertex* v2 = adjacent_halfedge->next->source;
    myVertex* v3 = adjacent_halfedge->prev->source;

    //////// Calculez deux vecteurs sur la face
    myVector3D vec1 = *(v2->point) - *(v1->point);
    myVector3D vec2 = *(v3->point) - *(v1->point);

    ////////std::cout << vec1.dX;

    //////// Produit vectoriel de vec1 et vec2
    myVector3D normalResult = vec1.crossproduct(vec2);

    //////// Normalisation
    normalResult.normalize();

    //////// Mettez à jour la normale de la face
    *(this->normal) = normalResult;

}
