#include <stdio.h>
#include <assert.h>
//#include <GL/gl.h>
#include <GL/glut.h>
#include "mesh.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "vertex_parent.h"
#include "glCanvas.h"
#define INITIAL_VERTEX 10000
#define INITIAL_EDGE 10000
#define INITIAL_TRIANGLE 10000
// =======================================================================
// CONSTRUCTORS & DESTRUCTORS
// =======================================================================

Mesh::Mesh() { // create mesh by vertices, trinagle and edge 
 vertices = new Array<Vertex*>(INITIAL_VERTEX);
 bvertices= new Array<Vertex*>(INITIAL_VERTEX);
 edges = new Bag<Edge*>(INITIAL_EDGE,Edge::extract_func);
 bedges= new Bag<Edge*>(INITIAL_EDGE,Edge::extract_func);
 triangles = new Bag<Triangle*>(INITIAL_TRIANGLE,Triangle::extract_func);
 vertex_parents = new Bag<VertexParent*>(INITIAL_VERTEX, VertexParent::extract_func);
 bbox = NULL;
 TriNorms = NULL;
 VerNorms= NULL;
 VerTangs=NULL;
 TriTangs=NULL;
 TribiTangs=NULL;
 VerbiTangs=NULL;
}
//DESTRUCTORS
Mesh::~Mesh() {
  delete vertices;  
  delete bvertices;
  vertices = NULL;
  delete edges;
  edges = NULL;
  delete bedges;
  bedges = NULL;
  delete triangles;
  triangles = NULL;
  delete bbox;
  bbox = NULL;
  delete[] TriNorms;
  delete[] VerNorms;
  delete[] VerTangs;
  delete[] TriTangs;
  delete[] VerbiTangs;
  delete[] TribiTangs;
// delete all dynamic veriable allocation
}
void Mesh::Allocate(int nbTris, int nbVert )
{
	//cout<< " nbTris " << nbTris;
	TriNorms = new Vector3f[ nbTris + 1 ];
	VerNorms = new Vector3f[ nbVert + 1 ];
	VerTangs= new Vector3f[ nbVert + 1 ];
	TriTangs= new Vector3f[ nbTris + 1 ];
	TribiTangs=new Vector3f[ nbTris + 1 ];
	VerbiTangs=new Vector3f[ nbVert + 1 ];
}
// =======================================================================
// MODIFIERS:   ADD & REMOVE
// =======================================================================
void Mesh::addBoundaryVertex(const Vec3f &position)
{
  int index = bnumVertices();
  Vertex *v = new Vertex(index, position);
  bvertices->Add(v);
}
Vertex* Mesh::addVertex(const Vec3f &position) {
  int index = numVertices();
  Vertex *v = new Vertex(index, position);
  vertices->Add(v);
  if (bbox == NULL) 
    bbox = new BoundingBox(position,position);
  // becouse we have just one vertex
  else 
    bbox->Extend(position);
  // if its not the first one 
  return v;
}
void Mesh::addTriangle(Vertex *a, Vertex *b, Vertex *c)
{
 // set the vertices indeces
  // create the triangle
  Triangle *t = new Triangle(); // have three vertices

  // create the edges
  Edge *ea = new Edge(a,t); // create edge between the vertices
  Edge *eb = new Edge(b,t);
  Edge *ec = new Edge(c,t);

  // point the triangle to one of its edges
  t->setEdge(ea);

  // connect the edges to each other
  ea->setNext(ec);
  eb->setNext(ea);
  ec->setNext(eb);

  // add them to the master list
  edges->Add(ea);
  edges->Add(eb);
  edges->Add(ec);

  // connect up with opposite edges (if they exist)
  Edge *ea_op = getEdge((*ea)[1],(*ea)[0]);
  Edge *eb_op = getEdge((*eb)[1],(*eb)[0]);
  Edge *ec_op = getEdge((*ec)[1],(*ec)[0]);  
  if (ea_op != NULL) { ea_op->setOpposite(ea); }
  if (eb_op != NULL) { eb_op->setOpposite(eb); }
  if (ec_op != NULL) { ec_op->setOpposite(ec); }

  // add the triangle to the master list
  triangles->Add(t); 
}
void Mesh::removeTriangle(Triangle *t) {

  Edge *ea = t->getEdge();
  Edge *eb = ea->getNext();
  Edge *ec = eb->getNext();
  assert (ec->getNext() == ea); // back to the same edge
  // remove elements from master lists
  edges->Remove(ea);
  edges->Remove(eb);
  edges->Remove(ec);
  triangles->Remove(t);
// clean up memory
  delete ea;
  delete eb;
  delete ec;
  delete t;
}
// get index vertices of the edge
Edge* Mesh::getEdge(Vertex *a, Vertex *b) const {
  assert (edges != NULL);
  return edges->Get(a->getIndex(),b->getIndex());
}
// sent two parent vertices to know the child vertex
Vertex* Mesh::getChildVertex(Vertex *p1, Vertex *p2) const {
  VertexParent *vp = vertex_parents->GetReorder(p1->getIndex(), p2->getIndex());
  if (vp == NULL) return NULL;
  return vp->get();
}
// set parent for specific child
void Mesh::setParentsChild(Vertex *p1, Vertex *p2, Vertex *child) {
  vertex_parents->Add(new VertexParent(p1,p2,child));
}
// =======================================================================
// the load function parses very simple .obj files
// the basic format has been extended to allow the specification 
// of crease weights on the edges.
// compute vertices normal, face normal, tangent and bi tangent 
// =======================================================================
/////////////////////// Reading and anlayzing Scan  //////////////////////
void Mesh::Load(const char *input_file) 
{
  //char file_name[80];
  cout << input_file;
  basePath = "Models/";   //<--- Base path to where the models are included.
  std::string file_name=basePath.append(string(input_file));
  const char *sptr = file_name.c_str() ;
  input_file="ss.obj";
  cout<<sptr;
  FILE *objfile = fopen(sptr,"r");
  if (objfile == NULL) {
    printf ("ERROR! CANNOT OPEN '%s'\n",input_file);
    return;
  }
  char line[200];
  char token[100];
  char atoken[100];
  char btoken[100];
  char ctoken[100];
  char dtoken[100];
  char etoken[100];
  float x,y,z;
  int a,b,c,d,e;
  int index = 0;
  int vert_count = 0;
  int vert_index = 1;
  
  while (fgets(line, 200, objfile)) {   
    
    if (line[strlen(line)-2] == '\\') {
      fgets(token, 100, objfile);	
      int tmp = strlen(line)-2;
      strncpy(&line[tmp],token,100);
    }
    int token_count = sscanf (line, "%s\n",token);
    if (token_count == -1) continue;
    a = b = c = d = e = -1;
    if (!strcmp(token,"usemtl") ||
	!strcmp(token,"g")) {
      vert_index = 1; //vert_count + 1;
      index++;
    } else if (!strcmp(token,"v")) 
    {
      vert_count++;
      sscanf (line, "%s %f %f %f\n",token,&x,&y,&z);
	//cout <<" The token " << token;
	 stdvertices.push_back(Vector3f(x,y,z));
      addVertex(Vec3f(x,y,z));
    } else if (!strcmp(token,"f")) {
      int num = sscanf (line, "%s %s %s %s %s %s\n",token,
			atoken,btoken,ctoken,dtoken,etoken);
      sscanf (atoken,"%d",&a);
      sscanf (btoken,"%d",&b);
      sscanf (ctoken,"%d",&c);
	 indices.push_back(unsigned int(a));
      indices.push_back(unsigned int(b));
      indices.push_back(unsigned int(c));
	 //cout << a << b << c << endl;
	 if (num > 4) sscanf (dtoken,"%d",&d);
      if (num > 5) sscanf (etoken,"%d",&e);
	 a -= vert_index;
      b -= vert_index;
      c -= vert_index;
	 //cout << a << b << c << endl;
      if (d >= 0) d -= vert_index;
      if (e >= 0) e -= vert_index;
      assert (a >= 0 && a < numVertices());
      assert (b >= 0 && b < numVertices());
      assert (c >= 0 && c < numVertices());
      addTriangle(getVertex(a),getVertex(b),getVertex(c));
	 if (d > -1) { assert (d < numVertices()); addTriangle(getVertex(a),getVertex(c),getVertex(d)); }
      if (e > -1) { assert (e < numVertices()); addTriangle(getVertex(a),getVertex(d),getVertex(e)); }
    } else if (!strcmp(token,"e")) {
      int num = sscanf (line, "%s %s %s %s\n",token,atoken,btoken,ctoken);
      assert (num == 4);
      sscanf (atoken,"%d",&a);
      sscanf (btoken,"%d",&b);
      if (!strcmp(ctoken,"inf")) 
	 x = 1000000; // this is close to infinity...
      else sscanf (ctoken,"%f",&x);
      Vertex *va = getVertex(a);
      Vertex *vb = getVertex(b);
      Edge *ab = getEdge(va,vb);
      Edge *ba = getEdge(vb,va);
      assert (ab != NULL);
      assert (ba != NULL);
      ab->setCrease(x);
      ba->setCrease(x);
    } else if (!strcmp(token,"vt")) {
    } else if (!strcmp(token,"vn")) {
    } else if (token[0] == '#') {
    } else {
      printf ("LINE: '%s'",line);
    }
  }
  cout<< numVertices()<< endl;
  cout<< numTriangles();
  Allocate( numTriangles(), numVertices() );
 // ComputeFaceNormal();
  //CalculateUVCoords();
  //CalculateTangentTri();
  FindBoundaryEdge();
  SaveObj();
}
// =======================================================================
// Working Normal
// =======================================================================
void Mesh::ComputeFaceNormal()
{
for ( int i = 0; i < numTriangles(); i++ )  // for loop the number of triangles 
{
TriNorms[i].x=0;
TriNorms[i].y=0;
TriNorms[i].z=0;
}
int i=0;
Iterator<Triangle*> *iter = triangles->StartIteration();
  while (Triangle *t = iter->GetNext()) 
  { Vec3f normal;
    Vec3f P0 = (*t)[0]->get();
    Vec3f P1 = (*t)[1]->get();
    Vec3f P2 = (*t)[2]->get();
    Vec3f::Cross3(normal,P1.operator-=(P0),(P2.operator-=(P0)));
    TriNorms[ i ].x=normal.x();
    TriNorms[ i ].y=normal.y();
    TriNorms[ i ].z=normal.z();
    TriNorms[ i ]=TriNorms[ i ].Normalize();  // normalize base of Magnitude
    i++;
}
triangles->EndIteration(iter);
CalculateVerNormals();
}
void Mesh::CalculateVerNormals()
{
int  NumAdjFace=0;
GLfloat angle=90;
GLfloat dot, cos_angle;
// calculate the cosine of the angle (in degrees) */
cos_angle = cos(angle * M_PI / 180.0); ////// weight based on angle
//cout << numTriangles();
for(int i = 0; i < numVertices(); i++)
{
	VerNorms[i].x=0;
	VerNorms[i].y=0;
	VerNorms[i].z=0;
	//cout<< VerNorms[i].x << VerNorms[i].y << VerNorms[i].z<< endl;
}
/* for every triangle, create a node for each vertex in it */
/* for every triangle, create a node for each vertex in it */
int nTri=0;
Iterator<Triangle*> *iter = triangles->StartIteration();
while (Triangle *t = iter->GetNext()) {
    Vec3f a = (*t)[0]->get();
    Vec3f b = (*t)[1]->get();
    Vec3f c = (*t)[2]->get();
    Vector3f vec1; 
     vec1.x=a.x();
	vec1.y=a.y();
	vec1.z=a.z();
	Vector3f vec2;
	vec2.x=b.x();
	vec2.y=b.y();
	vec2.z=b.z();
	Vector3f vec3;
	vec3.x=c.x();
	vec3.y=c.y();
	vec3.z=c.z();
     trianglevec.push_back(std::make_pair(nTri,vec1));
	trianglevec.push_back(std::make_pair(nTri,vec2));
	trianglevec.push_back(std::make_pair(nTri,vec3));
	nTri++;
 }
  triangles->EndIteration(iter);
/* calculate the average normal for each vertex */
// but we should find first all the adjace faces
for(int i = 0; i < numVertices(); i++)
{
///* calculate an average normal for this vertex by averaging the
//facet normal of every triangle this vertex is in */
double sx=((*vertices).operator[](i)->x());
double sy=((*vertices).operator[](i)->y());
double sz=((*vertices).operator[](i)->z());
Vector3f node; 
node.x=sx;  // specific node or vertices to search
node.y=sy;  // specific node or vertices to search
node.z=sz;  // specific node or vertices to search
int tr;
Vector3f facetNormal ;
// find frist adj tringle
for(int j = 0; j < trianglevec.size(); j++)
{
if ( trianglevec[j].second.x==node.x)
	if(trianglevec[j].second.y==node.y)
         if(trianglevec[j].second.z==node.z)
{
    tr = trianglevec[j].first; // get first triangle
    facetNormal = TriNorms[tr]; // get first normal
    VerNorms[i] = facetNormal; // set to first facet normal
break;
}
}
////////////////////// Weighted Vertex Normals ///////////////////////////
for(int j =1; j < trianglevec.size(); j++)
{
if ( trianglevec[j].second.x==node.x)
	if(trianglevec[j].second.y==node.y)
         if(trianglevec[j].second.z==node.z)
{
VerNorms[i].x=VerNorms[i].x+TriNorms[trianglevec[j].first].x;
VerNorms[i].y=VerNorms[i].y+TriNorms[trianglevec[j].first].y;
VerNorms[i].z=VerNorms[i].z+TriNorms[trianglevec[j].first].z;
}
}
VerNorms[i]=VerNorms[i].Normalize(); // or Normalize them up
}
}
void Mesh::DrawNormalFaces()
{
 // scale it so it fits in the window
 Vec3f center1; bbox->getCenter(center1);
 float s = 1/bbox->maxDim();
 //this offset prevents "z-fighting" bewteen the edges and faces
 //the edges will always win.
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.1,4.0);
 int i=0;
 Iterator<Triangle*> *iter = triangles->StartIteration();
 while (Triangle *t = iter->GetNext()) 
  { 
    Vec3f P0 = (*t)[0]->get();
    Vec3f P1 = (*t)[1]->get();
    Vec3f P2 = (*t)[2]->get();
    // compute the center of the poly
    Vec3f center = P0 + P1 + P2;
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(center.x()/3, center.y()/3, center.z()/3);
    glVertex3f(center.x()/3+(TriNorms[i].getX()*0.5),center.y()/3+(TriNorms[i].getY()*0.5),
    center.z()/3+(TriNorms[i].getZ()*0.5));
    glEnd();
    i++;
}
triangles->EndIteration(iter);
}
void Mesh::DrawNormalVertices()
{
    glDisable(GL_LIGHTING);
     for(int i = 0; i < numVertices(); i++)
	{
       glLineWidth(2.0f);
       glBegin(GL_LINES);
       glColor3f(1.0, 1.0, 0.0);
	  glVertex3f((*vertices)[i]->x(), (*vertices)[i]->y(), (*vertices)[i]->z());
       glVertex3f((*vertices)[i]->x()+(VerNorms[i].getX()*0.25),
	  ((*vertices)[i]->y()+VerNorms[i].getY()*0.25),((*vertices)[i]->z()+VerNorms[i].getZ()*0.25));
       //glVertex3f(VerNorms[i].getX(),VerNorms[i].getY(),VerNorms[i].getZ());
       glEnd();

}
}
double Mesh::CalculateTriArea( Vector3f v1,Vector3f v2,Vector3f v3)
{
// recive the vertices postion and cacluate the area
Vector3f cross= (v3-v1).Cross((v3-v2)); 
double area = 0.5 * (sqrtf( cross.x*cross.x + cross.y*cross.y + cross.z*cross.z ));
return area;
}
// =======================================================================
// Working Tangent
// =======================================================================
// using barycenteric coordinate to find the UV coordinate 
void Mesh::CalculateUVCoords()
{
int nTri=0;
Iterator<Triangle*> *iter = triangles->StartIteration();
while (Triangle *t = iter->GetNext()) 
{
	     int nV = nTri*3;
		int specificpoint=nV;
	     for ( int i=0; i<3; i++)
	     {
		if(specificpoint<numVertices())
	     {
		Vec3f a = (*t)[0]->get();
          Vec3f b = (*t)[1]->get();
          Vec3f c = (*t)[2]->get();
          Vector3f P0; 
          P0.x=a.x();
	     P0.y=a.y();
	     P0.z=a.z();
	     Vector3f P1;
	     P1.x=b.x();
	     P1.y=b.y();
	     P1.z=b.z();
	     Vector3f P2;
	     P2.x=c.x();
	     P2.y=c.y();
	     P2.z=c.z();
		int ind= indices[specificpoint];
		//cout << specificpoint;
          double sx=((*vertices).operator[](ind)->x());
          double sy=((*vertices).operator[](ind)->y());
          double sz=((*vertices).operator[](ind)->z());
           Vector3f P; 
           P.x=sx;  // specific node or vertices to search
           P.y=sy;  // specific node or vertices to search
           P.z=sz;  // specific node or vertices to search
		//cout << P.x << P.y << P.z;
		Vector3f v0 = P2-P0;
          Vector3f v1 = P1-P0;
          Vector3f v2 = P-P0;
		// Compute dot products
		double dot00 = v0.Dot(v0);
		double dot01 = v0.Dot(v1);
		double dot02 = v0.Dot(v2);
		double dot11 = v1.Dot(v1);
		double dot12 = v1.Dot(v2);
  		// Compute barycentric coordinates
          double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
          float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
          float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
		UVCoords.push_back(Vector2f(u,v));
		specificpoint =specificpoint+1;
		}
}        
nTri++;		
} 
triangles->EndIteration(iter);
}
void Mesh::CalculateTangentTri()
{
float det = 0.0f;
Vector3f tangent;
Vector3f bitangent;
for ( int i = 0; i < numTriangles(); i++ )  // for loop the number of triangles 
{
TriTangs[i].x=0;
TriTangs[i].y=0;
TriTangs[i].z=0;
}
int nTri=0;
Iterator<Triangle*> *iter = triangles->StartIteration();
while (Triangle *t = iter->GetNext()) 
{		int nV = nTri*3;
		int specificpoint=nV;
		for ( int i=0; i<3; i++)
		{
          if(nTri<numTriangles())
	     {
          Vec3f a = (*t)[0]->get();
          Vec3f b = (*t)[1]->get();
          Vec3f c = (*t)[2]->get();
          Vector3f P0; 
          P0.x=a.x();
	     P0.y=a.y();
	     P0.z=a.z();
	     Vector3f P1;
	     P1.x=b.x();
	     P1.y=b.y();
	     P1.z=b.z();
	     Vector3f P2;
	     P2.x=c.x();
	     P2.y=c.y();
	     P2.z=c.z();
		Vector2f texP0 = UVCoords[ indices[ nV ] ];
		Vector2f texP1 = UVCoords[ indices[ nV+1 ] ];
    	     Vector2f texP2 = UVCoords[ indices[ nV+2 ] ];
		// calculate the edage 
		Vector3f E1=P1-P0;
		Vector3f E2=P2-P0;
		// calculate the tex edage
		Vector2f texE1=texP1.operator-=(texP0);
		Vector2f texE2=texP2.operator-=(texP0);
		//////////////////////////////////////////////////
		det = texE1.x * texE2.y - texE2.x * texE1.y;
		if (fabs(det) < 1e-6f)
        {
            tangent.x = 1.0f;
            tangent.y = 0.0f;
            tangent.z = 0.0f;
            bitangent.x = 0.0f;
		  bitangent.y = 1.0f;
		  bitangent.z = 0.0f;
		}
        else
           {
            det = 1.0f / det;
		  tangent.x= (texE2.y * E1.x - texE1.y * E2.x) * det;
		  tangent.y = (texE2.y * E1.y - texE1.y * E2.y) * det;
            tangent.z = (texE2.y * E1.z - texE1.y * E2.z ) * det;
            bitangent.x = (-texE2.x * E1.x + texE1.x * E2.x) * det;
		  bitangent.y = (-texE2.x * E1.y + texE1.x * E2.y) * det;
            bitangent.z = (-texE2.x * E1.z + texE1.x*  E2.z) * det;
		}

          TriTangs[nTri].x=tangent.getX();
		//TriTangs[nTri].y=tangent.getY();
		//TriTangs[nTri].z=tangent.getZ();
		//TriTangs[nTri].Normalize();
		//TribiTangs[nTri]=bitangent;
		//TribiTangs[nTri].Normalize();
		}
		}
nTri++;	
}
triangles->EndIteration(iter);
CalculateTangentVertices();
}
void Mesh :: CalculateTangentVertices()
{
for(int i = 0; i < numVertices(); i++)
{
	VerTangs[i].x=0;
	VerTangs[i].y=0;
	VerTangs[i].z=0;
	VerbiTangs[i].x=0;
	VerbiTangs[i].y=0;
	VerbiTangs[i].z=0;
	
}
/* calculate the average normal for each vertex */
// but we should find first all the adjace faces
for(int i = 0; i < numVertices(); i++)
{
///* calculate an average normal for this vertex by averaging the
//facet normal of every triangle this vertex is in */
double sx=((*vertices).operator[](i)->x());
double sy=((*vertices).operator[](i)->y());
double sz=((*vertices).operator[](i)->z());
Vector3f node; 
node.x=sx;  // specific node or vertices to search
node.y=sy;  // specific node or vertices to search
node.z=sz;  // specific node or vertices to search
Vector3f facetNormal ;
////////////////////// Weighted Vertex Tangent ///////////////////////////
for(int j =0; j < trianglevec.size(); j++)
{
if ( trianglevec[j].second.x==node.x)
	if(trianglevec[j].second.y==node.y)
         if(trianglevec[j].second.z==node.z)
{
VerTangs[i].x=VerTangs[i].x+TriTangs[trianglevec[j].first].x;
VerTangs[i].y=VerTangs[i].y+TriTangs[trianglevec[j].first].y;
VerTangs[i].z=VerTangs[i].z+TriTangs[trianglevec[j].first].z;
VerbiTangs[i].x=VerbiTangs[i].x+TribiTangs[trianglevec[j].first].x;
VerbiTangs[i].y=VerbiTangs[i].y+TribiTangs[trianglevec[j].first].y;
VerbiTangs[i].z=VerbiTangs[i].z+TribiTangs[trianglevec[j].first].z;
}
}
VerbiTangs[i].Normalize();
}
for(int i = 0; i < numVertices(); i++)
{
	float nDotT= VerNorms[i].Dot(VerTangs[i]);
	Vector3f nt =VerTangs[i]-VerNorms[i];
	nt.x= nt.x*nDotT;
	nt.y= nt.y*nDotT;
	nt.z= nt.z*nDotT;
	VerTangs[i]= nt;
	VerTangs[i].Normalize();
	VerbiTangs[i] = VerNorms[i].Cross(VerTangs[i]);
	VerbiTangs[i].Normalize();
}
}
void Mesh::DrawTangentVertices()
{
 glDisable(GL_LIGHTING);
     for(int i = 0; i < numVertices(); i++)
	{
       glLineWidth(3.0f);
       glBegin(GL_LINES);
       //glColor3f(1.0, 1.0, 0.0);
	  glColor3f(.5f,.0f,.5f); // purple
	  glVertex3f((*vertices)[i]->x(), (*vertices)[i]->y(), (*vertices)[i]->z());
       glVertex3f((*vertices)[i]->x()+(VerTangs[i].getX()*0.25),(*vertices)[i]->y()
		  +(VerTangs[i].getY()*0.25),(*vertices)[i]->z()+(VerTangs[i].getZ()*0.25));
       //glVertex3f(VerNorms[i].getX(),VerNorms[i].getY(),VerNorms[i].getZ());
       glEnd();
}
	for(int i = 0; i < numVertices(); i++)
	{
       glLineWidth(3.0f);
       glBegin(GL_LINES);
       glColor3f( 0.95f, 0.207, 0.031f ); //red
	  glVertex3f((*vertices)[i]->x(), (*vertices)[i]->y(), (*vertices)[i]->z());
       glVertex3f((*vertices)[i]->x()+(VerbiTangs[i].getX()*0.25),
	  (*vertices)[i]->y()+(VerbiTangs[i].getY()*0.25),(*vertices)[i]->z()+(VerbiTangs[i].getZ()*0.25));
       //glVertex3f(VerNorms[i].getX(),VerNorms[i].getY(),VerNorms[i].getZ());
       glEnd();
	}
}
// =======================================================================
// PAINT
// =======================================================================
Vec3f ComputeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
  Vec3f v12 = p2;
  v12 -= p1;
  Vec3f v23 = p3;
  v23 -= p2;
  Vec3f normal;
  Vec3f::Cross3(normal,v12,v23);
  normal.Normalize();
  return normal;
  
}
void InsertNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3) {
  Vec3f normal = ComputeNormal(p1,p2,p3);
  glNormal3f(normal.x(), normal.y(), normal.z());
}
/*void Mesh::Paint() {
  // draw the triangles
  glColor3f(0.6f,0.6f,0.6f);
  glLineWidth(2.0f);
  glColor3f(0.3,0.3,0.3); // grey
  Iterator<Triangle*> *iter = triangles->StartIteration();
  glBegin (GL_TRIANGLES);
  while (Triangle *t = iter->GetNext()) {
    Vec3f a = (*t)[0]->get();
    Vec3f b = (*t)[1]->get();
    Vec3f c = (*t)[2]->get();
    InsertNormal(a,b,c); 
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glVertex3f(c.x(),c.y(),c.z());
  }
  triangles->EndIteration(iter);
  glEnd();
 // glDisable(GL_POLYGON_OFFSET_FILL); 
 // HandleGLError(); 
 /* if (args->wireframe) 
  {
    glDisable(GL_LIGHTING);

    // draw all the interior, non-crease edges
    glLineWidth(1);
    glColor3f(0,0,0);
    glBegin (GL_LINES);
    Iterator<Edge*> *iter = edges->StartIteration();
    while (Edge *e = iter->GetNext()) {
      if (e->getOpposite() == NULL || e->getCrease() > 0) continue;
      Vec3f a = (*e)[0]->get();
      Vec3f b = (*e)[1]->get();
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
    }
    edges->EndIteration(iter);
    glEnd();

    // draw all the interior, crease edges
    glLineWidth(3);
    glColor3f(1,1,0);
    glBegin (GL_LINES);
    iter = edges->StartIteration();
    while (Edge *e = iter->GetNext()) {
      if (e->getOpposite() == NULL || e->getCrease() == 0) continue;
      Vec3f a = (*e)[0]->get();
      Vec3f b = (*e)[1]->get();
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
    }
    edges->EndIteration(iter);
    glEnd();

    // draw all the boundary edges
    glLineWidth(3);
    glColor3f(1,1,0);
    glBegin (GL_LINES);
    iter = edges->StartIteration();
    while (Edge *e = iter->GetNext()) {
      if (e->getOpposite() != NULL) continue;
      assert (e->getCrease() == 0);
      Vec3f a = (*e)[0]->get();
      Vec3f b = (*e)[1]->get();
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
    }
    edges->EndIteration(iter);
    glEnd();

    glEnable(GL_LIGHTING);
  }
    
  //DrawNormalFaces();
  //DrawNormalVertices();
    DrawTangentVertices();
}*/
void Mesh::PaintBB() 
{ 
	bbox->paint();
}
void Mesh::Paint() 
{   
   // glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.3,0.3,0.3); // grey
    glPushMatrix();
    Iterator<Triangle*> *iter = triangles->StartIteration();
    glBegin (GL_TRIANGLES);
    while (Triangle *t = iter->GetNext()) {
    Vec3f a = (*t)[0]->get();
    Vec3f b = (*t)[1]->get();
    Vec3f c = (*t)[2]->get();
    InsertNormal(a,b,c); 
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glVertex3f(c.x(),c.y(),c.z());
  }
  triangles->EndIteration(iter);
  glEnd(); 
  glLoadIdentity(); // reset all transformations
  glPopMatrix();
  // draw all the interior, non-crease edges
    glDisable(GL_LIGHTING);
    // draw all the interior, non-crease edges
    glLineWidth(1);
    glColor3f(0,0,0);
    glBegin (GL_LINES);
    Iterator<Edge*> *iter1 = edges->StartIteration();
    while (Edge *e = iter1->GetNext())
    {
      if (e->getOpposite() == NULL || e->getCrease() > 0) continue;
      Vec3f a = (*e)[0]->get();
      Vec3f b = (*e)[1]->get();
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
    }
    edges->EndIteration(iter1);
    glEnd();
    // draw all the interior, crease edges
    glLineWidth(3);
    glColor3f(1,1,0);
    glBegin (GL_LINES);
    iter1 = edges->StartIteration();
    while (Edge *e = iter1->GetNext())
    {
      if (e->getOpposite() == NULL || e->getCrease() == 0) continue;
      Vec3f a = (*e)[0]->get();
      Vec3f b = (*e)[1]->get();
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
    }
    edges->EndIteration(iter1);
    glEnd();
   
}
void Mesh::BoundaryPaint() 
{   
 // draw all the boundary edges
    glLineWidth(3);
    glColor3f(1,1,0);
    glBegin (GL_POINTS);
    Iterator<Triangle*> *iter = triangles->StartIteration();
  //  glBegin (GL_TRIANGLES); 
    int i=0;
    while (Triangle *t = iter->GetNext())
    {
	    if (i<12)
	     if ((*t).getEdge()->getOpposite()!=NULL
			  && (*t).getEdge()!=NULL
			  && (*t).getEdge()->getOpposite() !=NULL
			  && (*t).getEdge()->getNext()->getOpposite()!=NULL )
		{
	    Edge *op=(*t).getEdge()->getOpposite()->getNext();

    /*Iterator<Edge*> *iter1 = edges->StartIteration();
    iter1 = edges->StartIteration();*/
   
  /*  while (Edge *e = iter1->GetNext()) 
    {*/
	 /*Edge *next=(*e).getNext();
	 Edge *nextnext=(*next).getNext();
	  Edge *nnextnext=(*nextnext).getNext();
	  //Edge *op=(*next).getOpposite();
	  Edge *t=(*next).getOpposite()->getTriangle()->getEdge();*/
       Edge *next=(*t).getEdge();
	  Edge *nextnext=(next)->getNext();
	    
	    if ((op->getOpposite()!= NULL) 
		 && (next->getOpposite() != NULL)
		 && (nextnext->getOpposite() != NULL))
	    {
      Vec3f a = (*t)[0]->get();
      Vec3f b = (*t)[1]->get();
	 Vec3f c = (*t)[2]->get();
	/* Vec3f d =(*t)[1]->get();
	 Vec3f f =(*t)[0]->get();*/
	 Vec3f g =(*op)[0]->get();
	// cout << " here nadine we draw " ;
	 glColor3f(1,1,0);
      glVertex3f(a.x(),a.y(),a.z());
	 glColor3f(0.3,0.5,0);
      glVertex3f(b.x(),b.y(),b.z());
	 glColor3f(1,0,1);
	 glVertex3f(c.x(),c.y(),c.z());
	 glColor3f(1,0,0);
	/* glVertex3f(d.x(),d.y(),d.z());
	 glColor3f(0,0,0);
	 glVertex3f(f.x(),f.y(),f.z());*/
	 glColor3f(1,1,1);
	 glVertex3f(g.x(),g.y(),g.z());
//	 cout << i; 
	 i++;
		}
		}
	
    }

     triangles->EndIteration(iter);
   // edges->EndIteration(iter1);
    glEnd();
    HandleGLError();
    //glEnable(GL_LIGHTING);
}
void Mesh::FindBoundaryEdge()
{

  Iterator<Edge*> *iter1 = edges->StartIteration();
    iter1 = edges->StartIteration();
    while (Edge *e = iter1->GetNext()) 
    { 
	 Edge *next=(*e).getNext();
	 Edge *nextnext=(*next).getNext();

	 if ((e->getOpposite()!= NULL) 
	&& (next->getOpposite() != NULL)
	&& (nextnext->getOpposite() != NULL))continue;
      assert (e->getCrease() == 0);
      const Vec3f a = (*e)[0]->get();
      const Vec3f b = (*e)[1]->get();
	 addBoundaryVertex(a);
	 addBoundaryVertex(b);
	 BEdge.push_back(std::make_pair(a,b));
	 bedges->Add(e);
    }
    edges->EndIteration(iter1);
    cout <<endl<<BEdge.size();
    cout<<endl<<bedges->Count();
    cout<<endl<<bnumVertices();

    FindHolesEdge();
}
void Mesh::HolesPaint() 
{
glLineWidth(3);
glColor3f(1,1,0);
glBegin (GL_LINES);
for (int i=0; i<1;i++)
	for (int j=0; j<HEdge[i].size();j++)
	{
		glVertex3f(HEdge[i].at(j).first.x(),
			HEdge[i].at(j).first.y(),
			HEdge[i].at(j).first.z());
		glVertex3f(HEdge[i].at(j).second.x(),
			HEdge[i].at(j).second.y(),
			HEdge[i].at(j).second.z());
	}
 glEnd();
 //HandleGLError();
}
void Mesh::FindHolesEdge()
{    
    BoundaryEdge IHEdge;
    int i=0,j=0;
    Iterator<Edge*> *iter1 = edges->StartIteration();
    iter1 = edges->StartIteration();
    while (Edge *e = iter1->GetNext()) 
    {  
	
	/* IHEdge.push_back(std::make_pair((*e)[0]->get(),(*e)[1]->get()));
	 IHEdge.push_back(std::make_pair((*N)[0]->get(),(*N)[1]->get()));
	 i++;
	 cout << i;
	 HEdge.push_back((BoundaryEdge)IHEdge);*/
    }
  edges->EndIteration(iter1);
}
///////////// write every thing inside obj file //////////
void Mesh :: SaveObj()
{
  // open and write data to the file
  std::string file_name ="Close";
  file_name= file_name.append("_Scan.obj");
  const char *sptr = file_name.c_str() ;
  /* open the file */
   FILE* objfile; 
   objfile = fopen(sptr, "w");
   if (!objfile) {
        fprintf(stderr, "WriteOBJ() failed: can't open file \"%s\" to write.\n",
            objfile);
        exit(1);
    }
    /* spit out a header */
    fprintf(objfile,"#  \n");
    fprintf(objfile,"#  Wavefront OBJ \n");
    fprintf(objfile,"#  \n");
    fprintf(objfile,"#  Nadine ^_____^ \n");
    fprintf(objfile,"#  \n");
    Iterator<Triangle*> *iter = triangles->StartIteration();
    /* spit out the vertices */
    fprintf(objfile,"# %d vertices\n", numVertices());
    for (int i = 0; i < numVertices(); i++)
    { 
    Vec3f a = (*vertices)[i]->get();
    //Vec3f b = (*t)[1]->get();
    //Vec3f c = (*t)[2]->get();
    fprintf(objfile, "v %f %f %f\n", 
	   a.x(),
        a.y(),
	   a.z());
    /*fprintf(objfile, "v %f %f %f\n", 
        b.x(),
        b.y(),
        b.z());
    fprintf(objfile, "v %f %f %f\n", 
        c.x(),
        c.y(),
        c.z());*/
    }
   // triangles->EndIteration(iter);
   /* spit out the smooth/flat normals */
   fprintf(objfile, "\n");
  // Iterator<Triangle*> *iter1 = triangles->StartIteration();
  //fprintf(objfile, "# %d Vertices normals\n",numVertices());
  //while (Triangle *t = iter1->GetNext())
        {
           /*fprintf(objfile, "vn %f %f %f\n", 
            VerNorms[i].x,
            VerNorms[i].y,
            VerNorms[i].z);*/
        }
       // fprintf(objfile, "\n");
	  fprintf(objfile, "# %d Face \n",numTriangles());
        for (int i = 0; i < numTriangles(); i++)
	   { int nV = i*3;
		//cout << indices[nV] << indices[nV+1] << indices[nV+2] << endl;
          fprintf(objfile, "f %d %d %d\n", 
          indices[nV],
          indices[nV+1],
          indices[nV+2]);
       }
  /* spit out the texture coordinates 
     fprintf(objfile, "\n");

  /*   fprintf(objfile, "# %d texcoords\n", UVCoords.size());
   for (int i = 0; i < UVCoords.size(); i++)
   {
               fprintf(objfile, "vt %f %f\n", 
               UVCoords[i].x,
               UVCoords[i].y);
   }
    fprintf(objfile, "\n");*/

 fclose(objfile);
//QMessageBox::information( this, tr("Load Model"), tr("You Saved Pose 1 FPS  !!!!") ); // notify the user that you didnt select any file to load
}
