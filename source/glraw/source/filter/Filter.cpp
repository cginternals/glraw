#include <glraw/filter/Filter.hpp>
#include <glraw/filter/Filter.h>

#define AddLibrary(instance,name)	inst.emplace(name, &Factory<instance>)

namespace glraw
{

	LibraryInstance Filter::instance;

bool Filter::InitializeLibrary()
{
	Add<BlockBlur>("blockblur");
	Add<Brightness>("brightness");
	Add<ColorBlend>("colorblend");
	Add<Contrast>("contrast");
	Add<Dilation>("dilation");
	Add<Erosion>("erosion");
	Add<Grayscale>("grayscale");
	Add<Invert>("invert");
	Add<Mirror>("mirror");
	Add<Rotate>("rotate");
	Add<Saturation>("saturation");
	Add<Scale>("scale");

	return true;
}

}