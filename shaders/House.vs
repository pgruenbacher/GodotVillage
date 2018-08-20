#version 130

uniform sampler2DArray _house;

varying vec2 _v_texCoords;
varying vec4 _v_pos;

varying vec3 _v_normal;
varying vec3 _v_normal_view;
varying vec4 _v_color;


void main(void)
{
        vec4 pos       = gl_Vertex;
        _v_color       = gl_Color;
	_v_normal      = gl_Normal;
	_v_normal_view = gl_NormalMatrix * gl_Normal;
        _v_texCoords   = gl_MultiTexCoord0.xy;
        
        // VARYING
        _v_pos = pos;
        
        // OUTPUT
        gl_Position  =  gl_ModelViewProjectionMatrix * pos;  

}

