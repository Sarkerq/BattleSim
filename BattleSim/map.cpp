#include "map.h"
#include "army.h"

double sumAttack(vector<Unit> attackers)
{
	double sum = 0;
	for (vector<Unit>::iterator it = attackers.begin(); it != attackers.end(); ++it)
	{
		double power = 0;
		//basic value
		power += it->attack;
		// unit modifiers
		power *= (1 + 0.01 * it->morale);
		power *= (1 + 0.1 * it->exp);
		power *= (0.01 * it->hit_points);

		sum += power;
	}
	return sum;
}

double sumDefend(int height, int width, vector<Unit> defenders, const Map & map)
{
	double sum = 0;
	for (vector<Unit>::iterator it = defenders.begin(); it != defenders.end(); ++it)
	{
		double power = 0;
		//basic value
		power += it->defence;
		// unit modifiers
		power *= (1 + 0.01 * it->morale);
		power *= (1 + 0.1 * it->exp);
		power *= (0.01 * it->hit_points);
		power *= (0.1 * it->fortification);
		//tile modifiers
		power *= (1 + 0.01 * map.modifier[height][width]);


		sum += power;
	}
	return sum;
}

Map::Map(int _size[], Army & army1, Army & army2)
{

	for (int i = 0; i < NUM_OF_DIMENSIONS; i++)
	{
		size[i] = _size[i];
	}

	// size[0] - height of the map
	// size[1] - width of the map

	modifier = new int*[size[0]];
	units = new vector<pair<Unit,int>>*[size[0]];
	clash = new bool*[size[0]];
	for (int i = 0; i < size[0]; i++)
	{
		modifier[i] = new int[size[1]];
		units[i] = new vector<pair<Unit,int>>[size[1]];
		clash[i] = new bool[size[1]];
	}
	type = generateMap(size);
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			clash[i][j] = false;
			switch (type[i][j])
			{
			case plain:
				modifier[i][j] = 0;
				break;
			case hill:
				modifier[i][j] = 50;
				break;
			default:
				break;
			}
			for (int k = 0; k < army1.noOfUnits(); k++)
			{
				if (army1.unitPosition(k, 0) == i && army1.unitPosition(k, 1) == j)
				{
					pair <Unit, int> pair(army1[k], army1.id());
					units[i][j].push_back(pair);
				}
			}
			for (int k = 0; k < army2.noOfUnits(); k++)
			{
				if (army2.unitPosition(k, 0) == i && army2.unitPosition(k, 1) == j)
				{
					pair <Unit, int> pair(army2[k], army2.id());
					units[i][j].push_back(pair);
				}
			}
		}
		
	}
}

Map::Map(const Map & map)
{

	for (int i = 0; i < NUM_OF_DIMENSIONS; i++)
	{
		size[i] = map.size[i];
	}

	// size[0] - height of the map
	// size[1] - width of the map

	modifier = new int*[size[0]];
	for (int i = 0; i < size[0]; i++)
	{
		modifier[i] = new int[size[1]];
	}
	type = new TileType*[size[0]];
	for (int i = 0; i < size[0]; i++)
	{
		type[i] = new TileType[size[1]];
	}
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			type[i][j] = map.type[i][j];
		}
	}

	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			switch (type[i][j])
			{
			case plain:
				modifier[i][j] = 0;
				break;
			case hill:
				modifier[i][j] = 50;
				break;
			default:
				break;
			}
		}

	}

}

Map::~Map()
{
	for (int i = 0; i < size[0]; i++)
	{
		delete[] type[i];
		delete[] modifier[i];
		delete[] units[i];
		delete[] clash[i];
	}
	delete[] type;
	delete[] modifier;
	delete[] units;
	delete[] clash;
}
ostream& operator<<(ostream& out, const Map & map)
{
	for (int i = 0; i < map.size[0]; i++)
	{
		for (int j = 0; j < map.size[1]; j++)
		{
			if (map.units[i][j].size() == 0)
			{
				if (map.type[i][j] == plain)
				{
					out << '\'';
				}
				if (map.type[i][j] == hill)
				{
					out << '^';
				}
			}
			if (map.units[i][j].size() >= 9) out << '9';
			if (map.units[i][j].size() < 9 && map.units[i][j].size() > 0) out << map.units[i][j].size();
		}
		out << endl;
	}
	return out;
}
TileType** Map::generateMap(int* size)
{
	srand(time(NULL));
	type = new TileType*[size[0]];
	for (int i = 0; i < size[0]; i++)
	{
		type[i] = new TileType[size[1]];
	}
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			if (rand() % NUM_OF_TERRAINS == 0) type[i][j] = plain;
			else type[i][j] = hill;
		}
	}
	return type;

}

void Map::move(Unit uni, int *position, int *target, int armyId)
{
	for (vector<pair<Unit, int>>::iterator it = units[position[0]][position[1]].begin(); it != units[position[0]][position[1]].end(); ++it)
	{
		if (uni == it->first)
		{
			units[position[0]][position[1]].erase(it);
			pair<Unit, int> pair(uni, armyId);
			units[target[0]][target[1]].push_back(pair);
			break;
		}
	}
}

double Map::strength(int height, int width, int attId, int defId)
{
	vector<Unit> attackers;
	vector<Unit> defenders;
	for (vector<pair<Unit, int>>::iterator it = units[height][width].begin(); it != units[height][width].end(); ++it)
	{
		if (it->second == attId) attackers.push_back(it->first);
		if (it->second == defId) defenders.push_back(it->first);
	}
	double attack = sumAttack(attackers);
	double defend = sumDefend(height, width, defenders, *this);
	return attack + defend;
}

double Map::battle(int height, int width, int attId, int defId)
{
	vector<Unit> attackers;
	vector<Unit> defenders;
	for (vector<pair<Unit, int>>::iterator it = units[height][width].begin(); it != units[height][width].end(); ++it)
	{
		if (it->second == attId) attackers.push_back(it->first);
		if (it->second == defId) defenders.push_back(it->first);
	}
	double attack = sumAttack(attackers);
	double defend = sumDefend(height, width, defenders, *this);
	return attack - defend;
}

void Map::applyBattleResult(int height, int width, Army & att, Army & def, double result)
{

	for (vector<pair<Unit, int>>::iterator it = units[height][width].begin(); it != units[height][width].end(); ++it)
	{

		if (it->second == att.id())
		{
			it->first.gainExp();
			it->first.getWinnerHurt();
			if (result > 0)	it->first.gainMorale();
			else it->first.loseMorale();
			att.update(it->first);
		}
		if (it->second == def.id())
		{
			it->first.gainExp();
			it->first.getWinnerHurt();
			if (result > 0)	it->first.loseMorale();
			else it->first.gainMorale();
			def.update(it->first);
		}
	}
	
}
void Map::retreatLosers(int height, int width, Army & att, Army & def, double result)
{
	if (result > 0) cout << "Commander of army " << def.armyName() << ", where do you want to retreat your units?" << endl;
	else cout << "Commander of army " << att.armyName() << ", where do you want to retreat your units?" << endl;
	for (vector<pair<Unit, int>>::iterator it = units[height][width].begin(); it != units[height][width].end(); ++it)
	{

		if ((it->second == att.id() && result <= 0) || (it->second == def.id() && result > 0))
		{
			cout << it->first << endl;
			int position[2];
			for (int i = 0; i < NUM_OF_DIMENSIONS; i++)
			{
				cin >> position[i];
			}
			it->first.move(position, *this, it->second);
			if (it->second == att.id()) att.update(it->first);
			else def.update(it->first);
		}
	}

}
void Map::updateClash()
{
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			bool army1Presence = false;
			bool army2Presence = false;
			for (vector<pair<Unit, int>>::iterator it = units[i][j].begin(); it != units[i][j].end(); ++it)
			{
				// static values kept for now:
				// 1 - army1
				// 2 - army2
				if (it->second == 1) army1Presence = true;
				if (it->second == 2) army2Presence = true;
				clash[i][j] = army1Presence&army2Presence;
			}
		}

	}
}
