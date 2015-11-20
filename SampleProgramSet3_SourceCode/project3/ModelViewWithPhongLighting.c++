// ModelViewWithPhongLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithPhongLighting.h"
#include "ShaderIF.h"

typedef float vec4[4];

ShaderIF* ModelViewWithPhongLighting::shaderIF = NULL;
int ModelViewWithPhongLighting::numInstances = 0;
GLuint ModelViewWithPhongLighting::shaderProgram = 0;

GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;

GLint ModelViewWithPhongLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithPhongLighting::pvaLoc_mcNormal = -2;

GLint ModelViewWithPhongLighting::ppuLoc_ka = -2;
GLint ModelViewWithPhongLighting::ppuLoc_kd = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ks = -2;
GLint ModelViewWithPhongLighting::ppuLoc_m = -2;

GLint ModelViewWithPhongLighting::ppuLoc_eye = -2;

GLint ModelViewWithPhongLighting::ppuLoc_p_ecLightPos = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightStrength = -2;
GLint ModelViewWithPhongLighting::ppuLoc_actualNumLights = -2;
GLint ModelViewWithPhongLighting::ppuLoc_globalAmbient = -2;

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

int ModelViewWithPhongLighting::lastMousePosition[2] = { 0, 0 };
PROJECTION_TYPE ModelViewWithPhongLighting::projType = PERSPECTIVE;

float lightPosition[ ] = {
   -1.0, 0.0, 1.0, 0.0, // source 0: directional
   0.0, 1.0, 1.0, 0.0,  // source 1: directional
   140.5, -133.0, 200.0, 1.0 // source 2: an actual location in the scene
   };
float lightStrength[ ] = {
   1.0, 1.0, 1.0, // source 0: full strength white
   0.4, 0.4, 0.4, // source 1: 40% white
   0.8, 0.8, 0.0  // source 2: 80% strength yellow
   };
float ambientStrength[ ] = { 0.15, 0.15, 0.15 }; // assumed ambient light

void ModelViewWithPhongLighting::establishLights()
{
	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);

    // If light sources defined in MC, transform them to EC:
	float lightPositionInEC[12];

	for (int i = 0; i < 12; i++) {
		lightPositionInEC[i] = lightPosition[i];
	}

    // Now send the EC geometric description along with the non-geometric data:
    int numLights = 3;
    glUniform4fv(ppuLoc_p_ecLightPos, numLights, lightPositionInEC);
    glUniform3fv(ppuLoc_lightStrength, numLights, lightStrength);
    glUniform1i(ppuLoc_actualNumLights, numLights);
    glUniform3fv(ppuLoc_globalAmbient, 1, ambientStrength);
}

ModelViewWithPhongLighting::ModelViewWithPhongLighting()
{
	if (shaderProgram == 0)
	{
		shaderIF = new ShaderIF(vShaderSource, fShaderSource);
		shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
	numInstances++;
}

ModelViewWithPhongLighting::~ModelViewWithPhongLighting()
{
	if (--numInstances == 0)
	{
		shaderIF->destroy();
		delete shaderIF;
		shaderIF = NULL;
		shaderProgram = 0;
	}
}

void ModelViewWithPhongLighting::fetchGLSLVariableLocations()
{
	if (shaderProgram > 0)
	{
		pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		pvaLoc_mcNormal = pvAttribLocation(shaderProgram, "mcNormal");
		ppuLoc_ka = ppUniformLocation(shaderProgram, "ka");
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_ks = ppUniformLocation(shaderProgram, "ks");
		ppuLoc_m = ppUniformLocation(shaderProgram, "m");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");
		ppuLoc_eye = ppUniformLocation(shaderProgram, "eye");
	}
}

void ModelViewWithPhongLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc)
{
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}

void ModelViewWithPhongLighting::handleMouseMotion( int x, int y )
{
	if (!mouseIsDown) {
		lastMousePosition[0] = x;
		lastMousePosition[1] = y;
		mouseIsDown = false;
		return;
	}

	double rxDeg = (static_cast<double>(x - lastMousePosition[0])) * (360.0/500.0);
	double ryDeg = (static_cast<double>(y - lastMousePosition[1])) * (360.0/500.0);

	addToGlobalRotationDegrees(rxDeg, ryDeg, 0);

	lastMousePosition[0] = x;
	lastMousePosition[1] = y;
}

void ModelViewWithPhongLighting::handleMouseFunc( int button, int state, int x, int y )
{
	if (button == Controller::LEFT_BUTTON) {
		mouseIsDown = true;
		lastMousePosition[0] = x;
		lastMousePosition[1] = y;
	}
	else {
		mouseIsDown = false;
	}
}

void ModelViewWithPhongLighting::handleScroll(bool isZommIn)
{
	if (isZommIn) {
		addToGlobalZoom(0.1);
	}
	else {
		addToGlobalZoom(-0.1);
	}
}














