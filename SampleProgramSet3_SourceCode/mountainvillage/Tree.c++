// Tree.c++ - concrete subclass of ModelView that represents a Tree

#include <iostream>
#include <cmath>

#include "Tree.h"
#include "ShaderIF.h"

typedef float vec2[2];

ShaderIF* Tree::shaderIF = NULL;
int Tree::numInstances = 0;
GLuint Tree::shaderProgram = 0;
GLint Tree::pvaLoc_mcPosition = -2;
GLint Tree::ppuLoc_scaleTrans = -2;
GLint Tree::ppuLoc_treePart = -2;

Tree::Tree(double xbIn, double ybIn, double rTreeTopIn, double rTrunkIn, double heightIn,
	int numTreeTopPointsIn, int numTrunkPointsIn) :
	xb(xbIn), yb(ybIn), rTreeTop(rTreeTopIn), rTrunk(rTrunkIn), height(heightIn),
	numTreeTopPoints(numTreeTopPointsIn), numTrunkPoints(numTrunkPointsIn)
{
	if (Tree::shaderProgram == 0)
	{
		// The first time an instance of this class is generated, create the common shader program:
		Tree::shaderIF = new ShaderIF("Tree.vsh", "Tree.fsh");
		Tree::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}

	// Now do instance-specific initialization:
	validateData();
	defineModel();
	Tree::numInstances++;
}

Tree::~Tree()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
	if (--Tree::numInstances == 0)
	{
		Tree::shaderIF->destroy();
		delete Tree::shaderIF;
		Tree::shaderIF = NULL;
		Tree::shaderProgram = 0;
	}
}

void Tree::defineModel()
{
	vao[0] = 0; // delete this line and the following "cout" when completing the exercise
	std::cout << "Tree::defineModel: Implementation left as an exercise.\n";
}

void Tree::fetchGLSLVariableLocations()
{
	if (Tree::shaderProgram > 0)
	{
		Tree::pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		Tree::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
		Tree::ppuLoc_treePart = ppUniformLocation(shaderProgram, "treePart");
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Tree::getMCBoundingBox(double* xyzLimits) const
{
	double dx1 = xb - rTreeTop; // min x of tree top
	double trunkTheta = asin(height/rTrunk);
	double dx2 = xb - rTrunk*(1.0 - cos(trunkTheta)); // min x of base
	if (dx1 < dx2)
		xyzLimits[0] = dx1;
	else
		xyzLimits[0] = dx2;
	xyzLimits[1] = 2.0*xb - xyzLimits[0];
	xyzLimits[2] = yb;
	xyzLimits[3] = yb + height + rTreeTop;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

void Tree::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderProgram);

	// define the mapping from MC to -1..+1 Logical Device Space:
	float scaleTrans[4];
	compute2DScaleTrans(scaleTrans);
	glUniform4fv(Tree::ppuLoc_scaleTrans, 1, scaleTrans);

	glBindVertexArray(vao[0]);
	// draw trunk first because tree top overwrites it
	glUniform1i(Tree::ppuLoc_treePart, 0); // '0' means trunk
	int numTrunkTriStripPoints = 2 * numTrunkPoints;
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTrunkTriStripPoints); // offset: 0
	// then draw tree top
	glUniform1i(Tree::ppuLoc_treePart, 1); // '1' means tree top
	//                            offset:                 num points:
	glDrawArrays(GL_TRIANGLE_FAN, numTrunkTriStripPoints, numTreeTopPoints);
	// restore the previous program
	glUseProgram(pgm);
}

void Tree::validateData()
{
	if (rTreeTop <= 0.0)
		rTreeTop = 1.0;
	if (height <= 0.0)
		height = 1.0;
	if (rTrunk < height)
		rTrunk = height;
	if (numTreeTopPoints < 5)
		numTreeTopPoints = 5;
	if (numTrunkPoints < 2)
		numTrunkPoints = 2;
}
