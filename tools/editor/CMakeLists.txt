cmake_minimum_required(VERSION 3.0)

if(NOT WIN32)
  message(FATAL_ERROR "NOT WIN32")
endif()

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/src)

set(srcs)

gb_add_class(NS.h src srcs)
gb_add_class(Editor src srcs)
gb_add_class(Main src  srcs)

# data
gb_add_class(NS.h src/data srcs)
gb_add_class(Main src/data srcs)


include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/src)

add_executable(gbEditor
  ${srcs}
  )


if(NOT ${CMAKE_VERSION} VERSION_LESS "3.6.0")
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT gbEditor)
endif()


set_target_properties(gbRender gbEditor
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

add_dependencies(gbEditor gbRender)

find_package(gbUtils REQUIRED)
find_package(gbPhysics REQUIRED)
find_package(lua REQUIRED)
target_link_libraries(gbEditor
  debug ${CMAKE_SOURCE_DIR}/Debug/gbRenderd.lib
  optimized ${CMAKE_SOURCE_DIR}/Release/gbRender.lib
  debug ${gbUtils_LIBRARIES_d}
  optimized ${gbUtils_LIBRARIES}
  debug ${gbPhysics_LIBRARIES_d}
  optimized ${gbPhysics_LIBRARIES}
  debug ${lua_LIBRARIES_d}
  optimized ${lua_LIBRARIES}
 )
