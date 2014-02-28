#include "GLCommandQueue.h"

#include <GL/glew.h>

#include <SDL_thread.h>

std::vector<GLCommand> GLCommandQueue::s_commandQueue;
SDL_mutex* GLCommandQueue::s_mutex = SDL_CreateMutex();


inline void GLCommandQueue::addCommand(GLCommand& command)
{
	SDL_LockMutex(s_mutex);
	s_commandQueue.emplace_back(command);
	SDL_UnlockMutex(s_mutex);
}

void GLCommandQueue::update()
{
	SDL_LockMutex(s_mutex);
	for (GLCommand command : s_commandQueue)
	{
		command.execute();
	}
	s_commandQueue.clear();
	SDL_UnlockMutex(s_mutex);
}

void GLCommandQueue::glqGenVertexArrays(GLsizei numVAOs, GLuint* vaos)
{
	class GenVertexArraysCommand : public GLCommand 
	{ 
	public:
		GenVertexArraysCommand(GLsizei numVAOs, GLuint* vaos) : m_numVAOs(numVAOs), m_vaos(vaos) {}
		GLsizei m_numVAOs;
		GLuint* m_vaos;
		virtual void execute() override 
		{ 
			glGenVertexArrays(m_numVAOs, m_vaos); 
		}
	};
	addCommand(GenVertexArraysCommand(numVAOs, vaos));
}

void GLCommandQueue::glqBindVertexArray(GLuint vao)
{
	class BindVertexArrayCommand : public GLCommand
	{
	public:
		BindVertexArrayCommand(GLuint vao) : m_vao(vao) {};
		virtual void execute()
		{
			glBindVertexArray(m_vao);
		}
		GLuint m_vao;
	};
	addCommand(BindVertexArrayCommand(vao));
}

void GLCommandQueue::glqGenBuffers(GLsizei numBuffers, GLuint* buffers)
{
	class GenBuffersCommand : public GLCommand
	{
	public:
		GenBuffersCommand(GLsizei numBuffers, GLuint* buffers) : m_numBuffers(numBuffers), m_buffers(buffers) {}
		GLsizei m_numBuffers;
		GLuint* m_buffers;
		virtual void execute()
		{
			glGenBuffers(m_numBuffers, m_buffers);
		}
	};
	addCommand(GenBuffersCommand(numBuffers, buffers));
}
void GLCommandQueue::glqBindBuffer(GLenum target, GLuint bufferID)
{
	class BindBufferCommand : public GLCommand
	{
	public:
		BindBufferCommand(GLenum target, GLuint bufferID) : m_target(target), m_bufferID(bufferID) {}
		GLenum m_target;
		GLuint m_bufferID;
		virtual void execute()
		{
			glBindBuffer(m_target, m_bufferID);
		}
	};
	addCommand(BindBufferCommand(target, bufferID));
}
void GLCommandQueue::glqBufferData(GLenum target, GLsizei size, const GLvoid* data, GLenum usage)
{
	class BufferDataCommand : public GLCommand
	{
	public:
		BufferDataCommand(GLenum target, GLsizei size, const GLvoid* data, GLenum usage)
			: m_target(target), m_size(size), m_data(data), m_usage(usage) {}
		GLenum m_target; 
		GLsizei m_size; 
		const GLvoid* m_data; 
		GLenum m_usage;
		virtual void execute()
		{
			glBufferData(m_target, m_size, m_data, m_usage);
		}
	};
	addCommand(BufferDataCommand(target, size, data, usage));
}


