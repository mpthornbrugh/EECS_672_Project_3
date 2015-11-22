// Bedfeet.c++

#include "Bedfeet.h"
#include "ShaderIF.h"

cryph::AffVector direction;

Bedfeet::Bedfeet(float inRadius, const cryph::AffPoint& bottom_midpoint, const cryph::AffPoint& top_midpoint, bool capped, float inColor[3])
{
	radius = inRadius;
	bottom = bottom_midpoint;
	top = top_midpoint;
	color[0] = inColor[0];
	color[1] = inColor[1];
	color[2] = inColor[2];
	direction = (top - bottom);
	direction.normalize();

	defineBedfeet();
	initializeCappingIndices();
	setBounds();
}

Bedfeet::~Bedfeet()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Bedfeet::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = bounds[0];
	xyzLimits[1] = bounds[1];
	xyzLimits[2] = bounds[2]; 
	xyzLimits[3] = bounds[3]; 
	xyzLimits[4] = bounds[4];    
	xyzLimits[5] = bounds[5];
}

void Bedfeet::defineBedfeet()
{
	int N_POINTS_AROUND_SLICE = 18;	

	typedef float vec3[3];
	int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];
	double theta = 0.0;
	double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;

	
	
	cryph::AffVector currVToPoint = getStartVector(); 
	cryph::AffVector moveDirection = direction.cross(currVToPoint);
	cryph::AffPoint currpoint_b = bottom + radius*currVToPoint;
	cryph::AffPoint currpoint_t = top + radius*currVToPoint;
	

	cryph::AffVector paralleltrash;
	cryph::AffVector perp_b;

	perp_b = (currpoint_b-bottom).cross(currpoint_t-currpoint_b).cross(currpoint_t-currpoint_b);
	
	
	for (int i=0 ; i<=N_POINTS_AROUND_SLICE ; i++)
	{
		int j = 2 * i;

		normals[j][0] = perp_b.dx; normals[j][1] = perp_b.dy; normals[j][2] = perp_b.dz;
		coords[j][0] = currpoint_b.x; coords[j][1] = currpoint_b.y; coords[j][2] = currpoint_b.z;
		
		normals[j+1][0] = perp_b.dx; normals[j+1][1] = perp_b.dy; normals[j+1][2] = perp_b.dz;
		coords[j+1][0] = currpoint_t.x; coords[j+1][1] = currpoint_t.y; coords[j+1][2] =currpoint_t.z;
		
		if(i == N_POINTS_AROUND_SLICE)
		  break;
		moveDirection = direction.cross(currVToPoint); 
		currpoint_b = bottom + ((radius*cos(dTheta))*currVToPoint + ((radius * sin(dTheta)) *moveDirection));
		currpoint_t = top + ((radius*cos(dTheta))*currVToPoint + ((radius * sin(dTheta)) *moveDirection));
		currVToPoint =(currpoint_b-bottom);
		currVToPoint.normalize();
	
		cryph::AffVector helperV = (currpoint_b-bottom).cross(currpoint_t-currpoint_b);
		perp_b = helperV.cross(currpoint_t-currpoint_b);
		
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), coords, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcNormal);

	delete [] coords;
	delete [] normals;
	
}

cryph::AffVector Bedfeet::getStartVector()
{
	cryph::AffVector v(direction.dx-1,direction.dy,direction.dz-1);
  
	cryph::AffVector result = v.cross(direction);
	 
	result.normalize();
	return result;
}

void Bedfeet::setBounds()
{
	//x
	bounds[0] = bottom.x - radius;
	bounds[1] = bottom.x + radius;
	//y
	bounds[2] = bottom.y - radius;
	bounds[3] = top.y + radius;
	//z
	bounds[4] = bottom.z - radius;
	bounds[5] = bottom.z + radius;
}

void Bedfeet::initializeCappingIndices()
{	
	int N_POINTS_AROUND_SLICE = 18;	

	for (int i=0 ; i<=N_POINTS_AROUND_SLICE ; i++)
	{
		unsigned int j = 2 * i;
		bottomCap[i]=j;
		topCap[i]=j+1;
	}
}

void Bedfeet::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));\

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	int N_POINTS_AROUND_SLICE = 18;

	vec4 kd, ka;

	kd[0] = ka[0] = color[0];
	kd[1] = ka[1] = color[1];
	kd[2] = ka[2] = color[2];
	kd[3] = ka[3] = 1.0;

	vec4 ks = {0.8, 0.8, 0.8, 1.0};

	float m = 10.0;

	sendPhongLightModel(ka, kd, ks, m);

	//glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));

	glUseProgram(pgm);
}
