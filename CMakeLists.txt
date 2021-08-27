cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmakes")
include(common)

if (NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "Sets the configuration to build (Debug, Release, etc...)")
endif()

if (WIN32 OR APPLE)
    set(CMAKE_DEBUG_POSTFIX "_d")
endif()

set(TARGET_PLATFORM "windows" CACHE STRING "target platform to build, only windows now")

# Compile and Link settings
set(COMPILE_AND_LINK_EXTRA_FLAGS "")
if(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} -fsigned-char")
elseif(NOT CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_C_STANDARD_REQUIRED TRUE)
    set(CMAKE_C_STANDARD "11")
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CXX_STANDARD "11")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} -Wall -Wextra -Wundef")
endif ()

option (WARNINGS_AS_ERRORS "Specifies whether to treat warnings as errors. Recommended at developing time." OFF)
if (WARNINGS_AS_ERRORS)
    if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} -Werror")
    elseif (MSVC)
        set(COMPILE_AND_LINK_EXTRA_FLAGS "${COMPILE_AND_LINK_EXTRA_FLAGS} /WX")
    else ()
        message (WARNING "You've specified \"WARNINGS_AS_ERRORS\" as \"ON\", but we don't know how to make it for your compiler. If you wish you can specifiy the required flags manually.")
    endif ()
endif ()

if (WIN32)
    add_compile_definitions(WINDOWS_ENABLED)
    add_compile_definitions(OPENGL_ENABLED)
    #add_compile_definitions(WASAPI_ENABLED)
    #add_compile_definitions(WINMIDI_ENABLED)
    add_compile_definitions(TYPED_METHOD_BIND)
    add_compile_definitions(WIN32)
    add_compile_definitions(MSVC)

	add_compile_options(/wd4244)
	add_compile_options(/wd4305)
endif(WIN32)
# For glad
if (${TARGET_PLATFORM} MATCHES "haiku|osx|windows|x11")
    add_compile_definitions(GLAD_ENABLED)
    add_compile_definitions(GLES_OVER_GL)
endif()

if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()
# Activating the default multi-processor build setting for all Visual Studio versions
if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COMPILE_AND_LINK_EXTRA_FLAGS}")

## Search for ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(ENV(CCACHE_CPP2) "yes")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
################################################################################

set(PROJECT_NAME "JDI")
# project settings
project(${PROJECT_NAME})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bullet
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cvtt
    #${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/freetype/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/jpeg-compressor
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libpng
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/mbedtls/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nanosvg
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pcre2/src
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pvrtccompressor
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/squish
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_rect_pack
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vhacd/public
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vhacd/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zstd
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/minizip
)

if (${TARGET_PLATFORM} MATCHES "haiku|osx|windows|x11")
    include_directories(thirdparty/glad)
    add_subdirectory(thirdparty/glad)
endif()

#add_subdirectory(thirdparty/bullet)
add_subdirectory(thirdparty/cvtt)
#add_subdirectory(thirdparty/freetype)
add_subdirectory(thirdparty/jpeg-compressor)
add_subdirectory(thirdparty/libpng)
add_subdirectory(thirdparty/mbedtls)
add_subdirectory(thirdparty/misc)
add_subdirectory(thirdparty/nanosvg)
add_subdirectory(thirdparty/pcre2)
add_subdirectory(thirdparty/pvrtccompressor)
add_subdirectory(thirdparty/squish)
add_subdirectory(thirdparty/stb_rect_pack)
add_subdirectory(thirdparty/vhacd)
add_subdirectory(thirdparty/zlib)
add_subdirectory(thirdparty/zstd)
add_subdirectory(thirdparty/minizip)

#set_target_properties(freetype PROPERTIES LINKER_LANGUAGE CXX FOLDER "libraries")


file(GLOB_RECURSE CORE_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} core/*.h)
file(GLOB_RECURSE CORE_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} core/*.cpp)

file(GLOB_RECURSE DRIVERS_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} drivers/*.h)
file(GLOB_RECURSE DRIVERS_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} drivers/*.cpp)

file(GLOB_RECURSE SCENE_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} scene/*.h)
file(GLOB_RECURSE SCENE_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} scene/*.cpp)

file(GLOB_RECURSE SERVERS_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} servers/*.h)
file(GLOB_RECURSE SERVERS_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} servers/*.cpp)

file(GLOB_RECURSE PLATFORM_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} platform/*.h)
file(GLOB_RECURSE PLATFORM_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} platform/*.cpp)

file(GLOB_RECURSE MODULES_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} modules/*.h)
file(GLOB_RECURSE MODULES_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} modules/*.cpp)
list(REMOVE_ITEM MODULES_HEADER_FILES "modules/csg/csg_gizmos.h")
list(REMOVE_ITEM MODULES_SOURCE_FILES "modules/csg/csg_gizmos.cpp")

file(GLOB MAIN_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} main/*.h)
file(GLOB MAIN_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} main/*.cpp)

#file(GLOB_RECURSE MISC_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} misc/*.h misc/*.hpp)
#file(GLOB_RECURSE MISC_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} misc/*.c misc/*.cc misc/*.cpp)
#if (NOT ${TARGET_PLATFORM} STREQUAL "android")
#    list(REMOVE_ITEM MISC_HEADER_FILES "thirdparty/misc/ifaddrs-android.h")
#    list(REMOVE_ITEM MISC_SOURCE_FILES "thirdparty/misc/ifaddrs-android.cc")
#endif()

set(ALL_HEADER_FILES ${CORE_HEADER_FILES} ${DRIVERS_HEADER_FILES} ${SCENE_HEADER_FILES} ${SERVERS_HEADER_FILES} ${PLATFORM_HEADER_FILES} ${MODULES_HEADER_FILES} ${MAIN_HEADER_FILES})    # ${MISC_HEADER_FILES}
set(ALL_SOURCE_FILES ${CORE_SOURCE_FILES} ${DRIVERS_SOURCE_FILES} ${SCENE_SOURCE_FILES} ${SERVERS_SOURCE_FILES} ${PLATFORM_SOURCE_FILES} ${MODULES_SOURCE_FILES} ${MAIN_SOURCE_FILES})    # ${MISC_SOURCE_FILES}

set(ALL_CODE_FILES ${ALL_HEADER_FILES} ${ALL_SOURCE_FILES})

GROUP_FILES(ALL_CODE_FILES)

add_compile_definitions(DEBUG_ENABLED)
add_compile_definitions(VERSION_BRANCH="3.3")
add_compile_definitions(VERSION_FULL_CONFIG="3.3")
add_compile_definitions(PCRE2_CODE_UNIT_WIDTH=0)
#add_compile_definitions(FREETYPE_ENABLED)

if ("${TARGET_PLATFORM}" STREQUAL "unix")
    CHECK_INCLUDE_FILES(mntent.h, HAVE_MNTENT)
    if (HAVE_MNTENT)
        add_compile_definitions(HAVE_MNTENT)
    endif()
endif()


if (WIN32)
    link_libraries(winmm opengl32 dsound kernel32 ole32 oleaut32 user32 gdi32 IPHLPAPI Shlwapi wsock32 Ws2_32 shell32 advapi32 dinput8 dxguid imm32 bcrypt Avrt dwmapi)
endif(WIN32)

set(TARGET_NAME "${PROJECT_NAME}")
add_executable(${TARGET_NAME} ${ALL_CODE_FILES})

if (${TARGET_PLATFORM} MATCHES "haiku|osx|windows|x11")
	target_link_libraries(${TARGET_NAME} glad)
endif()
target_link_libraries(${TARGET_NAME} cvtt jpeg-compresssor libpng mbedtls misc pcre2-16 pcre2-32 pvrtcompressor squish vhacd zlib zstd minizip)

if (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--path ../3DTest")
    if (NOT ${CMAKE_VERSION} VERSION_LESS "3.6.0")
        set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
    endif ()
endif (MSVC)
