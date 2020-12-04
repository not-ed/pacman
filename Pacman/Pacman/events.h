#pragma once
#ifndef EVENTS_H
#define EVENTS_H

#include "S2D/S2D.h"
#include <vector>

enum class EventType {
	MUNCHIE_COLLECTED, // A munchie has been collected
	BONUS_COLLECTED, // A bonus item has been collected
	GHOST_CAUGHT, // A scared ghost got caught by the player
	PLAYER_CAUGHT, // The player has been caught by a roaming ghost
	POSITION_RESET, // Event to signify that
	OUT_OF_LIVES // Signifying that the current run has ended because the player is out of lives.
};

// Allows interaction between other objects without relying on each other knowing their derived type.
class EventDispatcher {
public:
	// Send an event to the dispatcher instance (AKA the object being communicated with's dispatcher)
	void SendEvent(EventType event);
	// TODO: remove std::vector<EventType>* GetPendingEvents() { return &pendingEvents; }

	// Any recieved and currently held events.
	std::vector<EventType> pendingEvents;

	// Discard all recieved and currently held events
	void ClearEvents();

};

#endif