#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <limits.h>
#include "boundingbox.h"
#include "edge.h"
// need edge to construct each triangle
// ===========================================================
class Triangle {
public:
  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Triangle() {
  edge = NULL; } //assgin NULL as begining 
  ~Triangle() {}
  // here's the hash function to use for triangles so they
  // can be efficiently accessed within the Bag data structure
  // tringle can access by sending tringle pointer 
  // by referance 3 integer
  static void extract_func(Triangle *t, int &a, int &b, int &c) {
    a = (*t)[0]->getIndex(); 
    b = (*t)[1]->getIndex(); 
    c = (*t)[2]->getIndex();
  }
  // =================
  // ACCESSORS
  ///=================
  Vertex* operator[](int i) const
  { 
    assert (edge != NULL); // make sure that edge equal null
    if (i==0) return edge->getVertex(); 
    // in specific postion 0 get the vertex
    if (i==1) return edge->getNext()->getNext()->getVertex();
    // in specific postion 1 get the next next edge vertex
    if (i==2) return edge->getNext()->getVertex();
    // in specific postion 2 get the next edge vertex
    assert(0);
  }
  Edge* getEdge() { 
    assert (edge != NULL); // get this specific tringle edge
    return edge; 
  }
  void setEdge(Edge *e) {
    assert (edge == NULL); // set edge for tringle
    edge = e;
  }

  // NOTE: If you want to modify a triangle, remove it from the mesh,
  // delete it, create a new copy with the changes, and re-add it.
  // This will ensure the edges get updated appropriately.

protected:
  // don't use this constructor
  Triangle& operator = (const Triangle &t) { assert(0); }
  
  // ==============
  // REPRESENTATION
  Edge *edge;

};
// ===========================================================
#endif
