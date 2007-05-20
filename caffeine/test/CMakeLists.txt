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

### includes
include_directories (
        /usr/local/include
        /usr/X11R6/include
        .
        ..
)

link_directories (
        /usr/lib
        /usr/local/lib
        /usr/X11R6/lib
        .
        ..
        src
        ../src
)

link_libraries (
        caffeine
)

### lstdl test sources
set (CAF_LSTDL_SRCS
        caf_lstdl.c)

### lstdl test sources
set (CAF_LSTC_SRCS
        caf_lstc.c)

### buffer test sources
set (CAF_BUFFER_SRCS
        caf_buffer.c)

### pidfile test sources
set (CAF_PIDFILE_SRCS
        caf_pidfile.c)

### ppm test sources
set (CAF_PPM_SRCS
        caf_ppm.c)

### tpm test sources
set (CAF_TPM_SRCS
        caf_tpm.c)

### ppm/tpm test sources
set (CAF_BOTH_PTM_SRCS
        caf_both_ptm.c)

### ppm/tpm test sources
set (CAF_PTH_KEY_SRCS
        caf_pth_key.c)

### thread mutex test sources
set (CAF_PTH_MUTEX_SRCS
        caf_mutex.c)

### thread mutex test sources
set (CAF_PTH_RWLOCK_SRCS
        caf_mutex.c)

### thread mutex test sources
set (CAF_IO_TAIL_SRCS
        caf_tail.c)

### compile flags
set (CFLAGS_DEFAULT
        "-Wall -Wextra -Wshadow -pedantic -std=c99 -march=prescott -O2")

set (CFLAGS_DEBUG
        "-DCAFFEINE_DEBUG -g3")

option (CAFFEINE_DEBUG
        "Build Caffeine Using Debug"
        ON)

### operating systems
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -D_GNU_SOURCE -DLINUX_SYSTEM=1")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|NetBSD|OpenBSD")
    set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -DBSD_SYSTEM=1")
endif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|NetBSD|OpenBSD")

### debug caffeine
if (CAFFEINE_DEBUG)
    set (CFLAGS_PROJECT "${CFLAGS_DEFAULT} ${CFLAGS_DEBUG}")
    set (LINK_FLAGS "${LINK_FLAGS} -pg")
else (CAFFEINE_DEBUG)
    set (CFLAGS_PROJECT "${CFLAGS_DEFAULT}")
endif (CAFFEINE_DEBUG)

### source files compile flags
set_source_files_properties (
        ${CAF_LSTDL_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_LSTC_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_BUFFER_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_PIDFILE_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_PPM_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_TPM_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_BOTH_PTM_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_PTH_KEY_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_PTH_MUTEX_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_PTH_RWLOCK_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
        ${CAF_IO_TAIL_SRCS}
        PROPERTIES
        COMPILE_FLAGS "${CFLAGS_PROJECT}")

if (CMAKE_USE_PTHREADS_INIT)
    LINK_LIBRARIES ("${CMAKE_THREAD_LIBS_INIT}")
endif (CMAKE_USE_PTHREADS_INIT)

### build the executables
add_executable (caf_lstdl ${CAF_LSTDL_SRCS})
add_executable (caf_lstc ${CAF_LSTC_SRCS})
add_executable (caf_buffer ${CAF_BUFFER_SRCS})
add_executable (caf_pidfile ${CAF_PIDFILE_SRCS})
add_executable (caf_ppm ${CAF_PPM_SRCS})
add_executable (caf_tpm ${CAF_TPM_SRCS})
add_executable (caf_both_ptm ${CAF_BOTH_PTM_SRCS})
add_executable (caf_pth_key ${CAF_PTH_KEY_SRCS})
add_executable (caf_mutex ${CAF_PTH_MUTEX_SRCS})
add_executable (caf_rwlock ${CAF_PTH_RWLOCK_SRCS})
add_executable (caf_tail ${CAF_IO_TAIL_SRCS})


