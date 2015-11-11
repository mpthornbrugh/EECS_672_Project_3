// Sphere.h

#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include "ModelView.h"
#include "ModelViewWithPhongLighting.h"

#define NumTimesToSubdivide 5
#define NumTriangles 4096
#define NumVertices (3 * NumTriangles)

typedef float vec3[3];

class Sphere : public ModelViewWithPhongLighting
{
 public:
  Sphere( vec3 loc, float radius, float* color );
  virtual ~Sphere();
  
  void tri( const vec3& veca,const vec3& vecb,const vec3& vecc,int& Index);
  
  void getMCBoundingBox( double* xyzLimitsF ) const;
  void defineSphere();
  void render();
  void render(float* color);

 private:
  void tetrahedron( int count );
  void divide_triangle( const vec3& a, const vec3& b, const vec3& c, int count );
  
  GLuint vao;
  GLuint vbo[2];

  double _limits[6];
  int _Index;
  
  vec3 _points[NumVertices] ;//= {{0}};
  vec3 _normals[NumVertices] ;//= {{0}};
  
  vec3 center;
  float sphColor[3];
  float sphR;
};

#endif
