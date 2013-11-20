#version 330 core

layout(location = 0) in uint in_chunkIdx;

out uint chunkIdx;

void main(void)
{
	chunkIdx = in_chunkIdx;
}