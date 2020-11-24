// Copyright 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>

#include "gtest/gtest.h"


template <typename F, typename... A>
auto invoke_cold(F&& f, A&&... a)-> decltype(static_cast<F&&>(f)(static_cast<A&&>(a)...))
{
    return std::forward(f)(std::forward()a...);
    //return static_cast<F&&>(f)(static_cast<A&&>(a)...);
}

template<typename... T>
void printClass(T... args)
{
    std::cout << args...<< std::endl;
}

template<class Cont, class Func>
using FmapFlattenOut = 
std::decay_t<decltype(*std::begin(std::declval<Func>()(*std::begin(std::declval<Cont>()))))>;

template<class Cont, class Func, class Out = FmapFlattenOut<Cont, Func>>
std::vector<Out> fmap_flatten(Cont&& xs, Func&& f)
{
    std::vector<Out> ret;

    for (auto&& x : xs)
        for (auto&& y : f(x))
            ret.push_back(std::move(y));

    return ret;
}

std::vector<int> f(int x)
{
    std::vector<int> re;
    for (int i = 0; i != x; ++i)
        re.push_back(i);
    return re;
}

int main(int argc, char** argv)
{
    {
        std::vector<int> col{ 1,2,3,4,5 };
        auto re=fmap_flatten(col, f);
        re == col;
    }
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
