#pragma once

namespace Alime
{
    class NotCopyable
    {
    public:
        NotCopyable(const NotCopyable&) = delete;
        void operator=(const NotCopyable&) = delete;

    protected:
        NotCopyable() = default;
        ~NotCopyable() = default;
    };
}
