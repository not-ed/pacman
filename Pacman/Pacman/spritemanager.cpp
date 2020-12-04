#include "spritemanager.h"

void SpriteManager::Initialize() {
	if (!initFlag) {

		sprites[Sprites::ID::WALL] = new Texture2D();
		sprites[Sprites::ID::WALL]->Load("Textures/walls.tga", false);

		sprites[Sprites::ID::MUNCHIE] = new Texture2D();
		sprites[Sprites::ID::MUNCHIE]->Load("Textures/munchie.tga", false);

		sprites[Sprites::ID::PLAYER] = new Texture2D();
		sprites[Sprites::ID::PLAYER]->Load("Textures/pacman.tga", false);

		sprites[Sprites::ID::BONUS] = new Texture2D();
		sprites[Sprites::ID::BONUS]->Load("Textures/bonus.png", false);

		sprites[Sprites::ID::ENEMY] = new Texture2D();
		sprites[Sprites::ID::ENEMY]->Load("Textures/ghosts.png", false);

		sprites[Sprites::ID::ENEMY_SCARED] = new Texture2D();
		sprites[Sprites::ID::ENEMY_SCARED]->Load("Textures/ghost_scared.png", false);

		sprites[Sprites::ID::LEVEL_BACKGROUND] = new Texture2D();
		sprites[Sprites::ID::LEVEL_BACKGROUND]->Load("Textures/background.tga", false);

		sprites[Sprites::ID::ENEMY_RESPAWN] = new Texture2D();
		sprites[Sprites::ID::ENEMY_RESPAWN]->Load("Textures/ghost_respawning.png", false);

		sprites[Sprites::ID::PLAYER_DIE] = new Texture2D();
		sprites[Sprites::ID::PLAYER_DIE]->Load("Textures/playerdie.png", false);

		sprites[Sprites::ID::LIVES_ICON] = new Texture2D();
		sprites[Sprites::ID::LIVES_ICON]->Load("Textures/lives_icon.png", false);

		sprites[Sprites::ID::LOGO] = new Texture2D();
		sprites[Sprites::ID::LOGO]->Load("Textures/logo.png", false);

		sprites[Sprites::ID::MENU_LIGHTS] = new Texture2D();
		sprites[Sprites::ID::MENU_LIGHTS]->Load("Textures/menu_lights.png", false);

		initFlag = true;
	}
}

void SpriteManager::Destroy() {
	if (initFlag) {
		for (int i = 0; i < Sprites::ID::Count; i++)
		{
			delete sprites[i];
		}
	}
}

Texture2D* SpriteManager::GetSprite(Sprites::ID sprite) {
	if (initFlag) {
		return sprites[sprite];
	}
}