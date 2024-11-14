#pragma once
#include <map>
#include <unordered_map>

class SafeMap
{
public:
	SafeMap();
	~SafeMap();

private:
	std::map<int, int> map;
	std::unordered_map<int, int> umap;
};

SafeMap::SafeMap()
{
}

SafeMap::~SafeMap()
{
}