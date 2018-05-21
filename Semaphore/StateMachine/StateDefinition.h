/*
 * StateDefinition.h
 *
 * Created: 2017-04-04 21:55:01
 *  Author: Tomas Kapin
 */ 

#ifndef STATEDEFINITION_H_
#define STATEDEFINITION_H_

// state definition flags
enum class StateDefinition : uint8_t {
	None = 0x00,
	Red = 0x01,
	Yellow = 0x02,
	Green = 0x04,
	// needed for immediate response in the game
	FastClickMode = 0x08,
};

// http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c
inline StateDefinition operator|(StateDefinition a, StateDefinition b) {
	return static_cast<StateDefinition>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

// and is used solely to test the flag presence, hence is casted to bool implicitly
inline bool operator&(StateDefinition a, StateDefinition b) {
	return static_cast<bool>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

#endif /* STATEDEFINITION_H_ */