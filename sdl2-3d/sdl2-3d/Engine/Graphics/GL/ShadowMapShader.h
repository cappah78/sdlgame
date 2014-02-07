#pragma once

#include "GBuffer.h"
#include "Shader.h"

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath);
	~ShadowMapShader() {};
	ShadowMapShader(const ShadowMapShader& copy) = delete;

	void begin() override;

	GBuffer m_gbuffer;
private:


};