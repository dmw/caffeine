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
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_process_pool.h"


proc_info_t *
ppm_pinfo_new (int num, pid_t pid, pid_t pgrp, lstdl_t *lst,
               CAF_ENTRY_POINT(point))
{
    proc_info_t *nfo = (proc_info_t *)NULL;
    if (pid > 0 && point) {
        nfo = (proc_info_t *)xmalloc (CAF_PINFO_SZ);
        if (nfo != (proc_info_t *)NULL) {
            nfo->number = num;
            nfo->pid = pid;
            nfo->pgrp = pgrp;
            nfo->lst = lst;
            nfo->point = point;
        }
    }
    return nfo;
}


void
ppm_info_delete (proc_info_t *nfo)
{
    if (nfo != (proc_info_t *)NULL) {
        xfree (nfo);
    }
}


proc_info_t *
ppm_daemonize (int num, pid_t pgrp, lstdl_t *lst, CAF_ENTRY_POINT(pt))
{
    proc_info_t *nfo = (proc_info_t *)NULL;
    pid_t pid = 0;
    if (pt) {
        pid = fork();
        switch (pid) {
            case -1:
                pid = 0;
                break;
            case 0:
#ifndef CAFFEINE_DEBUG
                fclose (stdout);
                fclose (stderr);
#endif
#ifdef LINUX
                setpgid (getpid(), pgrp);
#endif /* !LINUX */
                pt (lst);
                break;
            default:
                nfo = ppm_pinfo_new (num, pid, pgrp, lst, pt);
#ifdef LINUX
                setpgrp (pid, pgrp);
#endif /* !LINUX */
                break;
        }
    }
    return nfo;
}


int
ppm_kill (proc_info_t *nfo, int sig)
{
    if (nfo != (proc_info_t *)NULL) {
        return kill (nfo->pid, sig);
    }
    return CAF_ERROR;
}


lstdl_t *
ppm_pool_create (int c, lstdl_t *plst, CAF_ENTRY_POINT(pt))
{
    int count = 0;
    lstdl_t *lst = (lstdl_t *)NULL;
    proc_info_t *nfo = (proc_info_t *)NULL;
    pid_t pgrp = 0;
    if (c > 0 && pt) {
        pgrp = setsid ();
        lst = lstdl_create ();
        for (count = 1; count <= c; count++) {
            nfo = ppm_daemonize (count, pgrp, plst, pt);
            if (nfo != (proc_info_t *)NULL) {
                lstdl_push (lst, nfo);
            }
        }
    }
    return lst;
}


int
ppm_kill_pool (lstdl_t *lst, int sig)
{
    int c = 0;
    lstdln_t *n;
    proc_info_t *nfo;
    if (lst != (lstdl_t *)NULL && sig > 0) {
        n = lst->head;
        while (n != (lstdln_t *)NULL) {
            nfo = (proc_info_t *)n->data;
            ppm_kill (nfo, sig);
            n = n->next;
        }
    }
    return c;
}


int
ppm_kill_group (int sig)
{
    return killpg (getpgrp (), sig);
}

/* caf_process_pool.c ends here */
