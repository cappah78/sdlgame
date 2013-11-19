#version 330 core

layout(std140) uniform CameraTransform {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} camera;

uniform sampler2D tex;

in vec3 position;
in vec2 texCoord;

out vec4 color;

void main()
{
	color = texture(tex, texCoord);
	//color = vec4(texCoord, 0.0f, 1.0f);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}