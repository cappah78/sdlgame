#include "GLCommandQueue.h"

#include <vector>
#include <GL/glew.h>
#include <SDL_thread.h>
#include <SDL_video.h>
#include <SDL_atomic.h>

#include "../../Utils/Functor.h"
#include "../../../Game.h"

static const unsigned int MAX_GL_COMMAND_QUEUE_COUNT = 500;

std::vector<Func::Functor*> s_commandQueue;
SDL_mutex* s_mutex = SDL_CreateMutex();
SDL_atomic_t s_atomic = { MAX_GL_COMMAND_QUEUE_COUNT };

namespace GLCommandQueue
{
	void addCommand(Func::Functor* command)
	{
		while (SDL_AtomicGet(&s_atomic) <= 0)
		{
			//printf("GPU Limited");
			SDL_Delay(1);
		}

		SDL_LockMutex(s_mutex);
		SDL_AtomicAdd(&s_atomic, -1);
		s_commandQueue.push_back(command);
		SDL_UnlockMutex(s_mutex);
	}

	void update()
	{
		SDL_LockMutex(s_mutex);
		for (Func::Functor* command : s_commandQueue)
		{
			command->call();
		}
		s_commandQueue.clear();
		SDL_AtomicSet(&s_atomic, MAX_GL_COMMAND_QUEUE_COUNT);
		SDL_UnlockMutex(s_mutex);
	}
}

using namespace GLCommandQueue;
using namespace Func;

void glqSwapWindow()
{ 
	SDL_Window* window = Game::graphics.getWindow();
	addCommand(makeFunctor(SDL_GL_SwapWindow, window));
}

void glqAddFunctor(Func::Functor* functor)
{
	addCommand(functor);
}

bool __glqCheckGLError(const char *file, int line)
{
	bool error = false;

	for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
	{
		error = true;
		const GLubyte* str = gluErrorString(glErr);
		if (!str)
		{
			str = reinterpret_cast<const GLubyte *>("No Error message available.");
		}
		printf("GL Error in file: %s at line %i Error %i: %s \n", file, line, glErr, str);
	}

	return error;
}

void glqEnable(GLenum target) { addCommand(makeFunctor(glEnable, target)); }
void glqDisable(GLenum target) { addCommand(makeFunctor(glDisable, target)); }

/* Vertex Arrays */
void glqVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) { addCommand(makeFunctor(glVertexAttribPointer, index, size, type, normalized, stride, pointer)); }
void glqVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) { addCommand(makeFunctor(glVertexAttribIPointer, index, size, type, stride, pointer)); }
void glqEnableVertexAttribArray(GLuint index) { addCommand(makeFunctor(glEnableVertexAttribArray, index)); }
void glqDisableVertexAttribArray(GLuint index) { addCommand(makeFunctor(glDisableVertexAttribArray, index)); }
void glqPrimitiveRestartIndex(GLuint index) { addCommand(makeFunctor(glPrimitiveRestartIndex, index)); }
// Drawing Commands
void glqDrawArrays(GLenum mode, GLint first, GLsizei count) { addCommand(makeFunctor(glDrawArrays, mode, first, count)); }
void glqMultiDrawArrays(GLenum mode, const GLint* first, const GLsizei* count, GLsizei primcount) { addCommand(makeFunctor(glMultiDrawArrays, mode, first, count, primcount)); }
void glqDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) { addCommand(makeFunctor(glDrawElements, mode, count, type, indices)); }
void glqMultiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount) { addCommand(makeFunctor(glMultiDrawElements, mode, count, type, indices, primcount)); }
void glqDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices) { addCommand(makeFunctor(glDrawRangeElements, mode, start, end, count, type, indices)); }
void glqDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount) { addCommand(makeFunctor(glDrawArraysInstanced, mode, first, count, primcount)); }
void glqDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei primcount) { addCommand(makeFunctor(glDrawElementsInstanced, mode, count, type, indices, primcount)); }
void glqDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex) { addCommand(makeFunctor(glDrawElementsBaseVertex, mode, count, type, indices, basevertex)); }
void glqDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices, GLint basevertex) { addCommand(makeFunctor(glDrawRangeElementsBaseVertex, mode, start, end, count, type, indices, basevertex)); }
void glqDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex) { addCommand(makeFunctor(glDrawElementsInstancedBaseVertex, mode, count, type, indices, primcount, basevertex)); }
void glqMultiDrawElementsBaseVertex(GLenum mode, const GLsizei* count, GLenum type, const GLvoid* const *indices, GLsizei primcount, const GLint *basevertex) { addCommand(makeFunctor(glMultiDrawElementsBaseVertex, mode, count, type, indices, primcount, basevertex)); }

/* Buffer Objects */
void glqGenBuffers(GLsizei n, GLuint* buffers) { addCommand(makeFunctor(glGenBuffers, n, buffers)); }
void glqDeleteBuffers(GLsizei n, const GLuint* buffers) { addCommand(makeFunctor(glDeleteBuffers, n, buffers)); }
// Creating and Binding Buffer Objects
void glqBindBuffer(GLenum target, GLuint& buffer) { addCommand(makeFunctor(glBindBuffer, target, reference(buffer))); }
void glqBindBufferRange(GLenum target, GLuint& index, GLuint& buffer, GLintptr offset, GLsizeiptr size) { addCommand(makeFunctor(glBindBufferRange, target, reference(index), reference(buffer), offset, size)); }
void glqBindBufferBase(GLenum target, GLuint& index, GLuint& buffer) { addCommand(makeFunctor(glBindBufferBase, target, reference(index), reference(buffer))); }
// Creating Buffer Object Data Stores
void glqBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) { addCommand(makeFunctor(glBufferData, target, size, data, usage)); }
void glqBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) { addCommand(makeFunctor(glBufferSubData, target, offset, size, data)); }
// Mapping and Unmapping Buffer Data
void glqMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access, void*& out) { addCommand(makeFunctor(out, glMapBufferRange, target, offset, length, access)); }
void glqMapBuffer(GLenum target, GLenum access, void*& out) { addCommand(makeFunctor(out, glMapBuffer, target, access)); }
void glqFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length) { addCommand(makeFunctor(glFlushMappedBufferRange, target, offset, length)); }
void glqUnmapBuffer(GLenum target, GLboolean& out) { addCommand(makeFunctor(out, glUnmapBuffer, target)); }
// Copying Between Buffers
void glqCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size) { addCommand(makeFunctor(glCopyBufferSubData, readTarget, writeTarget, readoffset, writeoffset, size)); }
// Vertex Array Objects
void glqGenVertexArrays(GLsizei n, GLuint* arrays) { addCommand(makeFunctor(glGenVertexArrays, n, arrays)); }
void glqDeleteVertexArrays(GLsizei n, const GLuint* arrays) { addCommand(makeFunctor(glDeleteVertexArrays, n, arrays)); }

void glqBindVertexArray(GLuint& array) { addCommand(makeFunctor(glBindVertexArray, reference(array))); }
// Buffer Object Queries
void glqIsBuffer(GLuint& buffer, GLboolean& out) { addCommand(makeFunctor(out, glIsBuffer, reference(buffer))); }
void glqGetBufferParameteriv(GLenum target, GLenum pname, GLint* data) { addCommand(makeFunctor(glGetBufferParameteriv, target, pname, data)); }
void glqGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data) { addCommand(makeFunctor(glGetBufferSubData, target, offset, size, data)); }
void glqGetBufferPointerv(GLenum target, GLenum pname, GLvoid** params) { addCommand(makeFunctor(glGetBufferPointerv, target, pname, params)); }
// Vertex Array Object Queries
void glqIsVertexArray(GLuint& arr, GLboolean& out) { addCommand(makeFunctor(out, glIsVertexArray, reference(arr))); }

/* Viewport and Clipping */
// Controlling the Viewport
void glqDepthRange(GLclampd n, GLclampd f) { addCommand(makeFunctor(glDepthRange, n, f)); }
void glqViewport(GLint x, GLint y, GLsizei w, GLsizei h) { addCommand(makeFunctor(glViewport, x, y, w, h)); }
// Clipping
void glqClipPlane(GLenum p, const GLdouble eqn[4]) { addCommand(makeFunctor(glClipPlane, p, eqn)); }
// Color
void glqClampColor(GLenum target, GLenum clamp) { addCommand(makeFunctor(glClampColor, target, clamp)); }
// Flatshading
void glqProvokingVertex(GLenum provokeMode) { addCommand(makeFunctor(glProvokingVertex, provokeMode)); }

/* Rendering Control and Queries */
// Conditional Rendering
void glqBeginConditionalRender(GLuint& id, GLenum mode) { addCommand(makeFunctor(glBeginConditionalRender, reference(id), mode)); }
void glqEndConditionalRender() { addCommand(makeFunctor(glEndConditionalRender)); }
// Transform Feedback
void glqBeginTransformFeedback(GLenum primitiveMode) { addCommand(makeFunctor(glBeginTransformFeedback, primitiveMode)); }
void glqEndTransformFeedback() { addCommand(makeFunctor(glEndTransformFeedback)); }
// Asynchronous Queries
void glqBeginQuery(GLenum target, GLuint& id) { addCommand(makeFunctor(glBeginQuery, target, reference(id))); }
void glqEndQuery(GLenum target) { addCommand(makeFunctor(glEndQuery, target)); }
void glqGenQueries(GLsizei n, GLuint* ids) { addCommand(makeFunctor(glGenQueries, n, ids)); }
void glqDeleteQueries(GLsizei n, const GLuint* ids) { addCommand(makeFunctor(glDeleteQueries, n, ids)); }
// Asynchronous State Queries
void glqIsQuery(GLuint& id, GLboolean& out) { addCommand(makeFunctor(out, glIsQuery, reference(id))); }
void glqGetQueryiv(GLenum target, GLenum pname, GLint* params) { addCommand(makeFunctor(glGetQueryiv, target, pname, params)); }
void glqGetQueryObjectiv(GLuint& id, GLenum pname, GLint* params) { addCommand(makeFunctor(glGetQueryObjectiv, reference(id), pname, params)); }
void glqGetQueryObjectuiv(GLuint& id, GLenum pname, GLuint* params) { addCommand(makeFunctor(glGetQueryObjectuiv, reference(id), pname, params)); }

/* Shader and Programs */
// Shader Objects
void glqCreateShader(GLenum type, GLuint& out) { addCommand(makeFunctor(out, glCreateShader, type)); }
void glqShaderSource(GLuint& shader, GLsizei count, const GLchar** string, const GLint* length) { addCommand(makeFunctor(glShaderSource, reference(shader), count, string, length)); }
void glqCompileShader(GLuint& shader) { addCommand(makeFunctor(glCompileShader, reference(shader))); }
void glqDeleteShader(GLuint& shader) { addCommand(makeFunctor(glDeleteShader, reference(shader))); }
// Program Objects
void glqCreateProgram(GLuint& out) { addCommand(makeFunctor(out, glCreateProgram)); }
void glqAttachShader(GLuint& program, GLuint& shader) { addCommand(makeFunctor(glAttachShader, reference(program), reference(shader))); }
void glqDetachShader(GLuint& program, GLuint& shader) { addCommand(makeFunctor(glDetachShader, reference(program), reference(shader))); }
void glqLinkProgram(GLuint& program) { addCommand(makeFunctor(glLinkProgram, reference(program))); }
void glqUseProgram(GLuint& program) { addCommand(makeFunctor(glUseProgram, reference(program))); }
void glqDeleteProgram(GLuint& program) { addCommand(makeFunctor(glDeleteProgram, reference(program))); }
// Vertex Attributes
void glqGetActiveAttrib(GLuint& program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) { addCommand(makeFunctor(glGetActiveAttrib, reference(program), index, bufSize, length, size, type, name)); }
void glqGetAttribLocation(GLuint& program, const GLchar* name, GLint& out) { addCommand(makeFunctor(out, glGetAttribLocation, reference(program), name)); }
void glqBindAttribLocation(GLuint& program, GLuint index, const GLchar* name) { addCommand(makeFunctor(glBindAttribLocation, reference(program), index, name)); }
// Uniform variables
void glqGetUniformLocation(GLuint& program, const GLchar* name, GLint& out) { addCommand(makeFunctor(out, glGetUniformLocation, reference(program), name)); }
void glqGetUniformBlockIndex(GLuint& program, const GLchar* uniformBlockName, GLuint& out) { addCommand(makeFunctor(out, glGetUniformBlockIndex, reference(program), uniformBlockName)); }
void glqGetActiveUniformBlockName(GLuint& program, GLuint& uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName) { addCommand(makeFunctor(glGetActiveUniformBlockName, reference(program), reference(uniformBlockIndex), bufSize, length, uniformBlockName)); }
void glqGetActiveUniformBlockiv(GLuint& program, GLuint& uniformBlockIndex, GLenum pname, GLint* params) { addCommand(makeFunctor(glGetActiveUniformBlockiv, reference(program), reference(uniformBlockIndex), pname, params)); }
void glqGetUniformIndices(GLuint& program, GLsizei uniformCount, const GLchar** uniformNames, GLuint *uniformIndices) { addCommand(makeFunctor(glGetUniformIndices, reference(program), uniformCount, uniformNames, uniformIndices)); }
void glqGetActiveUniformName(GLuint& program, GLuint& uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName) { addCommand(makeFunctor(glGetActiveUniformName, reference(program), reference(uniformIndex), bufSize, length, uniformName)); }
void glqGetActiveUniform(GLuint& program, GLuint& index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) { addCommand(makeFunctor(glGetActiveUniform, reference(program), reference(index), bufSize, length, size, type, name)); }
void glqGetActiveUniformsiv(GLuint& program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params) { addCommand(makeFunctor(glGetActiveUniformsiv, reference(program), uniformCount, uniformIndices, pname, params)); }

/* Loading Uniform Variables in Default Uniform Block */
void glqUniform1i(GLint& location, GLint value) { addCommand(makeFunctor(glUniform1i, reference(location), value)); }
void glqUniform2i(GLint& location, GLint value1, GLint value2) { addCommand(makeFunctor(glUniform2i, reference(location), value1, value2)); }
void glqUniform3i(GLint& location, GLint value1, GLint value2, GLint value3) { addCommand(makeFunctor(glUniform3i, reference(location), value1, value2, value3)); }
void glqUniform4i(GLint& location, GLint value1, GLint value2, GLint value3, GLint value4) { addCommand(makeFunctor(glUniform4i, reference(location), value1, value2, value3, value4)); }
void glqUniform1iv(GLint& location, GLsizei count, const GLint* value) { addCommand(makeFunctor(glUniform1iv, reference(location), count, value)); }
void glqUniform2iv(GLint& location, GLsizei count, const GLint* value) { addCommand(makeFunctor(glUniform2iv, reference(location), count, value)); }
void glqUniform3iv(GLint& location, GLsizei count, const  GLint* value) { addCommand(makeFunctor(glUniform3iv, reference(location), count, value)); }
void glqUniform4iv(GLint& location, GLsizei count, const  GLint* value) { addCommand(makeFunctor(glUniform4iv, reference(location), count, value)); }
void glqUniform1ui(GLint& location, GLuint value) { addCommand(makeFunctor(glUniform1ui, reference(location), value)); }
void glqUniform2ui(GLint& location, GLuint value1, GLuint value2) { addCommand(makeFunctor(glUniform2ui, reference(location), value1, value2)); }
void glqUniform3ui(GLint& location, GLuint value1, GLuint value2, GLuint value3) { addCommand(makeFunctor(glUniform3ui, reference(location), value1, value2, value3)); }
void glqUniform4ui(GLint& location, GLuint value1, GLuint value2, GLuint value3, GLuint value4) { addCommand(makeFunctor(glUniform4ui, reference(location), value1, value2, value3, value4)); }
void glqUniform1uiv(GLint& location, GLsizei count, const GLuint* value) { addCommand(makeFunctor(glUniform1uiv, reference(location), count, value)); }
void glqUniform2uiv(GLint& location, GLsizei count, const GLuint* value) { addCommand(makeFunctor(glUniform1uiv, reference(location), count, value)); }
void glqUniform3uiv(GLint& location, GLsizei count, const GLuint* value) { addCommand(makeFunctor(glUniform1uiv, reference(location), count, value)); }
void glqUniform4uiv(GLint& location, GLsizei count, const GLuint* value) { addCommand(makeFunctor(glUniform1uiv, reference(location), count, value)); }
void glqUniform1f(GLint& location, GLfloat value) { addCommand(makeFunctor(glUniform1f, reference(location), value)); }
void glqUniform2f(GLint& location, GLfloat value1, GLfloat value2) { addCommand(makeFunctor(glUniform2f, reference(location), value1, value2)); }
void glqUniform3f(GLint& location, GLfloat value1, GLfloat value2, GLfloat value3) { addCommand(makeFunctor(glUniform3f, reference(location), value1, value2, value3)); }
void glqUniform4f(GLint& location, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4) { addCommand(makeFunctor(glUniform4f, reference(location), value1, value2, value3, value4)); }
void glqUniform1fv(GLint& location, GLsizei count, const GLfloat* value) { addCommand(makeFunctor(glUniform1fv, reference(location), count, value)); }
void glqUniform2fv(GLint& location, GLsizei count, const GLfloat* value) { addCommand(makeFunctor(glUniform2fv, reference(location), count, value)); }
void glqUniform3fv(GLint& location, GLsizei count, const GLfloat* value) { addCommand(makeFunctor(glUniform3fv, reference(location), count, value)); }
void glqUniform4fv(GLint& location, GLsizei count, const GLfloat* value) { addCommand(makeFunctor(glUniform4fv, reference(location), count, value)); }

void glqUniformMatrix2fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix2fv, reference(location), count, transpose, value)); }
void glqUniformMatrix3fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix3fv, reference(location), count, transpose, value)); }
void glqUniformMatrix4fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix4fv, reference(location), count, transpose, value)); }
void glqUniformMatrix2x3fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix2x3fv, reference(location), count, transpose, value)); }
void glqUniformMatrix3x2fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix3x2fv, reference(location), count, transpose, value)); }
void glqUniformMatrix2x4fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix2x4fv, reference(location), count, transpose, value)); }
void glqUniformMatrix4x2fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix4x2fv, reference(location), count, transpose, value)); }
void glqUniformMatrix3x4fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix3x4fv, reference(location), count, transpose, value)); }
void glqUniformMatrix4x3fv(GLint& location, GLsizei count, GLboolean transpose, const GLfloat* value)
{ addCommand(makeFunctor(glUniformMatrix4x3fv, reference(location), count, transpose, value)); }
// Uniform Buffer Object Bindings
void glqUniformBlockBinding(GLuint& program, GLuint& uniformBlockIndex, GLuint& uniformBlockBinding) { addCommand(makeFunctor(glUniformBlockBinding, reference(program), reference(uniformBlockIndex), reference(uniformBlockBinding))); }
// Varying Variables
void glqTransformFeedbackVaryings(GLuint& program, GLsizei count, const GLchar** varyings, GLenum bufferMode)  { addCommand(makeFunctor(glTransformFeedbackVaryings, reference(program), count, varyings, bufferMode)); }
void glqGetTransformFeedbackVarying(GLuint& program, GLuint& index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name) { addCommand(makeFunctor(glGetTransformFeedbackVarying, reference(program), reference(index), bufSize, length, size, type, name)); }
// Shader Execution (Validation)
void glqValidateProgram(GLuint& program) { addCommand(makeFunctor(glValidateProgram, reference(program))); }
// Geometry Shaders
void glqGetProgramiv(GLuint& program, GLenum pname, GLint* params) { addCommand(makeFunctor(glGetProgramiv, reference(program), pname, params)); }
// Fragment Shaders
void glqBindFragDataLocation(GLuint& program, GLuint colorNumber, const GLchar* name) { addCommand(makeFunctor(glBindFragDataLocation, reference(program), colorNumber, name)); }
void glqGetFragDataLocation(GLuint& program, const GLchar* name, GLint& out) { addCommand(makeFunctor(out, glGetFragDataLocation, reference(program), name)); }

/* Shader Queries */
void glqIsShader(GLuint& shader, GLboolean& out) { addCommand(makeFunctor(out, glIsShader, reference(shader))); }
void glqGetShaderiv(GLuint& shader, GLenum pname, GLint* params) { addCommand(makeFunctor(glGetShaderiv, reference(shader), pname, params)); }
void glqGetAttachedShaders(GLuint& program, GLsizei maxCount, GLsizei* count, GLuint* shaders) { addCommand(makeFunctor(glGetAttachedShaders, reference(program), maxCount, count, shaders)); }
void glqGetShaderInfoLog(GLuint& shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) { addCommand(makeFunctor(glGetShaderInfoLog, reference(shader), bufSize, length, infoLog)); }
void glqGetShaderSource(GLuint& shader, GLsizei bufSize, GLsizei* length, GLchar* source) { addCommand(makeFunctor(glGetShaderSource, reference(shader), bufSize, length, source)); }
//TODO: 
//void GetVertexAttrib{ dfi li lui }v(uint index, enum pname, double *params);
//void GetVertexAttribPointerv(uint index, enum pname, void **pointer);
//void GetUniform{if ui}v(uint program, int location, T *params);
void glqIsProgram(GLuint& program, GLboolean& out) { addCommand(makeFunctor(out, glIsProgram, reference(program))); }
void glqGetProgramInfoLog(GLuint& program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) { addCommand(makeFunctor(glGetProgramInfoLog, reference(program), bufSize, length, infoLog)); }

/* Rasterization */
// Multisampling
void glqGetMultisamplefv(GLenum pname, GLuint index, float* val) { addCommand(makeFunctor(glGetMultisamplefv, pname, index, val)); }
// Points
void glqPointSize(GLfloat size) { addCommand(makeFunctor(glPointSize, size)); }
void glqPointParameteri(GLenum pname, GLint param) { addCommand(makeFunctor(glPointParameteri, pname, param)); }
void glqPointParameterf(GLenum pname, GLfloat param) { addCommand(makeFunctor(glPointParameterf, pname, param)); }
void glqPointParameteriv(GLenum pname, const GLint* params) { addCommand(makeFunctor(glPointParameteriv, pname, params)); }
void glqPointParameterfv(GLenum pname, const GLfloat* params) { addCommand(makeFunctor(glPointParameterfv, pname, params)); }
// Line Segments
void glqLineWidth(GLfloat width) { addCommand(makeFunctor(glLineWidth, width)); }
// Polygons
void glqFrontFace(GLenum dir) { addCommand(makeFunctor(glFrontFace, dir)); }
void glqCullFace(GLenum mode) { addCommand(makeFunctor(glCullFace, mode)); }
// Polygon Rasterization & Depth Offset
void glqPolygonMode(GLenum face, GLenum mode) { addCommand(makeFunctor(glPolygonMode, face, mode)); }
void glqPolygonOffset(GLfloat factor, GLfloat units) { addCommand(makeFunctor(glPolygonOffset, factor, units)); }
// Pixel Rectangles
void glqPixelStorei(GLenum pname, GLint value) { addCommand(makeFunctor(glPixelStorei, pname, value)); }
void glqPixelstoref(GLenum pname, GLfloat value) { addCommand(makeFunctor(glPixelStoref, pname, value)); }

/* Texturing */
void glqActiveTexture(GLenum texture) { addCommand(makeFunctor(glActiveTexture, texture)); }
// Texture Image Specification
void glqTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data)
{ addCommand(makeFunctor(glTexImage3D, target, level, internalformat, width, height, depth, border, format, type, data)); }
void glqTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{ addCommand(makeFunctor(glTexImage2D, target, level, internalformat, width, height, border, format, type, data)); }
void glqTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data)
{ addCommand(makeFunctor(glTexImage1D, target, level, internalformat, width, border, format, type, data)); }
void glqCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{ addCommand(makeFunctor(glCopyTexImage2D, target, level, internalformat, x, y, width, height, border)); }
void glqCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{ addCommand(makeFunctor(glCopyTexImage1D, target, level, internalformat, x, y, width, border)); }
void glqTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* data)
{ addCommand(makeFunctor(glTexSubImage3D, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data)); }
void glqTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
{ addCommand(makeFunctor(glTexSubImage2D, target, level, xoffset, yoffset, width, height, format, type, data)); }
void glqTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* data)
{ addCommand(makeFunctor(glTexSubImage1D, target, level, xoffset, width, format, type, data)); }
void glqCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{ addCommand(makeFunctor(glCopyTexSubImage3D, target, level, xoffset, yoffset, zoffset, x, y, width, height)); }
void glqCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{ addCommand(makeFunctor(glCopyTexSubImage2D, target, level, xoffset, yoffset, x, y, width, height)); }
void glqCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{ addCommand(makeFunctor(glCopyTexSubImage1D, target, level, xoffset, x, y, width)); }
// Compressed Texture Images
void glqCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data)
{ addCommand(makeFunctor(glCompressedTexImage3D, target, level, internalformat, width, height, depth, border, imageSize, data)); }
void glqCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{ addCommand(makeFunctor(glCompressedTexImage2D, target, level, internalformat, width, height, border, imageSize, data)); }
void glqCompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data)
{ addCommand(makeFunctor(glCompressedTexImage1D, target, level, internalformat, width, border, imageSize, data)); }
void glqCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data)
{ addCommand(makeFunctor(glCompressedTexSubImage3D, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data)); }
void glqCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{ addCommand(makeFunctor(glCompressedTexSubImage2D, target, level, xoffset, yoffset, width, height, format, imageSize, data)); }
void glqCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data)
{ addCommand(makeFunctor(glCompressedTexSubImage1D, target, level, xoffset, width, format, imageSize, data)); }
// Multisample Textures
void glqTexImage3DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{ addCommand(makeFunctor(glTexImage3DMultisample, target, samples, internalformat, width, height, depth, fixedsamplelocations)); }
void glqTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{ addCommand(makeFunctor(glTexImage2DMultisample, target, samples, internalformat, width, height, fixedsamplelocations)); }
void glqTexBuffer(GLenum target, GLenum internalformat, GLuint& buffer) { addCommand(makeFunctor(glTexBuffer, target, internalformat, buffer)); }
void glqTexParameteri(GLenum target, GLenum pname, GLint param) { addCommand(makeFunctor(glTexParameteri, target, pname, param)); }
void glqTexParameterf(GLenum target, GLenum pname, GLfloat param) { addCommand(makeFunctor(glTexParameterf, target, pname, param)); }
void glqTexParameteriv(GLenum target, GLenum pname, const GLint* params) { addCommand(makeFunctor(glTexParameteriv, target, pname, params)); }
void glqTexParameterfv(GLenum target, GLenum pname, const GLfloat* params) { addCommand(makeFunctor(glTexParameterfv, target, pname, params)); }
//void TexParameterI{i ui}v(enum target, enum pname, T *params);
void glqGenerateMipmap(GLenum target) { addCommand(makeFunctor(glGenerateMipmap, target)); }
// Texture Objects
void glqBindTexture(GLenum target, GLuint& texture) { addCommand(makeFunctor(glBindTexture, target, texture)); }
void glqDeleteTextures(GLsizei n, const GLuint* textures) { addCommand(makeFunctor(glDeleteTextures, n, textures)); }
void glqGenTextures(GLsizei n, GLuint* textures) { addCommand(makeFunctor(glGenTextures, n, textures)); }
void glqGetTexParameteriv(GLenum target, GLenum value, GLint* data) { addCommand(makeFunctor(glGetTexParameteriv, target, value, data)); }
void glqGetTexParameterfv(GLenum target, GLenum value, GLfloat* data) { addCommand(makeFunctor(glGetTexParameterfv, target, value, data)); }
//void GetTexParameterI{i ui}v(enum target, enum value, T data);
void glqGetTexLevelParameteriv(GLenum target, GLint lod, GLenum value, GLint* data) { addCommand(makeFunctor(glGetTexLevelParameteriv, target, lod, value, data)); }
void glqGetTexLevelParameterfv(GLenum target, GLint lod, GLenum value, GLfloat* data) { addCommand(makeFunctor(glGetTexLevelParameterfv, target, lod, value, data)); }
//TODO: Texture Queries

/* Drawing, Reading and Copying Pixels */
// Reading Pixels
void glqReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data) { addCommand(makeFunctor(glReadPixels, x, y, width, height, format, type, data)); }
void glqReadBuffer(GLenum src) { addCommand(makeFunctor(glReadBuffer, src)); }
void glqBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{ addCommand(makeFunctor(glBlitFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter)); }

/* Per-Fragment Operations */
void glqScissor(GLint left, GLint bottom, GLsizei width, GLsizei height)
{ addCommand(makeFunctor(glScissor, left, bottom, width, height)); }
void glqSampleCoverage(GLclampf value, GLboolean invert)
{ addCommand(makeFunctor(glSampleCoverage, value, invert)); }
void glqSampleMaski(GLuint maskNumber, GLbitfield mask)
{ addCommand(makeFunctor(glSampleMaski, maskNumber, mask)); }
void glqStencilFunc(GLenum func, GLint ref, GLuint mask)
{ addCommand(makeFunctor(glStencilFunc, func, ref, mask)); }
void glqStencilFuncSeperate(GLenum face, GLenum func, GLint ref, GLuint mask)
{ addCommand(makeFunctor(glStencilFuncSeparate, face, func, ref, mask)); }
void glqStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{ addCommand(makeFunctor(glStencilOp, sfail, dpfail, dppass)); }
void glqStencilOpSeperate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{ addCommand(makeFunctor(glStencilOpSeparate, face, sfail, dpfail, dppass)); }
void glqDepthFunc(GLenum func)
{ addCommand(makeFunctor(glDepthFunc, func)); }

void glqBlendEquation(GLenum mode)
{ addCommand(makeFunctor(glBlendEquation, mode)); }
void glqBlendEquationSeperate(GLenum modeRGB, GLenum modeAlpha)
{ addCommand(makeFunctor(glBlendEquationSeparate, modeRGB, modeAlpha)); }
void glqBlendFuncSeperate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{ addCommand(makeFunctor(glBlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha)); }
void glqBlendFunc(GLenum src, GLenum dst)
{ addCommand(makeFunctor(glBlendFunc, src, dst)); }
void glqBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{ addCommand(makeFunctor(glBlendColor, red, green, blue, alpha)); }
void glqLogicOp(GLenum op)
{ addCommand(makeFunctor(glLogicOp, op)); }
void glqDrawBuffer(GLenum buf)
{ addCommand(makeFunctor(glDrawBuffer, buf)); }
void glqDrawBuffers(GLsizei n, const GLenum* bufs)
{ addCommand(makeFunctor(glDrawBuffers, n, bufs)); }
void glqColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{ addCommand(makeFunctor(glColorMask, r, g, b, a)); }
void glqColorMaski(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{ addCommand(makeFunctor(glColorMaski, buf, r, g, b, a)); }
void glqDepthMask(GLboolean mask)
{ addCommand(makeFunctor(glDepthMask, mask)); }
void glqStencilMask(GLuint mask)
{ addCommand(makeFunctor(glStencilMask, mask)); }
void glqStencilMaskSeparate(GLenum face, GLuint mask)
{ addCommand(makeFunctor(glStencilMaskSeparate, face, mask)); }
void glqClear(GLbitfield buf)
{ addCommand(makeFunctor(glClear, buf)); }
void glqClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{ addCommand(makeFunctor(glClearColor, r, g, b, a)); }
void glqClearIndex(GLfloat index)
{ addCommand(makeFunctor(glClearIndex, index)); }
void glqClearDepth(GLclampd d)
{ addCommand(makeFunctor(glClearDepth, d)); }
void glqClearStencil(GLint s)
{ addCommand(makeFunctor(glClearStencil, s)); }
void glqClearAccum(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{ addCommand(makeFunctor(glClearAccum, r, g, b, a)); }
void glqClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value)
{ addCommand(makeFunctor(glClearBufferiv, buffer, drawbuffer, value)); }
void glqClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
{ addCommand(makeFunctor(glClearBufferfv, buffer, drawbuffer, value)); }
void glqClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value)
{ addCommand(makeFunctor(glClearBufferuiv, buffer, drawbuffer, value)); }
void glqClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{ addCommand(makeFunctor(glClearBufferfi, buffer, drawbuffer, depth, stencil)); }
/* Framebuffer Objects */
void glqBindFramebuffer(GLenum target, GLuint& framebuffer)
{ addCommand(makeFunctor(glBindFramebuffer, target, framebuffer)); }
void glqDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{ addCommand(makeFunctor(glDeleteFramebuffers, n, framebuffers)); }
void glqGenFramebuffers(GLsizei n, GLuint *ids)
{ addCommand(makeFunctor(glGenFramebuffers, n, ids)); }
void glqBindRenderbuffer(GLenum target, GLuint& renderbuffer)
{ addCommand(makeFunctor(glBindRenderbuffer, target, renderbuffer)); }
void glqDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{ addCommand(makeFunctor(glDeleteRenderbuffers, n, renderbuffers)); }
void glqGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{ addCommand(makeFunctor(glGenRenderbuffers, n, renderbuffers)); }
void glqRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{ addCommand(makeFunctor(glRenderbufferStorageMultisample, target, samples, internalformat, width, height)); }
void glqRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{ addCommand(makeFunctor(glRenderbufferStorage, target, internalformat, width, height)); }
void glqFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint& renderbuffer)
{ addCommand(makeFunctor(glFramebufferRenderbuffer, target, attachment, renderbuffertarget, renderbuffer)); }
void glqFramebufferTexture(GLenum target, GLenum attachment, GLuint& texture, GLint level)
{ addCommand(makeFunctor(glFramebufferTexture, target, attachment, texture, level)); }
void glqFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint& texture, GLint level, GLint layer)
{ addCommand(makeFunctor(glFramebufferTexture3D, target, attachment, textarget, texture, level, layer)); }
void glqFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint& texture, GLint level)
{ addCommand(makeFunctor(glFramebufferTexture2D, target, attachment, textarget, texture, level)); }
void glqFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint& texture, GLint level)
{ addCommand(makeFunctor(glFramebufferTexture1D, target, attachment, textarget, texture, level)); }
void glqFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint& texture, GLint level, GLint layer)
{ addCommand(makeFunctor(glFramebufferTextureLayer, target, attachment, texture, level, layer)); }
void glqCheckFramebufferStatus(GLenum target, GLenum& out)
{ addCommand(makeFunctor(out, glCheckFramebufferStatus, target)); }
//TODO: Framebuffer Object Queries
//TODO: Renderbuffer Object Queries

/* Synchronization */
void glqFlush() { addCommand(makeFunctor(glFlush)); }
void glqFinish() { addCommand(makeFunctor(glFinish)); }
void glqFenceSync(GLenum condition, GLbitfield flags, GLsync& out) { addCommand(makeFunctor(out, glFenceSync, condition, flags)); }
void glqDeleteSync(GLsync& sync) { addCommand(makeFunctor(glDeleteSync, reference(sync))); }
void glqClientWaitSync(GLsync& sync, GLbitfield flags, GLuint64 timeout_ns, GLenum& out) { addCommand(makeFunctor(out, glClientWaitSync, reference(sync), flags, timeout_ns)); }
void glqWaitSync(GLsync& sync, GLbitfield flags, GLuint64 timeout_ns) { addCommand(makeFunctor(glWaitSync, reference(sync), flags, timeout_ns)); }
void glqGetSynciv(GLsync& sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) { addCommand(makeFunctor(glGetSynciv, reference(sync), pname, bufSize, length, values)); }
void glqIsSync(GLsync& sync, GLboolean& out) { addCommand(makeFunctor(out, glIsSync, reference(sync))); }
