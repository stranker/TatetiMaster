#pragma once
#include "Client.h"
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

class TatetiMatch {
private:
	vector<Client*> players;
	vector<string> players_names;
	int turn = 0;
	char board[3][3];
	char holes = 9;
	char input_type[2] = { 'X','O' };
public:
	void init();
	void show_board();
	bool check(char input);
	void insert_input(char input);
	void set_players(Client *c1, Client *c2);
	TatetiMatch();
	~TatetiMatch();
};

