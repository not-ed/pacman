#include "munchie.h"

Munchie::Munchie(float x, float y) {

	ID = Identity::MUNCHIE;

	spriteRect = new Rect(0.0f, 0.0f, 12, 12);

	rect = new Rect(x,y,12,12);
}

// No code is needed in update, but as it inherits a pure virtual function from 'Object', a definition of some kind is needed here, or it would cause a compilation error.
void Munchie::Update(int deltaTime) {

}

void Munchie::Draw() {
	// Blink sprite frame/color every 16 frames.
	if (frameCount > 32) {
		frameCount = 0;
	}

	spriteRect->X = 0 + (12 * (frameCount > 16));

	SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::MUNCHIE), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

void Munchie::OnCollision(Object* other) {
	if (other->GetID() == Identity::PLAYER) {
		if (!pruneFlag) {
			pruneFlag = true;

			sounds->Play(Sounds::ID::MUNCHIE);

			gameEvents->SendEvent(EventType::MUNCHIE_COLLECTED);
		}
	}
}