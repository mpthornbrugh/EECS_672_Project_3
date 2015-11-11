// Bed.h

#ifndef Bed_H
#define Bed_H

#include "ModelViewWithPhongLighting.h"

class Bed : public ModelViewWithPhongLighting
{
public:
	Bed(const cryph::AffPoint& footLeft, const cryph::AffPoint& footRight, const cryph::AffPoint& headLeft, const cryph::AffPoint& headRight, float height);
	virtual ~Bed();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors
	GLuint ebo[3];

	float xmin, xmax, ymin, ymax, zmin, zmax;

	static GLuint indexList[3][4];

	void defineBed(const cryph::AffPoint verts[]);
	void renderBlock(float* color);
	void updateXYZBounds(const cryph::AffPoint& p);
};

#endif
