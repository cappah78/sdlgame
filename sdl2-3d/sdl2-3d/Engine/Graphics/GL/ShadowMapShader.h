#ifndef SHADOW_MAP_SHADER_H_
#define SHADOW_MAP_SHADER_H_

#include "GBuffer.h"
#include "Shader.h"

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath);
	~ShadowMapShader() {};
	ShadowMapShader(const ShadowMapShader& copy) = delete;

	void begin() override;
private:

	GBuffer m_gbuffer;
};

#endif //SHADOW_MAP_SHADER_H_