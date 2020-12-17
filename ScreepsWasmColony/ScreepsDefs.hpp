#ifndef SCREEPSDEFS_HPP
#define SCREEPSDEFS_HPP
#include "constants.h"
#include "cppreeps.hpp"
#include <map>

using namespace std;

class ParsedVal
{
protected:
	ParsedVal()
	{
		
	}
	ParsedVal(const val& src)
	{
		orig = &src;
	}
	~ParsedVal() = default;

	const val* orig;
};

class Room : protected ParsedVal
{
public:
	Room() : ParsedVal() // Needed because it's the value type for WorldState::Rooms
	{
	}
	Room(val src) : ParsedVal(src)
	{
		roomName = src["name"].as<string>();
	}

	string roomName;

	val GetRoomPosition(int x, int y) const
	{
		return orig->call<val>("getPositionAt", x, y);
	}
};

class WorldState
{
public:
	static void Init(val g, val m, val rm)
	{
		Game = &g;
		Memory = &m;
		RawMemory = &rm;

		Rooms = std::map<string, Room>();
		auto rooms_map = screeps::js_object_to_map(g["rooms"]);
		for (auto const kv : rooms_map)
		{
			auto name = kv.first;
			auto v = kv.second;
			Rooms[name] = Room(v);
		}
	}

	static val* Game;
	static val* Memory;
	static val* RawMemory;

	static map<string, Room> Rooms;
};

class RoomPosition : protected ParsedVal
{
public:
	RoomPosition(const val& src) : ParsedVal(src)
	{
		roomName = src["roomName"].as<string>();
		x = src["x"].as<int>();
		y = src["y"].as<int>();

		room = &WorldState::Rooms[roomName];
	}
	RoomPosition(const int& x, const int& y, const string& roomName) : ParsedVal()
	{
		this->roomName = roomName;
		this->x = x;
		this->y = y;

		room = &WorldState::Rooms[roomName];

		auto rtn = room->GetRoomPosition(x, y);

		orig = &rtn;
	}

	string roomName;
	Room* room;
	int x;
	int y;
};

class Store : protected ParsedVal
{
public:
	Store() : ParsedVal() {}
	Store(const val& src) : ParsedVal(src)
	{
		
	}

	int GetCapacity(string res = "") const
	{
		if (res.empty())
			return orig->call<int>("getCapacity");
		return orig->call<int>("getCapacity", res);
	}

	int GetFreeCapacity(string res = "") const
	{
		if (res.empty())
			return orig->call<int>("getFreeCapacity");
		return orig->call<int>("getFreeCapacity", res);
	}

	int GetUsedCapacity(string res = "") const
	{
		if (res.empty())
			return orig->call<int>("getUsedCapacity");
		return orig->call<int>("getUsedCapacity", res);
	}

	int operator[] (const string& res) const
	{
		return GetUsedCapacity(res);
	}
};

class RoomObject : protected ParsedVal
{
public:
	RoomObject() : ParsedVal() {}
	RoomObject(const val& src) : ParsedVal(src)
	{
		auto p = RoomPosition(src["pos"]);
		pos = &p;
	}
	
	RoomPosition* pos;
};

class Structure : public RoomObject
{
public:
	Structure() : RoomObject() {}
	Structure(const val& src) : RoomObject(src)
	{
		hits = src["hits"].as<int>();
		hitsMax = src["hitsMax"].as<int>();
		id = src["id"].as<string>();
	}

	int hits;
	int hitsMax;
	string id;
};

class OwnedStructure : public Structure
{
public:
	OwnedStructure() : Structure() {}
	OwnedStructure(const val& src) : Structure(src)
	{
		my = src["my"].as<bool>();
	}

	bool my;
};

class StructureSpawn_Spawning : protected ParsedVal
{
public:
	StructureSpawn_Spawning() : ParsedVal() {}
	StructureSpawn_Spawning(const val& src) : ParsedVal(src)
	{
		name = src["name"].as<string>();
		needTime = src["needTime"].as<int>();
		remainingTime = src["remainingTime"].as<int>();
	}

	string name;
	int needTime;
	int remainingTime;
};

class StructureSpawn : public OwnedStructure
{
public:
	StructureSpawn() : OwnedStructure() {}

	StructureSpawn(const val& src) : OwnedStructure(src)
	{
		store = Store(src["store"]);
		spawning = StructureSpawn_Spawning(src["spawning"]);
	}

	Store store;
	StructureSpawn_Spawning spawning;

	int SpawnCreep(vector<string> body, string name, bool dryRun = false) const
	{
		val opts = val::object();
		opts.set("dryRun", dryRun);
		
		return orig->call<int>("spawnCreep", body, name, opts);
	}
};


#endif
