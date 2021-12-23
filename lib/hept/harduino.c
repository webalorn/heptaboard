#include <math.h>
#include "stdlib.h"
#include "harduino.h"

#define DEF_FUN(FNAME, ...) \
	void Harduino__ ## FNAME ## _step(__VA_ARGS__, Harduino__ ## FNAME ## _out *out)

#define DEF_FUN_NOARGS(FNAME) \
	void Harduino__ ## FNAME ## _step(Harduino__ ## FNAME ## _out *out)

#define DEF_NODE(FNAME, ...) \
	void Harduino__ ## FNAME ## _step(__VA_ARGS__, Harduino__ ## FNAME ## _out *out, Harduino__ ## FNAME ## _mem *self)

#define DEF_NODE_RESET(FNAME) \
	void Harduino__ ## FNAME ## _reset(Harduino__ ## FNAME ## _mem *self)

/* Arduino library */

DEF_FUN(digitalWrite, int pin, int pinval) {
	digitalWrite(pin, pinval);
}
DEF_FUN(digitalRead, int pin) {
	out->o = digitalRead(pin);
}

DEF_NODE(analogRead, int pin) {
	out->o = self->next_o;
	analogRead(pin, &(self->next_o));
}
DEF_NODE_RESET(analogRead) {
	self->next_o = 0;
}

DEF_FUN(analogWrite, int pin, int pinval) {
	digitalWrite(pin, pinval);
}

DEF_NODE(tone, int pin, unsigned int freqDivider) {
	if (freqDivider > 0) {
		if (self->n_steps >= freqDivider) {
			volatile uint8_t *pin_port = portOutputRegister(digitalPinToPort(pin));
			volatile uint8_t pin_mask = digitalPinToBitMask(pin);

			*pin_port ^= pin_mask;
			self->n_steps = 0;
		} else {
			self->n_steps++;
		}
	} else {
		digitalWrite(pin, 0);
	}
}
DEF_NODE_RESET(tone) {
	self->n_steps = 0;
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
	out->o = cosf(x);
}
DEF_FUN(sin, float x) {
	out->o = sinf(x);
}
DEF_FUN(tan, float x) {
	out->o = tanf(x);
}
DEF_FUN(acos, float x) {
	out->o = acosf(x);
}
DEF_FUN(asin, float x) {
	out->o = asinf(x);
}
DEF_FUN(atan, float x) {
	out->o = atanf(x);
}

DEF_FUN(floor, float x) {
	out->o = floor(x);
}
DEF_FUN(ceil, float x) {
	out->o = ceil(x);
}
DEF_FUN(int2float, int x) {
	out->o = (float)x;
}
DEF_FUN(float2int, float x) {
	out->o = (int)x;
}
DEF_FUN(bool2int, bool x) {
	out->o = (int)x;
}
DEF_FUN(int2bool, int x) {
	out->o = x ? 1 : 0;
}

/* Helper functions */
DEF_NODE(clock, int timer) {
	out->o = self->cur_clock == 0;
	self->cur_clock = self->cur_clock < timer ? self->cur_clock + 1 : 0;
}
DEF_NODE_RESET(clock) {
	self->cur_clock = 0;
}