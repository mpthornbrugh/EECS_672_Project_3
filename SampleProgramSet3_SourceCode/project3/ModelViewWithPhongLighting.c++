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
GLint ModelViewWithPhongLighting::ppuLoc_ka = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ks = -2;
GLint ModelViewWithPhongLighting::ppuLoc_m = -2;
GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;
GLint ModelViewWithPhongLighting::ppuLoc_actualNumLights = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightPosition = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightStrength = -2;
GLint ModelViewWithPhongLighting::ppuLoc_globalAmbient = -2;

vec4 ModelViewWithPhongLighting::lightPositions[numLights] = {
	{-1.0, 0.0, 0.5, 0.0},
	{0.0, 1.0, 1.0, 0.0},
	{0.0, 0.0, 2.0, 1.0}
};
float ModelViewWithPhongLighting::lightStrengths[3 * numLights] = {
	1.0, 1.0, 1.0,
	0.6, 0.6, 0.6,
	1.0, 0.0, 0.0
};
vec4 ModelViewWithPhongLighting::ambientStrength = {0.25, 0.25, 0.25, 1.0};

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
		ppuLoc_ka = ppUniformLocation(shaderProgram, "ka");
		ppuLoc_ks = ppUniformLocation(shaderProgram, "ks");
		ppuLoc_m = ppUniformLocation(shaderProgram, "m");
		ppuLoc_lightPosition = ppUniformLocation(shaderProgram, "lightPositions");
		ppuLoc_lightStrength = ppUniformLocation(shaderProgram, "lightStrengths");
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

void ModelViewWithPhongLighting::switchProjectionType(int projectionType)
{
	if (projectionType == 0) {
		//Set to orthogonal
		setProjection(ORTHOGONAL);
	}
	else if (projectionType == 1) {
		//Set to projection
		setProjection(PERSPECTIVE);
	}
	else {
		//Set to oblique
		setProjection(OBLIQUE);
	}
}

void ModelViewWithPhongLighting::sendPhongLightModel( const vec4& ka, const vec4& kd, const vec4& ks, const float m )
{
	float lightPositionInEC[4 * numLights];

	// convert the light sources to Eye Coordinates
	for( short i = 0; i < numLights; ++i )
	{
		if( lightPositions[i][4] == 0.0f )
		{
			// already in eye coordinates
			lightPositionInEC[i] = lightPositions[i][0];
			lightPositionInEC[i+1] = lightPositions[i][1];
			lightPositionInEC[i+2] = lightPositions[i][2];
			lightPositionInEC[i+3] = lightPositions[i][3];
			continue;
		}

		cryph::Matrix4x4 mc_ec, ec_lds;
		getMatrices(mc_ec, ec_lds);

		cryph::AffVector tmpLightPosition( lightPositions[i] );

		cryph::AffVector ecLightPos = mc_ec * tmpLightPosition;
		ecLightPos.vComponents( lightPositionInEC, 3 * i );
	}

	glUniform4fv( ppuLoc_lightPosition, numLights, lightPositionInEC );
	glUniform3fv( ppuLoc_lightStrength, numLights, lightStrengths );
	glUniform1i( ppuLoc_actualNumLights, numLights );
	glUniform4fv( ppuLoc_globalAmbient, 1, ambientStrength );

	glUniform4fv( ppuLoc_ka, 1, ka );
	glUniform4fv( ppuLoc_kd, 1, kd );
	glUniform4fv( ppuLoc_ks, 1, ks );
	glUniform1f( ppuLoc_m, m );
}










































