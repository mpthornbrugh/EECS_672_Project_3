// Television.c++

#include "Television.h"
#include "ShaderIF.h"

typedef float vec3[3];

Television::Television(const cryph::AffPoint& bottomLeftPoint, float width, float height, float depth, float depthScreen, float inColor[3], float inScreenColor[3])
{
	xmin = bottomLeftPoint.x;
	xmax = bottomLeftPoint.x + width;
	ymin = bottomLeftPoint.y;
	ymax = bottomLeftPoint.y + height;
	zmin = bottomLeftPoint.z;
	zmax = bottomLeftPoint.z + depth;

	w = width;
	h = height;
	d = depth;
	d2 = depthScreen;

	color[0] = inColor[0];
	color[1] = inColor[1];
	color[2] = inColor[2];

	screencolor[0] = inScreenColor[0];
	screencolor[1] = inScreenColor[1];
	screencolor[2] = inScreenColor[2];	
}

void Television::defineTelevision()
{
	float inxmin, inxmax, inymin, inymax, inzmin, inzmax;

	inxmin = xmin + w/10.0;
	inxmax = xmax - w/10.0;
	inymin = ymin + h/10.0;
	inymax = ymax - h/10.0;
	inzmin = zmin;
	inzmax = zmin + d2;
	//Need a of total 34 vertices (some are used multiple times)
	vec3 vtx[] = {
		{xmin ,ymin, zmax},
		{xmin, ymin, zmin},
		{xmin, ymax, zmax}, 
		{xmin, ymax, zmin}, 
		{xmax, ymax, zmax}, 
		{xmax, ymax, zmin}, 
		{xmax, ymin, zmax}, 
		{xmax, ymin, zmin}, 
		{xmin ,ymin, zmax}, 
		{xmin, ymin, zmin},
		{inxmin, inymin, inzmax}, 
		{inxmin, inymin, inzmin}, 
		{inxmin, inymax, inzmax}, 
		{inxmin, inymax, inzmin}, 
		{inxmax, inymax, inzmax}, 
		{inxmax, inymax, inzmin}, 
		{inxmax, inymin, inzmax}, 
		{inxmax, inymin, inzmin}, 
		{inxmin, inymin, inzmax}, 
		{inxmin, inymin, inzmin},
		{xmin, ymin, zmin}, 
		{inxmin, inymin, inzmin}, 
		{xmax, ymin, zmin}, 
		{inxmax, inymin, inzmin}, 
		{xmax, ymax, zmin}, 
		{inxmax, inymax, inzmin}, 
		{xmin, ymax, zmin}, 
		{inxmin, inymax, inzmin}, 
		{xmin, ymin, zmin}, 
		{inxmin, inymin, inzmin}, 
		{xmin ,ymin, zmax}, 
		{xmin, ymax, zmax}, 
		{xmax, ymin, zmax}, 
		{xmax, ymax, zmax},
		{inxmin, inymin, inzmax}, 
		{inxmin, inymax, inzmax}, 
		{inxmax, inymax, inzmax}, 
		{inxmax, inymin, inzmax} 
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

Television::~Television()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Television::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Television::renderTelevision(float *color, float *colorInner)
{
	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);

	//Left Face
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//Top Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	//Right Face
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	//Bottom Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, -1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 6, 4);
	//Inner Left Face
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);
	//Inner Top Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, -1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	//Inner Right Face
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);
	//Inner Bottom Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	//Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 10);
	//Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 30, 4);

	glUniform3fv(ppuLoc_kd, 1, colorInner);
	//Inner Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 34, 4);
	
	
}

void Television::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	float black[] = { 0.0, 0.0, 0.0 };

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderTelevision(color, screencolor);

	glUseProgram(pgm);
}
