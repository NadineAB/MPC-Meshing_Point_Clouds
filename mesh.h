#ifndef MESH_H
#define MESH_H
#include <QtGui>
#include <QString>
#include <vector>
#include "vectors.h"
#include "array.h"
#include "bag.h"
#include "boundingbox.h"
#include "argparser.h"
#include "math_3d.h"
// create mesh 
// we need vertex,Edge,Triangle and VertexParent
class Vertex;
class Edge;
class Triangle;
class VertexParent;

// ======================================================================
// ======================================================================
typedef std::vector< std::pair<Vec3f,Vec3f>> BoundaryEdge; // store the result of searching
typedef std::vector<BoundaryEdge> HolesEdges; // store the result of searching

class Mesh {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Mesh();
  virtual ~Mesh();
  void Load(const char *input_file);
  void LoadMesh(const QString input_file);
  void SaveObj();
  // =======================
  // BoundaryEdges and Holes
  // =======================
  BoundaryEdge BEdge;
  HolesEdges   HEdge;
  Bag<Edge*> *bedges;
  Array<Vertex*> *bvertices; // array of pointer to store the all vertices
  void addBoundaryVertex(const Vec3f &position);
  void FindBoundaryEdge();
  void FindHolesEdge();
  void HolesPaint();
  // ========
  // VERTICES
  // ========
  int bnumVertices() const { return bvertices->Count(); } // number of vertices
  int numVertices() const { return vertices->Count(); } // number of vertices
  Vertex* addVertex(const Vec3f &pos); // add vertex postion to the mesh
  // this creates a relationship between 3 vertices (2 parents, 1 child)
  void setParentsChild(Vertex *p1, Vertex *p2, Vertex *child);
  // this accessor will find a child vertex (if it exists) when given
  // two parent vertices
  Vertex* getChildVertex(Vertex *p1, Vertex *p2) const;
  // look up vertex by index from original .obj file
  Vertex* getVertex(int i) const 
  {
  assert (i >= 0 && i < numVertices()); // if the number of vertices
  // is less than zero and greater than number of vertices exist
  Vertex *v = (*vertices)[i];// define vertex by the index
  assert (v != NULL);
    return v;
  }
   // =====
   // EDGES
   // =====
  int numEdges() const { return edges->Count(); } // count the number of the edge
  // this efficiently looks for an edge with the given vertices, 
  // using a hash table
  Edge* getEdge(Vertex *a, Vertex *b) const;

  // =========
  // TRIANGLES
  // =========
  int numTriangles() const { return triangles->Count(); } // total number of tringles
  void addTriangle(Vertex *a, Vertex *b, Vertex *c); // add triangle by adding three vertices
  void removeTriangle(Triangle *t); // remove triangle by sending the exact triangle

  // ===============
  // OTHER ACCESSORS
  //
  BoundingBox* getBoundingBox() const { return bbox; }
  // calculate the min and max vertices in ordrer to compute
  // the bounding box
  // ===============
  // OTHER FUNCTIONS
  void Paint();
  void PaintBB();
  void BoundaryPaint();
  void Allocate(int nbTris, int nbVert );
  void CalculateVerNormals();// compute smooth normal
  void ComputeFaceNormal();// compute normal for face
  void DrawNormalFaces(); // draw faces noraml
  void DrawNormalVertices(); // draw vertices noraml
  double CalculateTriArea(Vector3f v1,Vector3f v2,Vector3f v3);
  void CalculateUVCoords();
  void CalculateTangentTri();
  void CalculateTangentVertices();
  void DrawTangentVertices();
private:
// ==============
  // REPRESENTATION
  std::vector<Vector3f> stdvertices; // a vector with coordinates (xpos, ypos, zpos) try to deal with static avoid pointer hun ^_^
  Array<Vertex*> *vertices; // array of pointer to store the all vertices
  Vector3f *TriNorms; // pointer of array where we can store normal for each face
  Vector3f *VerNorms; // pointer of array where we can store normal for each vertex
  Vector3f *VerTangs; // pointer of array where we can store tangent for each vertex
  Vector3f *TriTangs; // pointer of array where we can store tangent for each face
  Vector3f *TribiTangs; // pointer of array where we can store bitangent for each face
  Vector3f *VerbiTangs; // pointer of array where we can store bitangent for each vertex 
  std::vector< std::pair<int,Vector3f> > trianglevec;
  Bag<Edge*> *edges;
  Bag<Triangle*> *triangles;
  BoundingBox *bbox;
  Bag<VertexParent*> *vertex_parents;
  std::string basePath; // path and model name to load
  std::vector <unsigned int> indices; // define indices of the faces
  std::vector<Vector2f> UVCoords; // a vector with UV coordinates (xpos, ypos, zpos, wpos)
  };
// ======================================================================
// ======================================================================

#endif




