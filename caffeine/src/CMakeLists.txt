### proyecto
project(caffeine C)

### macro inclusions
include(CheckSymbolExists)
include(CheckIncludeFiles)
include(CheckTypeSize)
include(FindThreads)

### extra includes
set (MAKE_EXTRA_INCLUDE_FILES
        "sys/types.h")

link_directories (
        /usr/lib
        /usr/local/lib
        /usr/X11R6/lib
        .
        ..
)

### includes
include_directories (
        /usr/include
        /usr/local/include
        /usr/X11R6/include
        .
        ..
)

### sources
set (CAFFEINE_SRCS
    caf_data_bits.c
    caf_data_buffer.c
    caf_data_packer.c
    caf_data_conv.c
    caf_data_lstc.c
    caf_data_lstdl.c
    caf_data_lstdlc.c
    caf_data_mem.c
    caf_data_pidfile.c
    caf_hash_str.c
    caf_hash_table.c
    caf_io_file.c
    caf_io_tail.c
    caf_io_net.c
    caf_io_net_conpool.c
    caf_io_net_svcpool.c
    caf_evt_fio_common.c
    caf_evt_nio_poll.c
    caf_evt_nio_select.c
    caf_evt_nio_pool_poll.c
    caf_evt_nio_pool_select.c
    caf_evt_nio_common.c
    caf_process_pool.c
    caf_thread_attr.c
    caf_thread_key.c
    caf_thread_mutex.c
    caf_thread_cond.c
    caf_thread_once.c
    caf_thread_pool.c
    caf_thread_rwlock.c
    caf_sem_svr4.c
    caf_sem_posix.c
    caf_ipc_msg.c
    caf_ipc_shm.c
)

### sources
set (CAFFEINE_HEADERS
    ../caf/caf.h
    ../caf/caf_data_bits.h
    ../caf/caf_data_buffer.h
    ../caf/caf_data_conv.h
    ../caf/caf_data_lstc.h
    ../caf/caf_data_lstdl.h
    ../caf/caf_data_lstdlc.h
    ../caf/caf_data_mem.h
    ../caf/caf_data_packer.h
    ../caf/caf_data_pidfile.h
    ../caf/caf_data_string.h
    ../caf/caf_data_struct.h
    ../caf/caf_data_var.h
    ../caf/caf_evt.h
    ../caf/caf_evt_fio.h
    ../caf/caf_evt_nio.h
    ../caf/caf_evt_nio_pool.h
    ../caf/caf_hash_str.h
    ../caf/caf_hash_table.h
    ../caf/caf_io.h
    ../caf/caf_io_file.h
    ../caf/caf_io_net.h
    ../caf/caf_io_net_conpool.h
    ../caf/caf_io_net_svcpool.h
    ../caf/caf_io_tail.h
    ../caf/caf_io_tool.h
    ../caf/caf_ipc_msg.h
    ../caf/caf_ipc_shm.h
    ../caf/caf_mem.h
    ../caf/caf_process.h
    ../caf/caf_process_pool.h
    ../caf/caf_regex_pcre.h
    ../caf/caf_sem.h
    ../caf/caf_sem_posix.h
    ../caf/caf_sem_svr4.h
    ../caf/caf_thread.h
    ../caf/caf_thread_attr.h
    ../caf/caf_thread_cond.h
    ../caf/caf_thread_key.h
    ../caf/caf_thread_mutex.h
    ../caf/caf_thread_once.h
    ../caf/caf_thread_pool.h
    ../caf/caf_thread_rwlock.h
    ../caf/caf_tool_macro.h
)

### compile flags
set (CFLAGS_DEFAULT
        "-Wall -Wextra -Wshadow -pedantic -std=c99 -march=prescott -O2")

set (CFLAGS_DEBUG
        "-DCAFFEINE_DEBUG -g3")

### operating systems
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -D_GNU_SOURCE -DLINUX_SYSTEM=1")
    set (CAFFEINE_SRCS
         ${CAFFEINE_SRCS}
         caf_evt_nio_epoll.c
         caf_evt_nio_pool_epoll.c
         caf_evt_fio_inotify.c)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|NetBSD|OpenBSD")
    set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -DBSD_SYSTEM=1")
    set (CAFFEINE_SRCS
         ${CAFFEINE_SRCS}
         caf_evt_nio_kevent.c
         caf_evt_nio_pool_kevent.c
         caf_evt_fio_kevent.c)
endif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|NetBSD|OpenBSD")

set (
    SOURCE_FILES
    ${CAFFEINE_HEADERS}
    ${CAFFEINE_SRCS}
)

### options
option (CAFFEINE_DEBUG
        "Build Caffeine Using Debug"
        ON)

### checks for includes
check_include_files (
        "stdlib.h;stdarg.h;stdio.h;string.h;float.h"
        STDC_HEADERS)

check_include_files (
        "stddef.h"
        HAVE_STDDEF_H)

check_include_files (
        "stdbool.h"
        HAVE_STDBOOL_H)

check_include_files (
        "malloc.h"
        HAVE_MALLOC_H)

check_include_files (
        "strings.h"
        HAVE_STRINGS_H)

check_include_files (
        "memory.h"
        HAVE_MEMORY_H)

check_include_files (
        "unistd.h"
        HAVE_UNISTD_H)

check_include_files (
        "sys/types.h"
        HAVE_SYS_TYPES_H)

check_include_files (
        "sys/uio.h"
        HAVE_SYS_UIO_H)

check_include_files (
        "fcntl.h"
        HAVE_FCNTL_H)

### operating systems
### CMAKE_SYSTEM_NAME

### checks in types.h
if (HAVE_SYS_TYPES_H)

    check_type_size (
            off_t
            OFF_T_SZ)
    if (OFF_T_SZ)
    else (OFF_T_SZ)
        set (OFF_T_SZ 0)
    endif (OFF_T_SZ)

    check_type_size (
            caddr_t
            CADDR_T_SZ)
    if (CADDR_T_SZ)
    else (CADDR_T_SZ)
        set (CADDR_T_SZ 0)
    endif (CADDR_T_SZ)

endif (HAVE_SYS_TYPES_H)

check_type_size (
        "void *"
        VOID_P_SZ)
if (VOID_P_SZ)
else (VOID_P_SZ)
    set (VOID_P_SZ 0)
endif (VOID_P_SZ)

check_type_size (
        "size_t"
        SIZE_T_SZ)
if (SIZE_T_SZ)
else (SIZE_T_SZ)
    set (SIZE_T_SZ 0)
endif (SIZE_T_SZ)

### config header
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

### find pcre
find_path (PCRE_INCLUDE pcre.h
        /usr/include
        /usr/local/include
        /usr/X11R6/include)

find_library (PCRE_LIB
        NAMES pcre
        PATHS /lib /usr/lib /usr/local/lib /usr/X11R6/lib
        DOC "Perl Compatible Regular Expressions Library")

### debug caffeine
if (CAFFEINE_DEBUG)
    set (CFLAGS_PROJECT "${CFLAGS_DEFAULT} ${CFLAGS_DEBUG}")
    set (LINK_FLAGS "${LINK_FLAGS} -pg")
else (CAFFEINE_DEBUG)
    set (CFLAGS_PROJECT "${CFLAGS_DEFAULT}")
endif (CAFFEINE_DEBUG)

if (CMAKE_USE_PTHREADS_INIT)
    set (CFLAGS_PROJECT "${CFLAGS_PROJECT} -pthread")
    set (LINK_AGAINST_LIBRARIES
         "${LINK_AGAINST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}")
else (CMAKE_USE_PTHREADS_INIT)
    message (FATAL_ERROR "Thread support is required")
endif (CMAKE_USE_PTHREADS_INIT)

if (PCRE_LIB AND PCRE_INCLUDE)
    set (LINK_AGAINST_LIBRARIES
         "${LINK_AGAINST_LIBRARIES} -lpcre")
else (PCRE_LIB AND PCRE_INCLUDE)
    message (FATAL_ERROR "PCRE support is required")
endif (PCRE_LIB AND PCRE_INCLUDE)

### source files compile flags
set_source_files_properties (
        ${CAFFEINE_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

### source headers
set_source_files_properties (
        ${CAFFEINE_HEADERS}
        PROPERTIES
        HEADER_FILE_ONLY 1)

# link against libraries
link_libraries ("${LINK_AGAINST_LIBRARIES}")

### build the library
add_library (caffeine SHARED ${SOURCE_FILES})

### link
target_link_libraries (caffeine)

### dependencies
add_dependencies (caffeine ${CAFFEINE_HEADERS})
