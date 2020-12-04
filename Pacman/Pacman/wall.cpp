#include "wall.h"

Wall::Wall(float x, float y) {
	ID = Identity::WALL;

	spriteRect = new Rect(0.0f, 0.0f, 32, 32);

	// Randomize which sub-sprite to use in the currently used row of wall sprites for that level.
	spriteRect->X = (rand() % 8) * 32;
	
	rect = new Rect(x, y, 32, 32);

}

// No code is needed in update, but as it inherits a pure virtual function from 'Object', a definition of some kind is needed here, or it would cause a compilation error.
void Wall::OnCollision(Object* other) {}
void Wall::Update(int deltaTime) {}

void Wall::Draw() {
	// Set the sprite row (wall color) based on levelIndex.
	spriteRect->Y = levelIndex * 32;
	SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::WALL), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}