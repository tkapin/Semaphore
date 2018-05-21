/*
* IoPinChangeEventQueue.h
*
* Created: 2017-03-20 22:56:09
* Author: Tomas Kapin
*/

#ifndef __IOPINCHANGEEVENTQUEUE_H__
#define __IOPINCHANGEEVENTQUEUE_H__

#include "IoEvent.h"

#include "../Core/TimeDefs.h"
#include "../Core/IoPort.h"

#include <avr/io.h>

// set default event buffer size
#ifndef IOEQ_BUFFER_SIZE
#define IOEQ_BUFFER_SIZE 8
#endif

// set the event buffer size mask and check if valid
#define IOEQ_BUFFER_MASK (IOEQ_BUFFER_SIZE - 1)
#if (IOEQ_BUFFER_SIZE & IOEQ_BUFFER_MASK)
#error BUFFER SIZE is not a power of 2
#endif

class IoEventQueue {
	
	private:

	// the event queue buffer
	uint8_t _states[IOEQ_BUFFER_SIZE];
	timestamp_t _times[IOEQ_BUFFER_SIZE];

	// event mask
	uint8_t _mask;

	volatile uint8_t _head = 0;
	volatile uint8_t _tail = 0;
	volatile bool _overflow = false;

	IoPort *_port;

	public:

	IoEventQueue(IoPort *port, uint8_t mask, timestamp_t currentTime);

	// this method should be called only in the ISR so it's guaranteed it's not interrupted
	void AddState(timestamp_t time);

	// Valid to be called only when IsEmpty() returns false
	IoEvent ProcessEvent();

	inline bool IsEmpty() {
		return _head == _tail;
	}

	inline bool IsOverflow() {
		return _overflow;
	}

};

#endif //__IOPINCHANGEEVENTQUEUE_H__
