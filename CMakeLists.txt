cmake_minimum_required(VERSION 2.8)
project(alib)

set(CMAKE_C_STANDARD 99)

set(HEADER_FILES
        include/atypes.h
        include/atomicvar.h
        include/amem.h
        include/aobj.h
        include/taggedpointer.h
        include/astr.h
        include/acom.h
        include/list.h
        include/dlnk.h
        include/dynapool.h
        include/avl.h
        include/stream.h
        include/dynabuf.h
        include/utf8.h)

set(SOURCE_FILES
        src/amem.c
        src/aobj.c
        src/astr.c
        src/acom.c
        src/list.c
        src/dlnk.c
        src/dynapool.c
        src/avl.c
        src/stream.c
        src/dynabuf.c
        src/utf8.c)

add_library(alib STATIC
        ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(alib
        PUBLIC include)
target_link_libraries(alib
        PUBLIC pthread)

if (UNIX)
    target_compile_options(alib
            PRIVATE -fPIC)
endif (UNIX)
