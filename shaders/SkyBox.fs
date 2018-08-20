#version 130

// TEXTURES
uniform samplerCube _texcube;

// VARYING
varying vec3        _v_cubeCoords;

// MAIN
void main ()
{
	vec4 color;
	color.rgb = textureCube(_texcube, _v_cubeCoords).rgb;
	color.a = 1.0;
	
	////////// OUTPUTS /////
	gl_FragColor = color;
	////////////////////////
}


