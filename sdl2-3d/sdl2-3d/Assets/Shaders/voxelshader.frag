#version 330 core

uniform sampler2DArray u_texArr;

layout (std140) uniform PerFrameData
{
	mat4 u_mvp;
	vec3 u_camPos;
	float u_fogStart;
	vec3 u_fogColor;
	float u_fogEnd;
};

in vec3 texCoord;
in float vertexAO;
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

	vec4 texCol = texture(u_texArr, texCoord);
	vec4 blockCol = vec4((texCol.rgb) * vertexAO, texCol.a);
	out_color = mix(blockCol, vec4(u_fogColor, 1.0), fogFactor);
}