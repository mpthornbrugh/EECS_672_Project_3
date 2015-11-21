#version 410 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.             

const int MAX_NUM_LIGHTS = 3;
uniform vec4 p_ecLightPos[MAX_NUM_LIGHTS];
uniform vec3 lightStrength[MAX_NUM_LIGHTS];
uniform vec3 globalAmbient;
uniform vec3 ka, kd, ks;
uniform vec3 eye;
uniform float m = 1.0;
uniform mat4 ec_lds;

// Per-vertex attributes
// 1. incoming vertex position in model coordinates
layout (location = 0) in vec3 mcPosition;
// 2. incoming vertex normal vector in model coordinates
in vec3 mcNormal; // incoming normal vector in model coordinates

out vec3 colorToFS;

vec3 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat)
{
	// Create a unit vector towards the viewer (method depends on type of projection!)
    // NOTE: GLSL matrices are indexed as M[col][row]!!!
    cryph::AffVector vHat = normalize(eye - ec_Q);

    // if we are viewing this point "from behind", we need to negate the incoming
    // normal vector since our lighting model expressions implicitly assume the normal
    // vector points toward the same side of the triangle that the eye is on.
    if (ec_nHat.dot(vHat) > 90 || ec_nHat.dot(vHat) < -90) {
    	ec_nHat = -1.0 * ec_nHat;
    }

    cryph::AffVector vHatParallel, vHatPerpendicular;

    decompose(vHat, vHatParallel, vHatPerpendicular);

    vec3 lightVal = ka * globalAmbient;

    for (int i = 0 ; i < MAX_NUM_LIGHTS ; i++)
    {
    	vec3 diffuseLight, specularLight;
    	if (p_ecLightPos[i].a > 0.0) {//Positional light
    		cryph::AffVector lightParallel, lightPerpendicular;
	    	cryph::AffVector li = normalize(ec_Q - p_ecLightPos[i]);

	    	decompose(li, lightParallel, lightPerpendicular);

	    	//Attenuation Part
	    	//Compute diffuse contribution
	    	vec3 diffuseLight = kd * (dot(ec_nHat, li));

	    	if (lightParallel.dot(vHatParallel) >= 90 || lightParallel.dot(vHatParallel) <= -90) {//Viewer is on right side
	    		vec3 riHat = ec_nHat;
	    		riHat = riHat * 2;
	    		riHat = riHat * li.dot(ec_nHat);
	    		riHat = riHat - li;

	    		specularLight = ks;
	    		double dotVal = riHat.dot(vHat);
	    		for (int j = 0; j < m; j++) {
	    			specularLight = specularLight * (dot())
	    		}
	    	}
	    	else {
	    		specularLight = {0.0,0.0,0.0};
	    	}
    	}
    	else { //Directional Light
    		//Compute diffuse contribution
    		//Compute specular contribution
    	}

    	lightVal = lightVal + (lightStrength[i] * (diffuseLight + specularLight));
    }




	// Simplistic lighting model:
	// Reflected color will be a function of angle between the normal
	// vector and the light source direction. Specifically:

	// NOTE: The use of "abs" here is a temporary hack. As we study
	//       lighting models more carefully, we will REMOVE "abs" while
	//       incorporating more sophisticated processing.

	//Iq = Ka * La + sum( Fi(Q) Li )

	float factor = abs(dot(normalize(p_ecLightSource.xyz), ec_nHat));

	return factor * kd;
}

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = mc_ec * vec4(mcPosition, 1.0);
	mat3 normalMatrix = transpose( inverse( mat3x3(mc_ec) ) );
	vec3 ec_nHat = normalize(normalMatrix * mcNormal);

	colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);

	// apply the projection matrix to compute the projective space representation
	// of the 3D logical device space coordinates of the input vertex:
	gl_Position = ec_lds * p_ecPosition;
}
