#pragma once
#include <string>

namespace Alime
{
	namespace detail
	{
        inline void append_internal(std::string& into, char c)
        {
            into += c;
        }

        template<class T, class = decltype(std::to_string(std::declval<T>()))>
        inline void append_internal(std::string& into, T x)
        {
            into += std::to_string(x);
        }

        inline void append_internal(std::string& into, const char* v)
        {
            into.append(v); 
        }

        inline void append_internal(std::string& into, const std::string& s)
        {
            into.append(s);
        }

        template<class T, class = decltype(std::declval<const T&>().to_string(std::declval<std::string&>()))>
        void append_internal(std::string& into, const T& t)
        {
            t.to_string(into);
        }

        template<class T, class = void, class = decltype(to_string(std::declval<std::string&>(), std::declval<const T&>()))>
        void append_internal(std::string& into, const T& t)
        {
            to_string(into, t);
        }

		template<typename STRING, typename CHAR>
		std::enable_if_t<std::is_same_v<typename STRING::value_type,CHAR>>
			append_internal(STRING& into, CHAR c)
		{
			into += c;
		}
	}
}

