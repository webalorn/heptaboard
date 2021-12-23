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

#define DECL_NODE(TY_OUT, FNAME, ...) \
	Harduino__ ## FNAME ## _mem; \
	typedef struct { \
		TY_OUT o; \
	} Harduino__ ## FNAME ## _out; \
	\
	void Harduino__ ## FNAME ## _step(__VA_ARGS__, Harduino__ ## FNAME ## _out*, Harduino__ ## FNAME ## _mem*); \
	void Harduino__ ## FNAME ## _reset(Harduino__ ## FNAME ## _mem*);

/* Arduino library */

DECL_FUN_VOID(pinMode, int, int)
DECL_FUN_VOID(digitalWrite, int, int)
DECL_FUN(int, digitalRead, int)

DECL_FUN_VOID(analogWrite, int, int)

typedef struct { int next_o; }
DECL_NODE(int, analogRead, int);

typedef struct { int n_steps; }
DECL_NODE(int, tone, int, unsigned int)

DECL_FUN_VOID(randomSeed, int)
DECL_FUN(int, random, int)
DECL_FUN_NOARGS(float, random_f)

DECL_FUN_NOARGS(int, millis)

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

/* Helper functions */
typedef struct { int cur_clock; }
DECL_NODE(bool, clock, int)

#endif // HARDUINO_H

