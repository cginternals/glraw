#include <glraw/filter/Filter.hpp>
#include <glraw/filter/Filter.h>


namespace glraw
{

	LibraryInstance Filter::instance;

bool Filter::InitializeLibrary()
{
	Add<BlockBlur>("blockblur", "Block blurs the image, weights all neighbouring pixels equally.");
	Add<Brightness>("brightness", "Adjusts brightness by applying an additive value.");
	Add<ColorBlend>("colorblend", "Blends the image and a selected colour with the specified mode.");
	Add<Contrast>("contrast", "Adjusts contrast by applying a multiplicative value.");
	Add<Dilation>("dilation", "Dilates the image by applying max in the given neighbourhood.");
	Add<EdgeDetection>("edgedetection", "Uses the 3x3 -1,8,-1 kernel for edge detection.");
	Add<Erosion>("erosion", "Erodes the image by applying min in the given neighbourhood.");
	Add<GaussianBlur>("gaussianblur", "Blurs the image using a gaussian distribution for weighting.");
	Add<Grayscale>("grayscale", "Converts the colours of the image into grayscale.");
	Add<Invert>("invert", "Inverts the colours of the image.");
	Add<Mirror>("mirror", "Mirrors the image along the x- and/or y-axis.");
	Add<MotionBlur>("motionblur", "Applies motion blurring, i.e. blurs in the same direction.");
	Add<RadialBlur>("radialblur", "Applies radial blurring, i.e. blurs towards a center.");
	Add<Rotate>("rotate", "Rotates the image by multiples of 90 degrees.");
	Add<Saturation>("saturation", "Adjusts saturation by interpolating with the given value.");
	Add<Scale>("scale", "Scales the image with the specified mode.");
	Add<Sobel>("sobel", "Applies the sobel or scharr operator.");
	Add<Sharpening>("sharpening", "Sharpens the image by using a kernel.");
	Add<UnsharpMask>("unsharpmask", "Sharpens the image by using the difference to the blurred image.");

	return true;
}

}