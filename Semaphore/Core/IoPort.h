/*
* IoPort.h
*
* Created: 2017-03-20 22:03:55
*  Author: Tomas Kapin
*/

#ifndef __IOPORT_H__
#define __IOPORT_H__

#include <avr/io.h>

class IoPort {

	private:

	// Port X Data Register
	volatile uint8_t *_portRegister;
	// Port X Data Direction Register
	volatile uint8_t *_ddrRegister;
	// Port X Input Pins Address
	volatile uint8_t *_pinRegister;

	public:

	IoPort(volatile uint8_t *portRegister, volatile uint8_t *ddrRegister, volatile uint8_t *pinRegister);

	inline void SetAsOutput(uint8_t pin) {
		*_ddrRegister |= (1 << pin);
	}

	// set PORTX to 1 on input to enable pull-up resistors
	inline void SetAsInputAndEnablePullUp(uint8_t pin) {
		*_ddrRegister &= ~(1 << pin);
		*_portRegister |= (1 << pin);
	}

	inline uint8_t GetCurrentState() {
		return *_pinRegister;
	}

	inline void SetPinHigh(uint8_t pin) {
		*_portRegister |= (1 << pin);
	}

	inline void SetPinLow(uint8_t pin) {
		*_portRegister &= ~(1 << pin);
	}

	inline void SetPinHighLow(uint8_t pin, bool setHigh) {
		setHigh ? SetPinHigh(pin) : SetPinLow(pin);
	}

	inline void TogglePin(uint8_t pin) {
		*_portRegister ^= (1 << pin);
	}

	inline bool IsPinHigh(uint8_t pin) {
		return *_pinRegister & pin;
	}

	inline bool IsPinLow(uint8_t pin) {
		return !IsPinHigh(pin);
	}

};
#endif // __IOPORT_H__ */