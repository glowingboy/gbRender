cmake_minimum_required(VERSION 3.0)

project(gbRender)

include(gbCMake/gbCommon.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/gbCMake)
find_package(gbUtils REQUIRED)
find_package(gbPhysics REQUIRED)

option(GB_RENDER_STATIC_LIB "build shared lib?" ON)

set(GB_RENDER_VERSION_MAJOR 0)
set(GB_RENDER_VERSION_MINOR 0)
# if( SHARED_LIB)
#   set(GB_RENDER_STATIC_LIB)
# endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/Config.h)

set(srcs)

# data
gb_add_class(DataNS src/data srcs)
gb_add_class(Mesh src/data srcs)
gb_add_class(Atlas src/data srcs)
gb_add_class(Font src/data srcs)
gb_add_class(Texture src/data srcs)
gb_add_class(Entity src/data srcs)
gb_add_class(Element src/data srcs)
gb_add_class(Transform src/data srcs)
gb_add_class(Camera src/data srcs)
gb_add_class(Render src/data srcs)
gb_add_class(Material src/data srcs)
gb_add_class(Shader src/data srcs)
gb_add_class(Text src/data srcs)
gb_add_class(FPVController src/data srcs)
gb_add_class(EntityPicker src/data srcs)

# file
gb_add_class(FileNS src/file srcs)
gb_add_class(Font src/file srcs)

# src
gb_add_class(RenderNS src srcs)
gb_add_class(Type src srcs)
gb_add_class(Device src srcs)
gb_add_class(GL src srcs)
gb_add_class(GLVariable src srcs)
gb_add_class(GLDraw src srcs)
gb_add_class(GLTexture src srcs)
gb_add_class(Input src srcs)
# 
gb_add_class(Director src srcs)
gb_add_class(Scene src srcs)
gb_add_class(GBObject src srcs)
gb_add_class(Entity src srcs)
# element
gb_add_class(Element src srcs)
gb_add_class(Transform src srcs)
gb_add_class(Camera src srcs)
gb_add_class(Render src srcs)
gb_add_class(Texture src srcs)
gb_add_class(Sprite src srcs)
gb_add_class(Text src srcs)
gb_add_class(FPVController src srcs)
gb_add_class(EntityPicker src srcs)
# resource
gb_add_class(Resource src/resource srcs)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdpartylib)


set(lib_type STATIC)
if(NOT GB_RENDER_STATIC_LIB)
  set(lib_type SHARED)
endif()

add_library(gbRender ${lib_type}
  ${srcs}
  )

set_target_properties(gbRender
  PROPERTIES
  DEBUG_OUTPUT_NAME gbRenderd
  RELEASE_OUTPUT_NAME gbRender
  )

if(NOT GB_RENDER_STATIC_LIB)
  set_target_properties(gbRender
    PROPERTIES
    COMPILE_FLAGS -DGB_RENDER_SHARED_LIB
    )
endif()

target_link_libraries(gbRender
  debug ${gbUtils_LIBRARIES_d}
  optimized ${gbUtils_LIBRARIES}
  debug ${gbPhysics_LIBRARIES_d}
  optimized ${gbPhysics_LIBRARIES}
  )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools/editor)
# install

set(CMAKE_INSTALL_PREFIX ${GB_DEFAULT_INSTALL_DIR}/gbRender)

install(TARGETS gbRender DESTINATION .)
install(DIRECTORY src/ DESTINATION .
  FILES_MATCHING PATTERN "*.h"
  PATTERN "thirdpartylib" EXCLUDE
  )

