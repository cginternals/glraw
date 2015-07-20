#pragma once

#include <functional>
#include <type_traits>
#include <string>
#include <map>

#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

typedef std::function<AbstractFilter * (const QVariantMap &)> FactoryFunction;
typedef std::map<std::string, FactoryFunction> LibraryInstance;

class GLRAW_API Filter
{
public:
	Filter() = delete;

	static AbstractFilter * CreateByName(const std::string & name, const QVariantMap& options)
	{
		auto it = Library().find(name);

		if(it == Library().end())
		{
			return nullptr;
		}
		else
		{
			return it->second(options);
		}
	}

	template<typename FilterType>
	static AbstractFilter * Factory(const QVariantMap& options)
	{
		static_assert(std::is_base_of<AbstractFilter, FilterType>::value, "Filters must inherit from AbstractFilter!");
		return new FilterType(options);
	}

	template<typename FilterType>
	static void Add(const std::string & name)
	{
		instance.emplace(name, &Factory<FilterType>);
	}

	static bool Exists(const std::string & name)
	{
		return Library().find(name) != Library().end();
	}

	static std::vector<std::string> All()
	{
		std::vector<std::string> output;

		for(auto entry : Library())
		{
			output.push_back(entry.first);
		}
		return output;
	}

	static LibraryInstance & Library()
	{
		static bool dummy = InitializeLibrary();
		return instance;
	}

private:

	static bool InitializeLibrary();

	static LibraryInstance instance;
};

}