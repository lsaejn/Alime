#pragma once

/// <summary>
/// Base type in boost
/// </summary>
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
