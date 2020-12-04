#include "events.h"

void EventDispatcher::SendEvent(EventType event) {
	pendingEvents.push_back(event);
}

void EventDispatcher::ClearEvents() {
	pendingEvents.clear();
}