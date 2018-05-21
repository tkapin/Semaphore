/*
 * State.h
 *
 * Created: 2018-05-21 21:39:45
 *  Author: Tomas Kapin
 */ 

#ifndef STATE_H_
#define STATE_H_

#include "StateId.h"
#include "StateDefinition.h"

struct State {

	StateId Id;
	StateDefinition Definition;

	State() {}

	State(StateId id, StateDefinition definition) {
		Id = id;
		Definition = definition;
	}

};

#endif /* STATE_H_ */