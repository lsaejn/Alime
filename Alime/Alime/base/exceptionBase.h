#pragma once

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
			:description_(new wchar_t[_description.length() + 1]{ 0 }),
			memoryLocated_(true)
		{
			memcpy_s((void*)description_, _description.length() * sizeof(wchar_t),
				_description.c_str(), _description.length() * sizeof(wchar_t));
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

		const wchar_t* Message() const
		{
			return description_;
		}

	private:
		bool memoryLocated_;
		const wchar_t* description_;
	};

	inline std::wstring ErrnoStr(DWORD e)
	{
		LPTSTR buf;
		std::wstring result = L" unknow error";
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
			| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			e,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&buf,
			0,
			NULL))
		{
			result = buf;
			LocalFree(buf);
		}
		return result;
	}
}

#define CHECK_ERROR(condition,msgInfo) do{if(!(condition))throw Alime::base::Error(msgInfo);}while(0)
