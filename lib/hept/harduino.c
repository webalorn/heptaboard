#include <math.h>
#include "stdlib.h"
#include "harduino.h"

#define DEF_FUN(FNAME, ...) \
	void Harduino__ ## FNAME ## _step(__VA_ARGS__, Harduino__ ## FNAME ## _out *out)

#define DEF_FUN_NOARGS(FNAME) \
	void Harduino__ ## FNAME ## _step(Harduino__ ## FNAME ## _out *out)

/* Arduino library */

DEF_FUN(digitalWrite, int pin, int pinval) {
	digitalWrite(pin, pinval);
}
DEF_FUN(digitalRead, int pin) {
	out->o = digitalRead(pin);
}

DEF_FUN(randomSeed, int seed) {
	if (seed != 0) {
		srandom(seed);
	}
}
DEF_FUN(random, int howbig)  {
	out->o = howbig == 0 ? 0 : random() % howbig;
}
DEF_FUN_NOARGS(random_f)  {
	out->o = (float)random() / (float)RAND_MAX;
}

DEF_FUN_NOARGS(millis) {
	out->o = millis();
}
DEF_FUN_NOARGS(micros) {
	out->o = micros();
}

/* Math library */

DEF_FUN(abs, int x) {
	out-> o = abs(x);
}
DEF_FUN(abs_f, float x) {
	out->o = abs(x);
}
DEF_FUN(constrain, int x, int y, int z) {
	out->o = constrain(x, y, z);
}
DEF_FUN(constrain_f, float x, float y, float z) {
	out->o = constrain(x, y, z);
}
DEF_FUN(map_interval, long x, long in_min, long in_max, long out_min, long out_max) {
	out->o = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
DEF_FUN(max, int x, int y) {
	out->o = max(x, y);
}
DEF_FUN(max_f, float x, float y) {
	out->o = max(x, y);
}
DEF_FUN(min, int x, int y) {
	out->o = min(x, y);
}
DEF_FUN(min_f, float x, float y) {
	out->o = min(x, y);
}
DEF_FUN(pow, float x, float y) {
	out->o = pow(x, y);
}
DEF_FUN(sqrt, float x) {
	out->o = sqrt(x);
}
DEF_FUN(cos, float x) {
	out->o = cos(x);
}
DEF_FUN(sin, float x) {
	out->o = sin(x);
}
DEF_FUN(tan, float x) {
	out->o = tan(x);
}

DEF_FUN(floor, float x) {
	out->o = floor(x);
}
DEF_FUN(ceil, float x) {
	out->o = ceil(x);
}
DEF_FUN(cast_float, int x) {
	out->o = (float)x;
}
DEF_FUN(cast_int, float x) {
	out->o = (int)x;
}