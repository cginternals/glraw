#include <glraw/filter/Filter.hpp>
#include <glraw/filter/Filter.h>

namespace glraw
{

LibraryInstance Filter::s_library = Filter::InitializeLibrary();

LibraryInstance Filter::InitializeLibrary()
{
	LibraryInstance tmp;
	tmp.emplace("grayscale", &Factory<Grayscale>);
	tmp.emplace( "blockblur", &Factory<BlockBlur> );
	tmp.emplace( "invert", &Factory<Invert> );
	tmp.emplace( "mirror", &Factory<Mirror> );
	tmp.emplace( "rotate", &Factory<Rotate> );
	tmp.emplace("scale", &Factory<Scale>);
	tmp.emplace("dilation", &Factory<Dilation>);
	tmp.emplace("erosion", &Factory<Erosion>);

	return tmp;
}

}