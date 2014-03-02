#include "GLCommandQueue.h"

#include <vector>
#include <GL/glew.h>
#include <SDL_thread.h>

std::vector<GLCommand> s_commandQueue;
SDL_mutex* s_mutex = SDL_CreateMutex();

#define __1ARGCOMMAND(type1, glCall, arg1) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1) : m_arg1(arg1) {}; \
	type1 m_arg1;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1); \
	}	\
};	\
	addCommand(Command(arg1)); \
}

#define __2ARGCOMMAND(type1, type2, glCall, arg1, arg2) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1, type2 arg2) : m_arg1(arg1), m_arg2(arg2) {}; \
	type1 m_arg1;	\
	type2 m_arg2;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1, m_arg2); \
	}	\
};	\
	addCommand(Command(arg1, arg2)); \
}

#define __3ARGCOMMAND(type1, type2, type3, glCall, arg1, arg2, arg3) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1, type2 arg2, type3 arg3) : m_arg1(arg1), m_arg2(arg2), m_arg3(arg3) {}; \
	type1 m_arg1;	\
	type2 m_arg2;	\
	type3 m_arg3;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1, m_arg2, m_arg3); \
	}	\
};	\
	addCommand(Command(arg1, arg2, arg3)); \
}

#define __4ARGCOMMAND(type1, type2, type3, type4, glCall, arg1, arg2, arg3, arg4) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1, type2 arg2, type3 arg3, type4 arg4) : m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4) {}; \
	type1 m_arg1;	\
	type2 m_arg2;	\
	type3 m_arg3;	\
	type4 m_arg4;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1, m_arg2, m_arg3, m_arg4); \
	}	\
};	\
	addCommand(Command(arg1, arg2, arg3, arg4)); \
}


namespace GLCommandQueue
{
	inline void addCommand(GLCommand& command)
	{
		SDL_LockMutex(s_mutex);
		s_commandQueue.emplace_back(command);
		SDL_UnlockMutex(s_mutex);
	}

	void update()
	{
		SDL_LockMutex(s_mutex);
		for (GLCommand command : s_commandQueue)
		{
			command.execute();
		}
		s_commandQueue.clear();
		SDL_UnlockMutex(s_mutex);
	}
}

using namespace GLCommandQueue;

void glqGenVertexArrays(GLsizei n, GLuint* arrays) { __2ARGCOMMAND(GLsizei, GLuint*, glGenVertexArrays, n, arrays); }
void glqGenBuffers(GLsizei n, GLuint* buffers) { __2ARGCOMMAND(GLsizei, GLuint*, glGenBuffers, n, buffers); }
