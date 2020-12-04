#include "player.h"

Player::Player(float x, float y) {

	ID = Identity::PLAYER;

	spriteRect = new Rect(0.0f, 0.0f, 32, 32);

	rect = new Rect(x, y, 32, 32);
	
	startingPosition.X = rect->X;
	startingPosition.Y = rect->Y;
}

void Player::Update(int deltaTime) {
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (alive) {
		// Update last position before doing anything else.
		lastPosition = Vector2(rect->X, rect->Y);

		// Update next direction number based on input
		if (keyboardState->IsKeyDown(Input::Keys::A) && movementDirection != 2) {
			if (movementDirection == 0) {
				movementDirection = 2;
				moveSpeed = 0.1f;
			}
			nextMovementDirection = 2;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::D) && movementDirection != 0) {
			if (movementDirection == 2) {
				movementDirection = 0;
				moveSpeed = 0.1f;
			}
			nextMovementDirection = 0;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::W) && movementDirection != 3) {
			if (movementDirection == 1) {
				movementDirection = 3;
				moveSpeed = 0.1f;
			}
			nextMovementDirection = 3;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S) && movementDirection != 1) {
			if (movementDirection == 3) {
				movementDirection = 1;
				moveSpeed = 0.1f;
			}
			nextMovementDirection = 1;
		}

		//Switch movement direciton if aligned to the level "grid".
		if (isAligned()) {
			movementDirection = nextMovementDirection;
			AlignToGrid();
			moveSpeed = 0.1f;
		}

		switch (movementDirection)
		{
		case 0: // Right
			rect->X += (1 * moveSpeed) * deltaTime;
			break;
		case 1: // Down
			rect->Y += (1 * moveSpeed) * deltaTime;
			break;
		case 2: // Left
			rect->X += (-1 * moveSpeed) * deltaTime;
			break;
		case 3: // Up
			rect->Y += (-1 * moveSpeed) * deltaTime;
			break;
		}

		QueryBoundsPositioning();
	}
}

void Player::Draw() {
	if (alive) {
		// Set sprite to appropriate row based on direction.
		spriteRect->Y = movementDirection * 32;

		if (frameCount > 3) {
			frameCount = 0;
			currentFrame++;
			spriteRect->X = (currentFrame % 4) * 32;
		}

		SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::PLAYER), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else { // Death animation frames.
		if (frameCount > 10 && currentFrame < 12) {
			frameCount = 0;
			currentFrame++;
			spriteRect->X = currentFrame * 32;
		}

		SpriteBatch::Draw(sprites->GetSprite(Sprites::ID::PLAYER_DIE), rect, spriteRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
}

void Player::OnCollision(Object* other) {
	if (other->GetID() == Identity::WALL) {
		moveSpeed = 0.0f;
		AlignToGrid();
	}
}

void Player::Kill() {
	if (alive) {
		sounds->Play(Sounds::ID::PLAYER_DIE);
		alive = false;

		// Reset and prepare animation frames to prepare for player death animation.
		currentFrame = 0;
		frameCount = 0;
		spriteRect->X = 0;
		spriteRect->Y = 0;
	}
}

void Player::DispatchEvent(EventType event){
	if (event == EventType::PLAYER_CAUGHT) {
		Kill();
	}
	if (event == EventType::POSITION_RESET) { //Respawn
		alive = true;

		//Return to starting position
		rect->X = startingPosition.X;
		rect->Y = startingPosition.Y;

		// Reset animation
		spriteRect->X = 0;
		spriteRect->Y = 0;
	}
}