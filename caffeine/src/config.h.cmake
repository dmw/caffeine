/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode cstyle; */
#ifndef _CAF_CONFIG_H
#define _CAF_CONFIG_H 1

#cmakedefine        STDC_HEADERS                1

#cmakedefine        HAVE_STDDEF_H               1
#cmakedefine        HAVE_STDBOOL_H              1

#cmakedefine        HAVE_MALLOC_H               1
#cmakedefine        HAVE_STRINGS_H              1
#cmakedefine        HAVE_MEMORY_H               1
#cmakedefine        HAVE_SYS_TYPES_H            1

#cmakedefine        HAVE_UNISTD_H               1
#cmakedefine        HAVE_SYS_UIO_H              1

#cmakedefine        HAVE_FCNTL_H                1

#cmakedefine        HAVE_DLFCN_H                1

#cmakedefine        HAVE_AIO_H                  1

#cmakedefine        CADDR_T_SZ                  ${CADDR_T_SZ}
#cmakedefine        OFF_T_SZ                    ${OFF_T_SZ}

#cmakedefine        SIZE_T_SZ                   ${SIZE_T_SZ}
#cmakedefine        VOID_P_SZ                   ${VOID_P_SZ}

#cmakedefine        HAVE_DLFUNC_T               1
#cmakedefine        HAVE_DLFUNC                 1

#cmakedefine        HAVE_AIO_WAITCOMPLETE       1

#if SIZE_T_SZ != VOID_P_SZ
#error strange compiler behavoir: sizeof(void *) != sizeof(size_t)
#endif /* SIZE_T_SZ == VOID_P_SZ */

#endif /* _CAF_CONFIG_H */
/* config.h ends here */
