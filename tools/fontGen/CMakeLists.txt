cmake_minimum_required(VERSION 3.0)

project(gbFontGen)

include(../../gbCMake/gbCommon.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../gbCMake/)
find_package(libfreetype REQUIRED)
include_directories(SYSTEM ${libfreetype_INCLUDE_DIRS})

find_package(gbUtils REQUIRED)

set(fontGen_srcs)

gb_add_class(fontGen src fontGen_srcs)
gb_add_class(freetypeLoader src fontGen_srcs)

add_executable(fontGen
  ${fontGen_srcs}
)

target_link_libraries(fontGen
  debug ${gbUtils_LIBRARIES_d}
  optimized ${gbUtils_LIBRARIES}
  debug ${libfreetype_LIBRARIES_d}
  optimized ${libfreetype_LIBRARIES}
  -lpng
  -lz
  -lharfbuzz
  )
