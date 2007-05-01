/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode none; */
/*
    Caffeine - C Application Framework
    Copyright (C) 2006 Daniel Molina Wegener

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_pidfile.h"
#include "caf/caf_thread_attr.h"
#include "caf/caf_thread_pool.h"


void *pth_rtn(void *p);

int
main (void)
{
    int rt = 0;
    pth_attri_t *attr = (pth_attri_t *)NULL;
    pth_pool_t *pth_pool = (pth_pool_t *)NULL;
    attr = pth_attri_new ();
    if (attr != (pth_attri_t *)NULL) {
        printf ("%d: attr = %p\n", getpid (), (void *)attr);
        rt = pth_attr_init (attr);
        if (rt == 0) {
            printf ("%d: attr init = %d\n", getpid (), rt);
            rt = pth_attri_set (attr, PTH_ATTR_DETACHED, (void *)NULL);
            if (rt == 0) {
                printf ("%d: attr PTHDR_ATTR_DETACHED = %d\n", getpid (), rt);
                pth_pool = pth_pool_create (attr, pth_rtn, 3, (void *)NULL);
                printf ("%d: threads: %d\n", getpid (),
                        lstdl_length (pth_pool->threads));
            }
        }
    }
    sleep (15);
    pth_pool_delete (pth_pool);
    sleep (5);
    pth_attri_destroy (attr);
    sleep (5);
    pthread_exit (NULL);
}

void *
pth_rtn (void *p)
{
    int c = 0;
    pthread_t pt = pthread_self ();
    while (c < 11) {
        printf ("%d: pthread_self = %p\n", getpid (), (void *)&pt);
        sleep (1);
        c++;
    }
    printf ("%d: pthread_self[exit](%p) = %p\n", getpid (), p, (void *)&pt);
    pthread_exit (NULL);
}

/** }@ */
/* caf_tpm.c ends here */
