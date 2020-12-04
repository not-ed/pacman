#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "S2D/S2D.h"
#include "id.h"
#include "audiomanager.h"
#include "spritemanager.h"
#include "events.h"

using namespace S2D;

class Object
{
public:
	~Object();

	// Pure virtual function declaration
	// Classes derived from this (AKA any kind of game object) MUST have a definition hereafter, or it will cause a compilation error.
	virtual void Update(int deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void OnCollision(Object* other) = 0;
	
	// Check for collision with other object, and handle accordingly using OnCollision if it does.
	void QueryCollision(Object* other);

	// Check if the object has recieved any events, and dispatch them accordingly if they have.
	void HandleEvents();

	// Check if object is pending destruction.
	bool MarkedForPruning() { return pruneFlag; }

	Identity GetID() { return ID; }

	// Get hit-box/rectangle of object.
	Rect& GetRect() { return *rect; }

	void IncrementFrameCount();

	AudioManager* sounds;
	SpriteManager* sprites;
	EventDispatcher* gameEvents;

	EventDispatcher* GetEventsDispatcher() { return &events; }

	int levelIndex;

protected:
	// Is the object aligned with the level "grid" (32px)
	bool isAligned();

	// Snap the object's position to the level "grid" (32px)
	void AlignToGrid();

	// Check if object is out of the level/viewport's bounds, and wrap them back around on the appropriate axis is necessary..
	void QueryBoundsPositioning();

	Identity ID = Identity::UNDEFINED;

	// How many frames the object has existed for. This should automatically be maintained by the Pacman game class using IncrementFrameCount().
	int frameCount = 0;

	// Whether the object should be deleted/destroyed at the end of the current frame.
	bool pruneFlag = false;

	// The object's hit-box/rectangle.
	Rect* rect;

	// Represents what area of a sprite to draw from when drawing the object on screen.
	Rect* spriteRect;

	// The object's position in the last frame.
	Vector2 lastPosition;

	// Each object has their own dispatcher, which allows object's to interact without knowing explicitly what their derived type or implementation is.
	// If an object recieves an irrelevant event, it will simply be dropped, granting a bit more code safety.
	EventDispatcher events;
	virtual void DispatchEvent(EventType event) {};

};

#endif