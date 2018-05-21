/*
 * Transition.h
 *
 * Created: 2018-05-21 21:39:55
 *  Author: Tomas Kapin
 */ 

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include "StateId.h"
#include "State.h"

class StateMachine;

class Transition {
	
	private:

	StateId _from;
	StateId _to;

	// the predicate and action functions has to be static in order to be used in the transitions
	bool (*_predicate)(StateMachine *, timestamp_t);
	void (*_action)(StateMachine *);
	
	public:

	Transition(StateId from, StateId to, bool (*predicate)(StateMachine *, timestamp_t)) : Transition(from, to, predicate, nullptr) {
	}

	Transition(StateId from, StateId to, bool (*predicate)(StateMachine *, timestamp_t), void (*action)(StateMachine *))  {
		_from = from;
		_to = to;
		_predicate = predicate;
		_action = action;
	}

	inline bool IsPredicateTrue(StateMachine *sm, timestamp_t currentTime) {
		return _predicate(sm, currentTime);
	}

	void DoAction(StateMachine *sm) {
		if (_action != nullptr) {
			_action(sm);
		}
	}

	inline bool IsExecutable(State &state, StateMachine *sm, timestamp_t currentTime) {
		return IsFrom(state) && IsPredicateTrue(sm, currentTime);
	}

	inline bool IsLoopBack() {
		return _from == _to;
	}

	inline bool IsFrom(State &state) {
		return _from == state.Id;
	}

	inline bool IsTo(State &state) {
		return _to == state.Id;
	}

};

#endif /* TRANSITION_H_ */