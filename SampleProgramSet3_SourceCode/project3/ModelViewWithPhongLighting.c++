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

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

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
	cryph::Matrix4x4 M_ECu = cryph::Matrix4x4::lookAt(ModelView::eye, ModelView::center, ModelView::up);
	cryph::Matrix4x4 mc_ec = dynamic_view * M_ECu;

    // If light sources defined in MC, transform them to EC:
	for (int i = 0; i < 3; i++) {
		vec4 newPoint = mc_ec * vec4(lightPosition[(i*4)], lightPosition[(i*4)+1], lightPosition[(i*4)+2], 1.0);
		lightPosition[(i*4)] = newPoint.x;
		lightPosition[(i*4)+1] = newPoint.y;
		lightPosition[(i*4)+2] = newPoint.z;
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
