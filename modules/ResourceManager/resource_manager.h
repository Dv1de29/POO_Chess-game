#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <memory>

template<typename Resource>
struct ResourceLoader {
	static bool load(std::shared_ptr<Resource>& resource, const std::string& path) {
		return resource->loadFromFile(path);
	}
};

#include "SFML/Graphics.hpp"

template<>
struct ResourceLoader<sf::Font> {
	static bool load(std::shared_ptr<sf::Font>& resource, const std::string& path) {
		return resource->openFromFile(path);
	}
};

template <typename Resource>
class ResourceManager {
	std::map<std::string, std::shared_ptr<Resource>> resources;

public:
ResourceManager() = default;
~ResourceManager() = default;
	std::shared_ptr<Resource> load(const std::string& id, const std::string& path) {
		auto it = resources.find(id);
		if (it != resources.end()) {
			return it->second;
		}

		auto resource = std::make_shared<Resource>();
        if (!ResourceLoader<Resource>::load(resource, path)) {
	        throw std::runtime_error("Failed to load resource");
        }

        resources[id] = std::move(resource);
        return resources[id];
	}

	std::shared_ptr<Resource> get(const std::string& id) {
		return resources.at(id);
	}
};



#endif //RESOURCE_MANAGER_H
