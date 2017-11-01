# CMakeLists.txt

PROJECT(PCRE2 C)

# Increased minimum to 2.8.0 to support newer add_test features. Set policy
# CMP0026 to avoid warnings for the use of LOCATION in GET_TARGET_PROPERTY.

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
CMAKE_POLICY(SET CMP0026 OLD)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${PROJECT_SOURCE_DIR}/src")

# Configuration checks

INCLUDE(CheckIncludeFile)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)

CHECK_INCLUDE_FILE(dirent.h HAVE_DIRENT_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS_H)

CHECK_FUNCTION_EXISTS(bcopy HAVE_BCOPY)
CHECK_FUNCTION_EXISTS(memmove HAVE_MEMMOVE)
CHECK_FUNCTION_EXISTS(strerror HAVE_STRERROR)

# User-configurable options
#
# Note: CMakeSetup displays these in alphabetical order, regardless of
# the order we use here.

SET(BUILD_SHARED_LIBS OFF CACHE BOOL
        "Build shared libraries instead of static ones.")

OPTION(PCRE2_BUILD_PCRE2_8 "Build 8 bit PCRE2 library" ON)

OPTION(PCRE2_BUILD_PCRE2_16 "Build 16 bit PCRE2 library" OFF)

OPTION(PCRE2_BUILD_PCRE2_32 "Build 32 bit PCRE2 library" OFF)

OPTION(PCRE2_DEBUG "Include debugging code" OFF)

SET(PCRE2_EBCDIC OFF CACHE BOOL
        "Use EBCDIC coding instead of ASCII. (This is rarely used outside of mainframe systems.)")

SET(PCRE2_EBCDIC_NL25 OFF CACHE BOOL
        "Use 0x25 as EBCDIC NL character instead of 0x15; implies EBCDIC.")

SET(PCRE2_LINK_SIZE "2" CACHE STRING
        "Internal link size (2, 3 or 4 allowed). See LINK_SIZE in config.h.in for details.")

SET(PCRE2_PARENS_NEST_LIMIT "250" CACHE STRING
        "Default nested parentheses limit. See PARENS_NEST_LIMIT in config.h.in for details.")

SET(PCRE2_HEAP_LIMIT "20000000" CACHE STRING
        "Default limit on heap memory (kilobytes). See HEAP_LIMIT in config.h.in for details.")

SET(PCRE2_MATCH_LIMIT "10000000" CACHE STRING
        "Default limit on internal looping. See MATCH_LIMIT in config.h.in for details.")

SET(PCRE2_MATCH_LIMIT_DEPTH "MATCH_LIMIT" CACHE STRING
        "Default limit on internal depth of search. See MATCH_LIMIT_DEPTH in config.h.in for details.")

SET(PCRE2GREP_BUFSIZE "20480" CACHE STRING
        "Buffer starting size parameter for pcre2grep. See PCRE2GREP_BUFSIZE in config.h.in for details.")

SET(PCRE2GREP_MAX_BUFSIZE "1048576" CACHE STRING
        "Buffer maximum size parameter for pcre2grep. See PCRE2GREP_MAX_BUFSIZE in config.h.in for details.")

SET(PCRE2_NEWLINE "LF" CACHE STRING
        "What to recognize as a newline (one of CR, LF, CRLF, ANY, ANYCRLF, NUL).")

SET(PCRE2_HEAP_MATCH_RECURSE OFF CACHE BOOL
        "Obsolete option: do not use")

SET(PCRE2_SUPPORT_UNICODE ON CACHE BOOL
        "Enable support for Unicode and UTF-8/UTF-16/UTF-32 encoding.")

SET(PCRE2_SUPPORT_BSR_ANYCRLF OFF CACHE BOOL
        "ON=Backslash-R matches only LF CR and CRLF, OFF=Backslash-R matches all Unicode Linebreaks")

SET(PCRE2_NEVER_BACKSLASH_C OFF CACHE BOOL
        "If ON, backslash-C (upper case C) is locked out.")

SET(PCRE2_SUPPORT_VALGRIND OFF CACHE BOOL
        "Enable Valgrind support.")

OPTION(PCRE2_SHOW_REPORT "Show the final configuration report" ON)

IF (MINGW)
    OPTION(NON_STANDARD_LIB_PREFIX
            "ON=Shared libraries built in mingw will be named pcre2.dll, etc., instead of libpcre2.dll, etc."
            OFF)

    OPTION(NON_STANDARD_LIB_SUFFIX
            "ON=Shared libraries built in mingw will be named libpcre2-0.dll, etc., instead of libpcre2.dll, etc."
            OFF)
ENDIF (MINGW)

IF (MSVC)
    OPTION(PCRE2_STATIC_RUNTIME
            "ON=Compile against the static runtime (/MT)."
            OFF)
    OPTION(INSTALL_MSVC_PDB
            "ON=Install .pdb files built by MSVC, if generated"
            OFF)
ENDIF (MSVC)

# Prepare build configuration

IF (NOT BUILD_SHARED_LIBS)
    SET(PCRE2_STATIC 1)
ENDIF (NOT BUILD_SHARED_LIBS)

IF (NOT PCRE2_BUILD_PCRE2_8 AND NOT PCRE2_BUILD_PCRE2_16 AND NOT PCRE2_BUILD_PCRE2_32)
    MESSAGE(FATAL_ERROR "At least one of PCRE2_BUILD_PCRE2_8, PCRE2_BUILD_PCRE2_16 or PCRE2_BUILD_PCRE2_32 must be enabled")
ENDIF (NOT PCRE2_BUILD_PCRE2_8 AND NOT PCRE2_BUILD_PCRE2_16 AND NOT PCRE2_BUILD_PCRE2_32)

IF (PCRE2_BUILD_PCRE2_8)
    SET(SUPPORT_PCRE2_8 1)
ENDIF (PCRE2_BUILD_PCRE2_8)

IF (PCRE2_BUILD_PCRE2_16)
    SET(SUPPORT_PCRE2_16 1)
ENDIF (PCRE2_BUILD_PCRE2_16)

IF (PCRE2_BUILD_PCRE2_32)
    SET(SUPPORT_PCRE2_32 1)
ENDIF (PCRE2_BUILD_PCRE2_32)

IF (PCRE2_SUPPORT_BSR_ANYCRLF)
    SET(BSR_ANYCRLF 1)
ENDIF (PCRE2_SUPPORT_BSR_ANYCRLF)

IF (PCRE2_NEVER_BACKSLASH_C)
    SET(NEVER_BACKSLASH_C 1)
ENDIF (PCRE2_NEVER_BACKSLASH_C)

IF (PCRE2_SUPPORT_UNICODE)
    SET(SUPPORT_UNICODE 1)
ENDIF (PCRE2_SUPPORT_UNICODE)


SET(NEWLINE_DEFAULT "")

IF (PCRE2_NEWLINE STREQUAL "CR")
    SET(NEWLINE_DEFAULT "1")
ENDIF (PCRE2_NEWLINE STREQUAL "CR")
IF (PCRE2_NEWLINE STREQUAL "LF")
    SET(NEWLINE_DEFAULT "2")
ENDIF (PCRE2_NEWLINE STREQUAL "LF")
IF (PCRE2_NEWLINE STREQUAL "CRLF")
    SET(NEWLINE_DEFAULT "3")
ENDIF (PCRE2_NEWLINE STREQUAL "CRLF")
IF (PCRE2_NEWLINE STREQUAL "ANY")
    SET(NEWLINE_DEFAULT "4")
ENDIF (PCRE2_NEWLINE STREQUAL "ANY")
IF (PCRE2_NEWLINE STREQUAL "ANYCRLF")
    SET(NEWLINE_DEFAULT "5")
ENDIF (PCRE2_NEWLINE STREQUAL "ANYCRLF")
IF (PCRE2_NEWLINE STREQUAL "NUL")
    SET(NEWLINE_DEFAULT "6")
ENDIF (PCRE2_NEWLINE STREQUAL "NUL")

IF (NEWLINE_DEFAULT STREQUAL "")
    MESSAGE(FATAL_ERROR "The PCRE2_NEWLINE variable must be set to one of the following values: \"LF\", \"CR\", \"CRLF\", \"ANY\", \"ANYCRLF\".")
ENDIF (NEWLINE_DEFAULT STREQUAL "")

IF (PCRE2_EBCDIC)
    SET(EBCDIC 1)
ENDIF (PCRE2_EBCDIC)

IF (PCRE2_EBCDIC_NL25)
    SET(EBCDIC 1)
    SET(EBCDIC_NL25 1)
ENDIF (PCRE2_EBCDIC_NL25)

# Output files

CONFIGURE_FILE(config-cmake.h.in
        ${PROJECT_BINARY_DIR}/config.h
        @ONLY)

set(PCRE2_MAJOR "10")
set(PCRE2_MINOR "30")
set(PCRE2_PRERELEASE "")
set(PCRE2_DATE "2017-08-14")

CONFIGURE_FILE(src/pcre2.h.in
        ${PROJECT_SOURCE_DIR}/include/pcre2.h
        @ONLY)

# What about pcre2-config and libpcre2.pc?

# Character table generation

OPTION(PCRE2_REBUILD_CHARTABLES "Rebuild char tables" OFF)
IF (PCRE2_REBUILD_CHARTABLES)
    ADD_EXECUTABLE(dftables src/dftables.c)
    ADD_CUSTOM_COMMAND(
            COMMENT "Generating character tables (pcre2_chartables.c) for current locale"
            DEPENDS dftables
            COMMAND dftables
            ARGS ${PROJECT_BINARY_DIR}/pcre2_chartables.c
            OUTPUT ${PROJECT_BINARY_DIR}/pcre2_chartables.c
    )
ELSE (PCRE2_REBUILD_CHARTABLES)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/pcre2_chartables.c.dist
            ${PROJECT_BINARY_DIR}/pcre2_chartables.c
            COPYONLY)
ENDIF (PCRE2_REBUILD_CHARTABLES)

# Source code

SET(PCRE2_HEADERS
        ${PROJECT_SOURCE_DIR}/include/pcre2.h
        src/pcre2_ucp.h
        src/pcre2_intmodedep.h
        src/pcre2_internal.h)

SET(PCRE2_SOURCES
        src/pcre2_auto_possess.c
        ${PROJECT_BINARY_DIR}/pcre2_chartables.c
        src/pcre2_compile.c
        src/pcre2_config.c
        src/pcre2_context.c
        src/pcre2_convert.c
        src/pcre2_dfa_match.c
        src/pcre2_error.c
        src/pcre2_find_bracket.c
        src/pcre2_maketables.c
        src/pcre2_match.c
        src/pcre2_match_data.c
        src/pcre2_newline.c
        src/pcre2_ord2utf.c
        src/pcre2_pattern_info.c
        src/pcre2_serialize.c
        src/pcre2_string_utils.c
        src/pcre2_study.c
        src/pcre2_substitute.c
        src/pcre2_substring.c
        src/pcre2_tables.c
        src/pcre2_ucd.c
        src/pcre2_valid_utf.c
        src/pcre2_xclass.c
        )

SET(PCRE2POSIX_HEADERS include/pcre2posix.h)
SET(PCRE2POSIX_SOURCES src/pcre2posix.c)

IF (MINGW AND NOT PCRE2_STATIC)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2.rc)
        ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_SOURCE_DIR}/pcre2.o
                PRE-LINK
                COMMAND windres ARGS pcre2.rc pcre2.o
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMENT Using pcre2 coff info in mingw build)
        SET(PCRE2_SOURCES
                ${PCRE2_SOURCES} ${PROJECT_SOURCE_DIR}/pcre2.o
                )
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2.rc)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2posix.rc)
        ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_SOURCE_DIR}/pcre2posix.o
                PRE-LINK
                COMMAND windres ARGS pcre2posix.rc pcre2posix.o
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMENT Using pcre2posix coff info in mingw build)
        SET(PCRE2POSIX_SOURCES
                ${PCRE2POSIX_SOURCES} ${PROJECT_SOURCE_DIR}/pcre2posix.o
                )
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2posix.rc)
ENDIF (MINGW AND NOT PCRE2_STATIC)

IF (MSVC AND NOT PCRE2_STATIC)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2.rc)
        SET(PCRE2_SOURCES
                ${PCRE2_SOURCES} pcre2.rc)
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2.rc)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2posix.rc)
        SET(PCRE2POSIX_SOURCES
                ${PCRE2POSIX_SOURCES} pcre2posix.rc)
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcre2posix.rc)
ENDIF (MSVC AND NOT PCRE2_STATIC)

# Fix static compilation with MSVC: https://bugs.exim.org/show_bug.cgi?id=1681
# This code was taken from the CMake wiki, not from WebM.

IF (MSVC AND PCRE2_STATIC_RUNTIME)
    MESSAGE(STATUS "** MSVC and PCRE2_STATIC_RUNTIME: modifying compiler flags to use static runtime library")
    foreach (flag_var
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach ()
ENDIF (MSVC AND PCRE2_STATIC_RUNTIME)

# Build setup

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF (MSVC)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
ENDIF (MSVC)

SET(CMAKE_INCLUDE_CURRENT_DIR 1)
# needed to make sure to not link debug libs
# against release libs and vice versa
IF (WIN32)
    SET(CMAKE_DEBUG_POSTFIX "d")
ENDIF (WIN32)

SET(targets)

# 8-bit library

IF (PCRE2_BUILD_PCRE2_8)
    ADD_LIBRARY(pcre2-8
            ${PCRE2_HEADERS} ${PCRE2_SOURCES} ${PROJECT_BINARY_DIR}/config.h)
    target_include_directories(pcre2-8
            PUBLIC ${PROJECT_SOURCE_DIR}/include)
    SET_PROPERTY(TARGET pcre2-8
            PROPERTY COMPILE_DEFINITIONS PCRE2_CODE_UNIT_WIDTH=8)
    SET(targets ${targets} pcre2-8)
    ADD_LIBRARY(pcre2-posix ${PCRE2POSIX_HEADERS} ${PCRE2POSIX_SOURCES})
    target_include_directories(pcre2-posix
            PUBLIC include)
    SET_PROPERTY(TARGET pcre2-posix
            PROPERTY COMPILE_DEFINITIONS PCRE2_CODE_UNIT_WIDTH=8)
    SET(targets ${targets} pcre2-posix)
    TARGET_LINK_LIBRARIES(pcre2-posix
            PUBLIC pcre2-8)

    IF (MINGW AND NOT PCRE2_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcre2-8 pcre2-posix PROPERTIES PREFIX "")
        ENDIF (NON_STANDARD_LIB_PREFIX)
        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcre2-8 pcre2-posix PROPERTIES SUFFIX "-0.dll")
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE2_STATIC)
ENDIF (PCRE2_BUILD_PCRE2_8)

# 16-bit library

IF (PCRE2_BUILD_PCRE2_16)
    ADD_LIBRARY(pcre2-16 ${PCRE2_HEADERS} ${PCRE2_SOURCES} ${PROJECT_BINARY_DIR}/config.h)
    target_include_directories(pcre2-16
            PUBLIC ${PROJECT_BINARY_DIR}/include)
    SET_PROPERTY(TARGET pcre2-16
            PROPERTY COMPILE_DEFINITIONS PCRE2_CODE_UNIT_WIDTH=16)
    SET(targets ${targets} pcre2-16)

    IF (MINGW AND NOT PCRE2_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcre2-16 PROPERTIES PREFIX "")
        ENDIF (NON_STANDARD_LIB_PREFIX)
        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcre2-16 PROPERTIES SUFFIX "-0.dll")
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE2_STATIC)
ENDIF (PCRE2_BUILD_PCRE2_16)

# 32-bit library

IF (PCRE2_BUILD_PCRE2_32)
    ADD_LIBRARY(pcre2-32 ${PCRE2_HEADERS} ${PCRE2_SOURCES} ${PROJECT_BINARY_DIR}/config.h)
    target_include_directories(pcre2-32
            PUBLIC ${PROJECT_BINARY_DIR}/include)
    SET_PROPERTY(TARGET pcre2-32
            PROPERTY COMPILE_DEFINITIONS PCRE2_CODE_UNIT_WIDTH=32)
    SET(targets ${targets} pcre2-32)

    IF (MINGW AND NOT PCRE2_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcre2-32 PROPERTIES PREFIX "")
        ENDIF (NON_STANDARD_LIB_PREFIX)
        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcre2-32 PROPERTIES SUFFIX "-0.dll")
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE2_STATIC)
ENDIF (PCRE2_BUILD_PCRE2_32)

# Help, only for nice output
IF (BUILD_SHARED_LIBS)
    SET(BUILD_STATIC_LIBS OFF)
ELSE (BUILD_SHARED_LIBS)
    SET(BUILD_STATIC_LIBS ON)
ENDIF (BUILD_SHARED_LIBS)

IF (PCRE2_HEAP_MATCH_RECURSE)
    MESSAGE(WARNING "HEAP_MATCH_RECURSE is obsolete and does nothing.")
ENDIF (PCRE2_HEAP_MATCH_RECURSE)

IF (PCRE2_SHOW_REPORT)
    STRING(TOUPPER "${CMAKE_BUILD_TYPE}" buildtype)
    IF (CMAKE_C_FLAGS)
        SET(cfsp " ")
    ENDIF (CMAKE_C_FLAGS)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "")
    MESSAGE(STATUS "PCRE2 configuration summary:")
    MESSAGE(STATUS "")
    MESSAGE(STATUS "  Install prefix .................. : ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "  C compiler ...................... : ${CMAKE_C_COMPILER}")
    MESSAGE(STATUS "  C compiler flags ................ : ${CMAKE_C_FLAGS}${cfsp}${CMAKE_C_FLAGS_${buildtype}}")
    MESSAGE(STATUS "")
    MESSAGE(STATUS "  Build 8 bit PCRE2 library ....... : ${PCRE2_BUILD_PCRE2_8}")
    MESSAGE(STATUS "  Build 16 bit PCRE2 library ...... : ${PCRE2_BUILD_PCRE2_16}")
    MESSAGE(STATUS "  Build 32 bit PCRE2 library ...... : ${PCRE2_BUILD_PCRE2_32}")
    MESSAGE(STATUS "  Enable Unicode support .......... : ${PCRE2_SUPPORT_UNICODE}")
    MESSAGE(STATUS "  Newline char/sequence ........... : ${PCRE2_NEWLINE}")
    MESSAGE(STATUS "  \\R matches only ANYCRLF ......... : ${PCRE2_SUPPORT_BSR_ANYCRLF}")
    MESSAGE(STATUS "  \\C is disabled .................. : ${PCRE2_NEVER_BACKSLASH_C}")
    MESSAGE(STATUS "  EBCDIC coding ................... : ${PCRE2_EBCDIC}")
    MESSAGE(STATUS "  EBCDIC coding with NL=0x25 ...... : ${PCRE2_EBCDIC_NL25}")
    MESSAGE(STATUS "  Rebuild char tables ............. : ${PCRE2_REBUILD_CHARTABLES}")
    MESSAGE(STATUS "  Internal link size .............. : ${PCRE2_LINK_SIZE}")
    MESSAGE(STATUS "  Parentheses nest limit .......... : ${PCRE2_PARENS_NEST_LIMIT}")
    MESSAGE(STATUS "  Heap limit ...................... : ${PCRE2_HEAP_LIMIT}")
    MESSAGE(STATUS "  Match limit ..................... : ${PCRE2_MATCH_LIMIT}")
    MESSAGE(STATUS "  Match depth limit ............... : ${PCRE2_MATCH_LIMIT_DEPTH}")
    MESSAGE(STATUS "  Build shared libs ............... : ${BUILD_SHARED_LIBS}")
    MESSAGE(STATUS "  Build static libs ............... : ${BUILD_STATIC_LIBS}")
    MESSAGE(STATUS "  Support Valgrind .................: ${PCRE2_SUPPORT_VALGRIND}")

    IF (MINGW AND NOT PCRE2_STATIC)
        MESSAGE(STATUS "  Non-standard dll names (prefix) . : ${NON_STANDARD_LIB_PREFIX}")
        MESSAGE(STATUS "  Non-standard dll names (suffix) . : ${NON_STANDARD_LIB_SUFFIX}")
    ENDIF (MINGW AND NOT PCRE2_STATIC)

    MESSAGE(STATUS "")
ENDIF (PCRE2_SHOW_REPORT)

# end CMakeLists.txt
