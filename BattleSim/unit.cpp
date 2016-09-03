#pragma once
#include "unit.h"
#include "map.h"
int Unit::howMuch = 0;

Unit::Unit(UnitType _type, int* _position) : hit_points(100), morale(0), fortification(0), exp(0), type(_type)
{

	if (_position)
	{
		howMuch++;
		unitId = howMuch;
		for (int i = 0; i < NUM_OF_DIMENSIONS; i++)
		{
			position[i] = _position[i];
		}
	}
	else
	{
		position[0] = 0;
		position[1] = 0;
	}
	switch (type)
	{
	case infantry:
		attack = 10;
		defence = 10;
		movement = 2;
		break;
	case cavalry:
		attack = 15;
		defence = 5;
		movement = 3;
		break;
	case archery:
		attack = 5;
		defence = 15;
		movement = 1;
		break;
	default:
		break;
	}

}

Unit::~Unit()
{
}
ostream& operator<<(ostream& out, const Unit & unit)
{
	out << unit.unitId << ' ' << unit.type << ' ' << unit.hit_points << " (" << unit.position[0] << ',' << unit.position[1]
		<< ") " << unit.morale << ' ' << unit.exp << ' ' << unit.fortification;
	return out;
}

bool operator==(const Unit & u1, const Unit & u2)
{
	return u1.unitId == u2.unitId;
}

int Unit::showPosition(int dimension) const
{
	return position[dimension];
}
void Unit::getWinnerHurt()
{
	hit_points -= 5;
}
void Unit::getLoserHurt()
{
	hit_points -= 10;
}
void Unit::gainExp()
{
	exp++;
}
void Unit::gainMorale()
{
	morale += 5;
}
void Unit::loseMorale()
{
	morale -= 5;
}
bool Unit::move(int* target, Map & map, int armyId)
{
	if (abs(position[0] - target[0]) + abs(position[1] - target[1]) <= movement)
	{
		int orgPosition[] = { position[0],position[1] };
		fortification = 0;
		morale-= abs(position[0] - target[0]) + abs(position[1] - target[1]);
		position[0] = target[0];
		position[1] = target[1];
		map.move(*this, orgPosition, target, armyId);
		return true;
	}
	return false;
}
void Unit::fortify()
{
	if (type != cavalry && fortification < 5)
	{
		fortification++;
	}
}