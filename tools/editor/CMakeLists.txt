cmake_minimum_required(VERSION 3.0)

if(NOT WIN32)
  message(FATAL_ERROR "NOT WIN32")
endif()
# project(gbEditor)

# include(${CMAKE_CURRENT_SOURCE_DIR}/../../gbCMake/gbCommon.cmake)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../gbCMake)

# find_package(gbRender REQUIRED)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/src)

set(editor_srcs)

gb_add_class(Editor src editor_srcs)

add_executable(gbEditor
  ${editor_srcs}
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

target_link_libraries(gbEditor
  debug ${CMAKE_SOURCE_DIR}/Debug/gbRenderd.lib
  optimized ${CMAKE_SOURCE_DIR}/Release/gbRender.lib
 )
