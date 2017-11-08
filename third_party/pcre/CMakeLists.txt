cmake_minimum_required(VERSION 2.8)

# CMakeLists.txt

PROJECT(PCRE C CXX)

# Increased minimum to 2.8.0 to support newer add_test features. Set policy
# CMP0026 to avoid warnings for the use of LOCATION in GET_TARGET_PROPERTY.

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
CMAKE_POLICY(SET CMP0026 OLD)

include_directories(${PROJECT_SOURCE_DIR}/src)

# Configuration checks

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)

CHECK_INCLUDE_FILE(dirent.h HAVE_DIRENT_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS_H)

CHECK_INCLUDE_FILE_CXX(type_traits.h HAVE_TYPE_TRAITS_H)
CHECK_INCLUDE_FILE_CXX(bits/type_traits.h HAVE_BITS_TYPE_TRAITS_H)

CHECK_FUNCTION_EXISTS(bcopy HAVE_BCOPY)
CHECK_FUNCTION_EXISTS(memmove HAVE_MEMMOVE)
CHECK_FUNCTION_EXISTS(strerror HAVE_STRERROR)
CHECK_FUNCTION_EXISTS(strtoll HAVE_STRTOLL)
CHECK_FUNCTION_EXISTS(strtoq HAVE_STRTOQ)
CHECK_FUNCTION_EXISTS(_strtoi64 HAVE__STRTOI64)

CHECK_TYPE_SIZE("long long" LONG_LONG)
CHECK_TYPE_SIZE("unsigned long long" UNSIGNED_LONG_LONG)

# User-configurable options
#
# (Note: CMakeSetup displays these in alphabetical order, regardless of
# the order we use here)

SET(BUILD_SHARED_LIBS OFF CACHE BOOL
        "Build shared libraries instead of static ones.")

OPTION(PCRE_BUILD_PCRE8 "Build 8 bit PCRE library" ON)

OPTION(PCRE_BUILD_PCRE16 "Build 16 bit PCRE library" OFF)

OPTION(PCRE_BUILD_PCRE32 "Build 32 bit PCRE library" OFF)

OPTION(PCRE_BUILD_PCRECPP "Build the PCRE C++ library (pcrecpp)." OFF)

SET(PCRE_EBCDIC OFF CACHE BOOL
        "Use EBCDIC coding instead of ASCII. (This is rarely used outside of mainframe systems.)")

SET(PCRE_EBCDIC_NL25 OFF CACHE BOOL
        "Use 0x25 as EBCDIC NL character instead of 0x15; implies EBCDIC.")

SET(PCRE_LINK_SIZE "2" CACHE STRING
        "Internal link size (2, 3 or 4 allowed). See LINK_SIZE in config.h.in for details.")

SET(PCRE_PARENS_NEST_LIMIT "250" CACHE STRING
        "Default nested parentheses limit. See PARENS_NEST_LIMIT in config.h.in for details.")

SET(PCRE_MATCH_LIMIT "10000000" CACHE STRING
        "Default limit on internal looping. See MATCH_LIMIT in config.h.in for details.")

SET(PCRE_MATCH_LIMIT_RECURSION "MATCH_LIMIT" CACHE STRING
        "Default limit on internal recursion. See MATCH_LIMIT_RECURSION in config.h.in for details.")

SET(PCRE_NEWLINE "LF" CACHE STRING
        "What to recognize as a newline (one of CR, LF, CRLF, ANY, ANYCRLF).")

SET(PCRE_NO_RECURSE OFF CACHE BOOL
        "If ON, then don't use stack recursion when matching. See NO_RECURSE in config.h.in for details.")

SET(PCRE_POSIX_MALLOC_THRESHOLD "10" CACHE STRING
        "Threshold for malloc() usage. See POSIX_MALLOC_THRESHOLD in config.h.in for details.")

SET(PCRE_SUPPORT_JIT OFF CACHE BOOL
        "Enable support for Just-in-time compiling.")

SET(PCRE_SUPPORT_UTF ON CACHE BOOL
        "Enable support for Unicode Transformation Format (UTF-8/UTF-16/UTF-32) encoding.")

SET(PCRE_SUPPORT_UNICODE_PROPERTIES OFF CACHE BOOL
        "Enable support for Unicode properties (if set, UTF support will be enabled as well).")

SET(PCRE_SUPPORT_BSR_ANYCRLF OFF CACHE BOOL
        "ON=Backslash-R matches only LF CR and CRLF, OFF=Backslash-R matches all Unicode Linebreaks")

SET(PCRE_SUPPORT_VALGRIND OFF CACHE BOOL
        "Enable Valgrind support.")

OPTION(PCRE_SHOW_REPORT "Show the final configuration report" ON)

IF (MINGW)
    OPTION(NON_STANDARD_LIB_PREFIX
            "ON=Shared libraries built in mingw will be named pcre.dll, etc., instead of libpcre.dll, etc."
            OFF)

    OPTION(NON_STANDARD_LIB_SUFFIX
            "ON=Shared libraries built in mingw will be named libpcre-0.dll, etc., instead of libpcre.dll, etc."
            OFF)
ENDIF (MINGW)

IF (MSVC)
    OPTION(PCRE_STATIC_RUNTIME
            "ON=Compile against the static runtime (/MT)."
            OFF)
ENDIF (MSVC)

# Prepare build configuration

SET(pcre_have_type_traits 0)
SET(pcre_have_bits_type_traits 0)

IF (HAVE_TYPE_TRAITS_H)
    SET(pcre_have_type_traits 1)
ENDIF (HAVE_TYPE_TRAITS_H)

IF (HAVE_BITS_TYPE_TRAITS_H)
    SET(pcre_have_bits_type_traits 1)
ENDIF (HAVE_BITS_TYPE_TRAITS_H)

SET(pcre_have_long_long 0)
SET(pcre_have_ulong_long 0)

IF (HAVE_LONG_LONG)
    SET(pcre_have_long_long 1)
ENDIF (HAVE_LONG_LONG)

IF (HAVE_UNSIGNED_LONG_LONG)
    SET(pcre_have_ulong_long 1)
ENDIF (HAVE_UNSIGNED_LONG_LONG)

IF (NOT BUILD_SHARED_LIBS)
    SET(PCRE_STATIC 1)
ENDIF (NOT BUILD_SHARED_LIBS)

IF (NOT PCRE_BUILD_PCRE8 AND NOT PCRE_BUILD_PCRE16 AND NOT PCRE_BUILD_PCRE32)
    MESSAGE(FATAL_ERROR "At least one of PCRE_BUILD_PCRE8, PCRE_BUILD_PCRE16 or PCRE_BUILD_PCRE32 must be enabled")
ENDIF (NOT PCRE_BUILD_PCRE8 AND NOT PCRE_BUILD_PCRE16 AND NOT PCRE_BUILD_PCRE32)

IF (PCRE_BUILD_PCRE8)
    SET(SUPPORT_PCRE8 1)
ENDIF (PCRE_BUILD_PCRE8)

IF (PCRE_BUILD_PCRE16)
    SET(SUPPORT_PCRE16 1)
ENDIF (PCRE_BUILD_PCRE16)

IF (PCRE_BUILD_PCRE32)
    SET(SUPPORT_PCRE32 1)
ENDIF (PCRE_BUILD_PCRE32)

IF (PCRE_BUILD_PCRECPP AND NOT PCRE_BUILD_PCRE8)
    MESSAGE(STATUS "** PCRE_BUILD_PCRE8 must be enabled for the C++ library support")
    SET(PCRE_BUILD_PCRECPP OFF)
ENDIF (PCRE_BUILD_PCRECPP AND NOT PCRE_BUILD_PCRE8)

IF (PCRE_SUPPORT_BSR_ANYCRLF)
    SET(BSR_ANYCRLF 1)
ENDIF (PCRE_SUPPORT_BSR_ANYCRLF)

IF (PCRE_SUPPORT_UTF OR PCRE_SUPPORT_UNICODE_PROPERTIES)
    SET(SUPPORT_UTF 1)
    SET(PCRE_SUPPORT_UTF ON)
ENDIF (PCRE_SUPPORT_UTF OR PCRE_SUPPORT_UNICODE_PROPERTIES)

IF (PCRE_SUPPORT_UNICODE_PROPERTIES)
    SET(SUPPORT_UCP 1)
ENDIF (PCRE_SUPPORT_UNICODE_PROPERTIES)

IF (PCRE_SUPPORT_JIT)
    SET(SUPPORT_JIT 1)
ENDIF (PCRE_SUPPORT_JIT)

IF (PCRE_SUPPORT_VALGRIND)
    SET(SUPPORT_VALGRIND 1)
ENDIF (PCRE_SUPPORT_VALGRIND)

SET(NEWLINE "")

IF (PCRE_NEWLINE STREQUAL "LF")
    SET(NEWLINE "10")
ENDIF (PCRE_NEWLINE STREQUAL "LF")
IF (PCRE_NEWLINE STREQUAL "CR")
    SET(NEWLINE "13")
ENDIF (PCRE_NEWLINE STREQUAL "CR")
IF (PCRE_NEWLINE STREQUAL "CRLF")
    SET(NEWLINE "3338")
ENDIF (PCRE_NEWLINE STREQUAL "CRLF")
IF (PCRE_NEWLINE STREQUAL "ANY")
    SET(NEWLINE "-1")
ENDIF (PCRE_NEWLINE STREQUAL "ANY")
IF (PCRE_NEWLINE STREQUAL "ANYCRLF")
    SET(NEWLINE "-2")
ENDIF (PCRE_NEWLINE STREQUAL "ANYCRLF")

IF (NEWLINE STREQUAL "")
    MESSAGE(FATAL_ERROR "The PCRE_NEWLINE variable must be set to one of the following values: \"LF\", \"CR\", \"CRLF\", \"ANY\", \"ANYCRLF\".")
ENDIF (NEWLINE STREQUAL "")

IF (PCRE_EBCDIC)
    SET(EBCDIC 1)
    IF (PCRE_NEWLINE STREQUAL "LF")
        SET(NEWLINE "21")
    ENDIF (PCRE_NEWLINE STREQUAL "LF")
    IF (PCRE_NEWLINE STREQUAL "CRLF")
        SET(NEWLINE "3349")
    ENDIF (PCRE_NEWLINE STREQUAL "CRLF")
ENDIF (PCRE_EBCDIC)

IF (PCRE_EBCDIC_NL25)
    SET(EBCDIC 1)
    SET(EBCDIC_NL25 1)
    IF (PCRE_NEWLINE STREQUAL "LF")
        SET(NEWLINE "37")
    ENDIF (PCRE_NEWLINE STREQUAL "LF")
    IF (PCRE_NEWLINE STREQUAL "CRLF")
        SET(NEWLINE "3365")
    ENDIF (PCRE_NEWLINE STREQUAL "CRLF")
ENDIF (PCRE_EBCDIC_NL25)

IF (PCRE_NO_RECURSE)
    SET(NO_RECURSE 1)
ENDIF (PCRE_NO_RECURSE)

# Output files

CONFIGURE_FILE(config-cmake.h.in
        ${PROJECT_BINARY_DIR}/config.h
        @ONLY)

set(PCRE_MAJOR "8")
set(PCRE_MINOR "41")
set(PCRE_PRERELEASE "")
set(PCRE_DATE "2017-07-05")

CONFIGURE_FILE(src/pcre.h.in
        ${PROJECT_SOURCE_DIR}/include/pcre.h
        @ONLY)

# What about pcre-config and libpcre.pc?

IF (PCRE_BUILD_PCRECPP)
    CONFIGURE_FILE(src/cpp/pcre_stringpiece.h.in
            ${PROJECT_BINARY_DIR}/pcre_stringpiece.h
            @ONLY)

    CONFIGURE_FILE(src/cpp/pcrecpparg.h.in
            ${PROJECT_BINARY_DIR}/pcrecpparg.h
            @ONLY)
ENDIF (PCRE_BUILD_PCRECPP)

# Character table generation

OPTION(PCRE_REBUILD_CHARTABLES "Rebuild char tables" OFF)
IF (PCRE_REBUILD_CHARTABLES)
    ADD_EXECUTABLE(dftables src/dftables.c)

    GET_TARGET_PROPERTY(DFTABLES_EXE dftables LOCATION)

    ADD_CUSTOM_COMMAND(
            COMMENT "Generating character tables (pcre_chartables.c) for current locale"
            DEPENDS dftables
            COMMAND ${DFTABLES_EXE}
            ARGS ${PROJECT_BINARY_DIR}/pcre_chartables.c
            OUTPUT ${PROJECT_BINARY_DIR}/pcre_chartables.c
    )
ELSE (PCRE_REBUILD_CHARTABLES)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/pcre_chartables.c.dist
            ${PROJECT_BINARY_DIR}/pcre_chartables.c
            COPYONLY)
ENDIF (PCRE_REBUILD_CHARTABLES)

# Source code

SET(PCRE_HEADERS
        ${PROJECT_SOURCE_DIR}/include/pcre.h
        src/ucp.h
        src/pcre_internal.h)

IF (PCRE_BUILD_PCRE8)
    SET(PCRE_SOURCES
            src/pcre_byte_order.c
            ${PROJECT_BINARY_DIR}/pcre_chartables.c
            src/pcre_compile.c
            src/pcre_config.c
            src/pcre_dfa_exec.c
            src/pcre_exec.c
            src/pcre_fullinfo.c
            src/pcre_get.c
            src/pcre_globals.c
            src/pcre_jit_compile.c
            src/pcre_maketables.c
            src/pcre_newline.c
            src/pcre_ord2utf8.c
            src/pcre_refcount.c
            src/pcre_string_utils.c
            src/pcre_study.c
            src/pcre_tables.c
            src/pcre_ucd.c
            src/pcre_valid_utf8.c
            src/pcre_version.c
            src/pcre_xclass.c
            )

    SET(PCREPOSIX_HEADERS include/pcreposix.h)

    SET(PCREPOSIX_SOURCES src/pcreposix.c)

ENDIF (PCRE_BUILD_PCRE8)

IF (PCRE_BUILD_PCRE16)
    SET(PCRE16_SOURCES
            src/16/pcre16_byte_order.c
            src/16/pcre16_chartables.c
            src/16/pcre16_compile.c
            src/16/pcre16_config.c
            src/16/pcre16_dfa_exec.c
            src/16/pcre16_exec.c
            src/16/pcre16_fullinfo.c
            src/16/pcre16_get.c
            src/16/pcre16_globals.c
            src/16/pcre16_jit_compile.c
            src/16/pcre16_maketables.c
            src/16/pcre16_newline.c
            src/16/pcre16_ord2utf16.c
            src/16/pcre16_refcount.c
            src/16/pcre16_string_utils.c
            src/16/pcre16_study.c
            src/16/pcre16_tables.c
            src/16/pcre16_ucd.c
            src/16/pcre16_utf16_utils.c
            src/16/pcre16_valid_utf16.c
            src/16/pcre16_version.c
            src/16/pcre16_xclass.c
            )
ENDIF (PCRE_BUILD_PCRE16)

IF (PCRE_BUILD_PCRE32)
    SET(PCRE32_SOURCES
            src/32/pcre32_byte_order.c
            src/32/pcre32_chartables.c
            src/32/pcre32_compile.c
            src/32/pcre32_config.c
            src/32/pcre32_dfa_exec.c
            src/32/pcre32_exec.c
            src/32/pcre32_fullinfo.c
            src/32/pcre32_get.c
            src/32/pcre32_globals.c
            src/32/pcre32_jit_compile.c
            src/32/pcre32_maketables.c
            src/32/pcre32_newline.c
            src/32/pcre32_ord2utf32.c
            src/32/pcre32_refcount.c
            src/32/pcre32_string_utils.c
            src/32/pcre32_study.c
            src/32/pcre32_tables.c
            src/32/pcre32_ucd.c
            src/32/pcre32_utf32_utils.c
            src/32/pcre32_valid_utf32.c
            src/32/pcre32_version.c
            src/32/pcre32_xclass.c
            )
ENDIF (PCRE_BUILD_PCRE32)

IF (MINGW AND NOT PCRE_STATIC)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcre.rc)
        ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_SOURCE_DIR}/pcre.o
                PRE-LINK
                COMMAND windres ARGS pcre.rc pcre.o
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMENT Using pcre coff info in mingw build)
        SET(PCRE_SOURCES
                ${PCRE_SOURCES} ${PROJECT_SOURCE_DIR}/pcre.o
                )
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcre.rc)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcreposix.rc)
        ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_SOURCE_DIR}/pcreposix.o
                PRE-LINK
                COMMAND windres ARGS pcreposix.rc pcreposix.o
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMENT Using pcreposix coff info in mingw build)
        SET(PCREPOSIX_SOURCES
                ${PCREPOSIX_SOURCES} ${PROJECT_SOURCE_DIR}/pcreposix.o
                )
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcreposix.rc)
ENDIF (MINGW AND NOT PCRE_STATIC)

IF (MSVC AND NOT PCRE_STATIC)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcre.rc)
        SET(PCRE_SOURCES
                ${PCRE_SOURCES} pcre.rc)
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcre.rc)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/pcreposix.rc)
        SET(PCREPOSIX_SOURCES
                ${PCREPOSIX_SOURCES} pcreposix.rc)
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/pcreposix.rc)
ENDIF (MSVC AND NOT PCRE_STATIC)

# Fix static compilation with MSVC: https://bugs.exim.org/show_bug.cgi?id=1681
# This code was taken from the CMake wiki, not from WebM.

IF (MSVC AND PCRE_STATIC_RUNTIME)
    MESSAGE(STATUS "** MSVC and PCRE_STATIC_RUNTIME: modifying compiler flags to use static runtime library")
    foreach (flag_var
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach ()
ENDIF (MSVC AND PCRE_STATIC_RUNTIME)

SET(PCRECPP_HEADERS
        src/cpp/pcrecpp.h
        src/cpp/pcre_scanner.h
        ${PROJECT_BINARY_DIR}/pcrecpparg.h
        ${PROJECT_BINARY_DIR}/pcre_stringpiece.h
        )

SET(PCRECPP_SOURCES
        src/cpp/pcrecpp.cc
        src/cpp/pcre_scanner.cc
        src/cpp/pcre_stringpiece.cc
        )

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

# Libraries
# pcre
IF (PCRE_BUILD_PCRE8)
    ADD_LIBRARY(pcre
            ${PCRE_HEADERS} ${PCRE_SOURCES} ${PROJECT_BINARY_DIR}/config.h)
    target_include_directories(pcre
            PUBLIC ${PROJECT_SOURCE_DIR}/include)
    SET(targets ${targets} pcre)
    ADD_LIBRARY(pcreposix ${PCREPOSIX_HEADERS} ${PCREPOSIX_SOURCES})
    target_include_directories(pcreposix
            PUBLIC include)
    SET(targets ${targets} pcreposix)
    TARGET_LINK_LIBRARIES(pcreposix pcre)

    IF (MINGW AND NOT PCRE_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcre pcreposix PROPERTIES PREFIX "")
        ENDIF (NON_STANDARD_LIB_PREFIX)

        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcre pcreposix PROPERTIES SUFFIX "-0.dll")
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE_STATIC)

ENDIF (PCRE_BUILD_PCRE8)

IF (PCRE_BUILD_PCRE16)
    ADD_LIBRARY(pcre16 ${PCRE_HEADERS} ${PCRE16_SOURCES} ${PROJECT_BINARY_DIR}/config.h)
    SET(targets ${targets} pcre16)

    IF (MINGW AND NOT PCRE_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcre16
                    PROPERTIES PREFIX ""
                    )
        ENDIF (NON_STANDARD_LIB_PREFIX)

        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcre16
                    PROPERTIES SUFFIX "-0.dll"
                    )
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE_STATIC)

ENDIF (PCRE_BUILD_PCRE16)

IF (PCRE_BUILD_PCRE32)
    ADD_LIBRARY(pcre32 ${PCRE_HEADERS} ${PCRE32_SOURCES} ${PROJECT_BINARY_DIR}/config.h)
    SET(targets ${targets} pcre32)

    IF (MINGW AND NOT PCRE_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcre32
                    PROPERTIES PREFIX ""
                    )
        ENDIF (NON_STANDARD_LIB_PREFIX)

        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcre32
                    PROPERTIES SUFFIX "-0.dll"
                    )
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE_STATIC)

ENDIF (PCRE_BUILD_PCRE32)

# pcrecpp
IF (PCRE_BUILD_PCRECPP)
    ADD_LIBRARY(pcrecpp ${PCRECPP_HEADERS} ${PCRECPP_SOURCES})
    SET(targets ${targets} pcrecpp)
    TARGET_LINK_LIBRARIES(pcrecpp pcre)

    IF (MINGW AND NOT PCRE_STATIC)
        IF (NON_STANDARD_LIB_PREFIX)
            SET_TARGET_PROPERTIES(pcrecpp
                    PROPERTIES PREFIX ""
                    )
        ENDIF (NON_STANDARD_LIB_PREFIX)

        IF (NON_STANDARD_LIB_SUFFIX)
            SET_TARGET_PROPERTIES(pcrecpp
                    PROPERTIES SUFFIX "-0.dll"
                    )
        ENDIF (NON_STANDARD_LIB_SUFFIX)
    ENDIF (MINGW AND NOT PCRE_STATIC)
ENDIF (PCRE_BUILD_PCRECPP)

# help, only for nice output
IF (BUILD_SHARED_LIBS)
    SET(BUILD_STATIC_LIBS OFF)
ELSE (BUILD_SHARED_LIBS)
    SET(BUILD_STATIC_LIBS ON)
ENDIF (BUILD_SHARED_LIBS)

IF (PCRE_SHOW_REPORT)
    STRING(TOUPPER "${CMAKE_BUILD_TYPE}" buildtype)
    IF (CMAKE_C_FLAGS)
        SET(cfsp " ")
    ENDIF (CMAKE_C_FLAGS)
    IF (CMAKE_CXX_FLAGS)
        SET(cxxfsp " ")
    ENDIF (CMAKE_CXX_FLAGS)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "")
    MESSAGE(STATUS "PCRE configuration summary:")
    MESSAGE(STATUS "")
    MESSAGE(STATUS "  Install prefix .................. : ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "  C compiler ...................... : ${CMAKE_C_COMPILER}")
    MESSAGE(STATUS "  C++ compiler .................... : ${CMAKE_CXX_COMPILER}")
    MESSAGE(STATUS "  C compiler flags ................ : ${CMAKE_C_FLAGS}${cfsp}${CMAKE_C_FLAGS_${buildtype}}")
    MESSAGE(STATUS "  C++ compiler flags .............. : ${CMAKE_CXX_FLAGS}${cxxfsp}${CMAKE_CXX_FLAGS_${buildtype}}")
    MESSAGE(STATUS "")
    MESSAGE(STATUS "  Build 8 bit PCRE library ........ : ${PCRE_BUILD_PCRE8}")
    MESSAGE(STATUS "  Build 16 bit PCRE library ....... : ${PCRE_BUILD_PCRE16}")
    MESSAGE(STATUS "  Build 32 bit PCRE library ....... : ${PCRE_BUILD_PCRE32}")
    MESSAGE(STATUS "  Build C++ library ............... : ${PCRE_BUILD_PCRECPP}")
    MESSAGE(STATUS "  Enable JIT compiling support .... : ${PCRE_SUPPORT_JIT}")
    MESSAGE(STATUS "  Enable UTF support .............. : ${PCRE_SUPPORT_UTF}")
    MESSAGE(STATUS "  Unicode properties .............. : ${PCRE_SUPPORT_UNICODE_PROPERTIES}")
    MESSAGE(STATUS "  Newline char/sequence ........... : ${PCRE_NEWLINE}")
    MESSAGE(STATUS "  \\R matches only ANYCRLF ......... : ${PCRE_SUPPORT_BSR_ANYCRLF}")
    MESSAGE(STATUS "  EBCDIC coding ................... : ${PCRE_EBCDIC}")
    MESSAGE(STATUS "  EBCDIC coding with NL=0x25 ...... : ${PCRE_EBCDIC_NL25}")
    MESSAGE(STATUS "  Rebuild char tables ............. : ${PCRE_REBUILD_CHARTABLES}")
    MESSAGE(STATUS "  No stack recursion .............. : ${PCRE_NO_RECURSE}")
    MESSAGE(STATUS "  POSIX mem threshold ............. : ${PCRE_POSIX_MALLOC_THRESHOLD}")
    MESSAGE(STATUS "  Internal link size .............. : ${PCRE_LINK_SIZE}")
    MESSAGE(STATUS "  Parentheses nest limit .......... : ${PCRE_PARENS_NEST_LIMIT}")
    MESSAGE(STATUS "  Match limit ..................... : ${PCRE_MATCH_LIMIT}")
    MESSAGE(STATUS "  Match limit recursion ........... : ${PCRE_MATCH_LIMIT_RECURSION}")
    MESSAGE(STATUS "  Build shared libs ............... : ${BUILD_SHARED_LIBS}")
    MESSAGE(STATUS "  Build static libs ............... : ${BUILD_STATIC_LIBS}")
    MESSAGE(STATUS "  Support Valgrind .................: ${PCRE_SUPPORT_VALGRIND}")
    MESSAGE(STATUS "  Support coverage .................: ${PCRE_SUPPORT_COVERAGE}")

    IF (MINGW AND NOT PCRE_STATIC)
        MESSAGE(STATUS "  Non-standard dll names (prefix) . : ${NON_STANDARD_LIB_PREFIX}")
        MESSAGE(STATUS "  Non-standard dll names (suffix) . : ${NON_STANDARD_LIB_SUFFIX}")
    ENDIF (MINGW AND NOT PCRE_STATIC)

    MESSAGE(STATUS "")
ENDIF (PCRE_SHOW_REPORT)

# end CMakeLists.txt
