#version 330 core

uniform float u_lightDistance;

in float eyeDist;

layout (location = 0) out float out_color;

void main()
{
	float linDist = clamp(1.0 - (eyeDist / u_lightDistance), 0.0, 1.0);
	out_color = linDist;
}