// Basic_Arrow.c++

#include "Basic_Arrow.h"
#include "ShaderIF.h"

Basic_Arrow::Basic_Arrow(const cryph::AffPoint& connection, const cryph::Matrix4x4 &transformationMat, float inColor[3])
{
	
	bounds[0] = connection.x - 35.0;
	bounds[1] = connection.x + 35.0;
	bounds[2] = connection.y;
	bounds[3] = connection.y + 105.0;
	bounds[4] = connection.z - 25.0;
	bounds[5] = connection.z + 25.0;
	point = connection;
	transMat = transformationMat;
	color[0] = inColor[0];
	color[1] = inColor[1];
	color[2] = inColor[2];
	define_basic_arrow();
	
}

void Basic_Arrow::define_basic_arrow()
{
	float basexmin = point.x - 5.0, 
		basexmax = point.x + 5.0,
		baseymin = point.y - 5.0, 
		baseymax = point.y + 5.0,
		basezmin = point.z - 5.0, 
		basezmax = point.z + 5.0;
	
	
	
	vec3 vtx[] = {
		//Around the base
		{basexmin ,baseymin, basezmin},
		{basexmin ,baseymax, basezmin},
		{(basexmin - 5.0) ,baseymin, ((basezmax + basezmin)/2.0)},
		{(basexmin - 5.0) ,baseymax, ((basezmax + basezmin)/2.0)},
		{basexmin ,baseymin, basezmax},
		{basexmin ,baseymax, basezmax},
		{basexmax ,baseymin, basezmax},
		{basexmax, baseymax, basezmax},
		{(basexmax + 5.0), baseymin, ((basezmax + basezmin)/2.0)},
		{(basexmax + 5.0) ,baseymax, ((basezmax + basezmin)/2.0)},
		{basexmax ,baseymin, basezmin},
		{basexmax ,baseymax, basezmin},
		{basexmin ,baseymin, basezmin},
		{basexmin ,baseymax, basezmin},
		
		//Bottom of the base
		{basexmin ,baseymin, basezmin},
		{basexmin ,baseymin, basezmax},
		{basexmax ,baseymin, basezmax},
		{basexmax ,baseymin, basezmin},
		
		//Sharp edge of arrow front
		{basexmin ,baseymax, basezmin},
		{(basexmin - 5.0) ,baseymax, ((basezmax + basezmin)/2.0)},
		{(basexmin - 15.0) ,(baseymax + 30.0) , (basezmin + 2.0)},
		{(basexmin - 20.0) ,(baseymax + 30.0) , (basezmin + 2.5)},
		{((basexmax + basexmin)/2.0) ,(baseymax + 105.0), (basezmin + 2.0)},
		{((basexmax + basexmin)/2.0) ,(baseymax + 110.0), (basezmin + 5)},
		{(basexmax + 15.0) ,(baseymax + 30.0) , (basezmin + 2.0)},
		{(basexmax + 20.0) ,(baseymax + 30.0) , (basezmin + 2.5)},
		{basexmax ,baseymax, basezmin},
		{(basexmax + 5.0) ,baseymax, ((basezmax + basezmin)/2.0)},
		
		//Sharp edge of arrow back
		{basexmin ,baseymax, basezmax},
		{(basexmin - 5.0) ,baseymax, ((basezmax + basezmin)/2.0)},
		{(basexmin - 15.0) ,(baseymax + 30.0) , (basezmax - 2.0)},
		{(basexmin - 20.0) ,(baseymax + 30.0) , (basezmax - 2.5)},
		{((basexmax + basexmin)/2.0) ,(baseymax + 105.0), (basezmax - 2.0)},
		{((basexmax + basexmin)/2.0) ,(baseymax + 110.0), (basezmax - 5)},
		{(basexmax + 15.0) ,(baseymax + 30.0) , (basezmax - 2.0)},
		{(basexmax + 20.0) ,(baseymax + 30.0) , (basezmax - 2.5)},
		{basexmax ,baseymax, basezmax},
		{(basexmax + 5.0) ,baseymax, ((basezmax + basezmin)/2.0)},
		
		//Flat part of arrow front
		{((basexmax + basexmin)/2.0) ,(baseymax + 105.0), (basezmin + 2.0)},
		{(basexmin - 15.0) ,(baseymax + 30.0) , (basezmin + 2.0)},
		{(basexmax + 15.0) ,(baseymax + 30.0) , (basezmin + 2.0)},
		{basexmin ,baseymax, basezmin},
		{basexmax ,baseymax, basezmin},
		
		//Flat part of arrow back
		{((basexmax + basexmin)/2.0) ,(baseymax + 105.0), (basezmax - 2.0)},
		{(basexmin - 15.0) ,(baseymax + 30.0) , (basezmax - 2.0)},
		{(basexmax + 15.0) ,(baseymax + 30.0) , (basezmax - 2.0)},
		{basexmin ,baseymax, basezmax},
		{basexmax ,baseymax, basezmax}
	};

	for (int i = 0; i < 48; i++) {
		//This needs work
		//vtx[i] = vtx[i] * transMat;
	}
	
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 48*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

Basic_Arrow::~Basic_Arrow()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Basic_Arrow::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = bounds[0];
	xyzLimits[1] = bounds[1];
	xyzLimits[2] = bounds[2]; 
	xyzLimits[3] = bounds[3]; 
	xyzLimits[4] = bounds[4];    
	xyzLimits[5] = bounds[5];
}

void Basic_Arrow::render_basic_arrow(float *color)
{
	vec4 kd, ka;

	kd[0] = ka[0] = color[0];
	kd[1] = ka[1] = color[1];
	kd[2] = ka[2] = color[2];
	kd[3] = ka[3] = 1.0;

	vec4 ks = {1.0, 1.0, 1.0, 1.0};

	float m = 100.0;

	sendPhongLightModel(ka, kd, ks, m);

	glBindVertexArray(vao[0]);
	//glUniform3fv(ppuLoc_kd, 1, color);	
	//glUniform3fv(ppuLoc_ka, 1, color);
	//glUniform3fv(ppuLoc_ks, 1, color);
	//glUniform1f (ppuLoc_m, m);

	//Base Left Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Base Left Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);

	//Base Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	//Base Right Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 6, 4);

	//Base Right Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	//Base Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);

	//Base Bottom Face
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);

	//Sharp Bottom Left Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, -0.25, -0.25, 0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 18, 4);

	//Sharp Top Left Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, -0.25, 0.25, 0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

	//Sharp Top Right Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.25, 0.25, 0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 22, 4);

	//Sharp Bottom Right Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.25, -0.25, 0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);

	//Sharp Bottom Left Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, -0.25, -0.25, -0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);

	//Sharp Top Left Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, -0.25, 0.25, -0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 30, 4);

	//Sharp Top Right Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.25, 0.25, -0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);

	//Sharp Bottom Right Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.25, -0.25, -0.75);
	glDrawArrays(GL_TRIANGLE_STRIP, 34, 4);

	//Flat Front Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP,  38, 5);

	//Flat Back Face
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 43, 5);
	
}

void Basic_Arrow::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	render_basic_arrow(color);

	glUseProgram(pgm);
}
