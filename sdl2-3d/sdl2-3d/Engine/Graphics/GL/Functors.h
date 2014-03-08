#pragma once

namespace Func
{

class Functor
{
public:
	virtual ~Functor() {}
	virtual void operator()() = 0;
};

template <class TYPE>
class Referencer
{
public:
	Referencer(TYPE& type) : m_type(type) {};
	operator TYPE&() const
	{
		return m_type;
	}
private:
	TYPE& m_type;
};

template <class TYPE>
inline Referencer<TYPE> reference(TYPE& type)
{
	return Referencer<TYPE>(type);
}

template <class RET, class TYPE1, class PARAM1>
class Free1ArgFunctor : public Functor
{
public:
	Free1ArgFunctor(RET(*function)(TYPE1), const PARAM1& arg1) : m_function(function), m_arg1(arg1) {}
	virtual RET operator()() { return m_function(m_arg1); }
private:
	RET(*m_function)(TYPE1);
	const PARAM1& m_arg1;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2>
class Free2ArgFunctor : public Functor
{
public:
	Free2ArgFunctor(RET(*function)(TYPE1, TYPE2), const PARAM1& arg1, const PARAM2& arg2) : m_function(function),
		m_arg1(arg1), m_arg2(arg2) 
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2); }
private:
	RET(*m_function)(TYPE1, TYPE2);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
};


template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3>
class Free3ArgFunctor : public Functor
{
public:
	Free3ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3), const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3) : m_function(function),
		m_arg1(arg1), m_arg2(arg2), m_arg3(arg3)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4>
class Free4ArgFunctor : public Functor
{
public:
	Free4ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4), 
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4) 
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4, 
	class TYPE5, class PARAM5>
class Free5ArgFunctor : public Functor
{
public:
	Free5ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5), 
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5) 
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4, 
	class TYPE5, class PARAM5, class TYPE6, class PARAM6>
class Free6ArgFunctor : public Functor
{
public:
	Free6ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
};
		
template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4, 
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7>
class Free7ArgFunctor : public Functor
{
public:
	Free7ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6, const PARAM7& arg7)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
	const PARAM7& m_arg7;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4, 
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8>
class Free8ArgFunctor : public Functor
{
public:
	Free8ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6, 
		const PARAM7& arg7, const PARAM8& arg8)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7),
		m_arg8(arg8)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
	const PARAM7& m_arg7;
	const PARAM8& m_arg8;
};
		
template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4, 
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9>
class Free9ArgFunctor : public Functor
{
public:
	Free9ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
		const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7),
		m_arg8(arg8), m_arg9(arg9)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8, m_arg9); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
	const PARAM7& m_arg7;
	const PARAM8& m_arg8;
	const PARAM9& m_arg9;
};
		
template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4, 
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9, class TYPE10, class PARAM10>
class Free10ArgFunctor : public Functor
{
public:
	Free10ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
		const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9, const PARAM10& arg10)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7),
		m_arg8(arg8), m_arg9(arg9), m_arg10(arg10)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8, m_arg9, m_arg10); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
	const PARAM7& m_arg7;
	const PARAM8& m_arg8;
	const PARAM9& m_arg9;
	const PARAM10& m_arg10;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9, class TYPE10, class PARAM10, class TYPE11, class PARAM11>
class Free11ArgFunctor : public Functor
{
public:
	Free11ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
		const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9, const PARAM10& arg10, const PARAM11& arg11)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7),
		m_arg8(arg8), m_arg9(arg9), m_arg10(arg10), m_arg11(arg11)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8, m_arg9, m_arg10, m_arg11); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
	const PARAM7& m_arg7;
	const PARAM8& m_arg8;
	const PARAM9& m_arg9;
	const PARAM10& m_arg10;
	const PARAM11& m_arg11;
};

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9, class TYPE10, class PARAM10, class TYPE11, class PARAM11, class TYPE12, class PARAM12>
class Free12ArgFunctor : public Functor
{
public:
	Free12ArgFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11, TYPE12),
		const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
		const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9, const PARAM10& arg10, const PARAM11& arg11, const PARAM12& arg12)
		: m_function(function), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4), m_arg5(arg5), m_arg6(arg6), m_arg7(arg7),
		m_arg8(arg8), m_arg9(arg9), m_arg10(arg10), m_arg11(arg11), m_arg12(arg12)
	{}
	virtual RET operator()() { return m_function(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8, m_arg9, m_arg10, m_arg11, m_arg12); }
private:
	RET(*m_function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11, TYPE12);
	const PARAM1& m_arg1;
	const PARAM2& m_arg2;
	const PARAM3& m_arg3;
	const PARAM4& m_arg4;
	const PARAM5& m_arg5;
	const PARAM6& m_arg6;
	const PARAM7& m_arg7;
	const PARAM8& m_arg8;
	const PARAM9& m_arg9;
	const PARAM10& m_arg10;
	const PARAM11& m_arg11;
	const PARAM12& m_arg12;
};


/* Create a functor to a c free function, reference arguments must be wrapped with reference(arg) */
template <class RET, class TYPE1, class PARAM1>
inline Functor* createFunctor(RET(*function)(TYPE1), const PARAM1& arg1)
{
	return new Free1ArgFunctor<RET, TYPE1, PARAM1>(function, arg1);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2), const PARAM1& arg1, const PARAM2& arg2)
{
	return new Free2ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2>(function, arg1, arg2);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3)
{
	return new Free3ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3>(function, arg1, arg2, arg3);
}


template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4)
{
	return new Free4ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4>
		(function, arg1, arg2, arg3, arg4);
}


template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5)
{
	return new Free5ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5>
		(function, arg1, arg2, arg3, arg4, arg5);
}


template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6)
{
	return new Free6ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6>
		(function, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
	const PARAM7& arg7)
{
	return new Free7ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6,
		TYPE7, PARAM7>
		(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
	const PARAM7& arg7, const PARAM8& arg8)
{
	return new Free8ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6,
		TYPE7, PARAM7, TYPE8, PARAM8>
		(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
	const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9)
{
	return new Free9ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6,
		TYPE7, PARAM7, TYPE8, PARAM8, TYPE9, PARAM9>
		(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9, class TYPE10, class PARAM10>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
	const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9, const PARAM10& arg10)
{
	return new Free10ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6,
		TYPE7, PARAM7, TYPE8, PARAM8, TYPE9, PARAM9, TYPE10, PARAM10>
		(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
}

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9, class TYPE10, class PARAM10, class TYPE11, class PARAM11>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
	const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9, const PARAM10& arg10, const PARAM11& arg11)
{
	return new Free11ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6,
		TYPE7, PARAM7, TYPE8, PARAM8, TYPE9, PARAM9, TYPE10, PARAM10, TYPE11, PARAM11>
		(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}
	

template <class RET, class TYPE1, class PARAM1, class TYPE2, class PARAM2, class TYPE3, class PARAM3, class TYPE4, class PARAM4,
	class TYPE5, class PARAM5, class TYPE6, class PARAM6, class TYPE7, class PARAM7, class TYPE8, class PARAM8,
	class TYPE9, class PARAM9, class TYPE10, class PARAM10, class TYPE11, class PARAM11, class TYPE12, class PARAM12>
inline Functor* createFunctor(RET(*function)(TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11, TYPE12), 
	const PARAM1& arg1, const PARAM2& arg2, const PARAM3& arg3, const PARAM4& arg4, const PARAM5& arg5, const PARAM6& arg6,
	const PARAM7& arg7, const PARAM8& arg8, const PARAM9& arg9, const PARAM10& arg10, const PARAM11& arg11, const PARAM12& arg12)
{
	return new Free12ArgFunctor<RET, TYPE1, PARAM1, TYPE2, PARAM2, TYPE3, PARAM3, TYPE4, PARAM4, TYPE5, PARAM5, TYPE6, PARAM6,
		TYPE7, PARAM7, TYPE8, PARAM8, TYPE9, PARAM9, TYPE10, PARAM10, TYPE11, PARAM11, TYPE12, PARAM12>
		(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
}
}