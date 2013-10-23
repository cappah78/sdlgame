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

#ifndef COMBOSHADER_H_
#define COMBOSHADER_H_

#include <map>
#include <string>
#include <vector>
#include "SimpleShader.h"

class ComboShader
{
public:
	ComboShader(const char *vertexShaderFileName, const char *fragmentShaderFileName, SimpleShader::Context &context, const std::vector<std::string> &shadingModels = std::vector<std::string>());
	~ComboShader();

	bool link();

	void bindAttribLocation(GLint index, const GLchar* name);
	void bindFragDataLocation(GLuint location, const char *name);
	bool setUniformBufferSlot(const char *blockName, GLuint slotIndex);
	bool setUniform(const char *varName, int v);
	void begin(bool useAlphaTest, const std::string &shadingModel = "SHADING_MODEL_DEFAULT");
	void end();

protected:
	typedef std::map<std::string, SimpleShader *> ShaderMap;

	ShaderMap m_opaqueShaders;
	ShaderMap m_alphaTestedShaders;
	SimpleShader *m_currentShader;
};

#endif //COMBOSHADER_H_