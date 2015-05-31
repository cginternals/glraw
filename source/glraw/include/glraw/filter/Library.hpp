#pragma once

#include <functional>
#include <type_traits>
#include <string>
#include <map>

#include <glraw/filter/Filter.h>

class QVariantMap;

namespace glraw
{

class GLRAW_API Library
{
public:
	Library() = delete;

	typedef std::function<AbstractFilter * (const QVariantMap &)> FactoryFunction;
	typedef std::map<std::string, FactoryFunction> LibraryInstance;

	static AbstractFilter * FilterFactory(const std::string& name, const QVariantMap& options)
	{
		auto it = FilterLibrary().find(name);

		if (it == FilterLibrary().end())
		{
			return nullptr;
		}
		else
		{
			return it->second(options);
		}
	}

	template<typename Filter>
	static AbstractFilter * ConstructFilter(const QVariantMap& options)
	{
		static_assert(std::is_base_of<AbstractFilter, Filter>::value, "Filters must inherit from AbstractFilter!");
		return Filter(options);
	}

private:

	static LibraryInstance& FilterLibrary()
	{
		static LibraryInstance lib = InitializeFilterLibrary();
		return lib;
	}

	static LibraryInstance InitializeFilterLibrary()
	{
		LibraryInstance tmp;
		tmp.emplace("grayscale", &ConstructFilter<Grayscale>);
		tmp.emplace("blockblur", &ConstructFilter<BlockBlur>);

		return tmp;
	}
};

}