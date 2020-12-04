#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "S2D/S2D.h"
#include "object.h"
#include <time.h>

// Indicates what state the enemy is currently in.
enum class BehaviourState {
	ROAMING, // Default state - travelling level at random intervals.
	SCARED, // Bonus item is active - increased speed and killable by the player.
	RESPAWNING // Brief "Wait" state before respawning after being caught by the player when scared.
};

class Enemy : public Object
{
public:
	Enemy(float x, float y, int color_index);

private:
	void Update(int deltaTime);
	void Draw();
	void OnCollision(Object* other);

	// Handle recieved events, if any.
	void DispatchEvent(EventType event);

	// Keeping  track of what state the enemy is currently in.
	BehaviourState currentState = BehaviourState::ROAMING;

	// How many spaces the enemy has travelled in the same direction in one go.
	int spacesTravelled = 0;

	// Switch the enemy's current direction to a random one.
	void SwitchRandomDirection();

	// The enemy's initial position when placed into the level, which is used in cases such as respawning.
	Vector2 startingPosition;

	// Counter for keeping track of elapsed time inside the ghost's current state.
	int stateTime = 0;
	// Switch the enemy's behaviour state, and reset stateTime counter.
	void SwitchState(BehaviourState state);

	// What number of index to use to indicate the enemy's sprite color.
	int colorIndex = 0;

	// What current direction the enemy is currently moving in.
	int currentDirection = 0;
	
};

#endif