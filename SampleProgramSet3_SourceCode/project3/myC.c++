// GLFWController.c++: a basic GLFWController (in Model-View-GLFWController sense)

#include <stdlib.h>

#include "GLFWController.h"
#include "ModelView.h"

bool GLFWController::glfwInitialized = false;

myC::myC(const std::string& windowTitle, int rcFlags) :
	returnFromRun(false), runWaitsForAnEvent(true),
	lastPixelPosX(0), lastPixelPosY(0)
{
	buttonPressed = false;
	if (!glfwInitialized)
	{
		glfwInit();
		glfwInitialized = true;
	}

	// First create the window and its Rendering Context (RC)
	createWindowAndRC(windowTitle, rcFlags);
}

myC::~myC()
{
	// IF THIS IS THE LAST CONTROLLER
	{
		glfwTerminate();
		glfwInitialized = false;
	}
}

void myC::mouseFuncCB(GLFWwindow* window, int button, int action, int mods)
{
	if (curController != NULL)
	{
		Controller::MouseButton mButton;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			mButton = Controller::LEFT_BUTTON;
		else
			GLFWController::mouseFuncCB(window, button, action, mods);
		buttonPressed = (action == GLFW_PRESS);
		myC* c = dynamic_cast<myC*>(curController);
		c->handleMouseButton(
			mButton, buttonPressed, c->lastPixelPosX, c->lastPixelPosY, mapMods(mods));
	}
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

void myC::mouseMotionCB(GLFWwindow* window, double x, double y)
{
	if (curController != NULL)
	{
		myC* c = dynamic_cast<myC*>(curController);
		c->lastPixelPosX = static_cast<int>(x + 0.5);
		c->lastPixelPosY = static_cast<int>(y + 0.5);
		c->handleMouseMotion(c->lastPixelPosX, c->lastPixelPosY);
	}
}

void myC::scrollCB(GLFWwindow* window, double xOffset, double yOffset)
{
	dynamic_cast<myC*>(curController)->handleScroll(yOffset > 0.0);
}
