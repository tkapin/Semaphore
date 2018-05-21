/*
* main.cpp
*
*  Author: Tomas Kapin
*
* Demo project to demonstrate timers and interrupt handling on AVR ATinty85 (8kB flash, 512B RAM)
*
* Features:
*  - circular buffer based event queue
*  - state machine with state / transition definitions
*  - button input with debouncing, capable of click, double click and long press events handling
*  - three LED demo with simple "game" and button click demo modes
*
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Core/TimeDefs.h"
#include "Core/IoPort.h"
#include "Core/Led.h"
#include "IoEventQueue/IoEventQueue.h"
#include "Button/ExtendedButton.h"
#include "StateMachine/StateMachine.h"

// timer in ms
volatile timestamp_t now = 0;

// IO port
IoPort portb(&PORTB, &DDRB, &PINB);

IoEventQueue ioEventQueue(&portb, (1 << PB3) | (1 << PB4), now);

// timer - 32 bit unsigned integer, incremented by 1ms, overflows after ~49.7 days
ISR(TIMER0_COMPA_vect) {
	now++;
}

// PIN change interrupt
ISR(PCINT0_vect) {
	ioEventQueue.AddState(now);
}

int main(void)
{

	// set timer 0 to trigger TIMER0_COMPA interrupt once per 1ms
	OCR0A = 125;
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << OCIE0A);

	// enable PCINT interrupts on button pins
	GIMSK |= (1 << PCIE);
	PCMSK |= (1 << PB3) | (1 << PB4);

	// LEDs
	Led green(&portb, PB0);
	Led yellow(&portb, PB1);
	Led red(&portb, PB2);

	// buttons
	ExtendedButton up(&portb, PB3);
	ExtendedButton down(&portb, PB4);

	StateMachine sm = StateMachine(&red, &yellow, &green, &up, &down, now);

	// finally, enable interrupts
	sei();

	// main loop
	while (true) {
		
		// one loop round is run with a fixed time
		timestamp_t currentTime = now;
		
		// process pin change queue events (i.e., buttons)
		if (!ioEventQueue.IsEmpty()) {
			IoEvent e = ioEventQueue.ProcessEvent();
			up.LogEvent(e, currentTime);
			down.LogEvent(e, currentTime);
		}

		// execute the state machine transitions, the state machine and IO event queue are not coupled
		sm.ExecuteTransitions(currentTime);

		// if queue overflow happens, we want to 'brick' the chip so we notice
		if (ioEventQueue.IsOverflow()) {
			// disable interrupts
			cli();
			// turn on all lights
			red.TurnOn();
			yellow.TurnOn();
			green.TurnOn();
			// wait till the end of time
			while (true);
		}
	}
}

