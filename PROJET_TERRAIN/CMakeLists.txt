cmake_minimum_required(VERSION 3.0)

project(PROJET_TERRAIN LANGUAGES CXX)

set (PROJET_SHADERS_PATH $ENV{HOME} CACHE PATH "Path where to put GLSL shaders ...")

add_definitions("-DDEF_SHADERS_PATH=${PROJET_SHADERS_PATH}")

macro(compile)
  add_executable(${ARGV0} ${ARGV0}.cpp)
  target_link_libraries(${ARGV0} easycppogl)
endmacro()

set( src_list
terrain
)

foreach( exec_projet ${src_list})
	compile(${exec_projet})
endforeach()

add_compile_definitions(${exec_projet} PRIVATE DEF_DATA_PATH=${CMAKE_CURRENT_SOURCE_DIR}/data)
add_compile_definitions(${exec_projet} PRIVATE DEF_SHADERS_PATH=${CMAKE_CURRENT_SOURCE_DIR}/shaders)

# nice vs solution
set_target_properties(${src_list} PROPERTIES FOLDER PROJET_TERRAIN)
