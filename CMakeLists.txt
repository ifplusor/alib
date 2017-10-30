cmake_minimum_required(VERSION 2.8)
project(alib)

set(CMAKE_C_STANDARD 99)

set(alib_HEADER_FILES
        include/atypes.h
        include/atomicvar.h
        include/amem.h
        include/aobj.h
        include/taggedpointer.h
        include/astr.h
        include/dynastr.h
        include/acom.h
        include/list.h
        include/dlnk.h
        include/dynapool.h
        include/avl.h
        include/stream.h
        include/dynabuf.h
        include/utf8.h)

set(alib_SOURCE_FILES
        src/amem.c
        src/aobj.c
        src/astr.c
        src/dynastr.c
        src/acom.c
        src/list.c
        src/dlnk.c
        src/dynapool.c
        src/avl.c
        src/stream.c
        src/dynabuf.c
        src/utf8.c)

add_library(alib STATIC
        ${alib_HEADER_FILES} ${alib_SOURCE_FILES})
target_include_directories(alib
        PUBLIC include)
target_link_libraries(alib
        PUBLIC pthread)

if (UNIX)
    target_compile_options(alib
            PRIVATE -fPIC)
endif (UNIX)
