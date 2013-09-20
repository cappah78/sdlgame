#version 330 core
#extension GL_ARB_gpu_shader5 : enable

struct LightType
{
	vec4 position;
	vec4 color;
};

layout(std140) uniform LightData {
	LightType light[100];
} lightData;

layout(std140) uniform LightTransform {
	mat4 VPMatrix[600];
} lightTransforms;

layout(triangles, invocations = 10) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertexPosition[];

void main() 
{
	vec3 normal = cross(vertexPosition[2]-vertexPosition[0], vertexPosition[0]-vertexPosition[1]);
	vec3 light = vec3(lightData.light[gl_InvocationID].position) - vertexPosition[0];

	if (dot(normal, light) > 0.f) 
	{
		for (int i=0; i<3; ++i) 
		{
			gl_Position = lightTransforms.VPMatrix[gl_InvocationID] * vec4(vertexPosition[i], 1.f);
			gl_Layer = gl_InvocationID;
			EmitVertex();
		}
		EndPrimitive();
	}
}