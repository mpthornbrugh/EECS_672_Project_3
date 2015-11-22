// ModelViewWithPhongLighting.h

#ifndef MODELVIEWWITHPHONGLIGHTING_H
#define MODELVIEWWITHPHONGLIGHTING_H

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "ShaderIF.h"
#include "Matrix4x4.h"

class ModelViewWithPhongLighting : public ModelView
{
public:
	ModelViewWithPhongLighting();
	virtual ~ModelViewWithPhongLighting();

	static void setShaderSources(const std::string& vShader, const std::string& fShader);

	static void handleMouseMotion(int x, int y);
	static void handleMouseFunc(int button, int state, int x, int y);
	static void handleScroll(bool isZoomIn);

	static void switchProjectionType(int projectionType);

protected:

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_mcNormal;
	static GLint ppuLoc_lightPosition;
	static GLint ppuLoc_lightStrength;
	static GLint ppuLoc_actualNumLights;
	static GLint ppuLoc_globalAmbient;
	static GLint ppuLoc_kd, ppuLoc_ka, ppuLoc_ks;
	static GLint ppuLoc_mc_ec, ppuLoc_ec_lds;

	static vec4 _lightPosition[numLights];
	static float _lightStrength[3 * numLights];
	static vec4 _ambientStrength;

	static void fetchGLSLVariableLocations();

	void sendPhongLightModel( const vec4& ka, const vec4& kd, const vec4& ks, const float m );

private:
	static std::string vShaderSource, fShaderSource;

	static bool mouseIsDown;
	static double scale;
	static int lastMousePosition[2];
};

#endif
