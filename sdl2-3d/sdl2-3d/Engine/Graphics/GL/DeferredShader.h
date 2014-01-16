#ifndef DEFERRED_SHADER_H_
#define DEFERRED_SHADER_H_

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

#endif DEFERRED_SHADER_H_