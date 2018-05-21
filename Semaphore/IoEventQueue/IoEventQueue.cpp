/*
* IoPinChangeEventQueue.cpp
*
* Created: 2017-03-20 22:56:09
* Author: Tomas Kapin
*/

#include "IoEventQueue.h"

#include <avr/interrupt.h>

IoEventQueue::IoEventQueue(IoPort *port, uint8_t mask, timestamp_t currentTime) {
	_port = port;
	_mask = mask;
	_states[_head] = _port->GetCurrentState() & _mask;
	_times[_head] = currentTime;
}

void IoEventQueue::AddState(timestamp_t time) {
	
	uint8_t currentState = _port->GetCurrentState() & _mask;
	// return if current state is the same as the last logged state
	if (_states[_head] == currentState) {
		return;
	}

	_head = (_head + 1) & IOEQ_BUFFER_MASK;
	// check for overflow
	if (_head == _tail) {
		_overflow = true;
	}

	_states[_head] = currentState;
	_times[_head] = time;
}

// Valid to be called only when IsEmpty() returns false
IoEvent IoEventQueue::ProcessEvent() {
	
	// TODO: disable only pin change interrupts
	// disable interrupts here, we need to have this 'atomic'
	cli();
	uint8_t lastState = _states[_tail];
	timestamp_t lastTime = _times[_tail];
	_tail = (_tail + 1) & IOEQ_BUFFER_MASK;
	uint8_t thisState = _states[_tail];
	timestamp_t thisTime = _times[_tail];
	// enable interrupts back here
	sei();

	return IoEvent(thisState, thisState ^ lastState, thisTime, thisTime - lastTime);
}