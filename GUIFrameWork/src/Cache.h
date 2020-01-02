#pragma once
#include "_Texture2D.h"
#include <memory>
#include <unordered_map>
#include <string>

class Window;

template<typename Key, typename Value>
class Cache
{
	Window& window;
	std::unordered_map<Key, std::weak_ptr<Value>> cache;
public:
	Cache(Window& window) :window(window){}
	~Cache() {};

	std::shared_ptr<Value> CreateBlank(const Key& key) {
		std::shared_ptr<Value> value = cache[key].lock();
		cache[key] = value = std::make_shared<_Texture2D>(cache.find(key)->first, nullptr);
		return value;
	}
	std::shared_ptr<Value> Get(const Key& key) {
		std::shared_ptr<Value> value = cache[key].lock();
		if (!value)
			cache[key] = value = std::make_shared<Value>(cache.find(key)->first, &window);
		return value;
	}

	bool isLoaded(const Key& key) {
		return (bool)cache[key].lock();
	}

};



