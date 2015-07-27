#include <glraw/filter/Filter.hpp>
#include <glraw/filter/Filter.h>


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
	Add<EdgeDetection>("edgedetection");
	Add<Erosion>("erosion");
	Add<GaussianBlur>("gaussianblur");
	Add<Grayscale>("grayscale");
	Add<Invert>("invert");
	Add<Mirror>("mirror");
	Add<MotionBlur>("motionblur");
	Add<RadialBlur>("radialblur");
	Add<Rotate>("rotate");
	Add<Saturation>("saturation");
	Add<Scale>("scale");
	Add<Sobel>("sobel");
	Add<Sharpening>("sharpening");
	Add<UnsharpMask>("unsharpmask");

	return true;
}

}