
#include "game.h"

Game::Game(PlayerData *p, GameData *g) : _pdata(p), _gdata(g) {
	_gdata->balance_str = "BALANCE:";
	_gdata->command_str = "COMMAND:";
	_gdata->quit_str = "[q] quit";
}

void Game::setBalance() {

	int amnt;
	int maxAmountOfCharacters = 5;
	char tmp_amnt[maxAmountOfCharacters];

	UI::print_middle(_gdata, "AMOUNT: ");
	
	getnstr(tmp_amnt, maxAmountOfCharacters);
	std::string bet_amount(tmp_amnt);

	try {
		amnt = std::stoi(bet_amount);
	} catch(std::invalid_argument) {
		// Print error message.
		UI::print_middle(_gdata, "INVALID INPUT");
		return;
	}

	_pdata->setBalance(_pdata->getBalance() + amnt);
	UI::print_balance(_gdata, _pdata->getBalance());

	UI::clear_middle(_gdata);
}

PlayerData::PlayerData(int balance) : _balance(balance) {
}

bool PlayerData::hasAmount(int value) {
	return _balance >= value;
}

void UI::clear_game_end_status(GameData *d) {
	mvprintw((d->mY/2)-3, (d->mX/2) / 10, "                               ");
	mvprintw((d->mY/2)-3, (d->mX/2) + (d->mX/2) / 10, "                               ");
}

void UI::clear_command_input(GameData *d) {
	// Print out the command string again is needed for terminals that does not support backspaces.
	mvprintw(d->mY-1, 0, d->command_str.c_str());

	// Clear the input.
	for(int pos = 0; pos < d->input_length; pos++) mvprintw(d->mY-1, d->command_str.size() + pos, " ");

	// Move the cursor to accurate position.
	move(d->mY-1, d->command_str.size());
}

void UI::print_balance(GameData *d, int value) {
	// Clear the last balance for any overflow characters.
	mvprintw(d->mY-1, d->command_str.size() + 13 + d->balance_str.size(), "      ");

	// Print out the current balance.
	mvprintw(d->mY-1, d->command_str.size() + 13 + d->balance_str.size(), std::to_string(value).c_str());
}

void UI::print_middle(GameData *d, std::string str) {
	// Clears 30 characters from the middle of the status bar.
	mvprintw(d->mY-1, (d->mX/2)-15, "                              ");
	mvprintw(d->mY-1, (d->mX/2) - (str.size()/2), str.c_str());
}

void UI::clear_middle(GameData *d) {
	// Clears 30 characters from the middle of the status bar.
	mvprintw(d->mY-1, (d->mX/2)-15, "                              ");
}

void UI::clear_notification(GameData *d) {
	// Clear any previous message displayed on the notification tab.
	mvprintw(d->mY-1, d->mX-20, "                    ");
}

void UI::notification(GameData *d, std::string str) {

	// Clear any previous message.
	clear_notification(d);

	// Minus 1 extra for padding right.
	mvprintw(d->mY-1, d->mX - str.length() - 1, str.c_str());
}

void UI::print_layout(GameData *d, PlayerData *p) {

	// Vertical screen borders.
	for(int i = 0; i < (d->mY-2)/2; i++) {
		mvprintw(i, (d->mX)/2, "|");
	}

	// Horizontal screen borders.
	for(int i = 0; i < d->mX; i++) {
		mvprintw((d->mY-2)/2, i, "-");
	}

	// + sign in the middle of the screen.
	mvprintw((d->mY-2)/2, d->mX/2, "+");

	// Command/info bar border.
	for(int i = 0; i < d->mX; i++) {
		mvprintw(d->mY-2, i, "=");
	}

	// "COMMAND:" string.
	mvprintw(d->mY-1, 0, d->command_str.c_str());

	// "BALANCE:" string.
	mvprintw(d->mY-1, d->command_str.size() + 12, d->balance_str.c_str());

	// Sub-window information tags.
	mvprintw(0, 1, "[r] roulette");
	mvprintw(0, d->mX/2+2, "[s] slots");
	mvprintw(d->mY/2, 1, "[c] casino");
}
