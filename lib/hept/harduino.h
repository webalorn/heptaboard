#ifndef HARDUINO_H
#define HARDUINO_H

#include "../arduino/Arduino.h"

#define DECL_FUN(TY_OUT, FNAME, ...) \
	typedef struct { \
		TY_OUT o; \
	} Harduino__ ## FNAME ## _out; \
	\
	void Harduino__ ## FNAME ## _step(__VA_ARGS__, Harduino__ ## FNAME ## _out *);

#define DECL_FUN_VOID(FNAME, ...) DECL_FUN(int, FNAME, __VA_ARGS__)

/* Arduino library */

DECL_FUN_VOID(digitalWrite, int, int)
DECL_FUN(int, digitalRead, int)

/* Math library */

DECL_FUN(int, abs, int)
DECL_FUN(float, abs_f, float)
DECL_FUN(int, constrain, int, int, int)
DECL_FUN(float, constrain_f, float, float, float)
DECL_FUN(int, map_interval, int, int, int, int, int)
DECL_FUN(int, max, int, int)
DECL_FUN(float, max_f, float, float)
DECL_FUN(int, min, int, int)
DECL_FUN(float, min_f, float, float)
DECL_FUN(float, pow, float, float)
DECL_FUN(float, sqrt, float)
DECL_FUN(float, cos, float)
DECL_FUN(float, sin, float)
DECL_FUN(float, tan, float)

DECL_FUN(float, floor, float)
DECL_FUN(float, ceil, float)
DECL_FUN(float, cast_float, int)
DECL_FUN(int, cast_int, float)


#endif // HARDUINO_H

