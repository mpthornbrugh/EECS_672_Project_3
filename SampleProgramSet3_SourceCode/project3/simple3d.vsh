#version 410 core

// simple3d.vsh - A simplistic vertex shader that illustrates ONE WAY
//                to handle transformations along with a simple 3D
//                lighting model.

const int MAX_NUM_LIGHTS = 3;

// Transformation Matrices
uniform mat4 mc_ec =                                
	mat4(1.0, 0.0, 0.0, 0.0,
	     0.0, 1.0, 0.0, 0.0,
	     0.0, 0.0, 1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
uniform mat4 ec_lds =                               
	mat4(1.0, 0.0, 0.0, 0.0,
	     0.0, 1.0, 0.0, 0.0,
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);

uniform vec4 lightPositions[MAX_NUM_LIGHTS];
uniform vec3 lightStrengths[MAX_NUM_LIGHTS];
uniform int actualNumLights = 1;
uniform vec4 globalAmbient = vec4( 0.2, 0.2, 0.2, 1.0 ); 
uniform vec4 ka = vec4( 1.0, 0.0, 0.0, 1.0 );            
uniform vec4 kd = vec4( 0.0, 1.0, 0.0, 1.0 );            
uniform vec4 ks = vec4( 0.0, 0.0, 1.0, 1.0 );            
uniform float m = 1.0;                                   

layout (location = 0) in vec3 mcPosition;                
in vec3 mcNormal;                                        

out vec4 colorToFS;

vec4 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat)
{
	vec4 Iq = vec4(0.0, 0.0, 0.0, 0.0);

	//Ambient
	Iq = ka * globalAmbient;

	// Create a unit vector towards the viewer (method depends on type of projection) 
	// default value for orthogonal
	vec3 ec_v = vec3( 0.0, 0.0, 1.0 );
	if( ec_lds[3][3] != 1.0f )
	    {
		// perspective
		ec_v = -normalize( ec_Q );
	    }
	else if( ec_lds[1][0] != 0.0f && ec_lds[2][0] != 0.0f )
	    {
		ec_v = vec3( 0.0, 0.0995037, 0.995037 );
	    }

	// if we are viewing this point "from behind", we need to negate the incoming
	// normal vector since our lighting model expressions implicitly assume the normal
	// vector points toward the same side of the triangle that the eye is on.
	if( ec_v.z < ec_Q.z )
	    {
		ec_Q = -1.0f * ec_Q;
	    }

	for( int i = 0; i < actualNumLights; ++i )
	     {
			// create vector to light source
			vec3 ec_li;
			ec_li = lightPositions[i].xyz;
			if( lightPositions[i].w != 0.0 )
			{
				ec_li = ec_li - ec_Q;
			}
			ec_li = normalize( ec_li );

			// if light is behind this object, skip this light source 
			float diffuse = abs(dot(normalize(ec_li), ec_nHat));
			if( diffuse > 0.0f )
			{
				Iq.xyz += kd.xyz * diffuse * lightStrengths[i].xyz;

		        	// 2. if viewer on appropriate side of normal vector,
		       		// compute and accumulate specular contribution
				if( dot( ec_li, ec_nHat ) > 0.0 )
				{
					// compute r_i
					vec3 r_i = ec_nHat;
			 		r_i = r_i * 2;
					r_i = r_i * dot( ec_li, ec_nHat );
					r_i = r_i - ec_li;
				
					float specular = clamp( pow( max( dot( r_i, ec_v ), 0.0 ), m ), 0.0, 1.0 );

					Iq.xyz += ks.xyz * specular * lightStrengths[i].xyz;
				}
		    	}
	      }

     if( Iq.x > 1.0 )
      	  Iq.x = 1.0;
     if( Iq.y > 1.0 )
      	  Iq.y = 1.0;
     if( Iq.z > 1.0 )
          Iq.z = 1.0;

	return Iq;
}

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = mc_ec * vec4(mcPosition, 1.0);
	mat3 normalMatrix = transpose(inverse(mat3x3(mc_ec)));
	vec3 ec_nHat = normalize(normalMatrix * mcNormal);
	
	colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);

	// apply the projection matrix to compute the projective space representation
	// of the 3D logical device space coordinates of the input vertex:
	gl_Position = ec_lds * p_ecPosition;
}
