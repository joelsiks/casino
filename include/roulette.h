#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <random>

#include <unistd.h>

#include "game.h"

class Roulette : public Game {
private:
	std::vector<int> m_slotList;
	int m_row;
	int m_square_max_length;
	int m_current_choice;
	int m_current_bet;
	void changeOption(int[]);
	void spin(int, int);
	void print();
	void end_game(PlayerData*);

public:
	Roulette(const Game &g);
	void init();
	void start_game();
};

