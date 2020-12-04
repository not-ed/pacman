#include "Pacman.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	paused = false;
	pauseKeyDown = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 480, 640, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	Audio::Initialise();
	// Member AudioManager and SpriteManagers.
	sounds.Initialize();
	sprites.Initialize();
	
	// Pull high scores from file.
	score.PullHighScores();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

void Pacman::CreateObject(Object* object) {
	// Add the created object onto the existingObjects vector.
	existingObjects.push_back(object);

	// Pass address/location of SpriteManagers, AudioManagers, the game instance's EventDispatcher etc.
	object->sounds = &sounds;
	object->sprites = &sprites;
	object->levelIndex = levelIndex;
	object->gameEvents = &events;
}

void Pacman::LoadLevel(string level_path) {
	// Get rid of all existing objects from previous level (if any) first.
	existingObjects.clear();

	// Read-only file (in-file, as in reading an external source of data IN to the program).
	std::ifstream level_data(level_path);

	// If the level_file is not able to load for some reason, then the game will exit. 
	// Unlike high score submission for example, the game kind of needs a level.
	if (!level_data.is_open()) {
		exit(1);
	}

	// Update level index from file.
	level_data >> levelIndex;

	// Used to indicate how many objects have been read and placed into a row of the level being built.
	// A level is (15x20 tiles)
	int counter = 0;
	
	// Where to place the next object being placed.
	Vector2 wand(0, 0);

	// While there is still data in the file to read.
	while (!level_data.eof()) {
		// The next character (or "tile") being read from the file.
		char tile;
		level_data >> tile;

		if (tile == '.') { // Place a Munchie.
			CreateObject(new Munchie(wand.X + 10, wand.Y + 10));
		}
		else if (tile == '#') { // Place a Wall.
			CreateObject(new Wall(wand.X, wand.Y));
		}

		// Move the position of the next placed object along 32 pixels, or 1 tile's width.
		wand.X += 32;
		counter++;

		// If the "wand" has been moved 15 spaces (Meaning it is time to start a new row of objects to place).
		if (counter % 15 == 0) {
			wand.X = 0;
			
			// Move "wand" down 1 tile's height, or 32 pixels to start a new row.
			wand.Y += 32;
		}

		// If 20 15-tile-wide rows have been created/read, then any other data being read is more than is needed.
		if (counter >= 20*15) {
			break;
		}
	}
	level_data.close();

	// Create an enemy of each color in each corner of the level.
	CreateObject(new Enemy(1 * 32, 1 * 32,0));
	CreateObject(new Enemy(13 * 32, 1 * 32,1));
	CreateObject(new Enemy(1 * 32, 18 * 32,2));
	CreateObject(new Enemy(13 * 32, 18 * 32,3));

	// Create the player
	CreateObject(new Player(7 * 32, 10 * 32));

	// Create "Safety Walls" 
	// Placed on exits out of view bounds to prevent the unlikely chance that an enemy or player gets stuck if they change direction at the edge and travel out of bounds.
	CreateObject(new Wall(-1 * 32, 8 * 32));
	CreateObject(new Wall(-2*32,9*32));
	CreateObject(new Wall(-1 * 32, 10 * 32));
	CreateObject(new Wall(15 * 32, 8 * 32));
	CreateObject(new Wall(16 *32, 9 * 32));
	CreateObject(new Wall(15 * 32, 10 * 32));

	// Switch to and perform level startup sequence/state.
	SwitchGameState(GameState::STARTUP);
}

Pacman::~Pacman()
{
	Graphics::Destroy();
	Input::Destroy();
	Audio::Destroy();
	sounds.Destroy();
	sprites.Destroy();
}

void Pacman::LoadContent()
{
	// Most game content, such as sprites and audio are handled in seperate classes, so this function is not needed.
	// A definition, however, does still need to exist here, otherwise a compilation error will occur.
}

void Pacman::SwitchGameState(GameState state) {
	switch (state)
	{
	case GameState::START_MENU:
		ResetGame();
		break;

	case GameState::STARTUP:
		sounds.Play(Sounds::ID::JINGLE);
		paused = false;
		break;

	case GameState::WON:
		sounds.Play(Sounds::ID::WIN);
		paused = false;
		break;

	case GameState::DEAD:
		remainingLives--;
		break;

	case GameState::RESPAWN:
		// Tell each player and enemy object to return to their starting positions.
		for (Object* obj : existingObjects) {
			if (obj->GetID() == Identity::ENEMY || obj->GetID() == Identity::PLAYER) {
				obj->GetEventsDispatcher()->SendEvent(EventType::POSITION_RESET);
			}
		}

		paused = false;
		break;

	case GameState::GAME_OVER:
		// Submit score at end of game.
		score.SubmitHighScore(score.GetScore());
		break;
	}

	runTime = 0;
	currentState = state;
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard.
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	switch (currentState)
	{
	case GameState::START_MENU:
		if (keyboardState->IsKeyDown(Input::Keys::SPACE)) {
			// Load the first level, and start a new run.
			LoadLevel(LEVELS[completedLevels % 4]);
			SwitchGameState(GameState::STARTUP);
		}
		break;

	case GameState::STARTUP:
		// Display startup sequence for 4 seconds.
		if (runTime >= 4*1000) {
			SwitchGameState(GameState::RUNNING);
		}
		break;

	case GameState::RUNNING:
		// Pause menu input and toggling.
		if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !pauseKeyDown) {
			paused = !paused;
			pauseKeyDown = true;
			sounds.Play(Sounds::ID::PAUSE);
		}
		if (keyboardState->IsKeyUp(Input::Keys::ESCAPE)) {
			pauseKeyDown = false;
		}

		if (!paused) {
			// Update all existing Objects
			for (Object* obj : existingObjects) {
				obj->Update(elapsedTime);
				obj->IncrementFrameCount();
			}

			// Query Object Collisions
			for (Object* obj : existingObjects) {
				for (Object* other_obj : existingObjects) {
					obj->QueryCollision(other_obj);
				}
			}

			// Dispatch object events, if any at all.
			for (Object* obj : existingObjects) {
				obj->HandleEvents();
			}

			// Dispatch own Events, if any have been recieved.
			for (EventType event: events.pendingEvents)
			{
				DispatchEvent(event);
			}
			events.ClearEvents();
		}

		// Delete any objects marked for pruning.
		for (int i = 0; i < existingObjects.size(); i++)
		{
			if (existingObjects[i]->MarkedForPruning()) {
				delete existingObjects[i];
				existingObjects.erase(existingObjects.begin() + i);
			}
		}

		// Spawn a bonus item every 45 seconds in the player's starting position.
		if (runTime >= 45*1000) { 
			CreateObject(new BonusItem(7 * 32, 10 * 32));
			runTime = 0;
		}

		// If all munchies inside a level have been collected.
		if (LevelIsCompleted()) {
			SwitchGameState(GameState::WON);
		}
		break;

	case GameState::WON:
		// Display the win sequence for 4.25 seconds before loading and transitioning to the next level.
		if (runTime >= 4.25*1000) {
			completedLevels++;
			LoadLevel(LEVELS[completedLevels % 4]);
			SwitchGameState(GameState::STARTUP);
		}
		break;

	case GameState::RESPAWN:
		// Dispatch object events, if any at all.
		for (Object* m : existingObjects) {
			m->HandleEvents();
		}

		// Display the respawn sequence for 3 seconds before giving control back to the player.
		if (runTime >= 3*1000) {
			SwitchGameState(GameState::RUNNING);
		}
		break;

	case GameState::DEAD:
		// Increment ONLY the player's frame count for the sake of playing their death animation.
		for (Object* obj : existingObjects) {
			if (obj->GetID() == Identity::PLAYER) {
				obj->IncrementFrameCount();
			}
		}

		// Display death sequence for 3.5 seconds.
		if (runTime >= 3.5 * 1000) {
			// End game if lost last life, or continue game if there is still one left.
			if (remainingLives <= 0) {
				SwitchGameState(GameState::GAME_OVER);
			}
			else {
				SwitchGameState(GameState::RESPAWN);
			}
		}
		break;

	case GameState::GAME_OVER:
		// Display game over sequence for 3.5 seconds before returning to the main menu.
		if (runTime >= 3.5 * 1000) {
			SwitchGameState(GameState::START_MENU);
		}
		break;
	}

	if (!paused) {
		runTime += elapsedTime;
	}
}

void Pacman::DispatchEvent(EventType event) {
	switch (event)
	{
	case EventType::MUNCHIE_COLLECTED:
		// Score multiplier is 1(.1 x Beaten Levels).
		score.Add(100 * (1+(0.1*completedLevels)));
		break;
	case EventType::BONUS_COLLECTED:
		for (Object* obj : existingObjects) {
			obj->GetEventsDispatcher()->SendEvent(EventType::BONUS_COLLECTED);
		}
		break;
	case EventType::GHOST_CAUGHT:
		sounds.Play(Sounds::ID::BONUS);
		score.Add(2500);
		break;
	case EventType::PLAYER_CAUGHT:
		// Delete any bonus items, if any.
		for (int i = 0; i < existingObjects.size(); i++)
		{
			if (existingObjects[i]->GetID() == Identity::BONUSITEM) {
				delete existingObjects[i];
				existingObjects.erase(existingObjects.begin() + i);
			}
		}
		SwitchGameState(GameState::DEAD);
		
		break;
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();
	
	// Draw Main menu, if in that state.
	if (currentState == GameState::START_MENU) {
		SpriteBatch::DrawRectangle(&Rect(0, 0, Graphics::GetViewportWidth(), Graphics::GetViewportHeight()), Color::Black);

		// Draw Logo
		SpriteBatch::Draw(sprites.GetSprite(Sprites::ID::LOGO), &Rect(0, 71, 480, 69));

		// Draw high score border.
		{
			// Alternate each sprite's frame based on time to create a blinking light effect.
			int time = (runTime / (1000 / 3)) % 2;

			for (int i = 0; i < 11; i++)
			{
				for (int ii = 0; ii < 13; ii++)
				{
					// Only draw sprite at the edges, creating a border.
					if ((i == 0 || i == 11 - 1) || (ii == 0 || ii == 13 - 1)) {
						SpriteBatch::Draw(sprites.GetSprite(Sprites::ID::MENU_LIGHTS), &Rect(64 + (i * 32), 153 + (ii * 32), 32, 32), &Rect((time + (i + ii) % 2) * 32, 0, 32, 32), Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
					}
				}
			}
		}

		SpriteBatch::DrawString("HIGH SCORES", &Vector2(176, 185 + 24), &SCORE_COLORS[runTime / 50 % 10]);

		// Draw high score values.
		for (int i = 0; i < 10; i++)
		{
			// Draw relevant suffix (1st, 2nd, 3rd, etc).
			SpriteBatch::DrawString(SCORE_SUFFIXES[i].c_str(), &Vector2(109, 185 + 13 + 40 + (i * 32)), &SCORE_COLORS[i]);

			// Draw held score value.
			stringstream score_i;
			score_i << score.GetHighScore(i);
			SpriteBatch::DrawString(score_i.str().c_str(), &Vector2(109 + 161, 185 + 13 + 40 + (i * 32)), &SCORE_COLORS[i]);
		}

		// Draw blinking "Press Space" prompt.
		if ((runTime / 500) % 2 == 0) {
			SpriteBatch::DrawString("Press SPACE to play", &Vector2((Graphics::GetViewportHeight() / 2) - 166, Graphics::GetViewportHeight() - 48), Color::White);
		}
	}
	else { // Otherwise, draw the game as normal, as if it isn't in the menu, then it is running.
		//Draw background tiles to fill the screen.
		for (int i = 0; i < Graphics::GetViewportWidth() / 32; i++)
		{
			for (int ii = 0; ii < Graphics::GetViewportHeight() / 32; ii++)
			{
				Rect tile = Rect(i * 32, ii * 32, 32, 32);

				// Pick the background based on the level index. (e.g blue walls = blue floors).
				Rect spr = Rect(0, levelIndex * 32, 32, 32);

				SpriteBatch::Draw(sprites.GetSprite(Sprites::ID::LEVEL_BACKGROUND), &tile, &spr);
			}
		}

		// Draw existing objects.
		for (Object* obj : existingObjects) {
			obj->Draw();
		}

		// Draw "Get Ready!" message.
		if (currentState == GameState::STARTUP || currentState == GameState::RESPAWN) {
			SpriteBatch::DrawRectangle(&Rect(160, 290, 156, 28), Color::Black);
			SpriteBatch::DrawString("GET READY!", &Vector2(160 + 24, 288 + 23), Color::Yellow);
		}

		// Draw Score Counter
		{
			// Backdrop/box
			SpriteBatch::DrawRectangle(&Rect(260, 4, 184, 24), Color::Black);

			// Text/numbers
			std::stringstream scoreStream;
			scoreStream << score.GetScore();
			SpriteBatch::DrawString(scoreStream.str().c_str(), new Vector2((8 * 32) + 8, 22), Color::White);
		}

		//Draw remaining lives
		{
			Rect life_icon_box(196, 612, 23, 23);

			for (int i = 0; i < 3; i++)
			{
				SpriteBatch::DrawRectangle(&life_icon_box, Color::Black);

				if (i < remainingLives) {
					SpriteBatch::Draw(sprites.GetSprite(Sprites::ID::LIVES_ICON), &life_icon_box);
				}

				life_icon_box.X += 32;
			}
		}

		// Draw level counter
		{
			Rect lvl_counter(36, 4, 184, 24);
			std::stringstream lvl_stream;
			lvl_stream << "LEVEL " << completedLevels + 1;
			SpriteBatch::DrawRectangle(&lvl_counter, Color::Black);
			SpriteBatch::DrawString(lvl_stream.str().c_str(), new Vector2((1 * 32) + 8, 22), Color::White);
		}

		// Draw 'Game Over' screen, if relevant.
		if (currentState == GameState::GAME_OVER) {
			SpriteBatch::DrawRectangle(&Rect(160, 290, 156, 28), Color::Black);
			SpriteBatch::DrawString("GAME OVER", new Vector2(160 + 24, 288 + 23), Color::Red);
		}

		// Drawing the pause menu.
		if (paused) {
			SpriteBatch::DrawRectangle(&Rect(0, 0, Graphics::GetViewportWidth(), Graphics::GetViewportHeight()), &Color(0, 0, 0, 0.8));
			SpriteBatch::DrawString("PAUSED", &Vector2((Graphics::GetViewportWidth() / 2) - 36, (Graphics::GetViewportHeight() / 2) - 8), Color::White);
		}
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}

bool Pacman::LevelIsCompleted() {
	bool munchieFound = false;

	for (Object* m : existingObjects) {
		if (m->GetID() == Identity::MUNCHIE) {
			munchieFound = true;
			break;
		}
	}

	return !munchieFound;
}

void Pacman::ResetGame() {
	remainingLives = 3;
	score.Reset();
	existingObjects.clear();
	completedLevels = 0;
}