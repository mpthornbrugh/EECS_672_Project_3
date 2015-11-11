// Fancenter.c++

#include "Fancenter.h"
#include "ShaderIF.h"

Fancenter::Fancenter(float widthBase, float heightBase, float widthConnectionSpace, float heightConnectionSpace, float radiusSphere, const cryph::AffPoint& midPointSphere)
{
}

Fancenter::~Fancenter()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Fancenter::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1000.0; // xmin  Give real values!
	xyzLimits[1] = 1000.0;  // xmax         |
	xyzLimits[2] = -1234.5; // ymin         |
	xyzLimits[3] = -1011.2; // ymax         |
	xyzLimits[4] = -3000.0; // zmin         |
	xyzLimits[5] = -2000.0; // zmax        \_/
}

void Fancenter::render()
{
}
