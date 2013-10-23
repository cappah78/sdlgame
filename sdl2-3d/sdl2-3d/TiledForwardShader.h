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

#ifndef TILED_FORWARD_SHADER_H_
#define TILED_FORWARD_SHADER_H_

#include <glm/glm.hpp>
#include "TiledShaderPointLight.h"
#include "PerspectiveCamera.h"
#include "LightGrid.h"
#include "GlBufferObject.h"
#include "OBJModel.h"
#include <vector>


class ComboShader;
class SimpleShader;

typedef std::vector<OBJModel*> RenderList;

class TiledForwardShader
{
public:

	enum DeferredRenderTargetIndices
	{
		DRTI_Diffuse,
		DRTI_SpecularShininess,
		DRTI_Normal,
		DRTI_Ambient,
		DRTI_Depth,
		DRTI_Max,
	};

	enum TiledDeferredTextureUnits
	{
		TDTU_LightIndexData = OBJModel::TU_Max, // avoid those used by objmodel.
		TDTU_Diffuse,
		TDTU_SpecularShininess,
		TDTU_Normal,
		TDTU_Ambient,
		TDTU_Depth,
		TDTU_Max,
	};

	struct ShaderGlobals_Std140
	{
		glm::mat4 viewMatrix;
		glm::mat4 viewProjectionMatrix;
		glm::mat4 inverseProjectionMatrix;
		glm::mat4 normalMatrix;
		glm::vec3 worldUpDirection;
		float pad0;
		glm::vec3 ambientGlobal;
		float pad1;
		glm::vec2 invFbSize;
		glm::ivec2 fbSize;
		float recNear;
		float recLogSD1;
	} m_shaderGlobals;

	enum TiledDeferredUniformBufferSlots
	{
		TDUBS_Globals = OBJModel::UBS_Max, // use buffers that are not used by obj model.
		TDUBS_LightGrid,
		TDUBS_LightPositionsRanges,
		TDUBS_LightColors,
		TDUBS_Max,
	};


	TiledForwardShader();
	~TiledForwardShader();

	void updateShaderGlobals(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 &worldUpDirection, int width, int height, float near, float fov);
	void drawPreZPass();
	void downSampleDepthBuffer(std::vector<glm::vec2>& depthRanges);
	void render(const PerspectiveCamera& camera, const RenderList& renderList);
	void createFbos(int width, int height);
	void createShaders();
	void bindLightGridConstants(const LightGrid &lightGrid);
	void unbindLightGridConstants();
	void updateLightBuffers(const TiledShaderPointLights &lights);
	void bindObjModelAttributes(ComboShader *shader);
	void setObjModelUniformBindings(ComboShader *shader);
	template <typename T>
	void deleteIfThere(T *&shader);
	void setTiledLightingUniformBindings(ComboShader *shader);
	void bindTexture(GLenum type, int texUnit, int textureId);
	void deleteTextureIfUsed(GLuint texId);
	void checkFBO(uint32_t fbo);


	GLuint attachTargetTextureToFBO(GLuint fbo, GLenum attachment, int width, int height, GLenum internalFormat, GLenum format, GLenum type = GL_FLOAT, int msaaSamples = 0);

	inline GLenum getRenderTargetTextureTargetType()
	{
		return m_numMsaaSamples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
	}
private:
	uint32_t m_numMsaaSamples;

	glm::vec3 m_ambientLight;

	ComboShader* m_simpleShader;
	ComboShader* m_forwardShader;
	ComboShader* m_tiledForwardShader;

	SimpleShader* m_downSampleMinMaxShader;

	GLuint m_renderTargetTextures[DRTI_Max];
	GLuint m_forwardTargetTexture;
	GLuint m_minMaxDepthTargetTexture;

	GLuint m_deferredFbo;
	GLuint m_forwardFbo;
	GLuint m_minMaxDepthFbo;
	GlBufferObject<ShaderGlobals_Std140> m_shaderGlobalsGl;
	LightGrid m_LightGridTransparent;
	LightGrid m_LightGridOpaque;
	std::vector<TiledShaderPointLight> m_lights;


	GLuint m_tileLightIndexListsTexture;
	GlBufferObject<int> m_tileLightIndexListsBuffer;
	GlBufferObject<glm::ivec4> m_gridBuffer;

	GlBufferObject<glm::vec4> m_LightPositionRangeBuffer;
	GlBufferObject<glm::vec4> m_LightColorBuffer;


};

#endif //TILED_FORWARD_SHADER_H_