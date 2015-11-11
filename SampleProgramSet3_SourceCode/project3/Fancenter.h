// Fancenter.h

#ifndef Fancenter_H
#define Fancenter_H

#include "ModelViewWithPhongLighting.h"

class Fancenter : public ModelViewWithPhongLighting
{
public:
	Fancenter(float widthBase, float heightBase, float widthConnectionSpace, float heightConnectionSpace, float radiusSphere, const cryph::AffPoint& midPointSphere);
	virtual ~Fancenter();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
};

#endif
