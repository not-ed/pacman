#include "score.h"

void ScoreCounter::Add(int amount) {
	currentScore += amount;
}

void ScoreCounter::PullHighScores() {
	// Read-only file (in-file, as in reading an external source of data IN to the program).
	std::ifstream scores(HIGH_SCORES_PATH);

	// If the file isn't opening for whatever reason, then the program should still continue gracefully.
	if (scores.is_open()) {
		int scores_read = 0;

		// Read the whole list of scores or the first 10, whichever comes first.
		while(!scores.eof() && scores_read < 10)
		{
			scores >> highScores[scores_read];
			scores_read++;
		}

		scores.close();
	}
}

int ScoreCounter::GetHighScore(int index) {
	if (index >= 0 && index <= 9) {
		return highScores[index];
	}
	else return 0;
}

void ScoreCounter::SubmitHighScore(int amount) {
	// Theres no need to do this at all if an empty score has been submitted.
	if (amount != 0) {
		for (int i = 0; i < 10; i++)
		{
			if (amount > highScores[i]) {
				// Push lower scores down the table from where the new high score is being placed.
				for (int ii = 9; ii > i; ii--)
				{
					highScores[ii] = highScores[ii - 1];
				}
				highScores[i] = amount;
				break;
			}
		}

		// Out-file (as in writing data to an external source OUTSIDE of the program).
		// The entire high score file's contents will be overwritten with the highScores array
		// This should be fine as long as it has been loaded from at game start.
		std::ofstream scores(HIGH_SCORES_PATH);

		// If the file isn't opening for whatever reason and isn't able to make a fresh one if it doesn't exist, then the program should still continue gracefully.
		if (scores.is_open()) {
			//Write the 10 highest scores to the file, line-by-line.
			for (int i = 0; i < 10; i++)
			{
				scores << highScores[i] << "\n";
			}
			scores.close();
		}
	}
}