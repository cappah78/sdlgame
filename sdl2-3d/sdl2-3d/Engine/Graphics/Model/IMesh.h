#pragma once

#include <vector>

struct IMeshShaderAttributes
{

};

class IMesh
{
public:
	IMesh() {};
	virtual ~IMesh() {};
	
	virtual void setShaderAttributes()
	virtual void render();
private:
};