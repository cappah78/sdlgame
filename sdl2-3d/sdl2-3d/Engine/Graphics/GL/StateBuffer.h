#pragma once

#include "../Model/IStateBuffer.h"

class StateBuffer : public IStateBuffer
{
public:
	StateBuffer();
	virtual ~StateBuffer() override;

	virtual void setIndiceBuffer(std::auto_ptr<IIndiceBuffer>& buffer) override;
	virtual void addVertexBuffer(std::auto_ptr<IVertexBuffer>& buffer) override;
	virtual void addConstantBuffer(std::auto_ptr<IConstantBuffer>& buffer) override;

	virtual void enable() override;
	virtual void disable() override;
private:
	typedef unsigned int GLuint;
	GLuint m_vao;
	bool m_isEnabled;
	static bool s_isEnabled;
};