/*
* Led.cpp
*
* Created: 2017-03-20 22:26:17
* Author: Tomas Kapin
*/

#include "Led.h"

Led::Led(IoPort *port, uint8_t pin) {
	_port = port;
	_pin = pin;
	_port->SetAsOutput(_pin);
}
