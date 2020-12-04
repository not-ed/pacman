#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "S2D/S2D.h"
#include "object.h"

class Player : public Object
{
public:
	Player(float x, float y);

private:
	void Update(int deltaTime);
	void Draw();
	void OnCollision(Object* other);

	void Kill();

	// Indicate the desired change in direction of the player
	int nextMovementDirection = 0;
	// Indicate the players current direction of movement
	int movementDirection = 0;

	int currentFrame = 0;
	int spriteFrameOffsets[6] = { 0,32,64,96,64,32 };

	// How quickly the player moves around the level
	float moveSpeed = 0.0f;

	Vector2 startingPosition;

	void DispatchEvent(EventType event);

	bool alive = true;
};

#endif