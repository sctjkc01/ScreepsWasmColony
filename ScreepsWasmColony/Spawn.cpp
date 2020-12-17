#include "Spawn.h"

#include <iostream>


Spawn::Spawn(const val& jsData)
{
	spawnStruct = StructureSpawn(jsData);
}


Spawn::~Spawn()
{
}

void Spawn::PrintEnergy()
{
	cout << spawnStruct.store["energy"] << "/" << spawnStruct.store.GetCapacity("energy") << " energy";
}