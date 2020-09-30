#pragma once

#include <functional>

std::function<void(int)> f;
/*
* 这里演示lambda的编写
*/

namespace Alime
{
	template<typename ...TArgs>
	struct TypeTuple
	{
	};

	template<typename T>
	class Func
	{
	};

	template<typename T>
	struct LambdaRetriveType
	{
	};

	template<typename TObject, typename R, typename ...TArgs>
	struct LambdaRetriveType<R(__thiscall TObject::*)(TArgs...)const>
	{
		typedef Func<R(TArgs...)> Type;
		typedef R(FunctionType)(TArgs...);
		typedef R ResultType;
		typedef TypeTuple<TArgs...> ParameterTypes;
	};

	template<typename TObject, typename R, typename ...TArgs>
	struct LambdaRetriveType<R(__thiscall TObject::*)(TArgs...)>
	{
		typedef Func<R(TArgs...)> Type;
		typedef R(FunctionType)(TArgs...);
		typedef R ResultType;
		typedef TypeTuple<TArgs...> ParameterTypes;
	};

	//函数对象
	template<typename T>
	struct FunctionObjectRetriveType
	{
		typedef typename LambdaRetriveType<decltype(&T::operator())>::Type Type;
		typedef typename LambdaRetriveType<decltype(&T::operator())>::FunctionType FunctionType;
		typedef typename LambdaRetriveType<decltype(&T::operator())>::ResultType ResultType;
		typedef typename LambdaRetriveType<decltype(&T::operator())>::ParameterTypes ParameterTypes;
	};

	template<typename R, typename ...TArgs>
	struct FunctionObjectRetriveType<R(*)(TArgs...)>
	{
		typedef Func<R(TArgs...)> Type;
		typedef R(FunctionType)(TArgs...);
		typedef R ResultType;
		typedef TypeTuple<TArgs...> ParameterTypes;
	};

	class Object
	{

	};

	template<typename R, typename ...TArgs>
	class Invoker : public Object
	{
	public:
		virtual R Invoke(TArgs&& ...args) = 0;
	};

	template<typename R, typename ...TArgs>
	class StaticInvoker : public Invoker<R, TArgs...>
	{
	protected:
		R(*function)(TArgs ...args);

	public:
		StaticInvoker(R(*_function)(TArgs...))
			:function(_function)
		{
		}

		R Invoke(TArgs&& ...args)override
		{
			return function(ForwardValue<TArgs>(args)...);
		}
	};

	template<typename C, typename R, typename ...TArgs>
	class MemberInvoker : public Invoker<R, TArgs...>
	{
	protected:
		C* sender;
		R(C::* function)(TArgs ...args);

	public:
		MemberInvoker(C* _sender, R(C::* _function)(TArgs ...args))
			:sender(_sender)
			, function(_function)
		{
		}

		R Invoke(TArgs&& ...args)override
		{
			return (sender->*function)(ForwardValue<TArgs>(args)...);
		}
	};

	


}

