#pragma once

class IVertexBuffer;
class IConstantBuffer;

class IStateBuffer
{
public:
	IStateBuffer() {};
	virtual ~IStateBuffer() {};

	virtual void addVertexBuffer(IVertexBuffer& buffer) = 0;
	virtual void addConstantBuffer(IConstantBuffer& buffer) = 0;

	virtual void enable() = 0;
	virtual void disable() = 0;
private:
};