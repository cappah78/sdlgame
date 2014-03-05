#pragma once

#include <stdint.h>

#define NULL 0

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

typedef struct __GLsync* GLsync;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#else
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
void testFunctor();

class GLCommand
{
public:
	virtual void execute() {};
};

namespace GLCommandQueue
{
	void update();
	inline void addCommand(GLCommand& command);
}

/* OpenGL 3.2 Reference Card http://www.khronos.org/files/opengl-quick-reference-card.pdf */

void glqEnable(GLenum target);
void glqDisable(GLenum target);

/* Vertex Arrays */
void glqVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
void glqVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
void glqEnableVertexAttribArray(GLuint index);
void glqDisableVertexAttribArray(GLuint index);
void glqPrimitiveRestartIndex(GLuint index);
// Drawing Commands
void glqDrawArrays(GLenum mode, GLint first, GLsizei count);
void glqMultiDrawArrays(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);
void glqDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
void glqMultiDrawElements(GLenum mode, GLsizei* count, GLenum type, GLvoid** indices, GLsizei primcount);
void glqDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, GLvoid* indices);
void glqDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
void glqDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei primcount);
void glqDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
void glqDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex);
void glqDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
void glqMultiDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei primcount, GLint basevertex);

/* Buffer Objects */
void glqGenBuffers(GLsizei n, GLuint* buffers);
void glqDeleteBuffers(GLsizei n, const GLuint* buffers);
// Creating and Binding Buffer Objects
void glqBindBuffer(GLenum target, GLuint buffer);
void glqBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glqBindBufferBase(GLenum target, GLuint index, GLuint buffer);
// Creating Buffer Object Data Stores
void glqBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
void glqBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
// Mapping and Unmapping Buffer Data
void* glqMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access, void* out);
void* glqMapBuffer(GLenum target, GLenum access, void* out);
void glqFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
bool glqUnmapBuffer(GLenum target, bool* out);
// Copying Between Buffers
void* glqCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size, void* out);
// Vertex Array Objects
void glqGenVertexArrays(GLsizei n, GLuint* arrays);
void glqDeleteVertexArrays(GLsizei n, const GLuint* arrays);
void glqBindVertexArray(GLuint arr);
// Buffer Object Queries
bool glqIsBuffer(GLuint buffer, GLboolean* out);
void glqGetBufferParameteriv(GLenum target, GLenum pname, GLint* data);
void glqGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data);
void glqGetBufferPointerv(GLenum target, GLenum pname, GLvoid** params);
// Vertex Array Object Queries
bool glqIsVertexArray(GLuint arr, GLboolean* out);

/* Viewport and Clipping */
// Controlling the Viewport
void glqDepthRange(GLclampd n, GLclampd f);
void glqViewport(GLint x, GLint y, GLsizei w, GLsizei h);
// Clipping
void glqClipPlane(GLenum p, GLdouble eqn[4]);
// Color
void glqClampColor(GLenum target, GLenum clamp);
// Flatshading
void glqProvokingVertex(GLenum provokeMode);

/* Rendering Control and Queries */
// Conditional Rendering
void glqBeginConditionalRender(GLuint id, GLenum mode);
void glqEndConditionalRender();
// Transform Feedback
void glqBeginTransformFeedback(GLenum primitiveMode);
void glqEndTransformFeedback();
void glqBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
// Asynchronous Queries
void glqBeginQuery(GLenum target, GLuint id);
void glqEndQuery(GLenum target);
void glqGenQueries(GLsizei n, GLuint* ids);
void glqDeleteQueries(GLsizei n, const GLuint* ids);
// Asynchronous State Queries
bool glqIsQuery(GLuint id, GLboolean* out);
void glqGetQueryiv(GLenum target, GLenum pname, GLint* params);
void glqGetQueryObjectiv(GLuint id, GLenum pname, GLint* params);
void glqGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params);

/* Shader and Programs */
// Shader Objects
GLuint glqCreateShader(GLenum type, GLuint* out);
void glqShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
void glqCompileShader(GLuint shader);
void glqDeleteShader(GLuint shader);
// Program Objects
GLuint glqCreateProgram(GLuint* out);
void glqAttachShader(GLuint program, GLuint shader);
void glqDetachShader(GLuint program, GLuint shader);
void glqLinkProgram(GLuint program);
void glqUseProgram(GLuint program);
void glqDeleteProgram(GLuint program);
// Vertex Attributes
void glqGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
GLint glqGetAttribLocation(GLuint program, const GLchar* name, GLint* out);
void glqBindAttribLocation(GLuint program, GLuint index, const GLchar* name);
// Uniform variables
GLint glqGetUniformLocation(GLuint program, const GLchar* name, GLint* out);
GLuint glqGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName, GLuint* out);
void glqGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
void glqGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
void glqGetUniformIndies(GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint *uniformIndices);
void glqGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
void glqGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);

/* Loading Uniform Variables in Default Uniform Block */
void glqUniform1i(GLint location, GLint value);
void glqUniform2i(GLint location, GLint value1, GLint value2);
void glqUniform3i(GLint location, GLint value1, GLint value2, GLint value3);
void glqUniform4i(GLint location, GLint value1, GLint value2, GLint value3, GLint value4);
void glqUniform1iv(GLint location, GLsizei count, GLint* value);
void glqUniform2iv(GLint location, GLsizei count, GLint* value);
void glqUniform3iv(GLint location, GLsizei count, GLint* value);
void glqUniform4iv(GLint location, GLsizei count, GLint* value);
void glqUniform1ui(GLint location, GLuint value);
void glqUniform2ui(GLint location, GLuint value1, GLuint value2);
void glqUniform3ui(GLint location, GLuint value1, GLuint value2, GLuint value3);
void glqUniform4ui(GLint location, GLuint value1, GLuint value2, GLuint value3, GLuint value4);
void glqUniform1uiv(GLint location, GLsizei count, GLuint* value);
void glqUniform2uiv(GLint location, GLsizei count, GLuint* value);
void glqUniform3uiv(GLint location, GLsizei count, GLuint* value);
void glqUniform4uiv(GLint location, GLsizei count, GLuint* value);
void glqUniform1f(GLint location, GLfloat value);
void glqUniform2f(GLint location, GLfloat value1, GLfloat value2);
void glqUniform3f(GLint location, GLfloat value1, GLfloat value2, GLfloat value3);
void glqUniform4f(GLint location, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);
void glqUniform1fv(GLint location, GLsizei count, GLfloat* value);
void glqUniform2fv(GLint location, GLsizei count, GLfloat* value);
void glqUniform3fv(GLint location, GLsizei count, GLfloat* value);
void glqUniform4fv(GLint location, GLsizei count, GLfloat* value);
void glqUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glqUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
// Uniform Buffer Object Bindings
void glqUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
// Varying Variables
void glqTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar** varyings, GLenum bufferMode);
void glqGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
// Shader Execution (Validation)
void glqValidateProgram(GLuint program);
// Geometry Shaders
void glqGetProgramiv(GLuint program, GLenum pname, GLint* params);
// Fragment Shaders
void glqBindFragDataLocation(GLuint program, GLuint colorNumber, const GLchar* name);
GLint glqGetFragDataLocation(GLuint program, const GLchar* name, GLint* out);

/* Shader Queries */
GLboolean glqIsShader(GLuint shader, GLboolean* out);
void glqGetShaderiv(GLuint shader, GLenum pname, GLint* params);
void glqGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
void glqGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void glqGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source);
//TODO: 
//void GetVertexAttrib{ dfi li lui }v(uint index, enum pname, double *params);
//void GetVertexAttribPointerv(uint index, enum pname, void **pointer);
//void GetUniform{if ui}v(uint program, int location, T *params);
GLboolean glqIsProgram(GLuint program, GLboolean* out);
void glqGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

/* Rasterization */
// Multisampling
void glqGetMultisamplefv(GLenum pname, GLuint index, float* val);
// Points
void glqPointSize(GLfloat size);
void glqPointParameteri(GLenum pname, GLint param);
void glqPointParameterf(GLenum pname, GLfloat param);
void glqPointParameteriv(GLenum pname, const GLint* params);
void glqPointParameterfv(GLenum pname, const GLfloat* params);
// Line Segments
void glqLineWidth(GLfloat width);
// Polygons
void glqFrontFace(GLenum dir);
void glqCullFace(GLenum mode);
// Polygon Rasterization & Depth Offset
void glqPolygonMode(GLenum face, GLenum mode);
void glqPolygonOffset(GLfloat factor, GLfloat units);
// Pixel Rectangles
void glqPixelStorei(GLenum pname, GLint value);
void glqPixelstoref(GLenum pname, GLfloat value);

/* Texturing */
void glqActiveTexture(GLenum texture);
// Texture Image Specification
void glqTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, GLvoid* data);
void glqTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLvoid* data);
void glqTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, GLvoid* data);
void glqCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void glqCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void glqTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLvoid* data);
void glqTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data);
void glqTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, GLvoid* data);
void glqCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void glqCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void glqCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
// Compressed Texture Images
void glqCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, GLvoid* data);
void glqCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, GLvoid* data);
void glqCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, GLvoid* data);
void glqCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, GLvoid *data);
void glqCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, GLvoid *data);
void glqCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, GLvoid *data);
// Multisample Textures
void glqTexImage3DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void glqTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void glqTexBuffer(GLenum target, GLenum internalformat, GLuint buffer);
void glqTexParameteri(GLenum target, GLenum pname, GLint param);
void glqTexParameterf(GLenum target, GLenum pname, GLfloat param);
void glqTexParameteriv(GLenum target, GLenum pname, GLint* params);
void glqTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
//void TexParameterI{i ui}v(enum target, enum pname, T *params);
void glqGenerateMipmap(GLenum target);
// Texture Objects
void glqBindTexture(GLenum target, GLuint texture);
void glqDeleteTextures(GLsizei n, GLuint* textures);
void glqGenTextures(GLsizei n, GLuint* textures);
void glqGetTexParameteriv(GLenum target, GLenum value, GLint* data);
void glqGetTexParameterfv(GLenum target, GLenum value, GLfloat* data);
//void GetTexParameterI{i ui}v(enum target, enum value, T data);
void glqGetTexLevelParameteriv(GLenum target, GLint lod, GLenum value, GLint* data);
void glqGetTexLevelParameterfv(GLenum target, GLint lod, GLenum value, GLfloat* data);
//TODO: Texture Queries

/* Drawing, Reading and Copying Pixels */
// Reading Pixels
void glqReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data);
void glqReadBuffer(GLenum src);
void glqBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

/* Per-Fragment Operations */
void glqScissor(GLint left, GLint bottom, GLsizei width, GLsizei height);
void glqSampleCoverage(GLclampf value, GLboolean invert);
void glqSampleMaski(GLuint maskNumber, GLbitfield mask);
void glqStencilFunc(GLenum func, GLint ref, GLuint mask);
void glqStencilFuncSeperate(GLenum face, GLenum func, GLint ref, GLuint masl);
void glqStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
void glqStencilOpSeperate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void glqDepthFunc(GLenum func);
void glqBeginQUery(GLenum target, GLuint id);
void glqEndQuery(GLenum target);
void glqBlendEquation(GLenum mode);
void glqBlendEquationSeperate(GLenum modeRGB, GLenum modeAlpha);
void glqBlendFuncSeperate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void glqBlendFunc(GLenum src, GLenum dst);
void glqBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void glqLocitOp(GLenum op);
void glqDrawBuffer(GLenum buf);
void glqDrawBuffers(GLsizei n, const GLenum* bufs);
void glqColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void glqColorMaski(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void glqDepthMask(GLboolean mask);
void glqStencilMask(GLuint mask);
void glqStencilMaskSeparate(GLenum face, GLuint mask);
void glqClear(GLbitfield buf);
void glqClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);
void glqClearIndex(GLfloat index);
void glqClearDepth(GLclampd d);
void glqClearStencil(GLint s);
void glqClearAccum(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void glqClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value);
void glqClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value);
void glqClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value);
void glqClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);

/* Framebuffer Objects */
void glqBindFramebuffer(GLenum target, GLuint framebuffer);
void glqDeleteFramebuffers(GLsizei n, GLuint *framebuffers);
void glqGenFramebuffers(GLsizei n, GLuint *ids);
void glqBindRenderbuffer(GLenum target, GLuint renderbuffer);
void glqDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
void glqGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
void glqRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void glqRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void glqFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void glqFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
void glqFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
void glqFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glqFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glqFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLenum glqCheckFramebufferStatus(GLenum target, GLenum* out);
//TODO: Framebuffer Object Queries
//TODO: Renderbuffer Object Queries

/* Synchronization */
void glqFlush();
void glqFinish();
GLsync glqFenceSync(GLenum condition, GLbitfield flags, GLsync* out);
void glqDeleteSync(GLsync sync);
GLenum glqClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout_ns, GLenum* out);
void glqWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout_ns);
void glqGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
GLboolean glqIsSync(GLsync sync, GLboolean* out);