/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "wiring_private.h"

void init() {
	// timer 2 is used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle

	// set timer 2 prescale factor to 64
#if defined(TCCR2) && defined(CS22)
	sbi(TCCR2, CS22);
#elif defined(TCCR2B) && defined(CS22)
	sbi(TCCR2B, CS22);
//#else
	// Timer 2 not finished (may not be present on this CPU)
#endif

	// configure timer 2 for phase correct pwm (8-bit)
#if defined(TCCR2) && defined(WGM20)
	sbi(TCCR2, WGM20);
#elif defined(TCCR2A) && defined(WGM20)
	sbi(TCCR2A, WGM20);
//#else
	// Timer 2 not finished (may not be present on this CPU)
#endif

#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
	sbi(TCCR3B, CS31);		// set timer 3 prescale factor to 64
	sbi(TCCR3B, CS30);
	sbi(TCCR3A, WGM30);		// put timer 3 in 8-bit phase correct pwm mode
#endif

#if defined(TCCR4A) && defined(TCCR4B) && defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
	sbi(TCCR4B, CS42);		// set timer4 prescale factor to 64
	sbi(TCCR4B, CS41);
	sbi(TCCR4B, CS40);
	sbi(TCCR4D, WGM40);		// put timer 4 in phase- and frequency-correct PWM mode	
	sbi(TCCR4A, PWM4A);		// enable PWM mode for comparator OCR4A
	sbi(TCCR4C, PWM4D);		// enable PWM mode for comparator OCR4D
#else /* beginning of timer4 block for ATMEGA1280 and ATMEGA2560 */
#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
	sbi(TCCR4B, CS41);		// set timer 4 prescale factor to 64
	sbi(TCCR4B, CS40);
	sbi(TCCR4A, WGM40);		// put timer 4 in 8-bit phase correct pwm mode
#endif
#endif /* end timer4 block for ATMEGA1280/2560 and similar */	

#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
	sbi(TCCR5B, CS51);		// set timer 5 prescale factor to 64
	sbi(TCCR5B, CS50);
	sbi(TCCR5A, WGM50);		// put timer 5 in 8-bit phase correct pwm mode
#endif

#if defined(ADCSRA)
	// set a2d prescaler so we are inside the desired 50-200 KHz range.
	#if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
		sbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
		sbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
		cbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#else // 128 kHz / 2 = 64 KHz -> This is the closest you can get, the prescaler is 2
		cbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#endif
	// enable a2d conversions
	sbi(ADCSRA, ADEN);
#endif

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(UCSRB)
	UCSRB = 0;
#elif defined(UCSR0B)
	UCSR0B = 0;
#endif
}
