// myC.h - A concrete GLFWController subclass using the GLFW window interface

#ifndef MYC_H
#define MYC_H

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include "GLFW/glfw3.h"

#include "GLFWController.h"

class myC : public GLFWController
{
public:
	myC(const std::string& windowTitle, int rcFlags = 0);
	virtual ~myC();

private:

	static bool glfwInitialized;
	static bool buttonPressed;
	static void mouseFuncCB(GLFWwindow* window, int button, int action, int mods);
	static void mouseMotionCB(GLFWwindow* window, double x, double y);
	static void scrollCB(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
