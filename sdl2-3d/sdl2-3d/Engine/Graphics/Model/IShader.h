#pragma once

class IShader
{
public:
	IShader() {};
	virtual ~IShader() {};

	virtual void begin() = 0;
	virtual void end() = 0;
};