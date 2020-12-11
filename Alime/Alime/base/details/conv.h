#pragma once

#ifndef ALIME_CONV_H_
#define ALIME_CONV_H_
#endif // !ALIME_CONV_H_


#include <string>
#include <type_traits>
#include <tuple>

#include "string.h"
#include "Alime/base/third_party/double-conversion/double-conversion.h"
#include "Alime/base/compiler_specific.h"


namespace
{
    inline uint32_t digits10(uint64_t v)
    {
        uint32_t result = 1;
        while (true)
        {
            if (LIKELY(v < 10))
            {
                return result;
            }
            if (LIKELY(v < 100))
            {
                return result + 1;
            }
            if (LIKELY(v < 1000))
            {
                return result + 2;
            }
            if (LIKELY(v < 10000))
            {
                return result + 3;
            }
            v /= 10000U;
            result += 4;
        }
    }

    inline uint32_t uint64ToBufferUnsafe(uint64_t v, char* const buffer) {
        auto const result = digits10(v);
        // WARNING: using size_t or pointer arithmetic for pos slows down
        // the loop below 20x. This is because several 32-bit ops can be
        // done in parallel, but only fewer 64-bit ones.
        uint32_t pos = result - 1;
        while (v >= 10) {
            // Keep these together so a peephole optimization "sees" them and
            // computes them in one shot.
            auto const q = v / 10;
            auto const r = v % 10;
            buffer[pos--] = static_cast<char>('0' + r);
            v = q;
        }
        // Last digit is trivial to handle
        buffer[pos] = static_cast<char>(v + '0');
        return result;
    }

}
////////step-1
//我们将ts转成最后tuple, 然后拿最后一个参数的类型/值
template <typename... Ts>
auto getLastElement(Ts&&... ts) -> decltype(std::get<sizeof...(Ts) - 1>(
    std::forward_as_tuple(std::forward<Ts>(ts)...)))
{
    return std::get<sizeof...(Ts) - 1>(
        std::forward_as_tuple(std::forward<Ts>(ts)...));
}

/////////////step2
//我们仍然需要tuple，可以省掉很多工作量
template <size_t size, typename... Ts>
struct LastElementType : std::tuple_element<size - 1, std::tuple<Ts...>>
{
};

template <>
struct LastElementType<0>
{
    using type = void;
};

template <class... Ts>
struct LastElement
    : std::decay<typename LastElementType<sizeof...(Ts), Ts...>::type>
{
};


//helper
//支持u8和wchar_t

template <class T>
struct IsSomeString : std::false_type
{
};

template <typename Alloc>
struct IsSomeString<std::basic_string<char, std::char_traits<char>, Alloc>>
    : std::true_type
{
};

template <typename Alloc>
struct IsSomeString<std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc>>
    : std::true_type
{
};


//先声明一下，我们需要toAppend
template <class Ts>
void toAppendFit(const Ts&)
{
}

//声明
template <class Tgt>
typename std::enable_if<IsSomeString<Tgt>::value>::type
toAppend(Tgt*)
{
}

//好，开始体力活
#include <sstream>
///warning, dangerous implement
///对于wchar，我们不考虑效率 
template <class T>
void toAppend(T value, std::wstring* result)
{
    std::wostringstream os;
    os << value;
    result->append(os.str());
}

//char
template <class Tgt>
void toAppend(char value, Tgt* result)
{
    *result += value;
}

//const char*
template <class Tgt>
void toAppend(const char* value, Tgt* result)
{
    *result += value;
}

//string
template <class Tgt, class Src>
typename std::enable_if<
    IsSomeString<Src>::value&& IsSomeString<Tgt>::value>::type
    toAppend(const Src& value, Tgt* result)
{
    result->append(value);
}


/**
 * and StringPiece/string_view objects too
 */
 template <class Tgt>
 typename std::enable_if<IsSomeString<Tgt>::value>::type toAppend(
     std::string_view value,
     Tgt* result)
 {
     result->append(value.data(), value.size());
 }


//int32_t and int64_t
template <class Tgt, class Src>
typename std::enable_if<
    std::is_integral<Src>::value&& std::is_signed<Src>::value&&
    IsSomeString<Tgt>::value && sizeof(Src) >= 4>::type
    toAppend(Src value, Tgt* result)
{
    char buffer[20] = { 0 };
    if (value < 0)
    {
        result->push_back('-');
        result->append(
            buffer,
            uint64ToBufferUnsafe(~static_cast<uint64_t>(value) + 1, buffer));
    }
    else {
        result->append(buffer, uint64ToBufferUnsafe(uint64_t(value), buffer));
    }
}


/**
 * As above, but for uint32_t and uint64_t.
 */
template <class Tgt, class Src>
typename std::enable_if<
    std::is_integral<Src>::value && !std::is_signed<Src>::value&&
    IsSomeString<Tgt>::value && sizeof(Src) >= 4>::type
    toAppend(Src value, Tgt* result)
{
    char buffer[20];
    result->append(buffer, uint64ToBufferUnsafe(value, buffer));
}


/**
 * All small signed and unsigned integers to string go through 32-bit
 * types int32_t and uint32_t, respectively.
 */
template <class Tgt, class Src>
typename std::enable_if <
    std::is_integral<Src>::value&& IsSomeString<Tgt>::value &&
    sizeof(Src)<4 >::type
    toAppend(Src value, Tgt* result)
{
    typedef
        typename std::conditional<std::is_signed<Src>::value, int64_t, uint64_t>::
        type Intermediate;
    toAppend<Tgt>(static_cast<Intermediate>(value), result);
}

namespace detail 
{
    constexpr int kConvMaxDecimalInShortestLow = -6;
    constexpr int kConvMaxDecimalInShortestHigh = 21;
} // namespace detail

/** Wrapper around DoubleToStringConverter **/
template <class Tgt, class Src>
typename std::enable_if<
    std::is_floating_point<Src>::value&& IsSomeString<Tgt>::value>::type
    toAppend(
        Src value,
        Tgt* result,
        double_conversion::DoubleToStringConverter::DtoaMode mode,
        unsigned int numDigits) {
    using namespace double_conversion;
    DoubleToStringConverter conv(
        DoubleToStringConverter::NO_FLAGS,
        "Infinity",
        "NaN",
        'E',
        detail::kConvMaxDecimalInShortestLow,
        detail::kConvMaxDecimalInShortestHigh,
        6, // max leading padding zeros
        1); // max trailing padding zeros
    char buffer[256];
    StringBuilder builder(buffer, sizeof(buffer));
    switch (mode) {
    case DoubleToStringConverter::SHORTEST:
        conv.ToShortest(value, &builder);
        break;
    case DoubleToStringConverter::SHORTEST_SINGLE:
        conv.ToShortestSingle(static_cast<float>(value), &builder);
        break;
    case DoubleToStringConverter::FIXED:
        conv.ToFixed(value, int(numDigits), &builder);
        break;
    case DoubleToStringConverter::PRECISION:
    default:
        assert(mode == DoubleToStringConverter::PRECISION);
        conv.ToPrecision(value, int(numDigits), &builder);
        break;
    }
    const size_t length = size_t(builder.position());
    builder.Finalize();
    result->append(buffer, length);
}

/**
 * As above, but for floating point
 */
template <class Tgt, class Src>
typename std::enable_if<
    std::is_floating_point<Src>::value&& IsSomeString<Tgt>::value>::type
    toAppend(Src value, Tgt* result) {
    toAppend(
        value, result, double_conversion::DoubleToStringConverter::SHORTEST, 0);
}



template <class E>
constexpr std::underlying_type_t<E> to_underlying(E e) noexcept
{
    static_assert(std::is_enum<E>::value, "not an enum type");
    return static_cast<std::underlying_type_t<E>>(e);
}

/**
 * Enumerated values get appended as integers.
 */
template <class Tgt, class Src>
typename std::enable_if<
    std::is_enum<Src>::value&& IsSomeString<Tgt>::value>::type
    toAppend(Src value, Tgt* result)
{
    toAppend(to_underlying(value), result);
}


//好，重头戏来了,目标首先是标准字符串，或者，嗯..
//我们把单个T写入Tgt
template <class T, class Tgt>
typename std::enable_if<IsSomeString<typename std::remove_pointer<Tgt>::type>::value>::type
toAppendStrImpl(const T& v, Tgt result)
{
    toAppend(v, result);
}

//多参数，我们写第一个参数到result(...vs里最后),然后递归
template <class T, class... Ts>
typename std::enable_if< sizeof...(Ts) >= 2 && IsSomeString<typename std::remove_pointer<
    typename LastElement<const Ts&...>::type>::type>::value>::type
    toAppendStrImpl(const T& v, const Ts&... vs)
{
    toAppend(v, getLastElement(vs...));
    toAppendStrImpl(vs...);
}

//大于三个参数的时候，我们才给impl,这正是toAppendStrImpl里Ts>=2的原因
template <class... Ts>
typename std::enable_if<sizeof...(Ts) >= 3 &&
    IsSomeString<typename std::remove_pointer<
    typename LastElement<const Ts&...>::type>::type>::value>::type
    toAppend(const Ts&... vs)
{
    toAppendStrImpl(vs...);
}

//现在，我们打破这个循环，只需要提供一个内部接口给to
template <class... Ts>
typename std::enable_if<IsSomeString<typename std::remove_pointer<
    typename LastElement<const Ts&...>::type>::type>::value>::type
    toAppendFit(const Ts&... vs)
{
    toAppend(vs...);
}

/// <summary>
/// 参数个数不为1，或者参数为1但不是同类型
/// </summary>
template <class Tgt, class... Ts>
typename std::enable_if<IsSomeString<Tgt>::value &&
    (sizeof...(Ts) != 1 || !std::is_same<Tgt, typename LastElement<const Ts&...>::type>::value), Tgt>::type
    to(const Ts&... vs) {
    Tgt result;
    toAppendFit(vs..., &result);
    return result;
}

/// <summary>
/// 同上，参数为1，并且是浮点型
/// </summary>
template <class Tgt, class Src>
typename std::enable_if<
    IsSomeString<Tgt>::value&& std::is_floating_point<Src>::value, Tgt>::type
    to(Src value)
{
    Tgt result;
    toAppend(value, &result);
    return result;
}

template <class Delimiter, class Tgt>
typename std::enable_if<IsSomeString<Tgt>::value>::type toAppendDelim(
    const Delimiter& /* delim */,
    Tgt* /* result */) {}

template <class Delimiter, class T, class Tgt>
typename std::enable_if<
    IsSomeString<typename std::remove_pointer<Tgt>::type>::value>::type
    toAppendDelimStrImpl(const Delimiter& /* delim */, const T& v, Tgt result) {
    toAppend(v, result);
}

template <class Delimiter, class T, class... Ts>
typename std::enable_if<
    sizeof...(Ts) >= 2 &&
    IsSomeString<typename std::remove_pointer<
    typename LastElement<const Ts&...>::type>::type>::value>::type
    toAppendDelimStrImpl(const Delimiter& delim, const T& v, const Ts&... vs) {
    // we are really careful here, calling toAppend with just one element does
    // not try to estimate space needed (as we already did that). If we call
    // toAppend(v, delim, ....) we would do unnecesary size calculation
    toAppend(v, getLastElement(vs...));
    toAppend(delim, getLastElement(vs...));
    toAppendDelimStrImpl(delim, vs...);
}


template <class Delimiter, class... Ts>
typename std::enable_if<
    sizeof...(Ts) >= 3 &&
    IsSomeString<typename std::remove_pointer<
    typename LastElement<const Ts&...>::type>::type>::value>::type
    toAppendDelim(const Delimiter& delim, const Ts&... vs) {
    toAppendDelimStrImpl(delim, vs...);
}

/**
 * Detail in comment for toAppendFit
 */
template <class Delimiter, class... Ts>
typename std::enable_if<IsSomeString<typename std::remove_pointer<
    typename LastElement<const Ts&...>::type>::type>::value>::type
    toAppendDelimFit(const Delimiter& delim, const Ts&... vs) {
    toAppendDelim(delim, vs...);
}

template <class De, class Ts>
void toAppendDelimFit(const De&, const Ts&) {}

template <class Tgt, class Delim, class Src>
typename std::enable_if<
    IsSomeString<Tgt>::value&&
    std::is_same<Tgt, typename std::decay<Src>::type>::value,
    Tgt>::type
    toDelim(const Delim& /* delim */, Src&& value) {
    return std::forward<Src>(value);
}

/**
 * toDelim<SomeString>(delim, v1, v2, ...) uses toAppendDelim() as
 * back-end for all types.
 */
template <class Tgt, class Delim, class... Ts>
typename std::enable_if<
    IsSomeString<Tgt>::value &&
    (sizeof...(Ts) != 1 ||
        !std::is_same<Tgt, typename LastElement<const Ts&...>::type>::
        value),
    Tgt>::type
    toDelim(const Delim& delim, const Ts&... vs) {
    Tgt result;
    toAppendDelimFit(delim, vs..., &result);
    return result;
}

/*
std::string errnoStr_(int err) {
    int savedErrno = errno;

    char buf[1024];
    buf[0] = '\0';

    std::string result;

    // https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/strerror_r.3.html
    // http://www.kernel.org/doc/man-pages/online/pages/man3/strerror.3.html
#if defined(_WIN32) && (defined(__MINGW32__) || defined(_MSC_VER))
  // mingw64 has no strerror_r, but Windows has strerror_s, which C11 added
  // as well. So maybe we should use this across all platforms (together
  // with strerrorlen_s). Note strerror_r and _s have swapped args.
    int r = strerror_s(buf, sizeof(buf), err);
    if (r != 0) {
        result =
            result = to<std::string>(
                "Unknown error ", err, " (strerror_r failed with error ", errno, ")");
    }
    else {
        result.assign(buf);
    }
#else
    result.assign(strerror_r(err, buf, sizeof(buf)));
#endif

    return result;
}
*/

