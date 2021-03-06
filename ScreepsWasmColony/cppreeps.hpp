/// Mildly adapated from: https://github.com/screepers/cppreeps/blob/master/include/cppreeps.hpp
#ifndef CPPREEPS_HPP
#define CPPREEPS_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <utility>

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>


#define LOG_HEAD "[cppreeps]: "

namespace utils {

	using val = emscripten::val;

	using array_native = std::vector<val>;
	using object_native = std::map<std::string, val>;


	const static val gGlobal = val::global();           // Global JS scope
	const static val gObject = val::global("Object");   // Global JS Object


	/// @returns val of global object by its name
	inline val get_global(char const* name) {
		auto rtn = val::global(name);
		if (rtn == val::null()) {
			std::cout << "Get Global " << name << " returned null!\n";
		}
		if (rtn == val::undefined()) {
			std::cout << "Get Global " << name << " returned undefined!\n";
		}
		return rtn;
	}

	/// @returns val of global object by its name
	inline val get_global(std::string const& name) {
		auto rtn = val::global(name.c_str());
		//std::cout << "Get Global " << name << std::endl;
		return rtn;
	}

	/// @returns val of global constant by its name
	template <class T>
	inline val gCONST(T&& name) {
		return val::global(std::forward<T>(name));
	}


	/// Parses given val, @returns std::vector<val>
	inline array_native js_array_to_vector(val const& arr) {
		int size = arr["length"].as<int>();

		array_native res; res.reserve(size);
		for (int i = 0; i < size; ++i)
			res.emplace_back(arr[i]);

		return res;
	}

	/// Parses given val, @returns std::map<std::string, val>
	inline object_native js_object_to_map(val const& obj) {
		val keys = gObject.call<val>("keys", obj);
		int size = keys["length"].as<int>();

		object_native res;
		for (int i = 0; i < size; ++i) {
			auto key = keys[i].as<std::string>();
			res.emplace_hint(res.end(), key, obj[key]);
		}

		return res;
	}
}

namespace screeps {

	using namespace utils;

	struct tick_t {
		val Game = get_global("Game");
		val Memory = get_global("Memory");
		val RawMemory = get_global("RawMemory");
		val PathFinder = get_global("PathFinder");

		tick_t() {
			std::cout << LOG_HEAD << "TICK " << Game["time"].as<int>() << " CONSTRUCTED\n";
		}
		~tick_t() {
			std::cout << LOG_HEAD << "TICK " << Game["time"].as<int>() << " DESTROYED\n";
		}
	};

	static std::unique_ptr<tick_t> tick;

	/// Performs recaching of global game objects,
	/// resets previous tick data reference counters, releases objects for GC
	inline void INIT() { tick.reset(new tick_t{}); }
}

#undef LOG_HEAD
#endif