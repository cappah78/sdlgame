#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) flat out int instanceID;

void main()
{
	instanceID = gl_InstanceID;
}