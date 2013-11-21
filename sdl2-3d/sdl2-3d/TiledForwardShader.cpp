/****************************************************************************/
/* Copyright (c) 2011, Ola Olsson
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
/****************************************************************************/

#include "TiledForwardShader.h"

#include "Config.h"
#include "GameLoop.h"
#include "ShaderManager.h"
#include "ComboShader.h"

unsigned int g_numMsaaSamples = 1;
GLuint g_rgbaFpFormat = GL_RGBA16F;// GL_RGBA32F;// GL_R11F_G11F_B10F;//

const char* g_tiledDeferredTextureUnitNames[TiledForwardShader::TDTU_Max - TiledForwardShader::TDTU_LightIndexData] =
{
	"tileLightIndexListsTex",
	"diffuseTex",
	"specularShininessTex",
	"normalTex",
	"ambientTex",
	"depthTex",
};

TiledForwardShader::TiledForwardShader()
{
	glGenTextures(1, &m_tileLightIndexListsTexture);
	// initial size is 1, because glTexBuffer failed if it was empty, we'll shovel in data later.
	m_tileLightIndexListsBuffer.init(1);
	glBindTexture(GL_TEXTURE_BUFFER, m_tileLightIndexListsTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, m_tileLightIndexListsBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	CHECK_GL_ERROR();
}

void TiledForwardShader::createShaders()
{

	SimpleShader::Context shaderCtx;
	shaderCtx.setPreprocDef("NUM_POSSIBLE_LIGHTS", NUM_POSSIBLE_LIGHTS);
	shaderCtx.setPreprocDef("LIGHT_GRID_TILE_DIM_X", LIGHT_GRID_TILE_DIM_X);
	shaderCtx.setPreprocDef("LIGHT_GRID_TILE_DIM_Y", LIGHT_GRID_TILE_DIM_Y);
	shaderCtx.setPreprocDef("LIGHT_GRID_MAX_DIM_X", LIGHT_GRID_MAX_DIM_X);
	shaderCtx.setPreprocDef("LIGHT_GRID_MAX_DIM_Y", LIGHT_GRID_MAX_DIM_Y);
	shaderCtx.setPreprocDef("LIGHT_GRID_MAX_DIM_Z", LIGHT_GRID_MAX_DIM_Z);
	shaderCtx.setPreprocDef("NUM_MSAA_SAMPLES", int(g_numMsaaSamples));

	deleteIfThere(m_simpleShader);
	m_simpleShader = new ComboShader("shaders/simple_vertex.glsl", "shaders/simple_fragment.glsl", shaderCtx);
	bindObjModelAttributes(m_simpleShader);
	m_simpleShader->bindFragDataLocation(0, "resultColor");
	m_simpleShader->link();
	setObjModelUniformBindings(m_simpleShader);


	// tiled forward shader
	deleteIfThere(m_tiledForwardShader);
	m_tiledForwardShader = new ComboShader("shaders/tiled_forward_vertex.glsl", "shaders/tiled_forward_fragment.glsl", shaderCtx, std::vector<std::string>());	//;-;
	bindObjModelAttributes(m_tiledForwardShader);
	m_tiledForwardShader->bindFragDataLocation(0, "resultColor");
	m_tiledForwardShader->link();
	setObjModelUniformBindings(m_tiledForwardShader);
	setTiledLightingUniformBindings(m_tiledForwardShader);

	// downsample min/max shader
	deleteIfThere(m_downSampleMinMaxShader);
	m_downSampleMinMaxShader = new SimpleShader("shaders/tiled_deferred_vertex.glsl", "shaders/downsample_minmax_fragment.glsl", shaderCtx);
	m_downSampleMinMaxShader->bindFragDataLocation(0, "resultMinMax");
	m_downSampleMinMaxShader->link();
	m_downSampleMinMaxShader->setUniformBufferSlot("Globals", TDUBS_Globals);
}

TiledForwardShader::~TiledForwardShader()
{

}

void TiledForwardShader::render(const PerspectiveCamera& camera, const RenderList& renderList)
{
	updateShaderGlobals(camera.m_viewMatrix, camera.m_projectionMatrix, camera.m_up, 
		GameLoop::getWidth(), GameLoop::getHeight(), camera.getNear(), camera.getFov());

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	std::vector<glm::vec2> depthRanges;

//	drawPreZPass();
//	downSampleDepthBuffer(depthRanges);

	m_LightGridTransparent.build(
		glm::uvec2(LIGHT_GRID_TILE_DIM_X, LIGHT_GRID_TILE_DIM_Y),
		glm::uvec2(GameLoop::getWidth(), GameLoop::getHeight()),
		m_lights,
		camera.m_viewMatrix,
		camera.m_projectionMatrix,
		camera.getNear(),
		std::vector<glm::vec2>()
	);

	m_LightGridOpaque = m_LightGridTransparent;
	m_LightGridOpaque.prune(depthRanges);

	//m_LightGridTransparent.pruneFarOnly(camera.getNear(), depthRanges);

	bindLightGridConstants(m_LightGridOpaque);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, GameLoop::getWidth(), GameLoop::getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// if pre-z is enabled, we don't want to re-clear the frame buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto i = renderList.begin(); i != renderList.end(); ++i)
	{
		OBJModel* model = *i;
		model->render(m_tiledForwardShader, OBJModel::RF_Opaque);
		model->render(m_tiledForwardShader, OBJModel::RF_AlphaTested);
	}

	bindLightGridConstants(m_LightGridTransparent);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto i = renderList.begin(); i != renderList.end(); ++i)
	{
		OBJModel* model = *i;
		model->render(m_tiledForwardShader, OBJModel::RF_Opaque, camera.m_viewMatrix);
	}

	glDisable(GL_BLEND);
	unbindLightGridConstants();
}


void TiledForwardShader::updateShaderGlobals(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 &worldUpDirection, int width, int height, float near, float fov)
{
	m_shaderGlobals.viewMatrix = viewMatrix;
	m_shaderGlobals.viewProjectionMatrix = projectionMatrix * viewMatrix;
	m_shaderGlobals.inverseProjectionMatrix = glm::inverse(projectionMatrix);
	m_shaderGlobals.normalMatrix = glm::transpose(glm::inverse(viewMatrix));
	m_shaderGlobals.worldUpDirection = worldUpDirection;
	m_shaderGlobals.ambientGlobal = m_ambientLight;
	m_shaderGlobals.invFbSize = glm::vec2(1.0f / float(width), 1.0f / float(height));
	m_shaderGlobals.fbSize = glm::ivec2(width, height);

	m_shaderGlobals.recNear = 1.0f / near;
	unsigned int grid2dDimY = (height + LIGHT_GRID_TILE_DIM_Y - 1) / LIGHT_GRID_TILE_DIM_Y;
	float sD = 2.0f * tanf(glm::radians(fov) * 0.5f) / float(grid2dDimY);
	m_shaderGlobals.recLogSD1 = 1.0f / logf(sD + 1.0f);


	// copy to Gl
	m_shaderGlobalsGl.copyFromHost(&m_shaderGlobals, 1);
	// bind buffer.
	m_shaderGlobalsGl.bindSlot(GL_UNIFORM_BUFFER, TDUBS_Globals);
}

void TiledForwardShader::bindLightGridConstants(const LightGrid &LightGrid)
{
	// pack grid data in int4 because this will work on AMD GPUs, where constant registers are 4-vectors.
	static glm::ivec4 tmp[LIGHT_GRID_MAX_DIM_X * LIGHT_GRID_MAX_DIM_Y];
	{
		const int *counts = LightGrid.tileCountsDataPtr();
		const int *offsets = LightGrid.tileDataPtr();
		for (int i = 0; i < LIGHT_GRID_MAX_DIM_X * LIGHT_GRID_MAX_DIM_Y; ++i)
		{
			tmp[i] = glm::ivec4(counts[i], offsets[i], 0, 0);
		}
	}
	m_gridBuffer.copyFromHost(tmp, LIGHT_GRID_MAX_DIM_X * LIGHT_GRID_MAX_DIM_Y);

	if (LightGrid.getTotalTileLightIndexListLength())
	{
		m_tileLightIndexListsBuffer.copyFromHost(LightGrid.tileLightIndexListsPtr(), LightGrid.getTotalTileLightIndexListLength());
		// This should not be neccessary, but for amd it seems to be (HD3200 integrated)
		glBindTexture(GL_TEXTURE_BUFFER, m_tileLightIndexListsTexture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, m_tileLightIndexListsBuffer);
		CHECK_GL_ERROR();
	}
	bindTexture(GL_TEXTURE_BUFFER, TDTU_LightIndexData, m_tileLightIndexListsTexture);

	updateLightBuffers(LightGrid.getViewSpaceLights());

	m_gridBuffer.bindSlot(GL_UNIFORM_BUFFER, TDUBS_LightGrid);
	m_LightPositionRangeBuffer.bindSlot(GL_UNIFORM_BUFFER, TDUBS_LightPositionsRanges);
	m_LightColorBuffer.bindSlot(GL_UNIFORM_BUFFER, TDUBS_LightColors);

	for (int i = 0; i < DRTI_Max; ++i)
	{
		bindTexture(getRenderTargetTextureTargetType(), i + TDTU_Diffuse, m_renderTargetTextures[i]);
	}
}

void TiledForwardShader::unbindLightGridConstants()
{
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	bindTexture(GL_TEXTURE_BUFFER, TDTU_LightIndexData, 0);

	for (int i = 0; i < DRTI_Max; ++i)
	{
		bindTexture(getRenderTargetTextureTargetType(), i + TDTU_Diffuse, 0);
	}
}

void TiledForwardShader::drawPreZPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFbo);

	glViewport(0, 0, GameLoop::getWidth(), GameLoop::getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	glDepthFunc(GL_LEQUAL);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	m_simpleShader->begin(false);
	//g_model->render(0, OBJModel::RF_Opaque);
	m_simpleShader->end();
	m_simpleShader->begin(true);
	//g_model->render(0, OBJModel::RF_AlphaTested);
	m_simpleShader->end();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}


void TiledForwardShader::updateLightBuffers(const TiledShaderPointLights &lights)
{
	const size_t maxLights = NUM_POSSIBLE_LIGHTS;
	static glm::vec4 Light_position_range[maxLights];
	static glm::vec4 Light_color[maxLights];

	memset(Light_position_range, 0, sizeof(Light_position_range));
	memset(Light_color, 0, sizeof(Light_color));

	for (size_t i = 0; i < std::min(maxLights, lights.size()); ++i)
	{
		Light_position_range[i] = glm::vec4(lights[i].position, lights[i].range);
		Light_color[i] = glm::vec4(lights[i].color, 1.0f);
	}
	m_LightPositionRangeBuffer.copyFromHost(Light_position_range, NUM_POSSIBLE_LIGHTS);
	m_LightColorBuffer.copyFromHost(Light_color, NUM_POSSIBLE_LIGHTS);
}

void TiledForwardShader::downSampleDepthBuffer(std::vector<glm::vec2>& depthRanges)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_minMaxDepthFbo);

	glm::uvec2 tileSize = glm::uvec2(LIGHT_GRID_TILE_DIM_X, LIGHT_GRID_TILE_DIM_Y);
	glm::uvec2 resolution = glm::uvec2(GameLoop::getWidth(), GameLoop::getHeight());
	glm::uvec2 gridRes = (resolution + tileSize - glm::uvec2(1, 1)) / tileSize;
	{
		glViewport(0, 0, gridRes.x, gridRes.y);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		CHECK_GL_ERROR();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_downSampleMinMaxShader->begin();
		if (g_numMsaaSamples == 1)
		{
			m_downSampleMinMaxShader->setTexture2D("depthTex", m_renderTargetTextures[DRTI_Depth], 0);
		}
		else
		{
			m_downSampleMinMaxShader->setTexture2DMS("depthTex", m_renderTargetTextures[DRTI_Depth], 0);
		}

		glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();
		CHECK_GL_ERROR();

		m_downSampleMinMaxShader->end();

		CHECK_GL_ERROR();
		glDepthMask(GL_TRUE);
		glPopAttrib();
		CHECK_GL_ERROR();
	}
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		depthRanges.resize(gridRes.x * gridRes.y);
		glReadPixels(0, 0, gridRes.x, gridRes.y, GL_RG, GL_FLOAT, &depthRanges[0]);
		CHECK_GL_ERROR();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TiledForwardShader::setTiledLightingUniformBindings(ComboShader *shader)
{
	shader->setUniformBufferSlot("Globals", TDUBS_Globals);
	shader->setUniformBufferSlot("LightGrid", TDUBS_LightGrid);
	shader->setUniformBufferSlot("LightPositionsRanges", TDUBS_LightPositionsRanges);
	shader->setUniformBufferSlot("LightColors", TDUBS_LightColors);

	shader->begin(false);

	for (int i = TDTU_LightIndexData; i < TDTU_Max; ++i)
	{
		shader->setUniform(g_tiledDeferredTextureUnitNames[i - TDTU_LightIndexData], i);
	}
	shader->end();
}

void TiledForwardShader::createFbos(int width, int height)
{
	int maxSamples = 0;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

	for (int i = 0; i < DRTI_Max; ++i)
	{
		deleteTextureIfUsed(m_renderTargetTextures[i]);
	}

	// deferred render target
	if (!m_deferredFbo)
	{
		// only create if not already created.
		glGenFramebuffers(1, &m_deferredFbo);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFbo);
	for (int i = 0; i < DRTI_Depth; ++i)
	{
		m_renderTargetTextures[i] = attachTargetTextureToFBO(m_deferredFbo, GL_COLOR_ATTACHMENT0 + i, width, height, g_rgbaFpFormat, GL_RGBA, GL_FLOAT, g_numMsaaSamples);
	}
	m_renderTargetTextures[DRTI_Depth] = attachTargetTextureToFBO(m_deferredFbo, GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, g_numMsaaSamples);

	CHECK_GL_ERROR();
	GLenum bufs [] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(sizeof(bufs) / sizeof(bufs[0]), bufs);
	glReadBuffer(GL_NONE);


	checkFBO(m_deferredFbo);

	// forward shading render target
	if (!m_forwardFbo)
	{
		// only create if not already created.
		glGenFramebuffers(1, &m_forwardFbo);
	}
	deleteTextureIfUsed(m_forwardTargetTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFbo);
	m_forwardTargetTexture = attachTargetTextureToFBO(m_forwardFbo, GL_COLOR_ATTACHMENT0, width, height, GL_RGBA, GL_RGBA, GL_FLOAT, g_numMsaaSamples);
	// Shared with deferred
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getRenderTargetTextureTargetType(), m_renderTargetTextures[DRTI_Depth], 0);


	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	checkFBO(m_forwardFbo);

	if (!m_minMaxDepthFbo)
	{
		// only create if not already created.
		glGenFramebuffers(1, &m_minMaxDepthFbo);
	}
	deleteTextureIfUsed(m_minMaxDepthTargetTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, m_minMaxDepthFbo);

	glm::uvec2 tileSize = glm::uvec2(LIGHT_GRID_TILE_DIM_X, LIGHT_GRID_TILE_DIM_Y);
	glm::uvec2 resolution = glm::uvec2(width, height);
	glm::uvec2 gridRes = (resolution + tileSize - glm::uvec2(1, 1)) / tileSize;

	m_minMaxDepthTargetTexture = attachTargetTextureToFBO(m_minMaxDepthFbo, GL_COLOR_ATTACHMENT0, gridRes.x, gridRes.y, GL_RG32F, GL_RGBA, GL_FLOAT, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	checkFBO(m_minMaxDepthFbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TiledForwardShader::bindTexture(GLenum type, int texUnit, int textureId)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(type, textureId);
}

void TiledForwardShader::deleteTextureIfUsed(GLuint texId)
 {
	if (texId != 0)
	{
		glDeleteTextures(1, &texId);
	}
}

template <typename T>
void TiledForwardShader::deleteIfThere(T *&shader)
{
	if (shader)
	{
		delete shader;
		shader = 0;
	}
}

void TiledForwardShader::bindObjModelAttributes(ComboShader *shader)
{
	shader->bindAttribLocation(OBJModel::AA_Position, "position");
	shader->bindAttribLocation(OBJModel::AA_Normal, "normalIn");
	shader->bindAttribLocation(OBJModel::AA_TexCoord, "texCoordIn");
	shader->bindAttribLocation(OBJModel::AA_Tangent, "tangentIn");
	shader->bindAttribLocation(OBJModel::AA_Bitangent, "bitangentIn");
}

void TiledForwardShader::setObjModelUniformBindings(ComboShader *shader)
{
	shader->begin(false);
	shader->setUniform("diffuse_texture", OBJModel::TU_Diffuse);
	shader->setUniform("opacity_texture", OBJModel::TU_Opacity);
	shader->setUniform("specular_texture", OBJModel::TU_Specular);
	shader->setUniform("normal_texture", OBJModel::TU_Normal);
	shader->end();
	shader->setUniformBufferSlot("MaterialProperties", OBJModel::UBS_MaterialProperties);

	shader->setUniformBufferSlot("Globals", TDUBS_Globals);
}


void TiledForwardShader::checkFBO(uint32_t fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			printf("FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			printf("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			printf("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			printf("FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			printf("FRAMEBUFFER_UNSUPPORTED\n");
			break;
		default:
			printf("Unknown framebuffer problem %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
			break;
		}
		printf("Error: bad frame buffer config\n");
		DBG_BREAK();
		exit(-1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// helper function to create and attach a frame buffer target object. 
GLuint TiledForwardShader::attachTargetTextureToFBO(GLuint fbo, GLenum attachment, int width, int height, GLenum internalFormat, GLenum format, GLenum type, int msaaSamples)
{
	GLuint targetTexture;
	glGenTextures(1, &targetTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	CHECK_GL_ERROR();

	if (msaaSamples == 1)
	{
		glBindTexture(GL_TEXTURE_2D, targetTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);

		CHECK_GL_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, targetTexture, 0);
		CHECK_GL_ERROR();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, targetTexture);
		CHECK_GL_ERROR();

		// if nvidias CSAA is available, we use it with half the color samples to coverage samples.
		if (GLEW_NV_texture_multisample)
		{
			glTexImage2DMultisampleCoverageNV(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, msaaSamples / 2, internalFormat, width, height, false);
		}
		else
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, internalFormat, width, height, false);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, targetTexture, 0);

		CHECK_GL_ERROR();

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);


	return targetTexture;
}

