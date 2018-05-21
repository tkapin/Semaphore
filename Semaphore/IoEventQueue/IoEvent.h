/*
* IoPinChangeEvenet.h
*
* Created: 2017-03-20 22:55:47
* Author: Tomas Kapin
*/

#ifndef __IOPINCHANGEEVENT_H__
#define __IOPINCHANGEEVENT_H__

#include "../Core/TimeDefs.h"

class IoEvent {

	private:

	uint8_t _state;
	uint8_t _mask;
	timestamp_t _time;
	timespan_t _delta;

	public:

	IoEvent(uint8_t state, uint8_t mask, timestamp_t time, timespan_t delta);

	inline timestamp_t GetTime() {
		return _time;
	}

	inline timespan_t GetDelta() {
		return _delta;
	}

	inline bool IsEmpty() {
		return _mask;
	}

	inline bool IsOnChannel(uint8_t channel) {
		return _mask & (1 << channel);
	}

	inline bool IsLowToHigh(uint8_t channel) {
		return _state & (1 << channel);
	}

	inline bool IsHighToLow(uint8_t channel) {
		return !IsLowToHigh(channel);
	}

};

#endif //__IOPINCHANGEEVENT_H__
