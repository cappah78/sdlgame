#pragma once

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef char GLbyte;
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

class GLCommandQueue
{
public:
	GLCommandQueue();
	~GLCommandQueue();
	GLCommandQueue(const GLCommandQueue& copy) = delete;

	GLuint glCreateProgram();
	void glLinkProgram(GLuint programID);
	GLuint glCreateShader(GLenum type);
	void glShaderSource(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
	void glCompileShader(GLuint shaderID);
	void glAttachShader(GLuint programID, GLuint shaderID);
	void glDeleteShader(GLuint shaderID);

	void glGenVertexArrays(GLsizei numVAOs, GLuint* vaos);
	void glBindVertexArray(GLuint vao);
	void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* offset);
	void glEnableVertexAttribArray(GLuint index);
	void glDisableVertexAttribArray(GLuint index);
	void glVertexAttribDivisor(GLuint index, GLuint divisor);

	void glGenBuffers(GLsizei numBuffers, GLuint* buffers);
	void glBindBuffer(GLuint bufferID);
	void glBufferData(GLenum target, GLsizei size, const GLvoid* data, GLenum usage);
	void glBufferSubData(GLenum target, GLsizei offset, GLsizei size, const GLvoid* data);
	void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	GLuint glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
	void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
	void glDeleteBuffers(GLsizei numBuffers, GLuint* buffers);

	void glGenTextures(GLsizei numTextures, GLuint* textures);
	void glActiveTexture(GLenum texture);
	void glBindTexture(GLenum target, GLuint texture);
	void glTexParameteri(GLenum target, GLenum pname, GLint param);
	void glPixelStorei(GLenum pname, GLint param);
	void glDeleteTextures(GLsizei numTextures, GLuint* textures);

	void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
	void glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);

private:
};