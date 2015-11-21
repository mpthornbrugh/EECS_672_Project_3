#ifndef __MYC_H__
#define __MYC_H__

#include "Controller.h"
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "ModelViewWithPhongLighting.h"

class myC : public Controller
{
 public:
  enum {
    ScreenWidth = 360,
    ScreenHeight = 360
  };

 public:
  explicit myC( const std::string& name, int glutRCFlags = 0 );
  virtual ~myC();
  virtual void handleDisplay();

 private:
  
  virtual void establishInitialCallbacksForRC();
  virtual void handleMouseFunc( int button, int state, int x, int y );
  virtual void handleMouseMotion( int x, int y );
  virtual void handleMousePassiveMotion( int x, int y );
  static void mouseFuncCB( int button, int state, int x, int y );
  static void mouseMotionCB( int x, int y );
  static void mousePassiveMotionCB( int x, int y );

  static myC * curControllerSub;
  bool bDrawOpaque;
};

#endif
