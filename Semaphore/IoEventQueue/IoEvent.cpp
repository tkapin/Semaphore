/*
* IoPinChangeEvenet.cpp
*
* Created: 2017-03-20 22:55:47
* Author: Tomas Kapin
*/

#include "IoEvent.h"

IoEvent::IoEvent(uint8_t state, uint8_t mask, timestamp_t time, timespan_t delta) {
	_state = state;
	_mask = mask;
	_time = time;
	_delta = delta;
}
