#pragma once
#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <iostream>
#include <fstream>

class ScoreCounter
{
public:
	// Add 'amount' to currentScore.
	void Add(int amount);

	void Reset() { currentScore = 0; };

	int GetScore() { return currentScore; }

	// Read high-scores from file. Should only ever need to be called once at startup.
	void PullHighScores();

	// Submit a score for submission into the high-score array and file. If a score has been beaten, it will be added into the list and lower score positions will be adjusted accordingly.
	void SubmitHighScore(int amount);

	// Get the score held within a specific high-score index.
	int GetHighScore(int index);

private:
	int currentScore = 0;

	// File path for the file containing high score amounts.
	const std::string HIGH_SCORES_PATH = "Scores/HIGHSCORE.DAT";

	// Initialized values used in generation/display if not enough high-scores exist, or if a fresh file has to be generated.
	int highScores[10] = {0,0,0,0,0,0,0,0,0,0};

};

#endif