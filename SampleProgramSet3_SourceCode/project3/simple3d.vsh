#version 410 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.             

const int MAX_NUM_LIGHTS = 3;
uniform vec4 p_ecLightPos[MAX_NUM_LIGHTS];
uniform vec3 lightStrength[MAX_NUM_LIGHTS];
uniform vec3 globalAmbient;
uniform vec3 ka, kd, ks;
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
    vec3 vHat = 

    // if we are viewing this point "from behind", we need to negate the incoming
    // normal vector since our lighting model expressions implicitly assume the normal
    // vector points toward the same side of the triangle that the eye is on.
    …

    for (int i = 0 ; i < 3 ; i++)
    {
        // if light is behind this object, skip this light source
        // else:
        //     1. compute and accumulate diffuse contribution
        //     2. if viewer on appropriate side of normal vector,
        //        compute and accumulate specular contribution.
    }





	float factor = ka * la;
	for (int i = 0; i < numLights; i++) {
		factor += 
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
