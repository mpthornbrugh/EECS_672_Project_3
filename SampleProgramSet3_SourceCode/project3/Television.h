// Television.h

#ifndef Television_H
#define Television_H

#include "ModelViewWithPhongLighting.h"

class Television : public ModelViewWithPhongLighting
{
public:
	Television(const cryph::AffPoint& bottomLeftPoint, float width, float height, float depth, float depthScreen, float inColor[3], float inScreenColor[3]);
	virtual ~Television();

	float color[3];
	float screencolor[3];
	float w;
	float h;
	float d;
	float d2;

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	float xmin, xmax, ymin, ymax, zmin, zmax;

	void renderTelevision(float *color, float *colorInner);
	void defineTelevision();
};

#endif
