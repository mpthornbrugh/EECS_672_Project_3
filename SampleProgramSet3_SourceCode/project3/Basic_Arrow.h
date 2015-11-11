// Basic_Arrow.h

#ifndef Basic_Arrow_H
#define Basic_Arrow_H

#include "ModelViewWithPhongLighting.h"

class Basic_Arrow : public ModelViewWithPhongLighting
{
public:
	Basic_Arrow(const cryph::AffPoint& connection, float inColor[3]);
	virtual ~Basic_Arrow();
	
	cryph::AffPoint point;
	float bounds[6];
	float color[3];

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao[1];
	GLuint vbo[1];
	void define_basic_arrow();
	void render_basic_arrow(float *color);
};

#endif
