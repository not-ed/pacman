#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
// Vectors (Dynamic Arrays)
#include <vector>
// Strings and String Streams
#include <string>
#include <sstream>
// File Handling
#include <iostream>
#include <fstream>
// AudioManager / Sound Effects
#include "audiomanager.h"
// Scores
#include "score.h"
// Event Dispatches
#include "events.h"
// Object
#include "object.h"
// Player
#include "player.h"
// Munchie
#include "munchie.h"
// Wall
#include "wall.h"
// Bonus Item
#include "bonusitem.h"
// Enemy
#include "enemy.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Whether the game is paused or not.
	bool paused;
	// If pause key is currently being held down. Prevents the pause menu toggling every single frame for the few frames the user is bound to hold the key down when using it.
	bool pauseKeyDown;

	// All Objects that currently exist (Player, munchies, enemies etc)
	std::vector<Object*> existingObjects;
	void CreateObject(Object* obj);

	AudioManager sounds;
	SpriteManager sprites;

	ScoreCounter score;

	// The game instances own EventDispatcher, which allows other objects to communicate with it without causing a circular dependency.
	EventDispatcher events;
	void DispatchEvent(EventType event);

	// Level data file paths.
	const std::string LEVELS[4]{
		"Levels/lvl1.dat",
		"Levels/lvl2.dat",
		"Levels/lvl3.dat",
		"Levels/lvl4.dat"
	};

	// Build a level from one of the available level files.
	void LoadLevel(std::string level_path);

	bool LevelIsCompleted();

	// A number found in a level's file to indicate an index for something like the wall sprite's colors for that level etc.
	int levelIndex;

	// How many levels the player has beaten in a single run.
	int completedLevels = 0;
	
	// Indicator of the current state of the game.
	enum class GameState {
		START_MENU, // The 'press space' screen.
		STARTUP, // Interstitial / 'Get Ready' screen
		RUNNING, // In active gameplay
		WON, // Player level win sequence
		RESPAWN, // Interstital before player is placed back into gameplay after dying.
		DEAD, // Player death sequence
		GAME_OVER // 'Game over' screen
	};

	GameState currentState = GameState::START_MENU;

	// How much time in milliseconds the game has been running, this is reset whenever the GameState is changed and is for context-dependent use.
	int runTime = 0;

	// Switch the game state and reset 'runTime' to zero.
	void SwitchGameState(GameState state);

	// How many lives the player has left
	int remainingLives = 3;

	// Reset game-specific values such as lives, score etc. in preparation for a new run from the player.
	void ResetGame();

	// Suffixes for referring to scores contained within the high score table.
	const string SCORE_SUFFIXES[10] = {
			"1ST",
			"2ND",
			"3RD",
			"4TH",
			"5TH",
			"6TH",
			"7TH",
			"8TH",
			"9TH",
			"10TH"
	};

	// Colors for drawing text on the high score table.
	const Color SCORE_COLORS[10] = {
			Color((1.0 / 255) * 16,(1.0 / 255) * 192,(1.0 / 255) * 200),
			Color((1.0 / 255) * 248,(1.0 / 255) * 168,(1.0 / 255) * 48),
			Color((1.0 / 255) * 248,(1.0 / 255) * 168,(1.0 / 255) * 48),
			Color((1.0 / 255) * 248,(1.0 / 255) * 168,(1.0 / 255) * 48),
			Color((1.0 / 255) * 192,(1.0 / 255) * 192,(1.0 / 255) * 192),
			Color((1.0 / 255) * 192,(1.0 / 255) * 192,(1.0 / 255) * 192),
			Color((1.0 / 255) * 192,(1.0 / 255) * 192,(1.0 / 255) * 192),
			Color((1.0 / 255) * 192,(1.0 / 255) * 72,(1.0 / 255) * 0),
			Color((1.0 / 255) * 192,(1.0 / 255) * 72,(1.0 / 255) * 0),
			Color((1.0 / 255) * 192,(1.0 / 255) * 72,(1.0 / 255) * 0)
	};

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

};