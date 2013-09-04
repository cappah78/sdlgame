#version 330 core

layout(std140) uniform;

layout (location = 0) out vec4 fragColor;

in vec2 texCoord;
in vec3 scatteredLight;
in vec3 reflectedLight;

uniform sampler2D tex;

void main()
{
	vec4 color = texture(tex, texCoord);
	vec3 rgb = min(color.rgb * scatteredLight + reflectedLight, vec3(1.0));

	fragColor = vec4(rgb, color.a);
}
