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

	static AbstractFilter * CreateByName(const std::string& name, const QVariantMap& options)
	{
		auto it = s_library.find(name);

		if (it == s_library.end())
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

private:

	static LibraryInstance InitializeLibrary();

	static LibraryInstance s_library;
};

}