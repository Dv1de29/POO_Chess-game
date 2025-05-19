#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <map>
#include <string>
#include <memory>

template <typename Resource>
class ResourceManager {
	std::map<std::string, Resource> resources;

public:
ResourceManager() {}
~ResourceManager() {}
	std::shared_ptr<Resource> load(const std::string& id, const std::string& path) {
		auto it = resources.find(id);
		if (it == resources.end()) {
			return it->second;
		}

		auto resource = std::make_shared<Resource>();
        if ( !resource->loadFromFile(path) ) {
            throw std::runtime_error("Failed to load resource from file " + path);
        }
        resources[id] = resource;
        return resource;
	}

	std::shared_ptr<Resource> get(const std::string& id) {
		return resources.at(id);
	}
};



#endif //RESOURCE_MANAGER_H
