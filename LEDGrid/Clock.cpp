#include "Clock.h"

Clock::Clock() {
	Clock(12, 0, true);
}

Clock::Clock(uint8_t hour, uint8_t minute, bool am) {
	this->hour = hour;
	this->minute = minute;
	this->second = 0;
	this->am = am;

	this->time = 0;
}

int Clock::getTime() {
	return (hour * 100) + minute;
}

bool Clock::getAm() {
	return am;
}

void Clock::setTime(uint8_t hour, uint8_t minute, bool am) {
	setHour(hour);
	setMinute(minute);
	setAm(am);
}

void Clock::setHour(uint8_t hour) {
	this->hour = hour;
}

void Clock::setMinute(uint8_t minute) {
	this->minute = minute;
}

void Clock::setAm(bool am) {
	this->am = am;
}

void Clock::update() {

	if (millis() - time > SECOND) {
		time = millis();
		second += 1;

		if (second > MINUTE) {
			second = 0;
			minute += 1;
		}

		if (minute > HOUR) {
			minute = 0;
			hour += 1;

			if (hour == CYCLE) {
				am = am ? false : true;
			}
		}

		if (hour > CYCLE) {
			hour = 1;
		}
	}
}