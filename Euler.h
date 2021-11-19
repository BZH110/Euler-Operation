#pragma once

#include <iostream>
#include <string>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;
class Point;

class Scene
{
public:
  Solid *solidList;
};

class Solid
{
public:
  Solid() : prevS(NULL), nextS(NULL), Faces(NULL), Edges(NULL) {}
  Solid *prevS, *nextS;
  Face *Faces;
  Edge *Edges;
};

class Face
{
public:
  Face() : FSolid(NULL), prevF(NULL), nextF(NULL), FLoop(NULL) {}
  Solid *FSolid;
  Loop* FLoop;
  Face *prevF, *nextF;
  int size();
};

class Loop
{
public:
  Loop() : LFace(NULL), prevL(NULL), nextL(NULL), HEs(NULL), innerLoop(false) {}
  Face *LFace;
  Loop *prevL, *nextL;
  HalfEdge *HEs;
  int size();
  bool innerLoop;
  bool isInnerLoop() {
    return innerLoop;
  }
  void SetInnerLoop(bool flag) {
    innerLoop = flag;
    return;
  }
};

class HalfEdge
{
public:
  HalfEdge() : HELoop(NULL), prevHE(NULL), Edge(NULL), nextHE(NULL), partner(NULL), startV(NULL), endV(NULL) {}
  Loop *HELoop;
  HalfEdge *prevHE, *nextHE;
  HalfEdge *partner;
  Edge *Edge;
  Vertex *startV, *endV;
};

class Edge
{
public:
  Edge() : prevE(NULL), nextE(NULL), HE1(NULL), HE2(NULL) {}
  Edge *prevE, *nextE;
  HalfEdge *HE1, *HE2;
};

class Vertex
{
public:
  Vertex() : prevV(NULL), nextV(NULL), point(NULL) {}
  Vertex *prevV, *nextV;
  Point *point;
};

class Point
{
public:
  Point() : p(0, 0, 0) {
    color = glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f);
  }
  Point(double _x, double _y, double _z) : p(_x, _y, _z) {}
  void SetPoint(double _x, double _y, double _z)
  {
    p.x = _x;
    p.y = _y;
    p.z = _z;
  }
  void SetPoint(Point _point)
  {
    p.x = _point.p.x;
    p.y = _point.p.y;
    p.z = _point.p.z;
  }
  friend inline ostream &operator<<(ostream &os, Point &point)
  {
    os << "( " << point.p.x << ", " << point.p.y << ", " << point.p.z << " )";
    return os;
  }
  glm::vec3 p;
  glm::vec3 color;
};

Solid *mvfs(Point _point, Vertex *&_vertex);
HalfEdge *mev(Vertex *_vertex, Point _point, Loop *_loop);
Loop *mef(Vertex *_vertex_1, Vertex *_vertex_2, Loop*& _largeLoop);
Loop *kemr(Vertex *_vertex_1, Vertex *_vertex_2, Loop*& _largeLoop);
void kfmrh(Loop *_outter_loop, Loop *_inner_loop);
Solid *sweep(Face *_face, glm::vec3 direction, float distance);
