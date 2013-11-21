#version 330 core

//#extension GL_EXT_texture_array : enable

//uniform sampler2DArray tex;
uniform sampler2D tex;

in vec2 texCoord;
in float textureId;
in float lightLevel;

out vec4 color;

void main()
{
	color = texture(tex, texCoord);
	//color = texture2DArray(tex, vec3(texCoord, textureId)) * lightLevel;
}