#pragma once
#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include "S2D/S2D.h"

using namespace S2D;

namespace Sprites {
	// A namespace is needed in order to prevent name clashes with Audio, which uses an identical implementation to this.
	// Enum classes ideally cannot be used as the underlying/implied value of enums is handy for array indexing in a readable way.
	enum ID {
		WALL,
		MUNCHIE,
		PLAYER,
		BONUS,
		ENEMY,
		ENEMY_SCARED,
		LEVEL_BACKGROUND,
		ENEMY_RESPAWN,
		PLAYER_DIE,
		LIVES_ICON,
		LOGO,
		MENU_LIGHTS,
		Count // Implied value is used for dynamic array sizing, NOT for referencing a particular sprite!
	};
}
class SpriteManager
{
public:
	void Initialize();
	void Destroy();

	// Return a pointer to a loaded sprite using its respective ID, meaning only 1 instance of each sprite is loaded in memory.
	Texture2D* GetSprite(Sprites::ID sprite);

private:
	// Prevent use before initialization.
	bool initFlag = false;

	// Loaded sprites, once initialized.
	Texture2D* sprites[Sprites::ID::Count];
};

#endif