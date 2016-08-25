#pragma once
#include "army.h"
#include "map.h"
#include <iostream>
#define NUM_OF_PLAYERS 2
using namespace std;
void nextTurn(int &);
int main()
{
	cout << "-----BATTLESIM!-----" << endl;
	int mapSize[2];
	cout << "Choose map size: " << endl;
	for (int i = 0; i < NUM_OF_DIMENSIONS; i++)
	{
		cin >> mapSize[i];
	}
	int armyCount[NUM_OF_PLAYERS][NUM_OF_TYPES];
	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		cout << "Player" << i << "army:" << endl;
		for (int j = 0; j < NUM_OF_TYPES; j++)
		{
			cout << "Amount of " << unitType[j] << " : ";
			cin >> armyCount[i][j];
		}
	}
	// Placeholders instead of name and position choosing 
	int USAPosition[] = { 0,0 };
	int USSRPosition[] = { mapSize[0] - 1 , mapSize[1] - 1 };
	Army army1("USA", armyCount[0], USAPosition), army2("USSR", armyCount[1], USSRPosition);
	Map map(mapSize,army1,army2);
	int turn = 1;
	while (true)
	{
		cout << "Game turn: " << turn << endl;
		cout << map << army1 << army2;
		army1.move(map);
		battle(army1, army2, map);
		army2.move(map);
		battle(army2, army1, map);
		nextTurn(turn);
		getchar();

	}
	return 0;
}

void nextTurn(int & turn)
{
	turn++;
}

