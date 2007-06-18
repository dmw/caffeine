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
#include <caf/caf_hash_str.h>

int
main (int argc, char **argv)
{
   char string[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

   printf (" 1. RS Hash:   %u\n", caf_shash_rs (string, 36));
   printf (" 2. JS Hash:   %u\n", caf_shash_js (string, 36));
   printf (" 3. PJW Hash:  %u\n", caf_shash_pjw (string, 36));
   printf (" 4. ELF Hash:  %u\n", caf_shash_elf (string, 36));
   printf (" 5. BKDR Hash: %u\n", caf_shash_bkdr (string, 36));
   printf (" 6. SDBM Hash: %u\n", caf_shash_sdbm (string, 36));
   printf (" 7. DJB Hash:  %u\n", caf_shash_djb (string, 36));
   printf (" 8. DEK Hash:  %u\n", caf_shash_dek (string, 36));
   printf (" 9. BP Hash:   %u\n", caf_shash_bp (string, 36));
   printf ("10. FNV Hash:  %u\n", caf_shash_fnv (string, 36));

   return 0;
}
