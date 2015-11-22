#include <cmath>

#include "Sphere.h"
#include <iostream>
#include <math.h>
#include <cstring>
#include <string.h>


Sphere::Sphere(vec3 loc, float radius, float* color)
{

  sphColor[0] = color[0];
  sphColor[1] = color[1];
  sphColor[2] = color[2];
  
  std::memcpy( center, loc, sizeof( vec3 ) );
  
  sphR = radius;
  
  

  _limits[0] = -1.0; //xmin
  _limits[1] = 1.0; //xmax
  _limits[2] = -1.0; //ymin
  _limits[3] = 1.0; //ymax
  _limits[4] = -1.0; //zmin
  _limits[5] = 1.0; //zmax

  defineSphere();
}

Sphere::~Sphere()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );
  
  //delete [] _points;
  //delete [] _normals;

}



void Sphere::defineSphere()
{
    
       
  
  _Index = 0;
  tetrahedron( NumTimesToSubdivide );


  
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * NumVertices, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_mcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * NumVertices, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( pvaLoc_mcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_mcNormal );
} /* end Sphere::defineSphere() */




void Sphere::getMCBoundingBox( double* xyzLimitsF ) const
{
  std::memcpy( xyzLimitsF, _limits, 6 * sizeof( double ) );
} /* end Sphere::getMCBoundingBox() */


void Sphere::render()
{
  GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderProgram);

    cryph::Matrix4x4 mc_ec, ec_lds;
    getMatrices(mc_ec, ec_lds);
    float mat[16];
    glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
    glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

    
    float* color = sphColor;


	vec4 kd, ka;

	kd[0] = ka[0] = color[0];
	kd[1] = ka[1] = color[1];
	kd[2] = ka[2] = color[2];
	kd[3] = ka[3] = 1.0;

	vec4 ks = {1.0, 1.0, 1.0, 1.0};

	float m = 50.0;

	sendPhongLightModel(ka, kd, ks, m);

    //glUniform3fv(ppuLoc_ka, 1, CountertopColor);
    //glUniform3fv(ppuLoc_kd, 1, color);
    //glUniform3fv(ppuLoc_ks, 1, CountertopColor);
    //glUniform1f (ppuLoc_m, m);

  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  glUseProgram( pgm );
}

void Sphere::tetrahedron( int count )
{
  /* copied from "Interactive Computer Graphics" by E. Angel, p. 633 */
  vec3 v[4] = {
    { 0.0, 0.0, 1.0 },
    { 0.0, 0.942809, -0.333333 },
    { -0.816497, -0.471405, -0.333333 },
    { 0.816497, -0.471405, -0.333333 }
  };

  divide_triangle( v[0], v[1], v[2], count );
  divide_triangle( v[3], v[2], v[1], count );
  divide_triangle( v[0], v[3], v[1], count );
  divide_triangle( v[0], v[2], v[3], count );
} /* end Sphere::tetrahedron() */



void Sphere::tri( const vec3& veca,const vec3& vecb,const vec3& vecc,int& Index)//!> The index in _points and _normals at which to start
{
  cryph::AffVector a( veca );
  cryph::AffVector b( vecb );
  cryph::AffVector c( vecc );

  /* based on the example function quad() from Interactive Computer Graphics by E. Angel, p. 627 */
  cryph::AffVector normal = cryph::AffVector::cross( b - a, c - b );
  normal.normalize();
  normal.vComponents( _normals[Index] ); a.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); b.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); c.vComponents( _points[Index] ); Index++;
} /* end GeneralMV::tri() */



void Sphere::divide_triangle( const vec3& a, const vec3& b, const vec3& c, int count )
{
  /* copied from "Interactive Computer Graphics" by E. Angel, p. 633 */
  if( count > 0 )
    {
      cryph::AffVector tv1 = cryph::AffVector( a );
      tv1 += cryph::AffVector( b );
      tv1.normalize();
      
      cryph::AffVector tv2 = cryph::AffVector( a );
      tv2 += cryph::AffVector( c );
      tv2.normalize();

      cryph::AffVector tv3 = cryph::AffVector( b );
      tv3 += cryph::AffVector( c );
      tv3.normalize();

      vec3 v1, v2, v3;
      tv1.vComponents( v1 );
      tv2.vComponents( v2 );
      tv3.vComponents( v3 );

      divide_triangle( a, v1, v2, count - 1 );
      divide_triangle( c, v2, v3, count - 1 );
      divide_triangle( b, v3, v1, count - 1 );
      divide_triangle( v1, v3, v2, count - 1 );
    }
  else
    {
      vec3 aa, bb, cc;
      for( short i = 0; i < 3; ++i )
	{
	  aa[i] = a[i] * sphR + center[i];
	  bb[i] = b[i] * sphR + center[i];
	  cc[i] = c[i] * sphR + center[i];
	}
      tri( aa, bb, cc, _Index );
    }
} /* end Sphere::divide_triangle() */
