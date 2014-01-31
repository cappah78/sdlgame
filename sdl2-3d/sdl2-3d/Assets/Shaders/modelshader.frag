#version 330 core

uniform sampler2D u_diffuseTex;
uniform sampler2D u_normalTex;
uniform sampler2D u_specularTex;
uniform sampler2D u_opacityTex;

in vec4 position;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
	vec4 col = texture(u_diffuseTex, texcoord);
	out_color = col;
}