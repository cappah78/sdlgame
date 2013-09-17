#version 330 core

struct LightType
{
	vec4 position;
	vec4 color;
};

layout(std140) uniform LightData {
	LightType light[100];
} lightData;


layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

flat in int instanceID[];

out vec2 texCoord;
out vec3 lightPosition;
flat out vec4 lightColor;
flat out int lightID;

void main() 
{
	vec4 lightPos = lightData.light[instanceID[0]].position;
	lightColor = lightData.light[instanceID[0]].color;

	gl_Position = vec4( 1.0, 1.0, 0.5, 1.0 );
	texCoord = vec2( 1.0, 1.0 );
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	gl_Position = vec4(-1.0, 1.0, 0.5, 1.0 );
	texCoord = vec2( 0.0, 1.0 ); 
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	gl_Position = vec4( 1.0,-1.0, 0.5, 1.0 );
	texCoord = vec2( 1.0, 0.0 ); 
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	gl_Position = vec4(-1.0,-1.0, 0.5, 1.0 );
	texCoord = vec2( 0.0, 0.0 ); 
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	EndPrimitive();
}