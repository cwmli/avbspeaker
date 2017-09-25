#ifndef _CUSTOM_PALETTE_H
#define _CUSTOM_PALETTE_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <../FastLED/FastLED.h>

// Collection of custom CRGBPalette16s stored in PROGMEM

const PROGMEM TProgmemPalette16 lowLightClockPalette_p = {
	0x990000,
    0xff9900,
	0x990000,
	0xff9900,
	CRGB::YellowGreen, //PM
	CRGB::Teal,  //AM
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black,
	CRGB::Black
};
#endif
