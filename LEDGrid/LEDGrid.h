#ifndef LED_GRID_H_
#define LED_GRID_H_

#include "CustomPalette.h"
#include "GridControl.h"
#include "Clock.h"

#define LED_PIN     10
#define NUM_LEDS    ROWS * COLUMNS
#define BRIGHTNESS  32//2   // this is the default brightness
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

extern CRGB leds[NUM_LEDS];
extern CRGB targetColor;
extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;

extern Clock clk;

enum Effects {
    ROWTRIX,
    DOTS,
    EQ,
	LIGHT,
    CLOCK
};

static const enum Effects EffectsMap[] = { ROWTRIX, DOTS, EQ, LIGHT };

class LEDGrid {

    public:
		// doEffect() runs the effect during arduino update (call this in void loop())
		void doEffect();

		Effects getEffect();

		// selectEffect(effect) choose a display effect on the LED Grid
		void selectEffect(Effects effect);

		// setBrightness(b) set the global brightness of the LED Grid
		void setBrightness(uint8_t b);
    
    private:
		Effects currentEffect;
		// Collection of animations

		// rowtrix(colorIndex, spread, spreadSpd, spreadRate, fadeSpd, fadeRate)   
		// spread: controls length
		// spreadSpd: rate at which spread is created
		// spreadRate: number created per update
		// fadeSpd: rate at which fade is updated
		// fadeRate: controls smoothness
		void rowtrix(uint8_t colorIndex, uint8_t spread, uint8_t spreadSpd, uint8_t spreadRate,
					 uint8_t fadeSpd, uint8_t fadeRate);

		void solid();
    
		// text(string, scroll, scrollSpd) displays text in string to led grid
		void text(char *string, bool scroll, int scrollSpd);

		// clock(time) displays a clock to led grid
		// ex. time is supplied as '0124' to represent 1:24
		void clock(uint16_t time, bool am);

		// equalizer() displays audio frequency to led grid with text overlayed if parameter has
		// been set in vEQ
		// void equalizer();
};
#endif
