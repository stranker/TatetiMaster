#include "TatetiMatch.h"



void TatetiMatch::init() {
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			board[x][y] = '_';
		}
	}
}

void TatetiMatch::show_board() {
	for (size_t y = 0; y < 3; y++) {
		for (size_t x = 0; x < 3; x++) {
			cout << board[x][y];
			if (x < 2) {
				cout << " | ";
			}
		}
		cout << endl;
	}
}

bool TatetiMatch::check(char input) {
	bool retval = false;

	for (int a = 0; a < 3; a++) {
		if (board[0][a] == input && board[0][a] == board[1][a] && board[1][a] == board[2][a]) {
			retval = true;
			break;
		}
	}

	for (int a = 0; a < 3; a++) {
		if (board[a][0] == input && board[a][0] == board[a][1] && board[a][1] == board[a][2]) {
			retval = true;
			break;
		}
	}

	if (board[0][0] == input && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
		retval = true;
	}

	if (board[0][2] == input && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
		retval = true;
	}
	return retval;
}

void TatetiMatch::insert_input(char input) {
	int x, y;

	do {
		cout << "turno de " << input << endl << "Cordenada X:";
		cin >> x;
		cout << "Cordenada Y:";
		cin >> y;
		if (board[x - 1][y - 1] != '_')
			cout << "Posicion ocupada, intentelo de nuevo" << endl;
	} while (x < 1 || x > 3 || y < 1 || y > 3 || board[x - 1][y - 1] != '_');

	board[x - 1][y - 1] = input;
	holes--;
}

void TatetiMatch::set_players(Client *c1, Client *c2) {
	players.push_back(c1);
	players.push_back(c2);
	players_names.push_back(string(c1->get_name()));
	players_names.push_back(string(c2->get_name()));
}

TatetiMatch::TatetiMatch() {
	init();
	show_board();
}


TatetiMatch::~TatetiMatch() {
}
