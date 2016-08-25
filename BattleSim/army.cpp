#pragma once
#include "army.h"
#include "map.h" 
int Army::howMuch = 0;

Army::Army(string _name, int _unitCount[], int* position)
{
	howMuch++;
	armyId = howMuch;
	name = _name;
	armyMorale = 0;
	units = new Unit[sum(_unitCount, NUM_OF_TYPES)];
	int total = 0;
	for (int i = 0; i < NUM_OF_TYPES;i++)
	{
		unitCount[i] = _unitCount[i];
		for (int j = 0; j < unitCount[i]; j++)
		{
			units[total] = Unit(unitType[i],position);
			total++;
		}
	}
}
Army::Army(const Army & arm): name(arm.name), armyMorale(arm.armyMorale), armyId(arm.armyId)
{
	units = new Unit[sum(arm.unitCount, NUM_OF_TYPES)];
	int total = 0;
	for (int i = 0; i < NUM_OF_TYPES;i++)
	{
		unitCount[i] = arm.unitCount[i];
		for (int j = 0; j < unitCount[i]; j++)
		{
			units[total] = Unit(unitType[i]);
			total++;
		}
	}
}

Army::~Army()
{
	delete[] units;
}

int sum(const int* values, int amount)
{
	int sum = 0;
	for (int i = 0; i < amount; i++)
	{
		sum += values[i];
	}
	return sum;
}
int Army::noOfUnits() const
{
	return sum(unitCount, NUM_OF_TYPES);
}
int Army::unitPosition(int index, int dimension) const
{
	return units[index].showPosition(dimension);
}
ostream& operator<<(ostream& out, const Army & arm)
{
	out << arm.name << "army count: " << sum(arm.unitCount, NUM_OF_TYPES) << endl;
	out << arm.name << "army list: " << endl;
	for (int i = 0; i < sum(arm.unitCount, NUM_OF_TYPES); i++)
	{
		out << arm.units[i] << endl;
	}
	return out;
}

void Army::move( Map & map)
{
	int input;
	vector <int> moved;
	fortify();
	while (moved.size() != sum(unitCount, NUM_OF_TYPES))
	{
	
		cout << "Which unit do you want to move now?" << endl;
		cout << "Press 0 to end turn" << endl;
		cin >> input;
		if (input == 0)
		{
			moved.erase(moved.begin(), moved.end());
			break;
		}
		cout << "Where do you want to move unit " << input << "?" << endl;
		cout << units[input-1] << endl;
		int newPosition[2];
		for (int i = 0; i < NUM_OF_DIMENSIONS; i++)
		{
			cin >> newPosition[i];
		}
		if (units[input-1].move(newPosition,map,armyId))
		{
			cout << "Unit" << input << "moves to square (" << newPosition[0] << "," << newPosition[1] << ")" <<endl;
			moved.push_back(input -1);
		}
		else
			cout << "Unit" << input << "can't move to square (" << newPosition[0] << "," << newPosition[1] << ")" << endl;
	}
}
void Army::fortify()
{
	for ( int i = 0; i < sum(unitCount, NUM_OF_TYPES); i++)
	{
		units[i].fortify();
	}
	
}
Unit & Army::operator[](int i)
{
	return units[i];
}
void Army::update(Unit & uni)
{
	for (int i = 0; i < noOfUnits();i++)
	{
		if (uni == units[i]) units[i] = uni;
	}
}
int Army::id() const
{
	return armyId;
}
string Army::armyName() const
{
	return name;
}
Unit  Army::operator[](int i) const
{
	return units[i];
}

void battleResult(int height, int width, int strength, int result, string att)
{
	if (result > 0) cout << att << " has won a battle of the tile (" << height << ',' << width << ") with result : " << result << endl;
	if (result > 0) cout << att << " has list a battle of the tile (" << height << ',' << width << ") with result : " << abs(result) << endl;

}

void battle(Army & att, Army & def, Map & map)
{
	map.updateClash();
	for (int i = 0; i < map.size[0]; i++)
	{
		for (int j = 0; j < map.size[1]; j++)
		{
			if (map.clash[i][j])
			{
				double strength = map.strength(i, j, att.armyId, def.armyId);
				double result = map.battle(i, j, att.armyId,def.armyId);
				map.applyBattleResult(i, j, att, def, result);
				map.retreatLosers(i, j, att, def, result);
				battleResult(i , j ,strength, result, att.name);
			}
		}
	}

}