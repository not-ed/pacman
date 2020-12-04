#pragma once
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "S2D/S2D.h"

using namespace S2D;

namespace Sounds {
	// A namespace is needed in order to prevent name clashes with Sprites, which uses an identical implementation to this.
	// Enum classes ideally cannot be used as the underlying/implied value of enums is handy for array indexing in a readable way.
	enum ID {
		MUNCHIE,
		BONUS,
		JINGLE,
		PLAYER_DIE,
		WALL_HIT,
		WIN,
		PAUSE,
		Count // Implied value is used for dynamic array sizing, NOT for referencing a particular sound!
	};
}

class AudioManager
{
public:
	void Initialize();
	void Destroy();

	void Play(Sounds::ID sound);

private:
	// Prevent use before initialization.
	bool initFlag = false;

	// Loaded sound files, once initialized.
	SoundEffect* sounds[Sounds::ID::Count];
};

#endif
