cmake_minimum_required(VERSION 3.0)
include(../../gbCMake/gbCommon.cmake)

project(gbFontGen)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../gbCMake/)
find_package(libfreetype REQUIRED)
include_directories(SYSTEM ${libfreetype_INCLUDE_DIRS})

message("include dir: " ${ibfreetype_INCLUDE_DIRS} ${libfreetype_LIBRARIES})

# gb_add_src(FreeTypeLoader gbFreeTypeLoader)
# gb_add_src(SignedDistanceFieldGen gbSDF)
# gb_add_src("." gbFontGen)

# gb_set_target("gbFontGen")
# add_executable(${gb_target_name}
#   ${gb_headers}
#   ${gb_srcs})
# gb_target_link_libraries(${gb_target_name} "gbUtils")
# target_link_libraries(${gb_target_name} ${PROJECT_SOURCE_DIR}/../ThirdPartyLib/freetype/lib/libfreetype.a)
# include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/../ThirdPartyLib/freetype/include
#   ${PROJECT_SOURCE_DIR}/../ThirdPartyLib
#   )
