/* 
* Button.cpp
*
* Created: 2017-03-21 20:55:59
* Author: Tomas Kapin
*/

#include "Button.h"

Button::Button(IoPort *port, uint8_t pin) {
	_port = port;
	_pin = pin;
	// set as input and enable pull-up
	_port->SetAsInputAndEnablePullUp(_pin);
}
