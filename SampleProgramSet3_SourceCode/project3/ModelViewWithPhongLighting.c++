// ModelViewWithPhongLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithPhongLighting.h"
#include "ShaderIF.h"

ShaderIF* ModelViewWithPhongLighting::shaderIF = NULL;
int ModelViewWithPhongLighting::numInstances = 0;
GLuint ModelViewWithPhongLighting::shaderProgram = 0;
GLint ModelViewWithPhongLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithPhongLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithPhongLighting::ppuLoc_kd = -2;
GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

int ModelViewWithPhongLighting::lastMousePosition[2] = {0, 0};
bool ModelViewWithPhongLighting:: mouseIsDown = false;

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
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");
	}
}

void ModelViewWithPhongLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc)
{
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}

void ModelViewWithPhongLighting::handleMouseMotion(int x, int y) {
	if (!mouseIsDown) {
		lastMousePosition[0] = x;
		lastMousePosition[1] = y;
		mouseIsDown = false;
		return;
	}
	
	double rxDeg = (static_cast<double>(x - lastMousePosition[0])) * (360.0/500.0);
	double ryDeg = (static_cast<double>(y - lastMousePosition[1])) * (360.0/500.0);

	addToGlobalRotationDegrees(rxDeg, ryDeg, 0.0);

	lastMousePosition[0] = x;
	lastMousePosition[1] = y;
}

void ModelViewWithPhongLighting::handleMouseFunc(int button, int state, int x, int y)
{
	if (state == 1) {
		mouseIsDown = true;	
	}
	else {
		mouseIsDown = false;
	}

	if (button == Controller::LEFT_BUTTON) {
		lastMousePosition[0] = x;
		lastMousePosition[1] = y;
	}
}

void ModelViewWithPhongLighting::handleScroll(bool isZoomOut)
{
	if (!isZoomOut) {
		addToGlobalZoom(100.0);
	}
	else {
		addToGlobalZoom(-100.0);
	}
}










































