#ifndef SPAWN_H
#define SPAWN_H

#include <emscripten.h>
#include <emscripten/val.h>

#include "ScreepsDefs.hpp"
using val = emscripten::val;

class Spawn
{
public:
	Spawn(const val& jsData);
	~Spawn();

	void PrintEnergy();
private:
	StructureSpawn spawnStruct;
};

#endif