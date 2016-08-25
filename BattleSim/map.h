#pragma once
#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <random>
#include <time.h>

#ifndef NUM_OF_DIMENSIONS
#define NUM_OF_DIMENSIONS 2
#endif // !NUM_OF_DIMENSIONS
#define NUM_OF_TERRAINS 2

using namespace std;

enum TileType
{
	plain,
	hill

};
class Army;
class Unit;
class Map
{
public:
	Map(int[], Army &, Army &);
	~Map();
	Map(const Map &);
	Map & operator=(const Map &);
	TileType** generateMap(int*);
	friend ostream& operator<<(ostream& out, const Map & map);
	void move(Unit, int*, int*, int);
	friend void battle(Army &, Army &, Map &);
	double strength(int,int,int,int);
	double battle(int, int, int, int);
	void applyBattleResult(int, int, Army &, Army &, double);
	void retreatLosers(int, int, Army &, Army &, double);
	friend double sumDefend(int, int, vector<Unit>, const Map &);


private:
	void updateClash();
	int size[2];
	TileType** type;
	int** modifier;
	vector <pair<Unit,int>>** units;
	bool** clash;
};
#endif // !MAP_H