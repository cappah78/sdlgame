#pragma once

#include "../Model/IStateBuffer.h"

class StateBuffer : public IStateBuffer
{
public:
	StateBuffer();
	virtual ~StateBuffer() override;

	virtual void addVertexBuffer(IVertexBuffer& buffer) override;
	virtual void addConstantBuffer(IConstantBuffer& buffer) override;

	virtual void enable() override;
	virtual void disable() override;
private:
	typedef unsigned int GLuint;
	GLuint m_vao;
};