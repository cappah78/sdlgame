#pragma once

#include "Shader.h"
#include "GBuffer.h"

class DeferredShader : public Shader
{
public:
	DeferredShader();
	~DeferredShader() {}
	DeferredShader(const DeferredShader& copy) = delete;

	void begin() override;
	void end() override;
private:

	GLuint m_pointVao;
	Shader m_renderShader;
	GBuffer m_gbuffer;
};