#pragma once

#include <utility>
#include <functional>

namespace Func
{
	class Functor
	{
	public:
		virtual ~Functor() {}
		virtual void call() = 0;
	};

	namespace Helper
	{
		template <int... Is>
		struct index {};

		template <int N, int... Is>
		struct genSeq : genSeq<N - 1, N - 1, Is...> {};

		template <int... Is>
		struct genSeq<0, Is...> : index<Is...>{};
	}

	template <typename RET, typename... Ts>
	class FunctorImpl : public Functor
	{
	public:
		template <typename F, typename... Args>
		FunctorImpl(RET& ret, F&& func, Args&&... args) : f(std::forward<F>(func)), args(std::forward<Args>(args)...), m_retVal(ret) {}

		template <typename... Args, int... Is>
		RET func(std::tuple<Args...>& tup, Helper::index<Is...>)
		{
			return f(std::get<Is>(tup)...);
		}

		template <typename... Args>
		RET func(std::tuple<Args...>& tup)
		{
			return func(tup, Helper::genSeq<sizeof...(Args)>{});
		}

		virtual void call() { m_retVal = func(args); }

	private:
		std::function<RET(Ts...)> f;
		std::tuple<Ts...> args;
		RET& m_retVal;
	};

	template <typename... Ts>
	class FunctorImpl<void, Ts...> : public Functor
	{
	public:
		template <typename F, typename... Args>
		FunctorImpl(F&& func, Args&&... args) : f(std::forward<F>(func)), args(std::forward<Args>(args)...) {}

		template <typename... Args, int... Is>
		void func(std::tuple<Args...>& tup, Helper::index<Is...>)
		{
			return f(std::get<Is>(tup)...);
		}

		template <typename... Args>
		void func(std::tuple<Args...>& tup)
		{
			return func(tup, Helper::genSeq<sizeof...(Args)>{});
		}

		virtual void call() { func(args); }

	private:
		std::function<void(Ts...)> f;
		std::tuple<Ts...> args;
	};

	/* __cdecl */
	template <typename RET, typename... Args, typename... Args2>
	Functor* makeFunctor(RET& ret, RET(__cdecl*f)(Args2...), Args&&... args)
	{
		return new FunctorImpl<RET, Args2...>(ret, std::forward<RET(__cdecl*)(Args2...)>(f), std::forward<Args>(args)...);
	}
	template <typename... Args, typename... Args2>
	Functor* makeFunctor(void(__cdecl*f)(Args2...), Args&&... args)
	{
		return new FunctorImpl<void, Args2...>(std::forward<void(__cdecl*)(Args2...)>(f), std::forward<Args>(args)...);
	}


	/* __stdcall */
	template <typename RET, typename... Args, typename... Args2>
	Functor* makeFunctor(RET& ret, RET(__stdcall*f)(Args2...), Args&&... args)
	{
		return new FunctorImpl<RET, Args2...>(ret, std::forward<RET(__stdcall*)(Args2...)>(f), std::forward<Args>(args)...);
	}
	template <typename... Args, typename... Args2>
	Functor* makeFunctor(void(__stdcall*f)(Args2...), Args&&... args)
	{
		return new FunctorImpl<void, Args2...>(std::forward<void(__stdcall*)(Args2...)>(f), std::forward<Args>(args)...);
	}
}