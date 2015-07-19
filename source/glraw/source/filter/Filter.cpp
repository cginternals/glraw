#include <glraw/filter/Filter.hpp>
#include <glraw/filter/Filter.h>

#define AddLibrary(instance,name)	inst.emplace(name, &Factory<instance>)

namespace glraw
{

	LibraryInstance Filter::instance;

bool Filter::InitializeLibrary()
{
	AddFilter<BlockBlur>("blockblur");
	AddFilter<Brightness>("brightness");
	AddFilter<ColorBlend>("colorblend");
	AddFilter<Contrast>("contrast");
	AddFilter<Dilation>("dilation");
	AddFilter<Erosion>("erosion");
	AddFilter<Grayscale>("grayscale");
	AddFilter<Invert>("invert");
	AddFilter<Mirror>("mirror");
	AddFilter<Rotate>("rotate");
	AddFilter<Saturation>("saturation");
	AddFilter<Sharpening>("sharpening");
	AddFilter<Scale>("scale");

	return true;
}

}