#pragma once
#ifndef ID_H
#define ID_H

// Used to identify what an object is supposed to be when its derived class cannot be known.
enum class Identity
{
	UNDEFINED,
	PLAYER,
	WALL,
	MUNCHIE,
	BONUSITEM,
	ENEMY
};

#endif