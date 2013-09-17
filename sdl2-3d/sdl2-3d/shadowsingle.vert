#version 330 core

layout(location = 0) in vec3 inVertexPosition;

layout(std140) uniform lightTransform {
	mat4 VPMatrix[600];
} LightTransform;



uniform int lightID;

out gl_PerVertex {
	vec4 gl_Position;
};

void main(void) {

	/* calculate clip space position */
	gl_Position = LightTransform.VPMatrix[lightID] * vec4(inVertexPosition, 1.f);

}