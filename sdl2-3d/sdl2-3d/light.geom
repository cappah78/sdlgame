#version 420 core

struct LightType
{
	vec4 position;
	vec4 color;
};

layout(std140, binding = 1) uniform lightArray {
	LightType light[100];
} LightArray;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) flat in int instanceID[];

layout(location = 0) out vec2 interpTexCoord;
layout(location = 1) out vec3 lightPosition;
layout(location = 2) out vec4 lightColor;
layout(location = 3) flat out int lightID;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {

	vec4 lightPos = LightArray.light[instanceID[0]].position;
	lightColor = LightArray.light[instanceID[0]].color;

	gl_Position = vec4( 1.0, 1.0, 0.5, 1.0 );
	interpTexCoord = vec2( 1.0, 1.0 );
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	gl_Position = vec4(-1.0, 1.0, 0.5, 1.0 );
	interpTexCoord = vec2( 0.0, 1.0 ); 
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	gl_Position = vec4( 1.0,-1.0, 0.5, 1.0 );
	interpTexCoord = vec2( 1.0, 0.0 ); 
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	gl_Position = vec4(-1.0,-1.0, 0.5, 1.0 );
	interpTexCoord = vec2( 0.0, 0.0 ); 
	lightPosition = lightPos.xyz;
	lightID = instanceID[0];
	EmitVertex();

	EndPrimitive();

}