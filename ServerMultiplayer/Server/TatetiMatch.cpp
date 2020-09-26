#include "TatetiMatch.h"

void TatetiMatch::show_board() {
	cout << "******" << endl;
	for (size_t y = 0; y < 3; y++) {
		cout << " " << endl;
		for (size_t x = 0; x < 3; x++) {
			cout << board[x][y];
			if (x < 2) {
				cout << " | ";
			}
		}
		cout << endl;
	}
	cout << "******" << endl;
}

bool TatetiMatch::check_win() {
	int input = input_type[turn];
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

void TatetiMatch::insert_input(char row, char column) {
	char input = input_type[turn];
	int r_row = row - '0';
	int r_col = column - '0';
	board[r_col - 1][r_row - 1] = input;
	holes--;
}

void TatetiMatch::set_players(Client *c1, Client *c2) {
	players.push_back(c1);
	players.push_back(c2);
	players_names.push_back(string(c1->get_name()));
	players_names.push_back(string(c2->get_name()));
}

bool TatetiMatch::check_input(char row, char column) {
	int r_row = row - '0';
	int r_col = column - '0';
	return board[r_col - 1][r_row - 1] == '_';
}

void TatetiMatch::set_turn(char _turn) {
	turn = _turn;
}

void TatetiMatch::set_holes(char _holes) {
	holes = _holes;
}

char TatetiMatch::get_turn() const {
	return turn;
}

char TatetiMatch::get_holes() const {
	return holes;
}

void TatetiMatch::set_last_player(Client * p) {
	last_player = p;
}

void TatetiMatch::next_turn() {
	turn++;
	turn = turn % 2;
}

Client * TatetiMatch::get_last_player() const {
	return last_player;
}

vector<Client*> TatetiMatch::get_players() {
	return players;
}

TatetiMatch::TatetiMatch() {
}


TatetiMatch::~TatetiMatch() {
}
