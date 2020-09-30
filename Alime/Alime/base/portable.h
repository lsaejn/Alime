#pragma once

#include <sys/types.h>

#ifdef _WIN32
#include <basetsd.h> // @manual

#define HAVE_MODE_T 1

// This is a massive pain to have be an `int` due to the pthread implementation
// we support, but it's far more compatible with the rest of the windows world
// as an `int` than it would be as a `void*`
using pid_t = int;
// This isn't actually supposed to be defined here, but it's the most
// appropriate place without defining a portability header for stdint.h
// with just this single typedef.
using ssize_t = SSIZE_T;
// The Windows headers don't define this anywhere, nor do any of the libs
// that Folly depends on, so define it here.
using mode_t = unsigned short;
#endif
