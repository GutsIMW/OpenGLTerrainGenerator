cmake_minimum_required(VERSION 3.0)
project(ProjetProgGraphique)

option(PROJET_TERRAIN "Build PROJET_TERRAIN." ON)
if (WIN32)
	set (SYS_DATA_PATH $ENV{HOME} CACHE PATH "Path where to read image mesh ...")
else()
	set (SYS_DATA_PATH $ENV{HOMEPATH} CACHE PATH "Path where to read image mesh ...")
endif()
set_property(GLOBAL PROPERTY USE_FOLDERS ON) 
set(CMAKE_DEBUG_POSTFIX "_d")

add_definitions("-DDEF_DATA_PATH=${SYS_DATA_PATH}")


add_subdirectory(easycppogl_src)

if (${PROJET_TERRAIN})
	add_subdirectory(PROJET_TERRAIN)
endif()
