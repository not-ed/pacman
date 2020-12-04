#include "audiomanager.h"

void AudioManager::Initialize() {
	if (!initFlag) {
		sounds[Sounds::ID::MUNCHIE] = new SoundEffect();
		sounds[Sounds::ID::MUNCHIE]->Load("Sounds/munchie.wav");

		sounds[Sounds::ID::BONUS] = new SoundEffect();
		sounds[Sounds::ID::BONUS]->Load("Sounds/bonus.wav");

		sounds[Sounds::ID::JINGLE] = new SoundEffect();
		sounds[Sounds::ID::JINGLE]->Load("Sounds/jingle.wav");

		sounds[Sounds::ID::PLAYER_DIE] = new SoundEffect();
		sounds[Sounds::ID::PLAYER_DIE]->Load("Sounds/playerdie.wav");

		sounds[Sounds::ID::WALL_HIT] = new SoundEffect();
		sounds[Sounds::ID::WALL_HIT]->Load("Sounds/wallhit.wav");

		sounds[Sounds::ID::WIN] = new SoundEffect();
		sounds[Sounds::ID::WIN]->Load("Sounds/win.wav");

		sounds[Sounds::ID::PAUSE] = new SoundEffect();
		sounds[Sounds::ID::PAUSE]->Load("Sounds/pause.wav");

		initFlag = true;
	}
}

void AudioManager::Destroy() {
	if (initFlag) {
		for (int i = 0; i < Sounds::ID::Count; i++)
		{
			delete sounds[i];
		}
	}
}

void AudioManager::Play(Sounds::ID sound) {
	if (initFlag) {

		// Randomize the munchie sound's pitch whenever it is played to reduce repetition.
		if (sound == Sounds::ID::MUNCHIE) {
			sounds[sound]->SetPitch(1.0 + (0.75 / ((rand()%25)+1)));
		}

		Audio::Play(sounds[sound]);
	}
}