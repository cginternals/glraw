#include <glraw/filter/Filter.hpp>
#include <glraw/filter/Filter.h>

namespace glraw
{

LibraryInstance Filter::s_library = Filter::InitializeLibrary();

LibraryInstance Filter::InitializeLibrary()
{
	LibraryInstance tmp;
	tmp.emplace("grayscale", &Factory<Grayscale>);
	tmp.emplace("blockblur", &Factory<BlockBlur>);

	return tmp;
}

}