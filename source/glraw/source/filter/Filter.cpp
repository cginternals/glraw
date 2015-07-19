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
	AddFilter<GaussianBlur>("gaussianblur");
	AddFilter<Grayscale>("grayscale");
	AddFilter<Invert>("invert");
	AddFilter<Mirror>("mirror");
	AddFilter<MotionBlur>("motionblur");
	AddFilter<RadialBlur>("radialblur");
	AddFilter<Rotate>("rotate");
	AddFilter<Saturation>("saturation");
	AddFilter<Scale>("scale");
	AddFilter<Sharpening>("sharpening");
	AddFilter<UnsharpMask>("unsharpmask");

	return true;
}

}