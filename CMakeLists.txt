cmake_minimum_required(VERSION 2.8)
project(alib)

set(CMAKE_C_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

option(ALIB_DEBUG "enable alib debug flag." OFF)
option(__TYPEOF__ "enable typeof feature." OFF)

if(MINGW)
  set(__PTHREAD__ ON)
endif(MINGW)

configure_file(config-cmake.h.in ${PROJECT_SOURCE_DIR}/include/alib/config.h
               @ONLY)

set(alib_HEADER_FILES
    include/alib/config.h
    include/alib/atypes.h
    include/alib/amem.h
    include/alib/acom.h
    include/alib/collections/list/dlnk.h
    include/alib/collections/list/deque.h
    include/alib/collections/list/prique.h
    include/alib/collections/list/segarray.h
    include/alib/collections/map/avl.h
    include/alib/collections/map/rbtree.h
    include/alib/collections/map/strie.h
    include/alib/concurrent/threadlocal.h
    include/alib/io/alog.h
    include/alib/io/stream.h
    include/alib/memory/dynapool.h
    include/alib/memory/taggedpointer.h
    include/alib/object/aobj.h
    include/alib/object/pint.h
    include/alib/object/dstr.h
    include/alib/object/list.h
    include/alib/string/astr.h
    include/alib/string/dynabuf.h
    include/alib/string/utf8.h)

set(alib_SOURCE_FILES
    src/amem.c
    src/acom.c
    src/collections/list/dlnk.c
    src/collections/list/prique.c
    src/collections/list/segarray.c
    src/collections/map/avl.c
    src/collections/map/rbtree.c
    src/collections/map/strie.c
    src/concurrent/threadlocal.c
    src/io/stream.c
    src/memory/dynapool.c
    src/object/aobj.c
    src/object/dstr.c
    src/object/list.c
    src/string/astr.c
    src/string/dynabuf.c
    src/string/utf8.c)

if(WIN32)
  list(APPEND alib_SOURCE_FILES src/amem_win.cpp)
endif()

add_library(alib STATIC ${alib_HEADER_FILES} ${alib_SOURCE_FILES})
target_include_directories(alib PUBLIC include)

if((NOT WIN32) OR __PTHREAD__)
  target_link_libraries(alib PUBLIC pthread)
endif()

if(UNIX)
  target_compile_options(alib PRIVATE -fPIC)
endif(UNIX)
