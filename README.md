# GLraw

GLraw is a command-line tool which converts Qt-supported images into an OpenGL-compatible raw format.

## Features

With GLraw you can preconvert your texture assets and load them without the need of any image library. The generated raw 
files can easily be read with one of the prepared RawFile classes which you can copy into your project.

GLraw lets you edit your images before converting them. The following options are currently available:

__Output format and type__

Choose either a format and a type (e.g. `GL_RGB` and `GL_UNSIGNED_BYTE`) or one of the supported compressed formats (e.g. `GL_COMPRESSED_RGBA_S3TC_DXT3_EXT`).

__Mirroring__

GLraw can mirror images horizontally and vertically.

__Scaling__

Scale your images either to a choosen size in pixels or by a factor.

__GLSL Fragment Shader__

Write your own fragment shader and apply e.g. a Gaussian blur.


## Development

### Dependencies

The following dev-libraries and programs need to be provided for correct CMake configuration:

* C++11 compatible compiler (e.g. gcc 4.8, VS 2013)
* CMake (>= 2.8.9): http://www.cmake.org/
* Qt5 (>= 5.2): http://www.qt-project.org/

 

