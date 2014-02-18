#pragma once

enum EBufferMapFlagBits
{
	WRITE, READ, ASYNC, INVALIDATE_RANGE, INVALIDATE_BUFFER
};

typedef unsigned int BufferMapFlags;