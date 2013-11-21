#version 330 core

#extension GL_EXT_texture_array : enable

uniform sampler2DArray texArr;
uniform sampler2D tex;

in vec2 texCoord;
in float textureId;
in float occlusion;

out vec4 color;

void main()
{
	//color = texture(tex, texCoord);
	color = texture2DArray(texArr, vec3(texCoord, textureId)) * (1.0 - (occlusion * 0.2));
}