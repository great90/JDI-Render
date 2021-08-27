set(TARGET_NAME pcre2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB PCRE2_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h)
file(GLOB PCRE2_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.c)

# these files are included in pcre2_jit_compile.c
list(REMOVE_ITEM PCRE2_HEADER_FILES src/pcre2_jit_neon_inc.h)
list(REMOVE_ITEM PCRE2_HEADER_FILES src/pcre2_jit_simd_inc.h)
list(REMOVE_ITEM PCRE2_SOURCE_FILES src/pcre2_jit_match.c)
list(REMOVE_ITEM PCRE2_SOURCE_FILES src/pcre2_jit_misc.c)

set(ALL_CODE_FILES ${PCRE2_HEADER_FILES} ${PCRE2_SOURCE_FILES})
GROUP_FILES(ALL_CODE_FILES)

add_compile_definitions(PCRE2_STATIC HAVE_CONFIG_H SUPPORT_UNICODE)
if (${PCRE2_WITH_JIT})
	add_compile_definitions(SUPPORT_JIT)
endif()

add_compile_definitions(PCRE2_CODE_UNIT_WIDTH=16)
add_library(${TARGET_NAME}-16 STATIC ${ALL_CODE_FILES})
set_target_properties(${TARGET_NAME}-16 PROPERTIES LINKER_LANGUAGE CXX FOLDER "libraries")

add_compile_definitions(PCRE2_CODE_UNIT_WIDTH=32)
add_library(${TARGET_NAME}-32 STATIC ${ALL_CODE_FILES})
set_target_properties(${TARGET_NAME}-32 PROPERTIES LINKER_LANGUAGE CXX FOLDER "libraries")
