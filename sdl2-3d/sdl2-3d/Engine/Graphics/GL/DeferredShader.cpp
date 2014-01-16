#include "DeferredShader.h"

#include "../../Utils/ShaderManager.h"

DeferredShader::DeferredShader()
	: Shader("Assets/Shaders/gbuffer.frag", NULL, "Assets/Shaders/gbuffer.vert")
	, m_renderShader(NULL, "Assets/Shaders/fullscreenquad.geom", "Assets/Shaders/deferredgeompass.frag")
	, m_gbuffer(1)
{
	glGenVertexArrays(1, &m_pointVao);
}

void DeferredShader::begin()
{
	Shader::begin();
	m_gbuffer.bindForWriting();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void DeferredShader::end()
{
	Shader::end();

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	m_renderShader.begin();
	m_gbuffer.bindForReading();
	glBindVertexArray(m_pointVao);
	glDrawArrays(GL_POINTS, 0, 1);
	//..
	m_renderShader.end();
}