cmake_minimum_required(VERSION 2.8)
project(alib)

set(CMAKE_C_STANDARD 99)

#option(ALIB_DEBUG "enable debug alib." OFF)
#CONFIGURE_FILE(config-cmake.h.in
#        ${PROJECT_SOURCE_DIR}/include/config.h
#        @ONLY)
#ADD_DEFINITIONS(-DALIB_HAVE_CONFIG_H)

add_subdirectory(third_party)

set(alib_HEADER_FILES
        include/atypes.h
        include/alog.h
        include/atomicvar.h
        include/amem.h
        include/obj/aobj.h
        include/taggedpointer.h
        include/std/astr.h
        include/obj/dynastr.h
        include/acom.h
        include/obj/list.h
        include/dlnk.h
        include/dynapool.h
        include/avl.h
        include/stream.h
        include/dynabuf.h
        include/utf8.h)

set(alib_SOURCE_FILES
        src/amem.c
        src/obj/aobj.c
        src/std/astr.c
        src/obj/dynastr.c
        src/acom.c
        src/obj/list.c
        src/dlnk.c
        src/dynapool.c
        src/avl.c
        src/stream.c
        src/dynabuf.c
        src/utf8.c)

if (WIN32)
    list(APPEND alib_SOURCE_FILES src/amem_win.cpp)
endif ()

add_library(alib STATIC
        ${alib_HEADER_FILES} ${alib_SOURCE_FILES})
target_include_directories(alib
        PUBLIC include)

if (UNIX)
    target_compile_options(alib
            PRIVATE -fPIC)
endif (UNIX)
