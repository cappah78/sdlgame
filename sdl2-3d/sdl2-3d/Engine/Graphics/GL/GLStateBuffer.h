#pragma once

#include "../Model/IStateBuffer.h"

class GLStateBuffer : public IStateBuffer
{
public:
	GLStateBuffer();
	GLStateBuffer(const GLStateBuffer& copy) = delete;
	virtual ~GLStateBuffer() override;

	virtual void setIndiceBuffer(std::unique_ptr<IIndiceBuffer>& buffer) override;
	virtual void addVertexBuffer(std::unique_ptr<IVertexBuffer>& buffer) override;
	virtual void addConstantBuffer(std::unique_ptr<IConstantBuffer>& buffer) override;

	virtual void enable() override;
	virtual void disable() override;
private:
	typedef unsigned int GLuint;
	GLuint m_vao;
	bool m_isEnabled;
	static bool s_isEnabled;
};