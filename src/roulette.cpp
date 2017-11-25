
#include "game.h"
#include "roulette.h"

static int calc_roulettepos(int maxlen, int slots_size) {
	return (maxlen - (slots_size * 3)) / 2;
}

Roulette::Roulette(const Game &g) : Game(g) {}

void Roulette::changeOption(int arr[]) {
	for(int i = 0; i < sizeof(*arr); i++) {
		mvprintw(_gdata->mY-1, arr[i], " ");
	}

	mvprintw(_gdata->mY-1, arr[m_current_choice], "X");
	UI::clear_command_input(_gdata);
}

void Roulette::spin(int nspins, int sec) {
	for(int i = 0; i < nspins; i++) {
		UI::clear_command_input(_gdata);
		refresh();
		usleep(sec * 1000);
		int tmp = m_slotList[0];
		m_slotList.erase(m_slotList.begin());
		m_slotList.push_back(tmp);
		print();
	}
}

void Roulette::init() {
	int row_pos = (_gdata->mY/4) - 1;
	int len = _gdata->mX;

	m_slotList = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};

	m_row = row_pos;
	m_square_max_length = len/2;

	print();
	mvprintw(m_row+1, ((m_square_max_length - (m_slotList.size() * 3)) / 2) + (m_slotList.size() * 3) / 2, "^");
}

void Roulette::print() {

	int xpos = calc_roulettepos(m_square_max_length, m_slotList.size());

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);

	for(int i = 0; i < m_slotList.size(); i++) {
		if(m_slotList[i] < 10) {
			if(m_slotList[i] % 2) {
				attron(COLOR_PAIR(1));
				mvprintw(m_row, xpos, "0%d", m_slotList[i]);
				attroff(COLOR_PAIR(1));
			} else {
				attron(COLOR_PAIR(2));
				mvprintw(m_row, xpos, "0%d", m_slotList[i]);
				attroff(COLOR_PAIR(2));
			} 
		} else {
			if(m_slotList[i] % 2) {
				attron(COLOR_PAIR(1));
				mvprintw(m_row, xpos, "%d", m_slotList[i]);
				attroff(COLOR_PAIR(1));
			} else {
				attron(COLOR_PAIR(2));
				mvprintw(m_row, xpos, "%d", m_slotList[i]);
				attroff(COLOR_PAIR(2));
			}
		}

		if(m_slotList[i] == 0) {
			attron(COLOR_PAIR(3));
			mvprintw(m_row, xpos, "0%d", m_slotList[i]);
			attroff(COLOR_PAIR(3));
		}

		xpos += 3;
	}
}

void Roulette::start_game() {

	// Initializing rng.
	std::random_device rd;
	std::mt19937 rnd(rd());
	std::uniform_int_distribution<int> random_int(20, 40);

	int amnt;
	char tmp_amnt[80];
	std::string options_str = "[ ] RED [ ] BLACK [ ] GREEN";
	m_current_choice = 1;

	// Clear the end status from roulette and slots.
	UI::clear_game_end_status(_gdata);

	UI::print_middle(_gdata, "BET: ");
	
	getnstr(tmp_amnt, 5);
	std::string bet_amount(tmp_amnt);

	try {
		amnt = std::stoi(bet_amount);
	} catch(std::invalid_argument) {
		UI::print_middle(_gdata, "INVALID INPUT");
		return;
	}

	if(!_pdata->hasAmount(amnt)) {
		UI::print_middle(_gdata, "NOT ENOUGH MONEY");
		return;
	}

	// Update current balance and output it to screen.
	_pdata->setBalance(_pdata->getBalance() - amnt);
	UI::print_balance(_gdata, _pdata->getBalance());

	// Sets member variable for reference later.
	m_current_bet = amnt;

	// Prints out options that are available.
	UI::print_middle(_gdata, options_str);
	UI::clear_command_input(_gdata);

	int keyboard_value = 0; // The holder for the pressed key value.
	int POS1 = ((_gdata->mX/2) - (options_str.length()/2))+1; // Needs to be seperate from array to prevent error.
	int positions[3] = {POS1, _gdata->mX/2 - 4, _gdata->mX/2 + 6}; // Positions on screen for the 'X's.

	// Output the standard selected value.
	mvprintw(_gdata->mY-1, positions[m_current_choice], "X");
	UI::clear_command_input(_gdata);

	while(keyboard_value != Keys::ENTER) {
		keyboard_value = getch();

		switch(keyboard_value) {
			case Keys::LEFT:
				if(m_current_choice != 0) m_current_choice -= 1;
				Roulette::changeOption(positions);
				break;

			case Keys::RIGHT:
				if(m_current_choice != 2) m_current_choice += 1;
				Roulette::changeOption(positions);
				break;

			case Keys::ONE:
				m_current_choice = 0;
				Roulette::changeOption(positions);
				break;

			case Keys::TWO:
				m_current_choice = 1;
				Roulette::changeOption(positions);
				break;

			case Keys::THREE:
				m_current_choice = 2;
				Roulette::changeOption(positions);
				break;

			default:
				UI::clear_command_input(_gdata);
				break;
		}
	}

	// Clear the middle screen of the selection menu.
	UI::clear_middle(_gdata);

	int spins = random_int(rnd);

	// Hardcoded initial spins to fill out the game.
	Roulette::spin(20, 90);
	Roulette::spin(2, 180);

	for(int i = spins; i > 0; i--) {
		if(i > 13)
			Roulette::spin(1, 200);
		else if(i > 10)
			Roulette::spin(1, 240);
		else if(i > 6)
			Roulette::spin(1, 280);
		else if(i > 4)
			Roulette::spin(1, 320);
		else 
			Roulette::spin(1, 640);
	}

	Roulette::end_game(_pdata);
}

void Roulette::end_game(PlayerData *p) {

	UI::clear_command_input(_gdata);

	if(m_slotList[11] % 2 && m_current_choice == 1) { // BLACK
		p->setBalance(p->getBalance() + (m_current_bet * 2));
		p->incrementStreak();
		mvprintw((_gdata->mY/2)-3, (_gdata->mX/2) / 10, "LANDED ON: %d - YOU WON!", m_slotList[11]);
	} else if(!(m_slotList[11] % 2) && m_current_choice == 0) { // RED
		p->setBalance(p->getBalance() + (m_current_bet * 2));
		p->incrementStreak();
		mvprintw((_gdata->mY/2)-3, (_gdata->mX/2) / 10, "LANDED ON: %d - YOU WON!", m_slotList[11]);
	} else if(m_slotList[11] == 0 && m_current_choice == 2) { // GREEN
		p->setBalance(p->getBalance() + (m_current_bet * 8));
		p->incrementStreak();
		mvprintw((_gdata->mY/2)-3, (_gdata->mX/2) / 10, "LANDED ON: %d - YOU WON!", m_slotList[11]);
	} else {
		p->discardStreak();
		mvprintw((_gdata->mY/2)-3, (_gdata->mX/2) / 10, "LANDED ON: %d - You lost.", m_slotList[11]);
	}

	UI::notification(_gdata, "WIN STREAK: " + std::to_string(p->getStreak()));
	UI::print_balance(_gdata, p->getBalance());
	UI::clear_command_input(_gdata);

	m_current_choice = 1;
	m_current_bet = 0;
}
