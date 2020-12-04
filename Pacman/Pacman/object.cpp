#include "object.h"

Object::~Object() {
	// Free objects from memory
	// This is automatically called by classes derived from Object, and will be handled even if the child implements its own destructor.
	delete rect;
	delete spriteRect;
}

void Object::QueryCollision(Object* other) {
	if (rect->Intersects(other->GetRect())) {
		OnCollision(other);
	}
}

bool Object::isAligned() {
	// Implicitly converting the position from float to int removes the possibility of floating-point imprecision.
	int rounded_x = rect->X;
	int rounded_y = rect->Y;

	if (rounded_x % 32 == 0 && rounded_y % 32 == 0) {
		return true;
	}
	else return false;
}

void Object::AlignToGrid() {
	int x_grid = round(rect->X / 32);
	int y_grid = round(rect->Y / 32);

	rect->X = int(x_grid * 32);
	rect->Y = int(y_grid * 32);
}

void Object::IncrementFrameCount() {
	frameCount++;
}

void Object::QueryBoundsPositioning() {
	if (rect->X > Graphics::GetViewportWidth()) {
		rect->X = -rect->Width;
	}
	if (rect->X < -rect->Width) {
		rect->X = Graphics::GetViewportWidth();
	}
	if (rect->Y > Graphics::GetViewportHeight()) {
		rect->Y = -rect->Height;
	}
	if (rect->Y < -rect->Height) {
		rect->Y = Graphics::GetViewportHeight();
	}
}

void Object::HandleEvents() {
	for (EventType e : events.pendingEvents) {
		DispatchEvent(e);
	}
	events.ClearEvents();
}