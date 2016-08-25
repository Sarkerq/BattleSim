#pragma once
#ifndef ARMY_H
#define ARMY_H
#include <string>
#include"unit.h"
#include <vector>
#include <iostream>
#include <algorithm> 
#define NUM_OF_TYPES 3
using namespace std;
class Map;
int sum(const int*, int);

class Army
{
public:
	Army(string, int[], int* = nullptr);
	~Army();
	Army(const Army &);
	int noOfUnits() const;
	int unitPosition(int, int) const;
	void move(Map &);
	void fortify();
	Army & operator=(const Army &);
	Unit  operator[](int) const; 
	Unit & operator[](int); 
	friend ostream& operator<<(ostream& out, const Army & arm);
	friend void battle(Army &, Army &, Map &);
	void update(Unit &);
	int id() const;
	string armyName() const;
private:
	int armyId;
	static int howMuch;
	int unitCount[NUM_OF_TYPES];
	Unit* units;
	int armyMorale;
	string name;
};
#endif
