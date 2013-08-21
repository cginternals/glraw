
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

set(PROJECT_NAME "glraw")  
project(${PROJECT_NAME} C CXX)

if(MSVC)
set (CMAKE_PREFIX_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64")
cmake_policy(SET CMP0020 NEW)
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(${QT_INCLUDES})

set(target ${PROJECT_NAME})
add_executable(${target} glraw.cpp)
qt5_use_modules(${target} OpenGL Gui Widgets)

if(MSVC)
set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS               "/nologo /Zc:wchar_t /Zc:forScope /GL /GF /GR /GS- /Zi /fp:precise /MP /wd4267 /wd4251"
    LINK_FLAGS_DEBUG            "/NOLOGO /INCREMENTAL:NO /NXCOMPAT /DYNAMICBASE:NO /DEBUG /LTCG"
    LINK_FLAGS_RELEASE          "/NOLOGO /INCREMENTAL:NO /NXCOMPAT /DYNAMICBASE:NO /OPT:REF /LTCG /OPT:ICF /DELAY:UNLOAD"
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")
elseif()
set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS               "-pthread -fno-exceptions -pipe -fPIC"
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")
endif()

if(MSVC)
	# disable exception handling -> /EHs-c- does not remove CXX flags
	string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_MODIFIED ${CMAKE_CXX_FLAGS}) 
	string(REPLACE "/GX" ""  CMAKE_CXX_FLAGS_MODIFIED ${CMAKE_CXX_FLAGS_MODIFIED}) 
	unset(CMAKE_CXX_FLAGS CACHE)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_MODIFIED} CACHE TYPE STRING)
endif()
