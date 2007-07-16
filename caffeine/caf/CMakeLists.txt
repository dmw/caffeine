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
set (CAFFEINE_HEADERS
    caf.h
    caf_data_bits.h
    caf_data_buffer.h
    caf_data_conv.h
    caf_data_lstc.h
    caf_data_lstdl.h
    caf_data_lstdlc.h
    caf_data_mem.h
    caf_data_packer.h
    caf_data_pidfile.h
    caf_data_string.h
    caf_data_struct.h
    caf_data_var.h
    caf_dsm.h
    caf_evt.h
    caf_evt_fio.h
    caf_evt_nio.h
    caf_evt_nio_pool.h
    caf_hash_str.h
    caf_hash_table.h
    caf_io.h
    caf_io_file.h
    caf_io_net.h
    caf_io_net_conpool.h
    caf_io_net_svcpool.h
    caf_io_tail.h
    caf_io_tool.h
    caf_ipc_msg.h
    caf_ipc_shm.h
    caf_mem.h
    caf_process.h
    caf_process_pool.h
    caf_regex_pcre.h
    caf_sem.h
    caf_sem_posix.h
    caf_sem_svr4.h
    caf_thread.h
    caf_thread_attr.h
    caf_thread_cond.h
    caf_thread_key.h
    caf_thread_mutex.h
    caf_thread_once.h
    caf_thread_pool.h
    caf_thread_rwlock.h
    caf_tool_macro.h
)

### operating systems
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -D_GNU_SOURCE -DLINUX_SYSTEM=1")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|NetBSD|OpenBSD")
    set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -DBSD_SYSTEM=1")
endif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|NetBSD|OpenBSD")

set (
    SOURCE_FILES
    ${CAFFEINE_HEADERS}
)

### options
option (CAFFEINE_DEBUG
        "Build Caffeine Using Debug"
        ON)

### source headers
set_source_files_properties (
        ${CAFFEINE_HEADERS}
        PROPERTIES
        HEADER_FILE_ONLY 1)

