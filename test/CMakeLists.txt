### -*- mode: cmake; -*-
### $Id$
### project
project(caffeine C)
cmake_minimum_required(VERSION 2.6)

### macro inclusions
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckTypeSize)
include(FindThreads)

### extra includes
set (MAKE_EXTRA_INCLUDE_FILES
	"sys/types.h")

### includes
include_directories (
	/usr/include
	/usr/local/include
	/usr/X11R6/include
	.
	..
	)

link_directories (
	/usr/lib
	/usr/local/lib
	/usr/X11R6/lib
	)

link_libraries (
	caffeine
	)

### deque test sources
set (CAF_DEQUE_SRCS
	caf_deque.c)

### cdeque test sources
set (CAF_CDEQUE_SRCS
	caf_cdeque.c)

### lstc test sources
set (CAF_LSTC_SRCS
	caf_lstc.c)

### buffer test sources
set (CAF_BUFFER_SRCS
	caf_buffer.c)

### pidfile test sources
set (CAF_HASH_STR_SRCS
	caf_hash_str.c)

### pidfile test sources
set (CAF_HASHTABLE_SRCS
	caf_hashtable.c)

### dsm test sources
set (CAF_DSM_SRCS
	caf_dsm.c)

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

### base64 test sources
set (CAF_BASE64_SRCS
	caf_base64.c)

### base64 test sources
set (CAF_BASE64_FILE_SRCS
	caf_base64_file.c)

### tail sources
set (CAF_IO_TAIL_SRCS
	caf_tail.c)

### msg sources
set (CAF_IPCMSG_SRCS
	caf_ipcmsg.c)

### compile flags
set (CFLAGS_DEFAULT
	"-Wall -Wextra -Wshadow -pedantic -std=c99")

set (CFLAGS_DEBUG
	"-DCAFFEINE_DEBUG -g3 -pg")

option (CAFFEINE_DEBUG
	"Build Caffeine Using Debug"
	OFF)

set (LINK_FLAGS "-O1")

### operating systems
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -D_GNU_SOURCE -DLINUX_SYSTEM=1")
	check_library_exists (
		"dl"
		"dlopen"
		"/lib:/usr/lib:/usr/local/lib:/opt/lib"
		HAVE_LIB_DL)
	check_library_exists (
		"rt"
		"aio_read"
		"/lib:/usr/lib:/usr/local/lib:/opt/lib"
		HAVE_LIB_RT)
	if (HAVE_LIB_DL)
		set (LINK_FLAGS	"${LINK_FLAGS} -ldl")
		link_libraries ("${LINK_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
	else (HAVA_LIB_DL)
		message (FATAL_ERROR "AIO support is required")
	endif (HAVE_LIB_DL)
	if (HAVE_LIB_RT)
		set (LINK_FLAGS "${LINK_FLAGS} -lrt")
		link_libraries ("${LINK_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
	else (HAVA_LIB_RT)
		message (FATAL_ERROR "RT support is required")
	endif (HAVE_LIB_RT)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES " FreeBSD|NetBSD|OpenBSD")
	set (CFLAGS_DEFAULT "${CFLAGS_DEFAULT} -DBSD_SYSTEM=1")
endif (${CMAKE_SYSTEM_NAME} MATCHES " FreeBSD|NetBSD|OpenBSD")

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
	set (LINK_FLAGS	"${LINK_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
	link_libraries ("${CMAKE_THREAD_LIBS_INIT}")
else (CMAKE_USE_PTHREADS_INIT)
	message (FATAL_ERROR "Thread support is required")
endif (CMAKE_USE_PTHREADS_INIT)

if (PCRE_LIB AND PCRE_INCLUDE)
	set (LINK_FLAGS "${LINK_FLAGS} -lpcre")
	link_libraries ("${CMAKE_THREAD_LIBS_INIT}")
else (PCRE_LIB AND PCRE_INCLUDE)
	message (FATAL_ERROR "PCRE support is required")
endif (PCRE_LIB AND PCRE_INCLUDE)

### source files compile flags
set_source_files_properties (
	${CAF_DEQUE_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_CDEQUE_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_LSTC_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_BUFFER_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_PIDFILE_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_DSM_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_HASH_STR_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_HASHTABLE_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_PPM_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_TPM_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_BOTH_PTM_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_PTH_KEY_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_PTH_MUTEX_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_PTH_RWLOCK_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_IO_TAIL_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_BASE64_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_BASE64_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_BASE64_FILE_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

set_source_files_properties (
	${CAF_IPCMSG_SRCS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${CFLAGS_PROJECT}")

if (CMAKE_USE_PTHREADS_INIT)
	LINK_LIBRARIES ("${CMAKE_THREAD_LIBS_INIT}")
endif (CMAKE_USE_PTHREADS_INIT)

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif (COMMAND cmake_policy)

### build the executables
add_executable (caf_deque ${CAF_DEQUE_SRCS})
add_executable (caf_cdeque ${CAF_CDEQUE_SRCS})
add_executable (caf_lstc ${CAF_LSTC_SRCS})
add_executable (caf_buffer ${CAF_BUFFER_SRCS})
add_executable (caf_dsm ${CAF_DSM_SRCS})
add_executable (caf_hash_str ${CAF_HASH_STR_SRCS})
add_executable (caf_hashtable ${CAF_HASHTABLE_SRCS})
add_executable (caf_tail ${CAF_IO_TAIL_SRCS})
add_executable (caf_ppm ${CAF_PPM_SRCS})
add_executable (caf_tpm ${CAF_TPM_SRCS})
add_executable (caf_both_ptm ${CAF_BOTH_PTM_SRCS})
add_executable (caf_pth_key ${CAF_PTH_KEY_SRCS})
add_executable (caf_mutex ${CAF_PTH_MUTEX_SRCS})
add_executable (caf_rwlock ${CAF_PTH_RWLOCK_SRCS})
add_executable (caf_base64 ${CAF_BASE64_SRCS})
add_executable (caf_base64_file ${CAF_BASE64_FILE_SRCS})
add_executable (caf_ipcmsg ${CAF_IPCMSG_SRCS})

set (CAFFEINE_TEST_TARGETS
	caf_deque
	caf_cdeque
	caf_lstc
	caf_buffer
	caf_dsm
	caf_hash_str
	caf_hashtable
	caf_tail
	caf_ppm
	caf_tpm
	caf_both_ptm
	caf_pth_key
	caf_mutex
	caf_rwlock
	caf_tail
	caf_base64
	caf_base64_file)

set_target_properties (
	${CAFFEINE_TEST_TARGETS}
	PROPERTIES
	LINK_FLAGS "${LINK_FLAGS}"
	COMPILE_FLAGS "${LINK_FLAGS}")
