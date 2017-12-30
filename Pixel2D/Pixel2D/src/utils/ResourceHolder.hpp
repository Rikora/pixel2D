#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include <assert.h>

//Template class for all kinds of different resources
template<typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void LoadResource(Identifier id, const std::string & filename);
	template<typename Parameter>
	void LoadResource(Identifier id, const std::string & filename, const Parameter & secondParam);

public:
	Resource & GetResource(Identifier id);
	const Resource & GetResource(Identifier id) const;

private:
	void InsertResource(Identifier id, std::unique_ptr<Resource> & resource);

private:
	std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

template<typename Resource, typename Identifier>
inline void ResourceHolder<Resource, Identifier>::LoadResource(Identifier id, const std::string & filename)
{
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::LoadResource - Failed to load" + filename);

	InsertResource(id, resource);
}

template<typename Resource, typename Identifier>
template<typename Parameter>
inline void ResourceHolder<Resource, Identifier>::LoadResource(Identifier id, const std::string & filename, const Parameter & secondParam)
{
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename, secondParam))
		throw std::runtime_error("ResourceHolder::LoadResource - Failed to load" + filename);

	InsertResource(id, resource);
}

template<typename Resource, typename Identifier>
inline Resource & ResourceHolder<Resource, Identifier>::GetResource(Identifier id)
{
	auto found = m_resourceMap.find(id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template<typename Resource, typename Identifier>
inline const Resource & ResourceHolder<Resource, Identifier>::GetResource(Identifier id) const
{
	auto found = m_resourceMap.find(id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template<typename Resource, typename Identifier>
inline void ResourceHolder<Resource, Identifier>::InsertResource(Identifier id, std::unique_ptr<Resource> & resource)
{
	auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}
