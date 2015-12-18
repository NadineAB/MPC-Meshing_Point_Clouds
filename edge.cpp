#ifndef _EDGE_H_
#define _EDGE_H_

#include "vertex.h"
#include "edge.h"
// constructor by sending the vertices and tringles 
Edge::Edge(Vertex *v, Triangle *t) {
  vertex = v;
  triangle = t;
  next = NULL;
  opposite = NULL;
  crease = 0;
}
// just opposite could be NULL
Edge::~Edge() { 
  if (opposite != NULL)
    opposite->opposite = NULL;
}
// extract vertices belong to specific edge 
// and edge have two vertices 
void Edge::extract_func(Edge *e, int &a, int &b, int &c) {
  a = e->getVertex()->getIndex(); 
  b = e->getNext()->getNext()->getVertex()->getIndex();
  c = 0;
}
#endif
