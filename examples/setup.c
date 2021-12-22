#include "Arduino.h"

const int potPin = A0;
const int ledRedPin = 9;
const int ledYellowPin = 10;
const int ledGreenPin = 11;
const int soundbuzzerPin = 3;
const int buttonPin = 2;

void setup() {
	pinMode(potPin, INPUT);
	pinMode(ledRedPin, OUTPUT);
	pinMode(ledYellowPin, OUTPUT);
	pinMode(ledGreenPin, OUTPUT);
	pinMode(soundbuzzerPin, OUTPUT);
	pinMode(buttonPin, INPUT_PULLUP);
}