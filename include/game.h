#pragma once

#include <string>
#include <stdexcept>
#include <curses.h>

#ifndef _CASINO_KEYS
#define _CASINO_KEYS

namespace Keys {
	static short const ENTER = 10;
	static short const RIGHT = 67;
	static short const LEFT = 68;

	static short const ONE = 49;
	static short const TWO = 50;
	static short const THREE = 51;
}

#endif

typedef struct GameData {
	std::string balance_str;
	std::string command_str;
	std::string quit_str;
	static const int input_length = 10;
	int mX;
	int mY;
} GameData;

class PlayerData {
private:
	int _balance;
	int _streak = 0;
	int initial_bet;
	int win;

public:
	PlayerData(int balance);
	bool hasAmount(int amount);

	inline void incrementStreak() {
		_streak++;
	}

	inline void discardStreak() {
		_streak = 0;
	}

	inline int getStreak() {
		return _streak;
	}

	inline void setBalance(int bal) {
		_balance = bal;
	}

	inline int getBalance() {
		return _balance;
	}
};

class Game {
public:
	Game(PlayerData *p, GameData *g);

	inline void setBalance(int val) const {
		_pdata->setBalance(val);
	}

	void setBalance();

protected:
	PlayerData *_pdata;
	GameData *_gdata;
};

namespace UI {
	void print_layout(GameData *d, PlayerData *p);
	void print_balance(GameData *d, int bal);
	void print_middle(GameData *d, std::string str);
	void clear_command_input(GameData *d);
	void clear_middle(GameData *d);
	void clear_notification(GameData *d);
	void clear_game_end_status(GameData *d);
	void notification(GameData *d, std::string str);
}
