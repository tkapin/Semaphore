/* 
* Led.h
*
* Created: 2017-03-20 22:26:17
* Author: Tomas Kapin
*/

#ifndef __LED_H__
#define __LED_H__

#include "IoPort.h"

class Led {
	
	private:

	IoPort *_port;
	uint8_t _pin;

	public:

	Led(IoPort *port, uint8_t pin);

	inline void TurnOn() {
		_port->SetPinHigh(_pin);
	}

	inline void TurnOff() {
		_port->SetPinLow(_pin);
	}

	inline void TurnOnOff(bool state) {
		_port->SetPinHighLow(_pin, state);
	}

	inline void Toggle() {
		_port->TogglePin(_pin);
	}

	inline bool IsOn() {
		return _port->IsPinHigh(_pin);
	}
};

#endif //__LED_H__
