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
static char Id[] = "$Id: caf_data_var.c 178 2007-01-31 22:17:18Z dmw $";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"
#include "caf/caf_data_conv.h"
#include "caf/caf_data_var.h"


cafvarspace_t *
cafvarspace_new (id, name)
int id;
char *name;
{
    cafvarspace_t *vs = (cafvarspace_t *)NULL;
    if (name != (char *)NULL && id > 0) {
        vs = xmalloc(CAF_VARSPACE_SZ);
        if (vs != (cafvarspace_t *)NULL) {
            vs->id = id;
            vs->name = name;
            vs->vars = caflst_new((void *)NULL);
            return vs;
        }
    }
    return (cafvarspace_t *)NULL;
}


void
cafvarspace_delete (vs)
cafvarspace_t *vs;
{
    if (vs != (cafvarspace_t *)NULL) {
        vs->id = (int)NULL;
        if (vs->name != (char *)NULL) {
            xstrdestroy(vs->name);
            vs->name = (char *)NULL;
        }
        caflst_destroy(vs->vars, cafvar_delete);
    }
    return (cafvarspace_t *)NULL;
}


cafvar_t *
cafvar_new (name, value, type)
char *name;
void *value;
int type;
{
    cafvar_t *var;
    if (name != (char *)NULL && value != (char *)NULL && type > 0) {
        var = (cafvar_t *)xmalloc (CAF_VARIABLE_SZ);
        if (var != (cafvar_t *)NULL) {
            var->name = strdup(name);
            if (var->name != (char *)NULL) {
                var->value = (void *)NULL;
                switch (type) {
                    /*
                    #define    CAFV_NATIVE_BYTE           unsigned int:8
                    #define    CAFV_NATIVE_CHAR           char
                    #define    CAFV_NATIVE_UCHAR          unsigned char
                    #define    CAFV_NATIVE_WCHAR          w_char_t
                    #define    CAFV_NATIVE_WUCHAR         unsigned w_char_t
                    #define    CAFV_NATIVE_SHORT          short
                    #define    CAFV_NATIVE_INT            int
                    #define    CAFV_NATIVE_UINT           unsigned int
                    #define    CAFV_NATIVE_LONG           long
                    #define    CAFV_NATIVE_ULONG          unsigned long
                    #define    CAFV_NATIVE_FLOAT          float
                    #define    CAFV_NATIVE_DOUBLE         double
                    #define    CAFV_NATIVE_STRING         char *
                    */
                case CAVF_BOOLEAN:
                    var->value = xmalloc(sizeof(CAFV_NATIVE_BOOLEAN));
                    if (var->value != (void *)NULL) {
                        *(var->value) = *((CAFV_NATIVE_BOOLEAN *)value);
                    }
                case CAFV_LONG:
                    var->value = (void *)str2long(value, 10);
                    var->type = CAFV_LONG;
                    break;
                case CAFV_FLOAT:
                    var->value = (void *)str2float(value);
                    var->type = CAFV_FLOAT;
                    break;
                case CAFV_DOUBLE:
                    var->value = (void *)str2double(value);
                    var->type = CAFV_DOUBLE;
                    break;
                case CAFV_STRING:
                    var->value = (void *)strdup(value);
                    var->type = CAFV_STRING;
                    break;
                case CAFV_BOOLEAN:
                    var->value = (void *)str2long(value, 10);
                    var->type = CAFV_BOOLEAN;
                    break;
                default:
                    xfree (var->name);
                    xfree (var);
                    return (cafvar_t *)NULL;
                }
                if (var->value != (void *)NULL) {
                    return var;
                }
            }
            xfree (var->name);
            xfree (var);
            return (cafvar_t *)NULL;
        }
    }
    return (cafvar_t *)NULL;
}



int
cafvar_delete (var)
cafvar_t *var;
{
    if (var != (cafvar_t *)NULL) {
        if (var->name != (char *)NULL) {
            xstrdestroy((char *)(var->name));
            return 1;
        }
        if (var->value != (char *)NULL) {
            xstrdestroy((char *)(var->value));
            return 1;
        }
        xdestroy(var, CAF_VARIABLE_SZ);
        return 1;
    }
    return 0;
}



int
cafvar_destroycb (data)
void *data;
{
    cafvar_t *var;
    if (data != (cafvar_t *)NULL) {
        var = (cafvar_t *)data;
        cafvar_delete(var);
        return 1;
    }
    return 0;
}


void
cafvar_dump (out, var)
FILE *out;
void *var;
{
    cafvar_t *n;
    if (var != (void *)NULL && out != (FILE *)NULL) {
        n = (cafvar_t *)var;
        if (n->name != (char *)NULL && n->value != (void *)NULL) {
            switch (n->type) {
            case CAFV_LONG:
                fprintf (out, "%p: [%p] '%s' = [%p] %li\n", (void *)n,
                         n->name, n->name, n->value, *(long *)n->value);
                break;
            case CAFV_FLOAT:
                fprintf (out, "%p: [%p] '%s' = [%p] %g\n", (void *)n,
                         n->name, n->name, n->value, *(float *)n->value);
                break;
            case CAFV_DOUBLE:
                fprintf (out, "%p: [%p] '%s' = [%p] %g\n", (void *)n,
                         n->name, n->name, n->value, *(double *)n->value);
                break;
            case CAFV_STRING:
                fprintf (out, "%p: [%p] '%s' = [%p] %s\n", (void *)n,
                         n->name, n->name, n->value, (char *)n->value);
                break;
            case CAFV_BOOLEAN:
                fprintf (out, "%p: [%p] '%s' = [%p] %li\n", (void *)n,
                         n->name, n->name, n->value, *(long *)n->value);
                break;
            default:
                fprintf (out, "%p: [%p] '%s' = [%p] (unknown type)\n", (void *)n,
                         n->name, n->name, n->value);
                break;
            }
        } else if (n->name != (char *)NULL && n->value == (void *)NULL) {
            printf("%p: [%p] '%s' = null\n", (void *)n, (void *)n->name,
                   n->name);
        } else {
            printf("%p: null = null (error in variable)\n", (void *)n);
        }
    }
}


void
cafvar_init (void) {
    _cafvar_list = caflst_new((void *)NULL);
}



void
cafvar_finish (void) {
    caflst_delete(_cafvar_list, cafvar_destroycb);
}



void
cafvar_dump (out)
FILE *out;
{
    caflst_dump(out, _cafvar_list, _cafvar_dump);
}


int
cafvar_add (name, value, type)
char *name;
char *value;
int type;
{
    cafvar_t *var;
    var = _cafvar_new(name, value, type);
    if (var != (cafvar_t *)NULL) {
        caflst_push(_cafvar_list, var);
    }
    return 0;
}



cafvar_t *
cafvar_get (name)
char *name;
{
    caflstn_t *n;
    cafvar_t *var;
    n = _cafvar_list->frst;
    while (n != (caflstn_t *)NULL) {
        var = (cafvar_t *)n->data;
        if (!strcmp(name, var->name))
            return var;
        n = n->next;
    }
    return (cafvar_t *)NULL;
}



void *
cafvar_getvalue (name)
char *name;
{
    cafvar_t *var;
    var = cafvar_get(name);
    if (var != (cafvar_t *)NULL)
        return var->value;
    return (char *)NULL;
}



void *
cafvar_setvalue (name, value)
char *name;
char *value;
{
    cafvar_t *var;
    var = cafvar_get(name);
    if (var != (cafvar_t *)NULL) {
        if (var->value != (char *)NULL)
            xstrdestroy(var->value);
        var->value = value;
        return var->value;
    }
    return (char *)NULL;
}

/* caf_data_var.c ends here */
