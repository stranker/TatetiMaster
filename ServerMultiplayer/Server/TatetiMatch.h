#pragma once

#include "Client.h"
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

class TatetiMatch {
private:
	vector<Client*> players;
	Client* last_player;
	vector<string> players_names;
	int turn = 0;
	char board[3][3] = { {'_','_','_'}, {'_','_','_'}, {'_','_','_'} };
	int holes = 9;
	char input_type[2] = { 'X','O' };
public:
	void show_board();
	void set_players(Client *c1, Client *c2);
	bool check_input(char row, char column);
	void insert_input(char row, char column);
	bool check_win();
	void set_turn(char _turn);
	void set_holes(char _holes);
	char get_turn() const;
	char get_holes() const;
	void set_last_player(Client* p);
	void next_turn();
	Client * get_last_player() const;
	vector<Client*> get_players();
	TatetiMatch();
	~TatetiMatch();
};

