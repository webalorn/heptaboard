#ifndef HARDUINO_H
#define HARDUINO_H

#include "../arduino/Arduino.h"
#include "harduino_types.h"

#define DECL_FUN(TY_OUT, FNAME, ...) \
	typedef struct { \
		TY_OUT o; \
	} Harduino__ ## FNAME ## _out; \
	\
	void Harduino__ ## FNAME ## _step(__VA_ARGS__, Harduino__ ## FNAME ## _out *);

#define DECL_FUN_NOARGS(TY_OUT, FNAME) \
	typedef struct { \
		TY_OUT o; \
	} Harduino__ ## FNAME ## _out; \
	\
	void Harduino__ ## FNAME ## _step(Harduino__ ## FNAME ## _out *);

#define DECL_FUN_VOID(FNAME, ...) DECL_FUN(int, FNAME, __VA_ARGS__)

/* Arduino library */

DECL_FUN_VOID(digitalWrite, int, int)
DECL_FUN(int, digitalRead, int)

DECL_FUN(int, analogRead, int)
DECL_FUN_VOID(analogWrite, int, int)

// DECL_FUN_VOID(tone, int, int, int)
// DECL_FUN_VOID(noTone, int)

DECL_FUN_VOID(randomSeed, int)
DECL_FUN(int, random, int)
DECL_FUN_NOARGS(float, random_f)

DECL_FUN_NOARGS(int, millis)
DECL_FUN_NOARGS(int, micros)

/* Math library */

DECL_FUN(int, abs, int)
DECL_FUN(float, abs_f, float)
DECL_FUN(int, constrain, int, int, int)
DECL_FUN(float, constrain_f, float, float, float)
DECL_FUN(int, map_interval, long, long, long, long, long)
DECL_FUN(int, max, int, int)
DECL_FUN(float, max_f, float, float)
DECL_FUN(int, min, int, int)
DECL_FUN(float, min_f, float, float)
DECL_FUN(float, pow, float, float)
DECL_FUN(float, sqrt, float)

DECL_FUN(float, cos, float)
DECL_FUN(float, sin, float)
DECL_FUN(float, tan, float)

DECL_FUN(float, acos, float)
DECL_FUN(float, asin, float)
DECL_FUN(float, atan, float)

DECL_FUN(float, floor, float)
DECL_FUN(float, ceil, float)
DECL_FUN(float, int2float, int)
DECL_FUN(int, float2int, float)
DECL_FUN(int, bool2int, bool)
DECL_FUN(bool, int2bool, int)

#endif // HARDUINO_H

