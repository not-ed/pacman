#include "bonusitem.h"

BonusItem::BonusItem(float x, float y) {
	ID = Identity::BONUSITEM;
	rect = new Rect(x,y,32,32);
	spriteRect = new Rect(0.0f, (rand()%5)*32, 32, 32);
}

void BonusItem::Update(int deltaTime) {
	spriteRect->X = ((frameCount/8) % 2) * 32;

	remainingTime -= deltaTime;

	// Vanish if not picked up by the player in time.
	if (remainingTime <= 0) {
		pruneFlag = true;
	}
}

void BonusItem::Draw() {
	SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::BONUS), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	
	// Draw Timer
	SpriteBatch::DrawRectangle(&Rect(rect->X+4,rect->Y-28,24,24),Color::Black);

	int remaining_time_int = int(remainingTime / 1000) + 1;
	std::stringstream time_txt;
	time_txt << remaining_time_int;

	// Timer text centering
	if (remaining_time_int >= 10) {
		SpriteBatch::DrawString(time_txt.str().c_str(),&Vector2(rect->X + 5,rect->Y-10),Color::White); // 2 Digits
	}
	else {
		SpriteBatch::DrawString(time_txt.str().c_str(), &Vector2(rect->X + 11, rect->Y - 10), Color::White); // 1 Digit
	}
}

void BonusItem::OnCollision(Object* other) {
	if (other->GetID() == Identity::PLAYER) {
		pruneFlag = true;

		sounds->Play(Sounds::ID::BONUS);

		gameEvents->SendEvent(EventType::BONUS_COLLECTED);
	}
}