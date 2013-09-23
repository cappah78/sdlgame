#version 330 core
#extension GL_ARB_gpu_shader5 : enable

struct LightType
{
	vec4 position;
	vec4 direction;
	vec4 color;
};

layout(std140) uniform LightData {
	LightType light[32];
} lightData;

layout(std140) uniform LightTransform {
	mat4 VPMatrix[32];
} lightTransforms;

layout(triangles, invocations = 32) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 position[];

void main() 
{
	vec3 normal = cross(position[2]-position[0], position[0]-position[1]);
	vec3 light = vec3(lightData.light[gl_InvocationID].position) - position[0];

	if (dot(normal, light) > 0.f) 
	{
		for (int i = 0; i < 3; ++i) 
		{
			gl_Position = lightTransforms.VPMatrix[gl_InvocationID] * vec4(position[i], 1.f);
			gl_Layer = gl_InvocationID;
			EmitVertex();
		}
		EndPrimitive();
	}
}