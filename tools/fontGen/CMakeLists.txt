cmake_minimum_required(VERSION 3.0)

project(gbFontGen)

include(../../gbCMake/gbCommon.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../gbCMake/)
find_package(libfreetype REQUIRED)
include_directories(SYSTEM ${libfreetype_INCLUDE_DIRS})

find_package(gbUtils REQUIRED)
find_package(gbPhysics REQUIRED)

set(fontGen_srcs)

gb_add_class(FontGen src fontGen_srcs)
gb_add_class(FreetypeLoader src fontGen_srcs)
# gb_add_class(Font ../../src/file fontGen_srcs)

add_executable(fontGen
  ${fontGen_srcs}
)
set(other_lib)
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL MSVC_COMPILER_ID)
  set(other_lib -lpng -lz -lharfbuzz -lptherad)
endif()

target_link_libraries(fontGen
  optimized ${gbUtils_LIBRARIES}
  debug ${gbUtils_LIBRARIES_d}
  optimized ${gbPhysics_LIBRARIES}
  debug ${gbPhysics_LIBRARIES_d}
  ${libfreetype_LIBRARIES}
  ${ohter_lib}
  )
