// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Bedfeet.h"
#include "Bedfeet.c++"
#include "Block.h"
#include "Block.c++"
#include "Sphere.h"
#include "Sphere.c++"
#include "Basic_Arrow.h"
#include "Basic_Arrow.c++"

typedef float vec3[3];

void addBasicArrowHead(Controller &c, const cryph::AffPoint& connectionPoint)
{
    float color[3] = {0.0/255.0, 255.0/255.0, 0.0/255.0};

    c.addModel(new Basic_Arrow(connectionPoint, color));
}

void addTelevision(Controller &c, const cryph::AffPoint& bottomLeftPoint, float width, float height, float depth, float depthScreen)
{
	float colorOuter[3] = {0.0,0.0,0.0};
	float colorInner[3] = {50.0/255.0, 50.0/255.0, 50.0/255.0};

	//Tv Base
	c.addModel(new Block(bottomLeftPoint.x, bottomLeftPoint.y, bottomLeftPoint.z, width, 30.0, depth, colorOuter));
	
	//Tv Stand
	cryph::AffPoint p0(bottomLeftPoint.x + width/2.0, bottomLeftPoint.y + 30.0, bottomLeftPoint.z + depth/2.0);
	cryph::AffPoint p1(p0.x, p0.y + 20.0, p0.z);
	c.addModel(new Bedfeet(depth/2.0, p0, p1, false, colorOuter));

	cryph::AffPoint tvPoint(bottomLeftPoint.x, bottomLeftPoint.y + 50.0, bottomLeftPoint.z);
	//Tv Outer
	c.addModel(new Block(tvPoint.x, tvPoint.y, tvPoint.z, width/10.0, height, depth, colorOuter));
	c.addModel(new Block(tvPoint.x, tvPoint.y, tvPoint.z, width, height/10.0, depth, colorOuter));
	c.addModel(new Block(tvPoint.x + 9.0*width/10.0, tvPoint.y, tvPoint.z, width/10.0, height, depth, colorOuter));
	c.addModel(new Block(tvPoint.x, tvPoint.y + 9.0*height/10.0, tvPoint.z, width, height/10.0, depth, colorOuter));
	//Tv Inner
	c.addModel(new Block(tvPoint.x + width/10.0, tvPoint.y + height/10.0, tvPoint.z + depthScreen, width - width/5.0, height - height/5.0, depth - depthScreen, colorInner));
}

void addBed(Controller &c, const cryph::AffPoint& headLeftBottomPoint, float bedWidth, float bedLength, float bedHeight, float feetHeight, float feetWidth)
{
	//Bed Mattress
	float bedColor[3] = {240.0/255.0, 248.0/255.0, 255.0/255.0};
	c.addModel(new Block(headLeftBottomPoint.x, headLeftBottomPoint.y + feetHeight, headLeftBottomPoint.z, bedWidth, bedHeight, bedLength, bedColor));

	//All feet are 3 inches in from the x and z ways

	//float radius, const cryph::AffPoint& bottom_midpoint, const cryph::AffPoint& top_midpoint, bool capped, float inColor[3]
	//Head Left Foot
	cryph::AffPoint p0(headLeftBottomPoint.x + 30.0, headLeftBottomPoint.y, headLeftBottomPoint.z + 30.0);
	cryph::AffPoint p1(headLeftBottomPoint.x + 30.0, headLeftBottomPoint.y + feetHeight, headLeftBottomPoint.z + 30.0);
	float color[3] = {47.0/255.0, 79.0/255.0, 79.0/255.0};
	c.addModel(new Bedfeet(feetWidth/2.0, p0, p1, false, color));

	//Head Right Foot
	p0.assign(headLeftBottomPoint.x + bedWidth - 30.0, headLeftBottomPoint.y, headLeftBottomPoint.z + 30.0);
	p1.assign(headLeftBottomPoint.x + bedWidth - 30.0, headLeftBottomPoint.y + feetHeight, headLeftBottomPoint.z + 30.0);
	c.addModel(new Bedfeet(feetWidth/2.0, p0, p1, false, color));

	//Foot Left Foot
	p0.assign(headLeftBottomPoint.x + 30.0, headLeftBottomPoint.y, headLeftBottomPoint.z + bedLength - 30.0);
	p1.assign(headLeftBottomPoint.x + 30.0, headLeftBottomPoint.y + feetHeight, headLeftBottomPoint.z + bedLength - 30.0);
	c.addModel(new Bedfeet(feetWidth/2.0, p0, p1, false, color));

	//Foot Right Foot
	p0.assign(headLeftBottomPoint.x + bedWidth - 30.0, headLeftBottomPoint.y, headLeftBottomPoint.z + bedLength - 30.0);
	p1.assign(headLeftBottomPoint.x + bedWidth - 30.0, headLeftBottomPoint.y + feetHeight, headLeftBottomPoint.z + bedLength - 30.0);
	c.addModel(new Bedfeet(feetWidth/2.0, p0, p1, false, color));
}

void addBookcase(Controller &c, const cryph::AffPoint& bottomFrontLeftPoint, float height, float width, float depth)
{
	//Making all shelf parts 1 inch thick

	//Each shelf is 12 inches from the last thing -1 because there won't be any on the top
	int numShelves = (height/120.0) - 1;

	//Bottom of bookcase
	float color[3] = {184.0/255.0, 134.0/255.0, 11.0/255.0};
	c.addModel(new Block(bottomFrontLeftPoint.x + 10.0, bottomFrontLeftPoint.y, bottomFrontLeftPoint.z, width - 20.0, 10.0, depth - 10.0, color));

	//Top of bookcase
	c.addModel(new Block(bottomFrontLeftPoint.x + 10.0, bottomFrontLeftPoint.y + height - 10.0, bottomFrontLeftPoint.z, width - 20.0, 10.0, depth - 10.0, color));

	//Back of bookcase
	c.addModel(new Block(bottomFrontLeftPoint.x, bottomFrontLeftPoint.y, bottomFrontLeftPoint.z, width, height, 10.0, color));

	//Right of bookcase
	c.addModel(new Block(bottomFrontLeftPoint.x + width - 10.0, bottomFrontLeftPoint.y, bottomFrontLeftPoint.z, 10.0, height, depth - 10.0, color));

	//Left of bookcase
	c.addModel(new Block(bottomFrontLeftPoint.x, bottomFrontLeftPoint.y, bottomFrontLeftPoint.z, 10.0, height, depth - 10.0, color));

	float shelfHeight = bottomFrontLeftPoint.y + 120.0;

	//Add the shelves
	for (int i = 0; i < numShelves; i++) {
		c.addModel(new Block(bottomFrontLeftPoint.x + 10.0, shelfHeight, bottomFrontLeftPoint.z, width - 20.0, 10.0, depth - 10.0, color));
		shelfHeight += 120.0;
	}
}

// The position variable is either 1,2,3 or 4 for the direction that the fan blade is facing.
void addFanblade(Controller &c, const cryph::AffPoint& connectionPoint, float width, float length, float depth, int position)
{
	float lengthConnector = length/5.0;
	float middleSectionLength = 3.0*lengthConnector;
	float lengthCapPiece = lengthConnector;
	float widthConnector = width/3.0;
	float fanColor[3] = {127.0/255.0, 255.0/255.0, 212.0/255.0};
	if (position == 1) { //Go along x axis
		//Block (xlow, ylow, zlow, xLength, yLength, zLength)
		//HalfCyl (width, height, bottom_midpoint, startAngle)

		//Connector
		c.addModel(new Block(connectionPoint.x, connectionPoint.y, connectionPoint.z - widthConnector/2.0, lengthConnector, depth, widthConnector, fanColor));

		//Middle Section
		c.addModel(new Block(connectionPoint.x + lengthConnector, connectionPoint.y, connectionPoint.z - width/2.0, middleSectionLength, depth, width, fanColor));

		//Cap
		cryph::AffPoint cylPoint (connectionPoint.x + lengthConnector + middleSectionLength, connectionPoint.y, connectionPoint.z);
		cryph::AffPoint cylPointTop (cylPoint.x, cylPoint.y + depth, cylPoint.z);
		c.addModel(new Bedfeet(width/2.0, cylPoint, cylPointTop, false, fanColor));
	}
	else if (position == 2) { //Go along z axis
		//Connector
		c.addModel(new Block(connectionPoint.x - widthConnector/2.0, connectionPoint.y, connectionPoint.z, widthConnector, depth, lengthConnector, fanColor));

		//Middle Section
		c.addModel(new Block(connectionPoint.x - width/2.0, connectionPoint.y, connectionPoint.z + lengthConnector, width, depth, middleSectionLength, fanColor));

		//Cap
		cryph::AffPoint cylPoint (connectionPoint.x, connectionPoint.y, connectionPoint.z - lengthConnector - middleSectionLength);
		cryph::AffPoint cylPointTop (cylPoint.x, cylPoint.y + depth, cylPoint.z);
		c.addModel(new Bedfeet(width/2.0, cylPoint, cylPointTop, false, fanColor));
	}
	else if (position == 3) { //Go along -x axis
		//Connector
		c.addModel(new Block(connectionPoint.x - lengthConnector, connectionPoint.y, connectionPoint.z - widthConnector/2.0, lengthConnector, depth, widthConnector, fanColor));

		//Middle Section
		c.addModel(new Block(connectionPoint.x - lengthConnector - middleSectionLength, connectionPoint.y, connectionPoint.z - width/2.0, middleSectionLength, depth, width, fanColor));

		//Cap
		cryph::AffPoint cylPoint (connectionPoint.x + lengthConnector + middleSectionLength, connectionPoint.y, connectionPoint.z);
		cryph::AffPoint cylPointTop (cylPoint.x, cylPoint.y + depth, cylPoint.z);
		c.addModel(new Bedfeet(width/2.0, cylPoint, cylPointTop, false, fanColor));
	}
	else if (position == 4) { //Go along -z axis
		//Connector
		c.addModel(new Block(connectionPoint.x - widthConnector/2.0, connectionPoint.y, connectionPoint.z - lengthConnector, widthConnector, depth, lengthConnector, fanColor));

		//Middle Section
		c.addModel(new Block(connectionPoint.x - width/2.0, connectionPoint.y, connectionPoint.z - middleSectionLength, width, depth, middleSectionLength, fanColor));

		//Cap
		cryph::AffPoint cylPoint (connectionPoint.x + lengthConnector + middleSectionLength, connectionPoint.y, connectionPoint.z);
		cryph::AffPoint cylPointTop (cylPoint.x, cylPoint.y + depth, cylPoint.z);
		c.addModel(new Bedfeet(width/2.0, cylPoint, cylPointTop, true, fanColor));
	}
}



void addFan(Controller &c, const cryph::AffPoint& topMiddlePoint, float baseHeight, float baseWidth, float connectorHeight, float connectorWidth, float sphereRadius)
{
	float fanBaseColor[3] = {255.0/255.0, 248.0/255.0, 220.0/255.0};
	//Base
	cryph::AffPoint connectorTopPoint(topMiddlePoint.x, topMiddlePoint.y - baseHeight, topMiddlePoint.z);
	c.addModel(new Bedfeet(baseWidth/2.0, topMiddlePoint, connectorTopPoint, true, fanBaseColor));
	//Connector
	cryph::AffPoint connectorBottomPoint(connectorTopPoint.x, connectorTopPoint.y - connectorHeight, connectorTopPoint.z);
	c.addModel(new Bedfeet(connectorWidth/2.0, connectorTopPoint, connectorBottomPoint, true, fanBaseColor));	

	cryph::AffPoint connectionPoint(connectorBottomPoint.x + connectorWidth/2.0, connectorBottomPoint.y + connectorHeight/2.0, connectorBottomPoint.z);

	//Fan Blades
	addFanblade(c, connectionPoint, 80.0, 360.0, 5.0, 1);
	connectionPoint.assign(connectorBottomPoint.x, connectorBottomPoint.y + connectorHeight/2.0, connectorBottomPoint.z + connectorWidth/2.0);
	addFanblade(c, connectionPoint, 80.0, 360.0, 5.0, 2);
	connectionPoint.assign(connectorBottomPoint.x - connectorWidth/2.0, connectorBottomPoint.y + connectorHeight/2.0, connectorBottomPoint.z);
	addFanblade(c, connectionPoint, 80.0, 360.0, 5.0, 3);
	connectionPoint.assign(connectorBottomPoint.x, connectorBottomPoint.y + connectorHeight/2.0, connectorBottomPoint.z - connectorWidth/2.0);
	addFanblade(c, connectionPoint, 80.0, 360.0, 5.0, 4);

	//Light
	vec3 sphereLocation = {connectorBottomPoint.x, connectorBottomPoint.y - sphereRadius/2.5, connectorBottomPoint.z};
	float lightColor[3] = {254.0/255.0, 254.0/255.0, 254.0/255.0};
	c.addModel(new Sphere(sphereLocation, sphereRadius/2.0, lightColor));
}

void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);

        double xmid = 0.5 * (xyz[0] + xyz[1]);
        double ymid = 0.5 * (xyz[2] + xyz[3]);
        double zmid = 0.5 * (xyz[4] + xyz[5]);
        cryph::AffPoint center(xmid, ymid, zmid);
        
        double maxDelta = xyz[1] - xyz[0];
        double delta = xyz[3] - xyz[2];
        if (delta > maxDelta)
            maxDelta = delta;
        delta = xyz[5] - xyz[4];
        if (delta > maxDelta)
            maxDelta = delta;
        double distEyeCenter = 2.0 * maxDelta;
        cryph::AffPoint eye(xmid, ymid, zmid + 1.25*distEyeCenter);
        
	cryph::AffVector up = cryph::AffVector::yu;

	// Set values for eye-center-up to produce a reasonable off-axis
	// view of your scene, then:

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	double ecZpp = -(distEyeCenter - 0.5*maxDelta);
        double ecZmin = ecZpp - maxDelta*2.0;
        double ecZmax = ecZpp + 0.5*maxDelta;

	// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
	// the EC system established above, then:

	ModelView::setProjection(PERSPECTIVE);
	ModelView::setProjectionPlaneZ(ecZpp);
	ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{
	GLFWController c("Bedroom", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	// create your scene, adding things to the Controller....
	cryph::AffPoint p0(1320.0,0.0,0.0);
	addBasicArrowHead(c, p0);

	cryph::AffPoint p1(1300.0,0.0,-50.0);
	//addBookcase(c, p1, 480.0, 480.0, 120.0);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
