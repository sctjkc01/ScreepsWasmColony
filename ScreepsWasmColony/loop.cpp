#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include "cppreeps.hpp"
#include "constants.h"

#include <iostream>

using namespace std;
using val = emscripten::val;

void loop(val Game, val Memory, val RawMemory) {
	using namespace utils;
	using namespace screeps;

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
	auto spawns_map = js_object_to_map(Game["spawns"]);
	for (auto const& kv : spawns_map) {
		auto const& name = kv.first;
		auto const& spawn = kv.second;

		cout << "Found a spawn named " << name << " with ";

		auto store = spawn["store"];
		auto held = store[RESOURCE_ENERGY].as<int>();

		cout << held << "/";

		auto capacity = store.call<int>("getCapacity", RESOURCE_ENERGY);

		cout << capacity << " energy inside!" << endl;
	}

	return;
}

EMSCRIPTEN_BINDINGS(loop) {
	emscripten::function("loop", &loop);
}