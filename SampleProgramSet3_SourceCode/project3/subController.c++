#include "subController.h"
#include "Matrix4x4.h"

using namespace std;

subController * subController::curControllerSub = NULL;

subController::subController( const std::string& name, int glutRCFlags ) : Controller( name, glutRCFlags )
{
  subController::curControllerSub = this;
  establishInitialCallbacksForRC();
} 

subController::~subController()
{
  if( this == curControllerSub )
    curControllerSub = NULL;
} 

void subController::establishInitialCallbacksForRC() {
  Controller::establishInitialCallbacksForRC();
  
  glutMouseFunc( ControllerSub::mouseFuncCB );
  glutMotionFunc( ControllerSub::mouseMotionCB );
  //glutPassiveMotionFunc( ControllerSub::mousePassiveMotionCB );
} 

void subController::handleMouseFunc( int button, int state, int x, int y )
{
  double ldsX, ldsY;
  screenXYToLDS( x, y, ldsX, ldsY );

  GeneralMV::handleMouseFunc( button, state, ldsX, ldsY );

  glutPostRedisplay();
} 

void subController::handleMouseMotion( int x, int y )
{
  GeneralMV::handleMouseMotion( x, y );
  glutPostRedisplay();
} 

void subController::handleMousePassiveMotion( int x, int y )
{
  double ldsX, ldsY;
  screenXYToLDS( x, y, ldsX, ldsY );
  GeneralMV::handleMousePassiveMotion( ldsX, ldsY );
} 

void subController::mouseFuncCB( int button, int state, int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMouseFunc( button, state, x, y );
} 

void subController::mouseMotionCB( int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMouseMotion( x, y );
} 

void subController::mousePassiveMotionCB( int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMousePassiveMotion( x, y );
} 

bool subController::drawingOpaque() const
{
  return bDrawOpaque;
} 

void subController::handleDisplay()
{
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_BLEND);
  drawAllOpaqueObjects();

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  drawAllTranslucentObjects();

  glutSwapBuffers();
} 

void subController::drawAllOpaqueObjects()
{
  bDrawOpaque = true;
  for( std::vector<ModelView*>::iterator it = models.begin(); it != models.end(); ++it )
    (*it)->render();
  //Controller::handleDisplay();
} 

void subController::drawAllTranslucentObjects()
{
  bDrawOpaque = false;
  for( std::vector<ModelView*>::iterator it = models.begin(); it != models.end(); ++it )
    (*it)->render();
  //Controller::handleDisplay();
}
