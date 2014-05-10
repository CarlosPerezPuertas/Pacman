#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cassert>
#include <memory>



template <typename Resource, typename EnumType>
class Resources
{
	public:
		void load(const EnumType type, const std::string filename);
		Resource& get(const EnumType type);
		const Resource& get(const EnumType type) const;

	private:
		std::map<EnumType, std::unique_ptr<Resource>> resources;
};


template <typename Resource, typename EnumType>
void Resources<typename Resource, typename EnumType>::load(const EnumType type, const  std::string filename)
{
	std::unique_ptr<Resource> new_resource(new Resource());
	assert(new_resource->loadFromFile(filename));

	//Insert in a map returns a std::pair. The second element is true when is inserted correctly
	auto insert_status = resources.insert(std::make_pair(type, std::move(new_resource)));
	assert(insert_status.second);
}

template <typename Resource, typename EnumType>
Resource& Resources<typename Resource, typename EnumType>::get(const EnumType type)
{
	auto itr = resources.find(type);
	assert(itr != resources.end());
	return *itr->second;
}

template <typename Resource, typename EnumType>
const Resource& Resources<typename Resource, typename EnumType>::get(const EnumType type) const
{
	auto itr = resources.find(type);
	assert(itr != resources.end());
	return *itr->second;
}

