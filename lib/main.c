#include "Arduino.h"

#include ENTRY_HEADER

#define CONCAT2(x, y) x ## y
#define CONCAT1(x, y) CONCAT2(x, y)
#define ENTRY_OUT CONCAT1(ENTRY, _out)
#define ENTRY_STEP CONCAT1(ENTRY, _step)
#define ENTRY_RESET CONCAT1(ENTRY, _reset)
#define ENTRY_MEM_OBJ CONCAT1(ENTRY, _mem)

#define DELAY_MICRO 100

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (*func)()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

int main(void)
{
	init();
	initVariant();
	
	setup();

	ENTRY_OUT output;
#if defined(ENTRY_MEM)
	ENTRY_MEM_OBJ memory;
	ENTRY_RESET(&memory);
#endif
	
	unsigned long loopTime = millis();
	unsigned long curTime;
	while (true) {
		// Main loop
		#if defined(ENTRY_MEM)
			ENTRY_STEP(&output, &memory);
		#else
			ENTRY_STEP(&output);
		#endif

		curTime = millis();
		if (curTime < loopTime) {
			// In case of overflow of the time counter
			loopTime = loopTime;
		} else {
			loopTime += LOOP_DELAY;
			while (curTime < loopTime) {
				delayMicroseconds(DELAY_MICRO);
				curTime = millis();
			}
		}
	}
				
	return 0;
}

