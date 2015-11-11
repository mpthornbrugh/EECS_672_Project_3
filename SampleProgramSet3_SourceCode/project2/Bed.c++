// Bed.c++

#include "Bed.h"
#include "ShaderIF.h"

GLuint Block::indexList[3][4] = {
	{ 6, 7, 0, 1 }, // xmin face
	{ 6, 0, 4, 2 }, // ymin face
	{ 1, 7, 3, 5 }  // ymax face
};

Bed::Bed(const cryph::AffPoint& footLeft, const cryph::AffPoint& footRight, const cryph::AffPoint& headLeft, const cryph::AffPoint& headRight, float height)
{
	xmin = footLeft.x; xmax = footRight.x;
	ymin = footLeft.y; ymax = footLeft.y - height;
	zmin = footLeft.z; zmax = headLeft.z;

	cryph::AffPoint p0(footLeft.x, footLeft.y-height, footLeft.z);
	cryph::AffPoint p1(footRight.x, footRight.y-height, footRight.z);
	cryph::AffPoint p2(headLeft.x, headLeft.y-height, headLeft.z);
	cryph::AffPoint p3(headRight.x, headRight.y-height, headRight.z);
	cryph::AffPoint verts[] = { p0, footLeft, p1, footRight, p3, headRight, p2, headLeft };
	defineBed(verts);
	minMax[0] = minMax[1] = footLeft.x;
	minMax[2] = minMax[3] = footLeft.y;
	minMax[4] = minMax[5] = footLeft.z;
	updateXYZBounds(footLeft);
	updateXYZBounds(footRight);
	updateXYZBounds(headLeft);
	updateXYZBounds(headRight);
	updateXYZBounds(p0);
	updateXYZBounds(p1);
	updateXYZBounds(p2);
	updateXYZBounds(p3);
}

Bed::~Bed()
{
	glDeleteBuffers(3,ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Bed::defineBed(const cryph::AffPoint verts[])
{
	typedef float vec3[3];

	// We need EIGHT vertices for GL_TRIANGLE_STRIP
	vec3 vtx[8];
	for (int i=0 ; i<8 ; i++)
		verts[i].aCoords(vtx, i);
		

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);
	
	glGenBuffers(3, ebo);
	for (int i=0 ; i<3 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}


// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Bed::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Bed::renderBed()
{
	float color[] = { 0.7, 0.7, 0.7 };

	glBindVertexArray(vao[0]);
	glUniform3fv(ppuLoc_kd, 1, color);
	
	// The three faces that can be drawn with glDrawArrays
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	// The three faces that are drawn with glDrawElements
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, -1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
}

void Bed::render()
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
	renderBed();
	
	glUseProgram(pgm);
}

void Bed::updateXYZBounds(const cryph::AffPoint& p)
{
	if (p.x < minMax[0])
		minMax[0] = p.x;
	else if (p.x > minMax[1])
		minMax[1] = p.x;

	if (p.y < minMax[2])
		minMax[2] = p.y;
	else if (p.y > minMax[3])
		minMax[3] = p.y;

	if (p.z < minMax[4])
		 minMax[4] = p.z;
	else if (p.z > minMax[5])
		minMax[5] = p.z;
}