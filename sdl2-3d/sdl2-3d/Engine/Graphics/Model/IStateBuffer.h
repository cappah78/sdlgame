#pragma once

class IVertexBuffer;
class IConstantBuffer;
class IIndiceBuffer;

#include <memory>

class IStateBuffer
{
public:
	IStateBuffer() {};
	virtual ~IStateBuffer() {};

	virtual void setIndiceBuffer(std::auto_ptr<IIndiceBuffer>& buffer) = 0;
	virtual void addVertexBuffer(std::auto_ptr<IVertexBuffer>& buffer) = 0;
	virtual void addConstantBuffer(std::auto_ptr<IConstantBuffer>& buffer) = 0;

	virtual void enable() = 0;
	virtual void disable() = 0;
private:
};