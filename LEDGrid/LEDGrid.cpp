#include "LEDGrid.h"

void LEDGrid::doEffect() {
	static uint8_t startIndex = 0;
	startIndex = startIndex + 1; /* motion speed */

	if (currentEffect == Effects::ROWTRIX) {
		rowtrix(startIndex, random(6), 30, 1, 10, 1);
	}
	else if (currentEffect == Effects::DOTS) {
		rowtrix(startIndex, random(3), 20, 3, 15, 10);
	}
	else if (currentEffect == Effects::CLOCK) {
		clk.update();
		clock(clk.getTime(), true);
	}
	else {
		// VISUAL EQUALIZER

		// Text can be overlayed on top:
		// Example text display
		// char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		// text(str, true, 200);

		//vEQ.setText(NULL, true, 200);
		//equalizer();
	}

	FastLED.show();
}

void LEDGrid::selectEffect(Effects effect) {
	currentEffect = effect;
}

void LEDGrid::setBrightness(uint8_t b) {
	FastLED.setBrightness(b);
}

void LEDGrid::rowtrix(uint8_t colorIndex, uint8_t spread, uint8_t spreadSpd, uint8_t spreadRate,
					  uint8_t fadeSpd, uint8_t fadeRate) {
	static unsigned long lastNewLedUpdate;
	static unsigned long lastFadeUpdate;

	unsigned long time = millis();

	resetActiveLeds();
	if (time - lastNewLedUpdate > spreadSpd) {
		lastNewLedUpdate = time;
		for (uint8_t i = 0; i < spreadRate; i++) {
			uint8_t row = random(ROWS);
			uint8_t col = random(COLUMNS);

			/* debugging
			Serial.print(row);
			Serial.print(", ");
			Serial.print(col);
			Serial.print("\n"); */

			setActiveLed(row, col);

			if (spread > 0) {
				// randomly select surrounding leds
				uint8_t length = random(spread);

				// random orientation choose 1 as vertical, 0 as horizontal
				uint8_t orientation = random(2);
				// radnom direction 0 as right/up, 1 as left/down
				uint8_t dir = random(2);
				
				fillActiveLed(row, col, length, orientation, dir);
			}
		}
	}

	if (time - lastFadeUpdate > fadeSpd) {
		lastFadeUpdate = time;
		for (uint8_t r = 0; r < ROWS; r++) {
			for (uint8_t c = 0; c < COLUMNS; c++) {
				if (getActiveLed(r, c) == 1) {
					leds[gridIndex(r, c)] = ColorFromPalette(currentPalette, colorIndex, 255, currentBlending);
				}
				else {
					leds[gridIndex(r, c)].fadeToBlackBy(fadeRate);
				}
			}
		}
	}
}

void LEDGrid::text(char *string, bool scroll, int scrollSpd) {

	const uint8_t fixedRow = 1;

	static uint8_t startCol = scroll ? 9 : 1;
	static uint8_t colorIndex = 0;

	static unsigned long lastColorUpdate;
	static unsigned long lastScrollUpdate;

	unsigned long time = millis();

	resetActiveLeds();

	uint8_t len = strlen(string);
	for (uint8_t i = 0; i < len; i++) {
		if (string[i] == 32) {
			continue;
		}

		fillChar(fixedRow, startCol + (i * (FONT_X + 1)), string[i]);
	}

	if (scroll && time - lastScrollUpdate > scrollSpd) {
		lastScrollUpdate = time;

		startCol -= 1;
	}

	if (time - lastColorUpdate > 500) {
		lastColorUpdate = time;
		targetColor = ColorFromPalette(currentPalette, random(255), 255, currentBlending);
	}

	for (uint8_t c = 0; c < COLUMNS; c++) {
		for (uint8_t r = 0; r < ROWS; r++) {
			if (getActiveLed(r, c) == 1) {
				leds[gridIndex(r, c)] = nblend(leds[gridIndex(r, c)], targetColor, 5);
			}
			else {
				leds[gridIndex(r, c)].fadeToBlackBy(15);
			}
		}
	}
}

void LEDGrid::clock(uint16_t time, bool am) {
	const uint8_t startRow = 1;
	const uint8_t startCol = 7;
	uint8_t index = 0;

	resetActiveLeds();

	for (uint8_t i = 0; i < 4; i++) {
		uint8_t digit = time % 10;
		uint8_t col = i < 2 ? startCol - (NUM_X * i) : startCol - 1 - (NUM_X * i);
		time /= 10;

		if (digit == 0 && i == 3) {
			continue;
		}

		fillNum(startRow, col, digit);
	}

	for (uint8_t c = 0; c < COLUMNS; c++) {
		if (c == 2 || c == 5 || c == 7) {
			index += 16;
		}

		for (uint8_t r = 0; r < ROWS; r++) {
			if (getActiveLed(r, c) == 1) {
				leds[gridIndex(r, c)] = ColorFromPalette(lowLightClockPalette_p, index, 255, currentBlending);
			}
			else {
				leds[gridIndex(r, c)].fadeToBlackBy(255);
			}
		}
	}

	// set the separator hour and minute separator
	// use different separator color based on am/pm
	uint8_t separator = clk.getAm() ? 80 : 64;

	leds[gridIndex(2, 4)] = ColorFromPalette(lowLightClockPalette_p, separator, 255, currentBlending);
	leds[gridIndex(3, 4)] = ColorFromPalette(lowLightClockPalette_p, separator, 255, currentBlending);
}

/*void LEDGrid::equalizer() {

	static unsigned long lastColorUpdate;

	unsigned long time = millis();

	// setup the frequency display first
	vEQ.update();

	// color the display ONLY for activeLeds set by updateFrequencyData()    
	if (time - lastColorUpdate > 500) {
		lastColorUpdate = time;

		for (uint8_t c = 0; c < COLUMNS; c++) {
			for (uint8_t r = 0; r < ROWS; r++) {
				if (getActiveLed(r, c) == 2) {
					// placeholder colorIndex value CHANGE
					leds[gridIndex(r, c)] = ColorFromPalette(currentPalette, 1, 255, currentBlending);
				}
				else if (getActiveLed(r, c) != 1) {
					leds[gridIndex(r, c)].fadeToBlackBy(15);
				}
			}
		}
	}

	// write text ontop
	if (vEQ.getString()) {
		this->text(vEQ.getString(), vEQ.getIfScroll(), vEQ.getStringSpd());
	}
}*/
