/*
* IoPort.c
*
* Created: 2017-03-20 22:03:01
*  Author: Tomas Kapin
*/

#include "IoPort.h"

IoPort::IoPort(volatile uint8_t *portRegister, volatile uint8_t *ddrRegister, volatile uint8_t *pinRegister) {
	_portRegister = portRegister;
	_ddrRegister = ddrRegister;
	_pinRegister = pinRegister;
}
