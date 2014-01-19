#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
	: Shader(vertexShaderPath, geomShaderPath, fragShaderPath)
	, m_gbuffer(0)
{

}

void ShadowMapShader::begin() 
{
	m_gbuffer.bindForWriting();
	Shader::begin();
}