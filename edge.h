#ifndef EDGE_H
#define EDGE_H

#include <limits.h>
#include <stdio.h>
#include <assert.h>
class Vertex; // we need to Vertex need 
class Triangle; // we need to Triangle need 
// ===================================================================
// half-edge data structure
class Edge { 
public:
  // ========================
  // CONSTRUCTORS & DESTRUCTOR
  Edge(Vertex *v, Triangle *t); // CONSTRUCTORS
  ~Edge(); //DESTRUCTOR

  // here's the hash function to use for edges so they
  // can be efficiently accessed within the Bag data structure
  static void extract_func(Edge *e, int &a, int &b, int &c); 
  // extract function that take as input edge and 3 integer which 
  // they are vertices index 
  // =========
  // ACCESSORS
  Vertex* getVertex() const { assert (vertex != NULL); return vertex; } 
  // get vertex for specific edge and exist if it equal NULL
  Edge* getNext() const { assert (next != NULL); return next; }
  // get the next edge to adjacency edge 
  Triangle* getTriangle() const { assert (triangle != NULL); return triangle; }
  // get the triangle that this belong to 
  Edge* getOpposite() const {
  // warning!  the opposite edge might be NULL! 
   return opposite; }
  float getCrease() const { return crease; }
  // Crease its float value { I dont knew indicate to what }
  Vertex* operator[](int i) const { // interst into 0 or the vertex that
   // belong to exact edge if 1 return the vertex that belong to the
   // next edge
    if (i==0) return getVertex();
    if (i==1) return getNext()->getNext()->getVertex();
    assert(0);
  }
  // =========
  // MODIFIERS
  // set values in opposite edge [ this is edge ]
  // so we need to check that its not equal to Null 
  // then assgin the value
  void setOpposite(Edge *e) {
    assert (opposite == NULL); // exit if NULL
    assert (e != NULL); // make sure that edge not equal NULL
    assert (e->opposite == NULL); // exit if 
    opposite = e; 
    e->opposite = this; 
  }
  // clear the value assgin to opposite edge 
  // by puting equal null
  void clearOpposite() { 
    if (opposite == NULL) return; 
    assert (opposite->opposite == this); 
    opposite->opposite = NULL;
    opposite = NULL; 
  }
  // set next edge 
  // insure value not equal to NULL
  void setNext(Edge *e) {
    assert (next == NULL);
    assert (e != NULL);
    assert (triangle == e->triangle);
    next = e;
  }
  void setCrease(float c) { crease = c; } // set crease values

private:

  Edge(const Edge&) { assert(0); } // empty constructor 
  Edge& operator=(const Edge&) { assert(0); } //  asgin 

  // ==============
  // REPRESENTATION
  // in the half edge data adjacency data structure, the edge stores everything!
  Vertex *vertex; // vertex pointer
  Triangle *triangle; // trinagle 
  Edge *opposite; // opposite edge 
  Edge *next; // next edge
  float crease;
};

// ===================================================================

#endif
