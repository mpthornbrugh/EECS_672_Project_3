

#include <stdlib.h>

#include "GLFWControllerSub.h"
#include "ModelView.h"
#include "ModelViewWithPhongLighting.h"

bool GLFWControllerSub::glfwInitialized = false;

GLFWControllerSub::GLFWControllerSub(const std::string& windowTitle, int rcFlags) :
	returnFromRun(false), runWaitsForAnEvent(true),
	lastPixelPosX(0), lastPixelPosY(0)
{
	if (!glfwInitialized)
	{
		glfwInit();
		glfwInitialized = true;
	}

	// First create the window and its Rendering Context (RC)
	createWindowAndRC(windowTitle, rcFlags);
}

GLFWControllerSub::~GLFWControllerSub()
{
	// IF THIS IS THE LAST CONTROLLER
	{
		glfwTerminate();
		glfwInitialized = false;
	}
}

void GLFWControllerSub::charCB(GLFWwindow* window, unsigned int theChar)
{
	if (theChar < 128)
	{
		GLFWControllerSub* c = dynamic_cast<GLFWControllerSub*>(curController);
		c->handleAsciiChar(
			static_cast<unsigned char>(theChar), c->lastPixelPosX, c->lastPixelPosY);
	}
}

void GLFWControllerSub::createWindowAndRC(const std::string& windowTitle, int rcFlags)
{
	// The following calls enforce use of only non-deprecated functionality.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if ((rcFlags & MVC_USE_DEPTH_BIT) == 0)
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
	else
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
	setClearFlags(rcFlags);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	int minor = 8; // Start AT LEAST one greater than where you really want to start
	theWindow = NULL;
	while ((theWindow == NULL) && (minor > 0))
	{
		minor--;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		theWindow = glfwCreateWindow(newWindowWidth, newWindowHeight, titleString(windowTitle).c_str(), NULL, NULL);
	}
	if (theWindow == NULL)
	{
		std::cerr << "\n**** COULD NOT CREATE A 4.x RENDERING CONTEXT ****\n\n";
		glfwTerminate();
		glfwInitialized = false;
	}

	glfwMakeContextCurrent(theWindow);
	if ((rcFlags & MVC_USE_DEPTH_BIT) == 0)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);
	establishInitialCallbacksForRC();
}

void GLFWControllerSub::displayCB(GLFWwindow* window) // CLASS METHOD
{
	if (curController != NULL)
		dynamic_cast<GLFWControllerSub*>(curController)->handleDisplay();
}

void GLFWControllerSub::establishInitialCallbacksForRC()
{
	glfwSetWindowSizeCallback(theWindow, reshapeCB);
	glfwSetCharCallback(theWindow, charCB);
	glfwSetKeyCallback(theWindow, keyboardCB);
	glfwSetMouseButtonCallback(theWindow, mouseFuncCB);
	glfwSetScrollCallback(theWindow, scrollCB);
	glfwSetCursorPosCallback(theWindow, mouseMotionCB);
}

void GLFWControllerSub::handleDisplay()
{
	glfwMakeContextCurrent(theWindow);
	int width, height;
	glfwGetFramebufferSize(theWindow, &width, &height);
	glViewport(0, 0, width, height);

	// clear the frame buffer
	glClear(glClearFlags);

	// draw the collection of models
	int which = 0;
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->render();

	glfwSwapBuffers(theWindow);

	checkForErrors(std::cout, "GLFWControllerSub::handleDisplay");
}

void GLFWControllerSub::keyboardCB(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (curController != NULL)
	{
		GLFWControllerSub* theC = dynamic_cast<GLFWControllerSub*>(curController);
		if (key == GLFW_KEY_ESCAPE)
			theC->handleAsciiChar(27, theC->lastPixelPosX, theC->lastPixelPosY);
	}
}

int GLFWControllerSub::mapMods(int glfwMods)
{
	int controllerMods = 0;
	if ((glfwMods & GLFW_MOD_SHIFT) != 0)
		controllerMods = MVC_SHIFT_BIT;
	if ((glfwMods & GLFW_MOD_CONTROL) != 0)
		controllerMods |= MVC_CTRL_BIT;
	if ((glfwMods & GLFW_MOD_ALT) != 0)
		controllerMods |= MVC_ALT_BIT;
	return controllerMods;
}

void GLFWControllerSub::mouseFuncCB(GLFWwindow* window, int button, int action, int mods)
{
	if (curController != NULL)
	{
		Controller::MouseButton mButton;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			mButton = Controller::LEFT_BUTTON;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mButton = Controller::RIGHT_BUTTON;
		else
			mButton = Controller::MIDDLE_BUTTON;
		bool pressed = (action == GLFW_PRESS);
		GLFWControllerSub* c = dynamic_cast<GLFWControllerSub*>(curController);
		c->handleMouseButton(
			mButton, pressed, c->lastPixelPosX, c->lastPixelPosY, mapMods(mods));
		ModelViewWithPhongLighting::handleMouseFunc(mButton, pressed, c->lastPixelPosX, c->lastPixelPosY);
	}
}

void GLFWControllerSub::mouseMotionCB(GLFWwindow* window, double x, double y)
{
	if (curController != NULL)
	{
		GLFWControllerSub* c = dynamic_cast<GLFWControllerSub*>(curController);
		c->lastPixelPosX = static_cast<int>(x + 0.5);
		c->lastPixelPosY = static_cast<int>(y + 0.5);
		ModelViewWithPhongLighting::handleMouseMotion(c->lastPixelPosX, c->lastPixelPosY);
		c->handleMouseMotion(c->lastPixelPosX, c->lastPixelPosY);
	}
}

void GLFWControllerSub::reportWindowInterfaceVersion(std::ostream& os) const
{
	os << "        GLFW: " << glfwGetVersionString() << '\n';
}

void GLFWControllerSub::reshapeCB(GLFWwindow* window, int width, int height)
{
	dynamic_cast<GLFWControllerSub*>(curController)->handleReshape(width, height);
}

void GLFWControllerSub::run()
{
	while (!glfwWindowShouldClose(theWindow) && !returnFromRun)
	{
		if (runWaitsForAnEvent)
			glfwWaitEvents();
		else
			glfwPollEvents();
		handleDisplay();
	}
	glfwDestroyWindow(theWindow);
	theWindow = NULL;
}

void GLFWControllerSub::scrollCB(GLFWwindow* window, double xOffset, double yOffset)
{
	dynamic_cast<GLFWControllerSub*>(curController)->handleScroll(yOffset > 0.0);
	ModelViewWithPhongLighting::handleScroll(yOffset > 0.0);
}

void GLFWControllerSub::setWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(theWindow, title.c_str());
}











































