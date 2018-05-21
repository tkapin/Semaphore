/*
 * StateMachine.h
 *
 * Created: 2018-05-21 21:28:09
 *  Author: Tomas Kapin
 */ 

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "../Core/TimeDefs.h"
#include "../Core/Led.h"
#include "../Button/ExtendedButton.h"
#include "State.h"
#include "Transition.h"

class StateMachine {

	private:

	Led *_red;
	Led *_yellow;
	Led *_green;

	ExtendedButton *_up;
	ExtendedButton *_down;

	// state description
	State _state;
	timestamp_t _firstStateEntryTime = 0;
	timestamp_t _lastStateEntryTime = 0;
	uint8_t _stateEntriesCount = 0;

	// constants
	const static timespan_t InitDelta = 1600;
	const static timespan_t GameWinDelta = 200;

	timespan_t _delta = InitDelta;
	
	// predicates

	static bool ButtonDownClick(StateMachine *sm, timestamp_t currentTime) {
		return sm->_down->TryProcessClick(currentTime);
	}

	static bool ButtonDownDoubleClick(StateMachine *sm, timestamp_t currentTime) {
		return sm->_down->TryProcessDoubleClick(currentTime);
	}

	static bool ButtonDownLongPress(StateMachine *sm, timestamp_t currentTime) {
		return sm->_down->TryProcessLongPress(currentTime);
	}

	static bool ButtonUpClick(StateMachine *sm, timestamp_t currentTime) {
		return sm->_up->TryProcessClick(currentTime);
	}

	static bool ButtonUpDoubleClick(StateMachine *sm, timestamp_t currentTime) {
		return sm->_up->TryProcessDoubleClick(currentTime);
	}

	static bool ButtonUpLongPress(StateMachine *sm, timestamp_t currentTime) {
		return sm->_up->TryProcessLongPress(currentTime);
	}

	static bool TimeoutDelta(StateMachine *sm, timestamp_t currentTime) {
		return IsFirstStateEntryTimeout(sm, currentTime, sm->_delta);
	}

	static bool Timeout1s(StateMachine *sm, timestamp_t currentTime) {
		return IsFirstStateEntryTimeout(sm, currentTime, 1000);
	}

	static bool Timeout10s(StateMachine *sm, timestamp_t currentTime) {
		return  IsFirstStateEntryTimeout(sm, currentTime, 10000);
	}

	static bool IsRoundWon(StateMachine *sm, timestamp_t currentTime) {
		return (sm->_delta > GameWinDelta) && ButtonDownClick(sm, currentTime);
	}

	static bool IsGameWon(StateMachine *sm, timestamp_t currentTime) {
		return (sm->_delta <= GameWinDelta) && ButtonDownClick(sm, currentTime);
	}

	static bool ButtonDownClickOrTimeout10s(StateMachine *sm, timestamp_t currentTime) {
		return IsFirstStateEntryTimeout(sm, currentTime, 10000) || ButtonDownClick(sm, currentTime);
	}

	static bool LastEntryTimeout10s(StateMachine *sm, timestamp_t currentTime) {
		return IsLastStateEntryTimeout(sm, currentTime, 10000);
	}

	static bool FastBlinkTimeout(StateMachine *sm, timestamp_t currentTime) {
		return IsLastStateEntryTimeout(sm, currentTime, 200);
	}

	static bool SlowBlinkTimeout(StateMachine *sm, timestamp_t currentTime) {
		return IsLastStateEntryTimeout(sm, currentTime, 1000);
	}

	// actions

	static void DecreaseDelta(StateMachine *sm) {
		timestamp_t newDelta = sm->_delta * 0.5;
		sm->_delta = newDelta > 100 ? newDelta : 100;
	}

	static void ResetDelta(StateMachine *sm) {
		sm->_delta = InitDelta;
	}

	static void ToggleRed(StateMachine *sm) {
		sm->_red->Toggle();
	}

	static void ToggleYellow(StateMachine *sm) {
		sm->_yellow->Toggle();
	}

	static void ToggleGreen(StateMachine *sm) {
		sm->_green->Toggle();
	}

	static void ToggleAll(StateMachine *sm) {
		ToggleRed(sm);
		ToggleYellow(sm);
		ToggleGreen(sm);
	}

	// helper functions for actions & predicates

	inline static bool IsFirstStateEntryTimeout(StateMachine *sm, timestamp_t currentTime, timespan_t timeout) {
		return currentTime > sm->_firstStateEntryTime + timeout;
	}

	inline static bool IsLastStateEntryTimeout(StateMachine *sm, timestamp_t currentTime, timespan_t timeout) {
		return currentTime > sm->_lastStateEntryTime + timeout;
	}

	// states
	const static uint8_t StatesCount = 9;

	// 2 bytes per state
	State _states[StatesCount] = {
		State(StateId::SemaphoreStop, StateDefinition::Red),
		State(StateId::SemaphoreReady, StateDefinition::Red | StateDefinition::Yellow),
		State(StateId::SemaphoreGo, StateDefinition::Green | StateDefinition::FastClickMode),
		State(StateId::SemaphoreSlow, StateDefinition::Yellow),
		State(StateId::SemaphoreDisabled, StateDefinition::Yellow),
		State(StateId::GameRoundWin, StateDefinition::Green),
		State(StateId::GameRoundLoss, StateDefinition::Red),
		State(StateId::GameWin, StateDefinition::Red | StateDefinition::Yellow | StateDefinition::Green),
		// demo
		State(StateId::Demo, StateDefinition::None),
	};

	// transitions
	const static uint8_t TransitionsCount = 27;

	// 1 + 1 + 2 + 2 = 6 bytes per transition
	// Transition(fromState, toState, predicate, action);
	Transition _transitions[TransitionsCount] = {
		// game mode
		Transition(StateId::SemaphoreStop, StateId::SemaphoreReady, TimeoutDelta),
		Transition(StateId::SemaphoreStop, StateId::GameRoundLoss, ButtonDownClick, ResetDelta),
		Transition(StateId::SemaphoreReady, StateId::SemaphoreGo, TimeoutDelta),
		Transition(StateId::SemaphoreReady, StateId::GameRoundLoss, ButtonDownClick, ResetDelta),
		Transition(StateId::SemaphoreGo, StateId::SemaphoreSlow, TimeoutDelta),
		Transition(StateId::SemaphoreGo, StateId::GameRoundWin, IsRoundWon, DecreaseDelta),
		Transition(StateId::SemaphoreGo, StateId::GameWin, IsGameWon, ResetDelta),
		Transition(StateId::SemaphoreSlow, StateId::SemaphoreStop, TimeoutDelta),
		Transition(StateId::SemaphoreSlow, StateId::GameRoundLoss, ButtonDownClick, ResetDelta),
		Transition(StateId::GameWin, StateId::GameWin, FastBlinkTimeout, ToggleAll),
		Transition(StateId::GameWin, StateId::SemaphoreStop, ButtonDownClickOrTimeout10s, ResetDelta),
		Transition(StateId::SemaphoreReady, StateId::SemaphoreDisabled, ButtonDownDoubleClick, ResetDelta),
		Transition(StateId::GameRoundWin, StateId::GameRoundWin, FastBlinkTimeout, ToggleGreen),
		Transition(StateId::GameRoundWin, StateId::SemaphoreStop, Timeout1s),
		Transition(StateId::GameRoundLoss, StateId::GameRoundLoss, FastBlinkTimeout, ToggleRed),
		Transition(StateId::GameRoundLoss, StateId::SemaphoreStop, Timeout1s),
		// semaphore disabled mode
		Transition(StateId::SemaphoreDisabled, StateId::SemaphoreDisabled, SlowBlinkTimeout, ToggleYellow),
		// extended button functionality demo
		Transition(StateId::Demo, StateId::Demo, ButtonDownClick, ToggleRed),
		Transition(StateId::Demo, StateId::Demo, ButtonDownDoubleClick, ToggleYellow),
		Transition(StateId::Demo, StateId::Demo, ButtonDownLongPress, ToggleGreen),
		// transitions between modes (upper button)
		Transition(StateId::SemaphoreStop, StateId::SemaphoreDisabled, ButtonUpClick),
		Transition(StateId::SemaphoreReady, StateId::SemaphoreDisabled, ButtonUpClick),
		Transition(StateId::SemaphoreGo, StateId::SemaphoreDisabled, ButtonUpClick),
		Transition(StateId::SemaphoreSlow, StateId::SemaphoreDisabled, ButtonUpClick),
		Transition(StateId::SemaphoreDisabled, StateId::Demo, ButtonUpClick),
		Transition(StateId::Demo, StateId::SemaphoreStop, ButtonUpClick),
		Transition(StateId::Demo, StateId::SemaphoreStop, LastEntryTimeout10s),
	};

	void OnFirstStateEnter() {
		// set LEDs
		_red->TurnOnOff(_state.Definition & StateDefinition::Red);
		_yellow->TurnOnOff(_state.Definition &  StateDefinition::Yellow);
		_green->TurnOnOff(_state.Definition & StateDefinition::Green);
		// set buttons (fast click mode)
		bool fcm = _state.Definition & StateDefinition::FastClickMode;
		_up->SimpleModeEnableDisable(fcm);
		_down->SimpleModeEnableDisable(fcm);
	}

	void SetState(State &state, timestamp_t currentTime, bool isLoopBack) {
		// set all attributes if coming from different state
		if (!isLoopBack) {
			_state = state;
			_firstStateEntryTime = currentTime;
			_stateEntriesCount = 0;
			OnFirstStateEnter();
		}
		_lastStateEntryTime = currentTime;
		_stateEntriesCount++;
	}
	
	void ChangeState(Transition &t, timestamp_t currentTime) {
		for (uint8_t i = 0; i < StatesCount; i++) {
			State &state = _states[i];
			if (t.IsTo(state)) {
				SetState(state, currentTime, t.IsLoopBack());
				return;
			}
		}
	}

	public:

	StateMachine(Led *red, Led *yellow, Led *green, ExtendedButton *up, ExtendedButton *down, timestamp_t currentTime) : _red(red), _yellow(yellow), _green(green), _up(up), _down(down) {
		SetState(_states[0], currentTime, false);
	}

	void ExecuteTransitions(timestamp_t currentTime) {
		for (uint8_t i = 0; i < TransitionsCount; i++) {
			Transition &t = _transitions[i];
			if (t.IsExecutable(_state, this, currentTime)) {
				t.DoAction(this);
				ChangeState(t, currentTime);
				return;
			}
		}
	}
};

#endif /* STATEMACHINE_H_ */