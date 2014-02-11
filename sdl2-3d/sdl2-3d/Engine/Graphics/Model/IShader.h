#pragma once

class IShader
{
public:
	IShader() {};
	virtual ~IShader() {};

	virtual void begin() = 0;
	virtual void end() = 0;
	virtual unsigned int getID() = 0;
};