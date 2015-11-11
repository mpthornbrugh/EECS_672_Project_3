// Bedfeet.h

#ifndef Bedfeet_H
#define Bedfeet_H

#include "ModelViewWithPhongLighting.h"

class Bedfeet : public ModelViewWithPhongLighting
{
public:
	Bedfeet(float inRadius, const cryph::AffPoint& bottom_midpoint, const cryph::AffPoint& top_midpoint, bool capped, float inColor[3]);
	virtual ~Bedfeet();

	cryph::AffPoint bottom;
	cryph::AffPoint top;
	float bounds[6];
	float color[3];
	float radius;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	bool capped;
	unsigned int topCap[1000];
	unsigned int bottomCap[1000];
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	cryph::AffVector getStartVector();
	void defineBedfeet();
	void setBounds();
	void initializeCappingIndices();
};

#endif
