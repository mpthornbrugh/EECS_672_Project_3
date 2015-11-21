#include "myC.h"
#include "Matrix4x4.h"

using namespace std;

myC * myC::curControllerSub = NULL;

myC::myC( const std::string& name, int glutRCFlags )
  :
  Controller( name, glutRCFlags )
{
  myC::curControllerSub = this;
  establishInitialCallbacksForRC();
} 

myC::~myC()
{
  if( this == curControllerSub )
    curControllerSub = NULL;
} 

void myC::establishInitialCallbacksForRC() {
  Controller::establishInitialCallbacksForRC();
  
  glutMouseFunc( myC::mouseFuncCB );
  glutMotionFunc( myC::mouseMotionCB );
} 

void myC::handleMouseFunc( int button, int state, int x, int y )
{
  double ldsX, ldsY;
  screenXYToLDS( x, y, ldsX, ldsY );

  ModelViewWithPhongLighting::handleMouseFunc( button, state, ldsX, ldsY );

  glutPostRedisplay();
} 

void myC::handleMouseMotion( int x, int y )
{
  ModelViewWithPhongLighting::handleMouseMotion( x, y );
  glutPostRedisplay();
} 

void myC::handleMousePassiveMotion( int x, int y )
{
  double ldsX, ldsY;
  screenXYToLDS( x, y, ldsX, ldsY );
  ModelViewWithPhongLighting::handleMousePassiveMotion( ldsX, ldsY );
} 

void myC::mouseFuncCB( int button, int state, int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMouseFunc( button, state, x, y );
} 

void myC::mouseMotionCB( int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMouseMotion( x, y );
} 

void myC::mousePassiveMotionCB( int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMousePassiveMotion( x, y );
}  

void myC::handleDisplay()
{
} 
