/*
* ExtendedButton.h
*
* Created: 2017-03-21 21:54:03
* Author: Tomas Kapin
*/

#ifndef __EXTENDEDBUTTON_H__
#define __EXTENDEDBUTTON_H__

#include "Button.h"

enum class ExtendedButtonEventId : uint8_t {
	None = 0,
	Click,
	DoubleClick,
	LongPress,
};

class ExtendedButton : public Button {

	private:

	// thresholds in ms
	static const timespan_t DoubleClickTimeout = 400;
	static const timespan_t LongPressTimeout = 800;

	// initial press time
	timestamp_t _initPressTime = 0;

	// numbers of presses and releases
	uint8_t _presses = 0;
	uint8_t _releases = 0;

	// in simple mode, double-click and long press are disabled so the click can be detected immediately
	bool _isSimpleModeEnabled;

	// these functions can be used only in situations where it's clear that _initPressTime is set (i.e., after the first press)
	inline bool IsDoubleClickTimeouted(timestamp_t currentTime) {
		return currentTime >= _initPressTime + DoubleClickTimeout;
	}

	inline bool IsLongPressTimeouted(timestamp_t currentTime) {
		return currentTime >= _initPressTime + LongPressTimeout;
	}

	public:

	ExtendedButton(IoPort *port, uint8_t pin) : Button(port, pin) {
	}

	void LogEvent(IoEvent &e, timestamp_t currentTime);

	// click event is detected as soon as it's clear it can't be any other event
	inline bool IsClicked(timestamp_t currentTime) {
		return _presses == 1 && (_isSimpleModeEnabled || (_releases == 1 && IsDoubleClickTimeouted(currentTime)));
	}

	// double click is detected as two releases before double click timeout
	inline bool IsDoubleClicked(timestamp_t currentTime) {
		return !_isSimpleModeEnabled && _presses == 2 && _releases == 2 && !IsDoubleClickTimeouted(currentTime);
	}

	// long press is detected as one press and no release after the long press timeout
	inline bool IsLongPressed(timestamp_t currentTime) {
		return !_isSimpleModeEnabled && _presses == 1 && _releases == 0 && IsLongPressTimeouted(currentTime);
	}

	inline void SimpleModeEnableDisable(bool value) {
		_isSimpleModeEnabled = value;
	}

	ExtendedButtonEventId CheckEvent(timestamp_t currentTime);

	bool TryProcessClick(timestamp_t currentTime);

	bool TryProcessDoubleClick(timestamp_t currentTime);

	bool TryProcessLongPress(timestamp_t currentTime);

	void Reset();

};

#endif //__EXTENDEDBUTTON_H__
