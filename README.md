![glraw logo](glraw-logo.png "glraw")

*glraw* is an utility that converts Qt-supported images into an OpenGL-compatible raw format. *glraw* reduces the process of loading textures at run-time to a minimum: A file-content dump into GPU memory. *glraw* is a cross-platform library licenced under the [MIT license](http://opensource.org/licenses/MIT).

The latest release is [glraw-1.0.3](https://github.com/hpicgs/glraw/releases/tag/v1.0.3).

## Project Health (master)

| Service | System | Compiler | Options | Status |
| ------- | ------ | -------- | ------- | ------ |
| Jenkins | Ubuntu 14.04 | GCC 4.8 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw-linux-gcc4.8)](http://jenkins.hpi3d.de/job/glraw-linux-gcc4.8)|
| Jenkins | Ubuntu 14.04 | GCC 4.9 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw-linux-gcc4.9)](http://jenkins.hpi3d.de/job/glraw-linux-gcc4.9)|
| Jenkins | Ubuntu 14.04 | Clang 3.5 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw-linux-clang3.5)](http://jenkins.hpi3d.de/job/glraw-linux-clang3.5) |
| Jenkins | OS X 10.10 | Clang 3.5 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw-osx-clang3.5)](http://jenkins.hpi3d.de/job/glraw-osx-clang3.5) |
| Jenkins | Windows 8.1 | MSVC 2013 Update 5 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw-windows-msvc2013)](http://jenkins.hpi3d.de/job/glraw-windows-msvc2013) |
| Jenkins | Windows 8.1 | MSVC 2015 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw-windows-msvc2015)](http://jenkins.hpi3d.de/job/glraw-windows-msvc2015) |

## Features

With *glraw* you can preconvert your texture assets and load them without the need of any image library. The generated raw files can easily be read. For this, glraw also provides a minimal Raw-File reader that you can either source-copy or integrate as C++ library into your project.
Image to OpenGL texture conversion can be done either by *glraw*s command line interface, e.g., within an existing tool-chain, or at run-time with *glraw* linked as asset library (requires linking Qt).

Using the command line interface to create, e.g., an uncompressed 8bit rgb-texture looks like this:
`>glraw-cmd -f GL_RGB -t GL_UNSIGNED_BYTE image.png`

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
 * [Doxygen Documentation](http://libglow.org/glraw/doxygen-master/index.html): A *glraw* source documentation.


## Status

build status (@hourly for master): ![status](http://jenkins.hpi3d.de/buildStatus/icon?job=glraw)

## Dependencies

The following dev-libraries and programs need to be provided for correct project configuration:

* C++11 compatible compiler (e.g. gcc 4.8, VS 2013)
* CMake (>= 2.8.9): http://www.cmake.org/
* Qt5 (>= 5.2): http://www.qt-project.org/
