#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include "Spawn.h"

#include <iostream>

using namespace std;

void loop(val Game, val Memory, val RawMemory) {
	//using namespace utils;
	//using namespace screeps;

	cout << "hello world" << endl;

	if (Game == val::undefined() || Game == val::null()) {
		cerr << "Game is undefined or null!\n";
	}
	if (Memory == val::undefined() || Memory == val::null()) {
		cerr << "Memory is undefined or null!\n";
	}
	if (RawMemory == val::undefined() || RawMemory == val::null()) {
		cerr << "RawMemory is undefined or null!\n";
	}

	WorldState::Init(Game, Memory, RawMemory);
	
	
	auto spawns_map = screeps::js_object_to_map(Game["spawns"]);
	for (auto const kv : spawns_map) {
		auto name = kv.first;
		auto spawn = kv.second;

		Spawn s(kv.second);

		cout << "Found a spawn named " << name << " with ";

		s.PrintEnergy();

		cout << " inside!" << endl;
	}

	return;
}

EMSCRIPTEN_BINDINGS(loop) {
	emscripten::function("loop", &loop);
}