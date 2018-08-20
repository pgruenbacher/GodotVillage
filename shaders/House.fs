#version 130

uniform sampler2DArray _house;

varying vec2 _v_texCoords;
varying vec4 _v_pos;

vec3 tex3Dcoord;

varying vec3 _v_normal;
varying vec3 _v_normal_view;
varying vec4 _v_color;

vec3 sun = vec3(3000.0, 3000.0, 500.0);

void main(void)
{
        vec4 color;

        vec3 normal;
        normal = _v_normal;

        
	float light = dot(normalize(sun), normal);

        tex3Dcoord = vec3 (_v_texCoords.xy, 0.0);
	color.a   = 1.0;


	color.rgb = _v_normal;
        
        color.rgb = _v_color.rgb;
        color.rgb =  (0.5 * color.rgb + 0.5 * light);
        gl_FragColor = color;    
}

