#version 330 core

uniform vec2 u_screenSize;
uniform sampler2D u_depthTex;
uniform sampler2D u_colorTex;

uniform mat4 u_invMvp;

layout (location = 0) out vec4 out_color;

vec3 positionFromDepth(float depth, vec2 uvCoord) 
{
	vec4 projectedPos = vec4(uvCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
	vec4 pos = u_invMvp * projectedPos;
	return pos.xyz / pos.w;
}

vec2 calcTexCoord()
{
    return gl_FragCoord.xy / u_screenSize;
}

void main()
{
	vec2 texCoord = calcTexCoord();
	float depth = texture(u_depthTex, texCoord).r;
	vec3 color = texture(u_colorTex, texCoord).rgb;
	vec3 position = positionFromDepth(depth, texCoord);
	//out_color = vec4(vec3(position * (1.0 - depth) * 100.0), 1.0);
	out_color = vec4(color, 1.0);
}