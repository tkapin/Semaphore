/*
 * StateId.h
 *
 * Created: 2017-04-04 21:53:33
 *  Author: Tomas Kapin
 */

#ifndef STATEID_H_
#define STATEID_H_

enum class StateId : uint8_t {
	Undef = 0,
	SemaphoreStop,
	SemaphoreReady,
	SemaphoreGo,
	SemaphoreSlow,
	SemaphoreDisabled,
	MenuSemaphore,
	MenuTimer,
	MenuGame,
	GameRoundWin,
	GameRoundLoss,
	GameWin,
	Demo,
};

#endif /* STATEID_H_ */