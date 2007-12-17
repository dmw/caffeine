### -*- mode: cmake; indent-tabs-mode: t; tab-width: 4; cmake-tab-width: 4; -*-
### $Id$
### project
project(caffeine C)

### macro inclusions
include(CheckSymbolExists)
include(CheckIncludeFiles)
include(CheckTypeSize)
include(CheckLibraryExists)
include(FindThreads)

### extra includes
set (MAKE_EXTRA_INCLUDE_FILES
	"sys/types.h"
	"dlfcn.h")

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
	caf_data_base64.c
	caf_data_bits.c
	caf_data_buffer.c
	caf_data_packer.c
	caf_data_conv.c
	caf_data_lstc.c
	caf_data_lstdl.c
	caf_data_lstdlc.c
	caf_data_mem.c
	caf_data_pidfile.c
	caf_dsm.c
	caf_dso.c
	caf_hash_str.c
	caf_hash_table.c
	caf_io_file.c
	caf_aio_file.c
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
	../caf/caf_data_base64.h
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
	../caf/caf_dsm.h
	../caf/caf_dso.h
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
	"-Wall -Wextra -Wshadow -pedantic -std=c99 -DHAVE_CONFIG_H=1")

set (CFLAGS_DEBUG
	"-DCAFFEINE_DEBUG -g3 -pg")

### operating systems
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -D_GNU_SOURCE -DLINUX_SYSTEM=1")
	set (CAFFEINE_SRCS
		${CAFFEINE_SRCS}
		caf_evt_nio_epoll.c
		caf_evt_nio_pool_epoll.c
		caf_evt_fio_inotify.c)
	check_library_exists (
		"dl"
		"dlopen"
		""
		HAVE_LIB_DL)
	check_library_exists (
		"rt"
		"aio_read"
		""
		HAVE_LIB_RT)
	if (HAVE_LIB_DL)
		set (LINK_AGAINST_LIBRARIES
			"${LINK_AGAINST_LIBRARIES} -ldl")
	endif (HAVE_LIB_DL)
	if (HAVE_LIB_RT)
		set (LINK_AGAINST_LIBRARIES
			"${LINK_AGAINST_LIBRARIES} -lrt")
	endif (HAVE_LIB_RT)
	set (LINK_AGAINST_LIBRARIES
		"${LINK_AGAINST_LIBRARIES}")
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

check_include_files (
	"dlfcn.h"
	HAVE_DLFCN_H)

check_include_files (
	"aio.h"
	HAVE_AIO_H)

### operating systems
### CMAKE_SYSTEM_NAME

### checks in types.h
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

try_compile (DLFUNC_T_COMPILES
    "${CMAKE_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/test/dlfcn.c"
    OUTPUT_VARIABLE DLFUNC_T_COMPILES_OUT
    )

check_symbol_exists (
	"dlfunc_t"
	"dlfcn.h"
	HAVE_DLFUNC_T)
if (HAVE_DLFUNC_T)
else (HAVE_DLFUNC_T)
	check_type_size (
		"dlfunc_t"
		HAVE_DLFUNC_SZ)
	if (HAVE_DLFUNC_SZ)
		set (HAVE_DLFUNC_T 1)
	else (HAVE_DLFUNC_SZ)
		set (HAVE_DLFUNC_T 0)
	endif (HAVE_DLFUNC_SZ)
endif (HAVE_DLFUNC_T)

if (DLFUNC_T_COMPILES)
	set (HAVE_DLFUNC_T 1)
else (DLFUNC_T_COMPILES)
	set (HAVE_DLFUNC_T 0)
endif (DLFUNC_T_COMPILES)

check_symbol_exists (
	"dlfunc"
	"dlfcn.h"
	HAVE_DLFUNC)
if (HAVE_DLFUNC)
else (HAVE_DLFUNC)
endif (HAVE_DLFUNC)

check_symbol_exists (
	"aio_waitcomplete"
	"aio.h"
	HAVE_AIO_WAITCOMPLETE)
if (HAVE_AIO_WAITCOMPLETE)
else (HAVE_AIO_WAITCOMPLETE)
	set (HAVE_AIO_WAITCOMPLETE 0)
endif (HAVE_AIO_WAITCOMPLETE)

### config header
configure_file(
	"${PROJECT_SOURCE_DIR}/../caf/config.h.cmake"
	"${PROJECT_SOURCE_DIR}/../caf/config.h")

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
	set (LINK_FLAGS "${LINK_FLAGS} -g3 -pg")
else (CAFFEINE_DEBUG)
	set (CFLAGS_PROJECT "${CFLAGS_DEFAULT}")
endif (CAFFEINE_DEBUG)

### caffeine optimizations
if (CAFFEINE_ARCH)
	set (CFLAGS_PROJECT "${CFLAGS_PROJECT} -march=${CAFFEINE_ARCH}")
endif (CAFFEINE_ARCH)

if (CAFFEINE_OPTLEVEL)
	set (CFLAGS_PROJECT "${CFLAGS_PROJECT} -O${CAFFEINE_OPTLEVEL}")
endif (CAFFEINE_OPTLEVEL)

### caffeine library usage
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
	COMPILE_FLAGS "${CFLAGS_PROJECT} -DCOMPILING_CAFFEINE=1")

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

### debug information
if (CAFFEINE_DEBUG)
	set (CAFFEINE_LIBRARY_TARGETS
		caffeine)
	set_target_properties (
		${CAFFEINE_LIBRARY_TARGETS}
		PROPERTIES
		LINK_FLAGS ${LINK_FLAGS})
endif (CAFFEINE_DEBUG)

