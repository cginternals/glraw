![glraw](https://raw.github.com/hpicgs/glraw/master/packages/glraw-logo.png)

*glraw* is an utility that converts Qt-supported images into an OpenGL-compatible raw format. *glraw* reduces the process of loading textures at run-time to a minimum: A file-content dump into GPU memory. 

## Features

With *glraw* you can preconvert your texture assets and load them without the need of any image library. The generated raw files can easily be read. For this, glraw also provides a minimal Raw-File reader that you can either source-copy or integrate as C++ library into your project.
Image to OpenGL texture conversion can be done either by *glraw*s command line interface, e.g., within an existing tool-chain, or at run-time with *glraw* linked as asset library (requires linking Qt).

When converting an input image, *glraw* allows basic operations on the input-image and gives you full control over format and type of your targeted asset specification:

* Output format and type: Choose either a format and a type (e.g., `GL_RGB` and `GL_UNSIGNED_BYTE`) or one of the supported compressed formats (e.g., `GL_COMPRESSED_RGBA_S3TC_DXT3_EXT`).

* Mirroring: Mirror images horizontally or vertically.

* Scaling: Scale your images either to a choosen size in pixels or by a factor.

* Arbitrary image processing: Provide your own GLSL shader for, e.g., blurring, sharpening, color conversion, or data encoding.

* Extensible file header: *glraw* supports two formats: 
  * .raw as a true raw format, where all asset meta information is either encoded in the file name or aggreed with the importer. 
  * .glraw, which extends the raw data by an arbitrary file header of binary key-value pairs (e.g., width, height, compression).

## Further Reading

 * [Use Cases](https://github.com/hpicgs/glraw/wiki/Use-Cases): Examples for using texture compression and user-defined image processing.
 * [Supported Formats and Types](https://github.com/hpicgs/glraw/wiki/Supported-Formats-and-Types): List of all currently supported OpenGL format-type combinations.
 * [glraw File Format](https://github.com/hpicgs/glraw/wiki/GLRaw-File-Format): Description of the extended raw format (.glraw).
 * [Installation on Mac OS X](https://github.com/hpicgs/glraw/wiki/Installation): Explanation about how to install *glraw* on Mac OS X with Homebrew
 * [Doxygen Documentation](http://libglow.org/glraw): A *glraw* source documentation.

## Development

### Dependencies

The following dev-libraries and programs need to be provided for correct project configuration:

* C++11 compatible compiler (e.g. gcc 4.8, VS 2013)
* CMake (>= 2.8.9): http://www.cmake.org/
* Qt5 (>= 5.2): http://www.qt-project.org/
