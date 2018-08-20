#version 130

// VARYINGS
varying vec3 _v_cubeCoords;

// MAIN
void main(void)
{
	/////////// INPUTS /////
	vec4 vertex = gl_Vertex;
	////////////////////////	
	
  	_v_cubeCoords.x = vertex.x;
  	_v_cubeCoords.y = vertex.z;
  	_v_cubeCoords.z = vertex.y;
	
  	
	////////// OUTPUTS /////
  	gl_Position = gl_ModelViewProjectionMatrix * vertex;
	////////////////////////
}
