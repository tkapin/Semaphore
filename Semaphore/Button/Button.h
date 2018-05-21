/* 
* Button.h
*
* Created: 2017-03-21 20:55:59
* Author: Tomas Kapin
*/

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "../Core/IoPort.h"
#include "../IoEventQueue/IoEvent.h"

class Button {

	private:

	IoPort *_port;
	uint8_t _pin;

	inline bool IsDebounced(IoEvent &e) {
		return e.GetDelta() >= 5;
	}

	public:

	Button(IoPort *port, uint8_t pin);

	inline bool IsPressed() {
		return _port->IsPinLow(_pin);
	}

	// debouncing is handled here because debouncing is a property of a button
	inline bool IsPressedEvent(IoEvent &e) {
		return e.IsOnChannel(_pin) && e.IsHighToLow(_pin) && IsDebounced(e);
	}

	inline bool IsReleasedEvent(IoEvent &e) {
		return e.IsOnChannel(_pin) && e.IsLowToHigh(_pin) && IsDebounced(e);
	}

};

#endif //__BUTTON_H__
