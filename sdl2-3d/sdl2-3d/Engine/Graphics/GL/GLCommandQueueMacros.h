#pragma once

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

#define __5ARGCOMMAND(type1, type2, type3, type4, type5, glCall, arg1, arg2, arg3, arg4, arg5) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) : m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5) {}; \
	type1 m_arg1;	\
	type2 m_arg2;	\
	type3 m_arg3;	\
	type4 m_arg4;	\
	type5 m_arg5;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5); \
	}	\
};	\
	addCommand(Command(arg1, arg2, arg3, arg4, arg5)); \
}

#define __6ARGCOMMAND(type1, type2, type3, type4, type5, type6, glCall, arg1, arg2, arg3, arg4, arg5, arg6) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) : m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6) {}; \
	type1 m_arg1;	\
	type2 m_arg2;	\
	type3 m_arg3;	\
	type4 m_arg4;	\
	type5 m_arg5;	\
	type6 m_arg6;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6); \
	}	\
};	\
	addCommand(Command(arg1, arg2, arg3, arg4, arg5, arg6)); \
}

#define __7ARGCOMMAND(type1, type2, type3, type4, type5, type6, type7, glCall, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
{ \
class Command : public GLCommand	\
{	\
public:	\
	Command(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7) : m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7) {}; \
	type1 m_arg1;	\
	type2 m_arg2;	\
	type3 m_arg3;	\
	type4 m_arg4;	\
	type5 m_arg5;	\
	type6 m_arg6;	\
	type7 m_arg7;	\
	virtual void execute() override	\
	{	\
		glCall(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7); \
	}	\
};	\
	addCommand(Command(arg1, arg2, arg3, arg4, arg5, arg6, arg7)); \
}