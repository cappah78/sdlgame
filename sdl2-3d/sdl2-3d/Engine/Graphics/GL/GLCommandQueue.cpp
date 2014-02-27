#include "GLCommandQueue.h"

#include <GL/glew.h>

#include <SDL_thread.h>

std::vector<GLCommand> GLCommandQueue::s_commandQueue;
SDL_mutex* GLCommandQueue::s_mutex = SDL_CreateMutex();

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
	s_commandQueue.emplace_back(GenVertexArraysCommand(numVAOs, vaos));
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