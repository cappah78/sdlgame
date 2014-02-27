#pragma once

#include <vector>

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef char GLchar;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

class GLCommand
{
public:
	virtual void execute() {};
};

struct SDL_mutex;
class GLCommandQueue
{
public:
	GLCommandQueue();
	~GLCommandQueue();
	GLCommandQueue(const GLCommandQueue& copy) = delete;

	static void update();

	GLuint glqCreateProgram();
	void glqLinkProgram(GLuint programID);
	GLuint glqCreateShader(GLenum type);
	void glqShaderSource(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
	void glqCompileShader(GLuint shaderID);
	void glqAttachShader(GLuint programID, GLuint shaderID);
	void glqDeleteShader(GLuint shaderID);

	void glqGenVertexArrays(GLsizei numVAOs, GLuint* vaos);
	void glqBindVertexArray(GLuint vao);
	void glqVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void glqVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* offset);
	void glqEnableVertexAttribArray(GLuint index);
	void glqDisableVertexAttribArray(GLuint index);
	void glqVertexAttribDivisor(GLuint index, GLuint divisor);

	void glqGenBuffers(GLsizei numBuffers, GLuint* buffers);
	void glqBindBuffer(GLuint bufferID);
	void glqBufferData(GLenum target, GLsizei size, const GLvoid* data, GLenum usage);
	void glqBufferSubData(GLenum target, GLsizei offset, GLsizei size, const GLvoid* data);
	void glqUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	GLuint glqGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
	void glqBindBufferBase(GLenum target, GLuint index, GLuint buffer);
	void glqDeleteBuffers(GLsizei numBuffers, GLuint* buffers);

	void glqGenTextures(GLsizei numTextures, GLuint* textures);
	void glqActiveTexture(GLenum texture);
	void glqBindTexture(GLenum target, GLuint texture);
	void glqTexParameteri(GLenum target, GLenum pname, GLint param);
	void glqPixelStorei(GLenum pname, GLint param);
	void glqDeleteTextures(GLsizei numTextures, GLuint* textures);

	void glqDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
	void glqDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);

private:

	static std::vector<GLCommand> s_commandQueue;
	static SDL_mutex* s_mutex;
};