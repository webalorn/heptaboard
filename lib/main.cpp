/*
	main.cpp - Main loop for Arduino sketches
	Copyright (c) 2005-2013 Arduino Team.  All right reserved.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"

extern "C" {
	#include ENTRY_HEADER
}
#define CONCAT2(x, y) x ## y
#define CONCAT1(x, y) CONCAT2(x, y)
#define ENTRY_OUT CONCAT1(ENTRY, _out)
#define ENTRY_STEP CONCAT1(ENTRY, _step)
#define ENTRY_RESET CONCAT1(ENTRY, _reset)
#define ENTRY_MEM_OBJ CONCAT1(ENTRY, _mem)

#define DELAY_MICRO 100

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

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

#if defined(USBCON)
	USBDevice.attach();
#endif
	
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

		if (serialEventRun) serialEventRun();

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

