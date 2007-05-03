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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"


lstdl_t *
lstdl_new (void *data)
{
    lstdl_t *lst;
    lstdln_t *n;
    lst = (lstdl_t *)xmalloc (CAF_LSTDL_SZ);
    if (lst != NULL) {
        n = (lstdln_t *)xmalloc (CAF_LSTDLNODE_SZ);
        if (n != (lstdln_t *)NULL) {
            if (data != (void *)NULL) {
                n->data = data;
            }
            n->prev = (void *)NULL;
            n->next = (void *)NULL;
            lst->frst = n;
            lst->last = n;
            lst->size = 1;
        } else {
            free (lst);
            lst = (lstdl_t *)NULL;
            n = (lstdln_t *)NULL;
        }
    }
    return lst;
}


lstdl_t *
lstdl_create (void)
{
    lstdl_t *lst;
    lst = (lstdl_t *)xmalloc (CAF_LSTDL_SZ);
    if (lst != NULL) {
        lst->frst = (lstdln_t *)NULL;
        lst->last = (lstdln_t *)NULL;
        lst->size = 0;
    }
    return lst;
}


int
lstdl_delete (lstdl_t *lst, CAF_LSTDLNODE_CBDEL(del))
{
    lstdln_t *cur, *destroy;
    int cnt = 0;
    if (lst != (lstdl_t *)NULL) {
        cur = lst->frst;
        while (cur != (lstdln_t *)NULL) {
            destroy = cur;
            cur = cur->next;
            if ((del (destroy->data)) == 0) {
                xfree(destroy);
                cnt++;
            } else {
                lst->frst = destroy;
                return cnt;
            }
        }
        if (cur != (lstdln_t *)NULL) {
            if ((del (cur->data)) == 0) {
                xfree (cur);
                cnt++;
            } else {
                lst->frst = cur;
                return cnt;
            }
        }
        xfree(lst);
        lst = (lstdl_t *)NULL;
        return CAF_OK;
    }
    return CAF_ERROR_SUB;
}


int
lstdl_delete_nocb (lstdl_t *lst)
{
    lstdln_t *cur, *destroy;
    int cnt = 0;
    if (lst != (lstdl_t *)NULL) {
        cur = lst->frst;
        while (cur != (lstdln_t *)NULL) {
            cnt++;
            destroy = cur;
            cur = cur->next;
            xfree(destroy);
        }
        if (cur != (lstdln_t *)NULL) {
            cnt++;
            xfree (cur);
        }
        xfree(lst);
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
lstdl_node_delete (lstdl_t *lst, lstdln_t *n, CAF_LSTDLNODE_CBDEL(del))
{
    lstdln_t *nr;
    lstdln_t *prev;
    lstdln_t *next;
    if (lst != (lstdl_t *)NULL && n != (void *)NULL && del != NULL) {
        nr = lst->frst;
        if (nr != (lstdln_t *)NULL) {
            while (nr != (lstdln_t *)NULL) {
                if (nr != (lstdln_t *)NULL) {
                    if (nr == n) {
                        if ((del (nr->data)) == 0) {
                            prev = nr->prev;
                            next = nr->next;
                            if (prev != (lstdln_t *)NULL &&
                                next != (lstdln_t *)NULL) {
                                prev->next = next;
                                next->prev = prev;
                            } else if (prev != (lstdln_t *)NULL &&
                                       next == (lstdln_t *)NULL) {
                                prev->next = next;
                            } else {
                                next->prev = prev;
                            }
                            xfree (nr);
                            lst->size--;
                            return CAF_OK;
                        }
                    }
                }
                nr = nr->next;
            }
        }
    }
    return CAF_ERROR;
}


int
lstdl_node_delete_by_data (lstdl_t *lst, void *n, CAF_LSTDLNODE_CBDEL(del))
{
    lstdln_t *nr;
    lstdln_t *prev;
    lstdln_t *next;
    if (lst != (lstdl_t *)NULL && n != (void *)NULL && del != NULL) {
        nr = lst->frst;
        if (nr != (lstdln_t *)NULL) {
            while (nr != (lstdln_t *)NULL) {
                if (nr != (lstdln_t *)NULL) {
                    if (nr->data == n) {
                        if ((del (nr->data)) == 0) {
                            prev = nr->prev;
                            next = nr->next;
                            if (prev != (lstdln_t *)NULL &&
                                next != (lstdln_t *)NULL) {
                                prev->next = next;
                                next->prev = prev;
                            } else if (prev != (lstdln_t *)NULL &&
                                       next == (lstdln_t *)NULL) {
                                prev->next = next;
                            } else {
                                next->prev = prev;
                            }
                            xfree (nr);
                            lst->size--;
                            return CAF_OK;
                        }
                    }
                }
                nr = nr->next;
            }
        }
    }
    return CAF_ERROR;
}


int
lstdl_length (lstdl_t *lst)
{
    int c;
    lstdln_t *cur;
    if (lst != (lstdl_t *)NULL) {
        cur = lst->frst;
        c = 0;
        while (cur != (lstdln_t *)NULL) {
            cur = cur->next;
            c++;
        }
        return c;
    }
    return 0;
}


lstdl_t *
lstdl_push (lstdl_t *lst, void *data)
{
    lstdln_t *last, *xnew;
    if (lst != (lstdl_t *)NULL) {
        xnew = (lstdln_t *)xmalloc (CAF_LSTDLNODE_SZ);
        if (xnew != (lstdln_t *)NULL) {
            if (lst->last != (lstdln_t *)NULL &&
                lst->frst != (lstdln_t *)NULL) {
                last = lst->last;
                last->next = xnew;
                xnew->prev = last;
                xnew->next = (lstdln_t *)NULL;
                xnew->data = data;
                lst->last = xnew;
                lst->size++;
            } else {
                xnew->prev = (lstdln_t *)NULL;
                xnew->next = (lstdln_t *)NULL;
                xnew->data = data;
                lst->frst = xnew;
                lst->last = xnew;
                lst->size++;
            }
            return lst;
        }
    }
    return (lstdl_t *)NULL;
}


lstdln_t *
lstdl_pop (lstdl_t *lst)
{
    lstdln_t *ret = (lstdln_t *)NULL;
    lstdln_t *ex;
    if (lst != (lstdl_t *)NULL) {
        ret = lst->last;
        if (ret != (lstdln_t *)NULL) {
            ex = ret->prev;
            if (ex != (lstdln_t *)NULL) {
                ret->next = (void *)NULL;
                ret->prev = (void *)NULL;
                ex->next = (void *)NULL;
                lst->last = ex;
                lst->size--;
            }
        }
    }
    return ret;
}


lstdln_t *
lstdl_first (lstdl_t *lst)
{
    lstdln_t *ret = (lstdln_t *)NULL;
    lstdln_t *ex;
    if (lst != (lstdl_t *)NULL) {
        ret = lst->frst;
        if (ret != (lstdln_t *)NULL) {
            ex = ret->next;
            if (ex != (lstdln_t *)NULL) {
                ret->next = (void *)NULL;
                ret->prev = (void *)NULL;
                ex->prev = (void *)NULL;
                lst->frst = ex;
                lst->size--;
            }
        }
    }
    return ret;
}


int
lstdl_set (lstdl_t *lst, int pos, void *data)
{
    lstdln_t *pn;
    int c;
    if (lst != (lstdl_t *)NULL) {
        c = 0;
        pn = lst->frst;
        while (pn != (lstdln_t *)NULL) {
            if (pos == c) {
                pn->data = data;
                return pos;
            }
            pn = pn->next;
            c++;
        }
    }
    return CAF_ERROR_SUB;
}


void *
lstdl_get (lstdl_t *lst, int pos)
{
    lstdln_t *pn;
    int c;
    if (lst != (lstdl_t *)NULL) {
        c = 0;
        pn = lst->frst;
        while (pn != (lstdln_t *)NULL) {
            if (pos == c) {
                return pn->data;
            }
            pn = pn->next;
            c++;
        }
    }
    return (void *)NULL;
}


int
lstdl_walk (lstdl_t *lst, CAF_LSTDLNODE_CBWALK(step))
{
    int c = 0;
    lstdln_t *n;
    if (lst != (lstdl_t *)NULL) {
        n = lst->frst;
        while (n != (lstdln_t *)NULL) {
            step (n->data);
            n = n->next;
            c++;
        }
        return c;
    }
    return c;
}


int
lstdl_walk_checked (lstdl_t *lst, CAF_LSTDLNODE_CBWALK(step))
{
    int c = 0;
    lstdln_t *n;
    if (lst != (lstdl_t *)NULL) {
        n = lst->frst;
        while (n != (lstdln_t *)NULL) {
            if ((step (n->data)) == 0) {
                n = n->next;
                c++;
            } else {
                return c;
            }
        }
        return c;
    }
    return c;
}


void *
lstdl_search (lstdl_t *lst, void *data, CAF_LSTDLNODE_CBSRCH(srch))
{
    int c = 0;
    lstdln_t *n;
    if (lst != (lstdl_t *)NULL) {
        n = lst->frst;
        while (n != (lstdln_t *)NULL) {
            if ((srch (n->data, data)) == 0) {
                return n->data;
            }
            n = n->next;
            c++;
        }
    }
    return (void *)NULL;
}


lstdln_t *
lstdl_search_node (lstdl_t *lst, void *data)
{
    int c = 0;
    lstdln_t *n;
    if (lst != (lstdl_t *)NULL) {
        n = lst->frst;
        while (n != (lstdln_t *)NULL) {
            if (n->data == data) {
                return n;
            }
            n = n->next;
            c++;
        }
        return (lstdln_t *)NULL;
    }
    return (lstdln_t *)NULL;
}


int
lstdl_delete_cb (void *ptr)
{
    xfree(ptr);
    return CAF_OK;
}


int
lstdl_str_delete_cb (void *ptr)
{
    xstrdestroy(ptr);
    return CAF_OK;
}


void
lstdl_dump (FILE *out, lstdl_t *lst, CAF_LSTDLNODE_CBDUMP(dmp))
{
    lstdln_t *cur;
    if (lst != (lstdl_t *)NULL) {
        cur = lst->frst;
        dmp(out, cur->data);
        while ((cur = cur->next) != (lstdln_t *)NULL) {
            dmp(out, cur->data);
        }
    }
}


int
lstdl_dump_ptr (FILE *out, lstdl_t *lst)
{
    int c = 0, a = 0;
    lstdln_t *cur;
    if (lst != (lstdl_t *)NULL && out != (FILE *)NULL) {
        cur = lst->frst;
        a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
                      (void *)cur, (void *)cur->next, cur->data);
        while ((cur = cur->next) != (lstdln_t *)NULL) {
            c++;
            a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
                          (void *)cur, (void *)cur->next, cur->data);
        }
    }
    return a;
}


int
lstdl_dump_str_cb (FILE *o, void *data)
{
    return fprintf (o, "%p = [len: %d] \"%s\"\n", data,
                    (int)strlen((char *)data), (char *)data);
}

/* caf_data_lstdl.c ends here */
