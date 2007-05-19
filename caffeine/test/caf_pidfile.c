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

void test_pidfile (void);

void test_kill_handler (int sig);

int
main (void)
{
    signal (SIGHUP, test_kill_handler);
    test_pidfile ();
    return 0;
}

void
test_pidfile (void)
{
    pidfile_create ("testpidfile.pid", getpid());
    usleep (1000);
    printf ("pid: %d\n", pidfile_getpid ("testpidfile.pid"));
    usleep (1000);
    pidfile_kill ("testpidfile.pid", SIGHUP);
    usleep (1000);
    printf ("sid: %d\n", pidfile_getsid ("testpidfile.pid"));
    usleep (1000);
    printf ("gid: %d\n", pidfile_getpgid ("testpidfile.pid"));
    usleep (1000);
    pidfile_destroy ("testpidfile.pid");
}

void
test_kill_handler (int sig)
{
    printf ("signal: %d\n", sig);
}

/* caf_pidfile.c ends here */
