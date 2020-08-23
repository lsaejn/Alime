#pragma once

#include <string>

namespace Alime::base
{
	class Error
	{
	public:
		Error(const wchar_t* _description)
			:description_(NULL), memoryLocated_(false)
		{
			description_ = _description;
		}

		Error(const std::wstring& _description)
			:description_(new wchar_t[_description.length()+1]),
			memoryLocated_(false)
		{
			memoryLocated_ = true;
		}

		~Error()
		{
			if (memoryLocated_)
				delete description_;
		}

		const wchar_t* What() const
		{
			return description_;
		}

	private:
		bool memoryLocated_;
		const wchar_t* description_;
	};
}


#define CHECK_ERROR(condtion, description) \
if(!(conditon)) \
	throw Alime::base::Error(description);
