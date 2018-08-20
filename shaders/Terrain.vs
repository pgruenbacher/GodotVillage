#version 130
#extension GL_EXT_texture_array: enable

uniform sampler2DArray _terrain_h;
uniform sampler2DArray _terrain_d;
uniform sampler2DArray _colors;

uniform float _maxElevation;

varying vec2 _v_texCoords;
varying vec3 _v_normal;
varying vec4 _v_pos;

vec3 DX = vec3(1.0/2048.0, 0.0, 0.0);
vec3 DY = vec3(0.0, 1.0/2048.0, 0.0); // Should not use 2048 but a uniform to ref the terrain resolution

vec3 tex3Dcoord;
void main(void)
{
        vec4 pos     = gl_Vertex;
        _v_texCoords = gl_MultiTexCoord0.xy;
        tex3Dcoord = vec3 (_v_texCoords.xy,0.0);
        
        float height1   = texture2DArray(_terrain_h, tex3Dcoord.xyz).r;
        float heightDX1 = texture2DArray(_terrain_h, tex3Dcoord.xyz + DX).r;
        float heightDY1 = texture2DArray(_terrain_h, tex3Dcoord.xyz + DY).r;

     

	float yyy = 0.002;        

	vec3 normal1;
        normal1.x  = height1 - heightDX1;                       
        normal1.z  = height1 - heightDY1;
        normal1.y  = yyy;
        normal1 = normalize (normal1);  
	
	_v_normal = normalize(normal1);

        // HEIGHT        
        
        if (_v_texCoords.x > 0.0005
                && _v_texCoords.x < 0.9995
                && _v_texCoords.y > 0.0005
                && _v_texCoords.y < 0.9995)
             
                pos.y = 1.0 * height1 * _maxElevation;
        
        else
                pos.y = -20.0;
                        
        
        
        // VARYING
        _v_pos = pos;
        
        // OUTPUT
        gl_Position  =  gl_ModelViewProjectionMatrix * pos;  

}

