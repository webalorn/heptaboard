#include "Arduino.h"
#include <avr/interrupt.h>

#include ENTRY_HEADER

#define CONCAT2(x, y) x ## y
#define CONCAT1(x, y) CONCAT2(x, y)

#define ENTRY_OUT CONCAT1(ENTRY, _out)
#define ENTRY_STEP CONCAT1(ENTRY, _step)
#define ENTRY_RESET CONCAT1(ENTRY, _reset)
#define ENTRY_MEM_OBJ CONCAT1(ENTRY, _mem)

#ifdef SETUP
#include SETUP_HEADER

#define SETUP_OUT CONCAT1(SETUP, _out)
#define SETUP_STEP CONCAT1(SETUP, _step)
#define SETUP_MEM_OBJ CONCAT1(SETUP, _mem)
#endif

#define DELAY_MICRO 100

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (*func)()) { return 0; }

void setup() __attribute__((weak));
void setup() {}

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

volatile bool runLoop;
unsigned long elapsed_millis = 0;
ISR(TIMER1_COMPA_vect){
	runLoop = true;
#if TIMER_FREQ != 0
	elapsed_millis += 1000 / TIMER_FREQ;
#endif
}

int main() {

#if TIMER_FREQ != 0
	long freq = TIMER_FREQ; // max 62500

	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A = (16000000UL / (256 * freq))-1;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12); // 256 prescaler
	TIMSK1 |= (1 << OCIE1A);
	sei();
#endif
	
	init();
	initVariant();

	setup();

#ifdef SETUP
	SETUP_OUT setup_output;
	#ifdef SETUP_MEM
		SETUP_MEM_OBJ setup_memory;
		SETUP_STEP(&setup_output, &setup_memory);
	#else
		SETUP_STEP(&setup_output);
	#endif
#endif

	ENTRY_OUT output;
#ifdef ENTRY_MEM
	ENTRY_MEM_OBJ memory;
	ENTRY_RESET(&memory);
#endif
	
	runLoop = false;
	while (true) {
#if TIMER_FREQ != 0
		while (!runLoop) {}
		runLoop = false;
#endif

		// Main loop
		#if defined(ENTRY_MEM)
			ENTRY_STEP(&output, &memory);
		#else
			ENTRY_STEP(&output);
		#endif
	}
				
	return 0;
}

/*
	Redefined functions
*/

int millis() {
	return elapsed_millis % (long)__INT_MAX__;
}