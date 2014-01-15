#version 330 core

uniform float u_fogStart;
uniform float u_fogEnd;
uniform vec3 u_fogColor;
uniform sampler2DArray texArr;

in vec3 texCoord;
in vec4 color;
in float eyeDist;

out vec4 out_color;

float computeLinearFogFactor()
{
   float factor = (eyeDist - u_fogStart) / (u_fogEnd - u_fogStart);
   factor = clamp( factor, 0.0, 1.0 );
   return factor;            
}

void main()
{
    float fogFactor = computeLinearFogFactor();

	vec4 texCol = texture(texArr, texCoord);
	vec4 blockCol = vec4((texCol.rgb + color.rgb) * color.a , texCol.a);

	out_color = mix(blockCol, vec4(u_fogColor, 1.0), fogFactor) ;
}