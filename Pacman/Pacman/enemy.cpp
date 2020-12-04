#include "enemy.h"

Enemy::Enemy(float x, float y, int color_index) {
	// Set psuedo-RNG seed based on current time.
	srand(time(NULL));

	ID = Identity::ENEMY;

	spriteRect = new Rect(0.0f, 0.0f, 32, 32);

	rect = new Rect(x, y, 32, 32);

	startingPosition.X = rect->X;
	startingPosition.Y = rect->Y;

	colorIndex = color_index;
}

void Enemy::Update(int deltaTime) {
	// In respawn state.
	if (currentState == BehaviourState::RESPAWNING) {
		// Return to starting position.
		rect->X = startingPosition.X;
		rect->Y = startingPosition.Y;

		// Return back to default roaming state after 8 seconds.
		if (stateTime >= 8*1000) {
			SwitchState(BehaviourState::ROAMING);
		}
	}
	else { // In either roaming or scared state.
		bool scared = (currentState == BehaviourState::SCARED);

		if (scared) {
			// Go out of scared state after 15 seconds.
			if (stateTime >= 15 * 1000) {
				SwitchState(BehaviourState::ROAMING);
			}
		}

		// Move in current direction.
		switch (currentDirection)
		{
		case 0: // Left
			rect->X -= (0.1 + (.075 * scared)) * deltaTime;
			break;
		case 1: // Right
			rect->X += (0.1 + (.075 * scared)) * deltaTime;
			break;
		case 2: // Up
			rect->Y -= (0.1 + (.075 * scared)) * deltaTime;
			break;
		case 3: // Down
			rect->Y += (0.1 + (.075 * scared)) * deltaTime;
			break;
		}

		QueryBoundsPositioning();

		if (isAligned()) {
			spacesTravelled++;

			// If enemy has travelled more than 6 spaces in the same direction.
			// Intended more for long stretches that may be in a level, so that an enemy isn't endlessly travelling through the same area repeatedly.
			if (spacesTravelled > 6) {
				SwitchRandomDirection();
			}
		}
	}

	// Increment state time counter by how much time (in miliseconds) since the last frame.
	stateTime += deltaTime;
}

void Enemy::Draw() {
		switch (currentState)
		{
		case BehaviourState::ROAMING:
			spriteRect->X = ((frameCount / 6) % 2) * 32;

			// Get appropriate color
			spriteRect->Y = colorIndex * 32;

			SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::ENEMY), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			break;
		case BehaviourState::SCARED:
			spriteRect->X = ((frameCount / 6) % 2) * 32;

			// The ghost will blink/flash in the last 5 seconds of its scared state to warn the player.
			spriteRect->Y = 32 * (stateTime > 10 * 1000 && (frameCount / 4) % 2 == 0);

			SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::ENEMY_SCARED), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			break;
		case BehaviourState::RESPAWNING:
			// Blink/flash different sprites in the last 3 seconds of to warn player of pending respawn.
			if (stateTime >= 5*1000 && (frameCount / 6) % 2 == 0) {
				spriteRect->X = ((frameCount / 6) % 2) * 32;

				// Get appropriate color
				spriteRect->Y = colorIndex * 32;

				SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::ENEMY), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
			else {
				spriteRect->X = ((frameCount / 6) % 2) * 32;
				spriteRect->Y = 0;

				SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::ENEMY_RESPAWN), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
			break;
		}
}

void Enemy::OnCollision(Object* other) {
	if (other->GetID() == Identity::PLAYER) {
		// Caught by player
		if (currentState == BehaviourState::SCARED) {
			gameEvents->SendEvent(EventType::GHOST_CAUGHT);
			SwitchState(BehaviourState::RESPAWNING);
		}
		else if (currentState == BehaviourState::ROAMING) { // Caught player
			other->GetEventsDispatcher()->SendEvent(EventType::PLAYER_CAUGHT);
			gameEvents->SendEvent(EventType::PLAYER_CAUGHT);
		}
	}

	if (other->GetID() == Identity::WALL) {
		if (currentState != BehaviourState::RESPAWNING) {
			AlignToGrid();
			SwitchRandomDirection();
		}
	}
}

void Enemy::SwitchRandomDirection() {
	currentDirection = (currentDirection + rand()) % 4;
	spacesTravelled = 0;
}

void Enemy::DispatchEvent(EventType event) {
	if (event == EventType::BONUS_COLLECTED) {
		// Ghosts cannot be scared again while they are waiting to respawn.
		if (currentState != BehaviourState::RESPAWNING) {
			SwitchState(BehaviourState::SCARED);
		}
	}
	if (event == EventType::POSITION_RESET) {
		// Return to starting position.
		rect->X = startingPosition.X;
		rect->Y = startingPosition.Y;

		SwitchState(BehaviourState::ROAMING);
	}
}

void Enemy::SwitchState(BehaviourState state) {
	currentState = state;
	stateTime = 0;
}