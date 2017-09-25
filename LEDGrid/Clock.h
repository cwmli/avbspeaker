#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdbool.h>
#include <Arduino.h>

#define SECOND 1000
#define MINUTE 59
#define HOUR   59
#define CYCLE  12

class Clock {

	public:
		// Clock() initialize a new clock with default time as 12:00am
		Clock();
		
		// Clock(hour, minute, am) initialize a new clock with hour, minute, and am
		// or pm
		Clock(uint8_t hour, uint8_t minute, bool am);

		// getTime() returns the current time in 12 hr format
		int getTime();

		bool getAm();
		
		// setTime(hour, minute, am) sets the current time to hour:minute:am
		// all fields can be nulled if they are not to be changed
		void setTime(uint8_t hour, uint8_t minute, bool am);
		
		// setHour(hour) set the current hour
		void setHour(uint8_t hour);

		// setMinute(minute) set the current minute
		void setMinute(uint8_t minute);

		// setAm(am) set whether the clock is am or pm
		void setAm(bool am);

		// update() update the current time using arduino's millis() fn
		void update();
	private:
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
		bool am;

		unsigned long time;
};
#endif
