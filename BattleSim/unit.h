#pragma once
#ifndef UNIT_H
#define UNIT_H

#ifndef NUM_OF_DIMENSIONS
#define NUM_OF_DIMENSIONS 2
#endif // !NUM_OF_DIMENSIONS
#include <string>
#include <iostream>
#include <vector>
using namespace std;
class Map;
enum UnitType
{
	infantry,
	cavalry,
	archery
};
const UnitType unitType[] =
{
	infantry,
	cavalry,
	archery
};
class Unit
{
public:
	Unit(UnitType = infantry, int* = nullptr);
	~Unit();
	bool move(int*, Map &, int);
	void fortify();
	int showPosition(int) const;
	friend ostream& operator<<(ostream& out, const Unit & unit);
	friend bool operator==(const Unit & u1, const Unit & u2);
	static int howMuch;
	friend double sumAttack(vector<Unit>);
	friend double sumDefend(int, int, vector<Unit>, const Map &);
	void getWinnerHurt();
	void getLoserHurt();
	void gainExp();
	void gainMorale();
	void loseMorale();

private:
	int unitId;
	int position[NUM_OF_DIMENSIONS];
	int hit_points;
	UnitType type;
	int attack;
	int defence;
	int movement;
	int morale;
	int exp;
	int fortification;
};

#endif // !UNIT_H