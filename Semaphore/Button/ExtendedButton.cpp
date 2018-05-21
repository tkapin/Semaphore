/*
* ExtendedButton.cpp
*
* Created: 2017-03-21 21:54:03
* Author: Tomas Kapin
*/


#include "ExtendedButton.h"

void ExtendedButton::LogEvent(IoEvent &e, timestamp_t currentTime) {
	if (IsPressedEvent(e)) {
		// set init press time if not set yet
		if (_initPressTime == 0) {
			_initPressTime = currentTime;
		}
		_presses++;
	}
	if (IsReleasedEvent(e)) {
		// don't start with release
		if (_initPressTime != 0) {
			_releases++;
		}
	}
	// reset on unsupported configurations
	if (_presses > 2) {
		Reset();
	}
}

bool ExtendedButton::TryProcessClick(timestamp_t currentTime) {
	if (IsClicked(currentTime)) {
		Reset();
		return true;
	}
	return false;
}

bool ExtendedButton::TryProcessDoubleClick(timestamp_t currentTime) {
	if (IsDoubleClicked(currentTime)) {
		Reset();
		return true;
	}
	return false;
}

bool ExtendedButton::TryProcessLongPress(timestamp_t currentTime) {
	if (IsLongPressed(currentTime)) {
		Reset();
		return true;
	}
	return false;
}

void ExtendedButton::Reset() {
	_initPressTime = 0;
	_presses = 0;
	_releases = 0;
}