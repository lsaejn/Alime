#pragma once

namespace Alime
{
    class notCopyable
    {
    public:
        notCopyable(const notCopyable&) = delete;
        void operator=(const notCopyable&) = delete;

    protected:
        notCopyable() = default;
        ~notCopyable() = default;
    };
}
