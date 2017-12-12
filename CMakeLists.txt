cmake_minimum_required(VERSION 3.0)

project(gbRender)

include(gbCMake/gbCommon.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/gbCMake)
find_package(gbUtils REQUIRED)
find_package(gbPhysics REQUIRED)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gbRenderConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/gbRenderConfig.h)

option(SHARED_LIB ON)

set(lib_type SHARED)
if(NOT SHARED_LIB)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGB_RENDER_STATIC_LIB=1")
  set(lib_type STATIC)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGB_RENDER_SHARED_LIB=1")
endif()

set(gb_render_srcs)

gb_add_class(device src gb_render_srcs)
gb_add_class(director src gb_render_srcs)
gb_add_class(gl src gb_render_srcs)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdpartylib)

add_library(gbRender ${lib_type}
  ${gb_render_srcs}
  )

set_target_properties(gbRender
  PROPERTIES
  DEBUG_OUTPUT_NAME gbRenderd
  RELEASE_OUTPUT_NAME gbRender
  )

target_link_libraries(gbRender
  ${gbUtils_LIBRARIES}
  ${gbPhysics_LIBRARIES}
  )

# install

set(CMAKE_INSTALL_PREFIX ${GB_DEFAULT_INSTALL_DIR}/gbRender)

install(TARGETS gbRender DESTINATION .)
install(DIRECTORY src/ DESTINATION .
  FILES_MATCHING PATTERN "*.h"
  PATTERN "thirdpartylib" EXCLUDE
  )

