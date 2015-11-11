// HalfCylinder.h

#ifndef HalfCylinder_H
#define HalfCylinder_H

#include "ModelViewWithPhongLighting.h"

class HalfCylinder : public ModelViewWithPhongLighting
{
public:
	HalfCylinder(float width, float height, const cryph::AffPoint& bottom_midpoint, float startAngle);
	virtual ~HalfCylinder();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	float radius, xmin, xmax, ymin, ymax, zmin, zmax;

	void renderHalfCylinder();
};

#endif
