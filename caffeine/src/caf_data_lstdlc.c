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
static char Id[] = "$Id: caf_data_lstdl.c 8 2007-05-03 00:50:25Z damowe $";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdlc.h"


lstdlc_t *
lstdlc_new (void *data)
{
    lstdlc_t *lst;
    lstdlcn_t *n;
    lst = (lstdlc_t *)xmalloc (CAF_LSTDLC_SZ);
    if (lst != NULL) {
        n = (lstdlcn_t *)xmalloc (CAF_LSTDLCNODE_SZ);
        if (n != (lstdlcn_t *)NULL) {
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
            lst = (lstdlc_t *)NULL;
            n = (lstdlcn_t *)NULL;
        }
    }
    return lst;
}


lstdlc_t *
lstdlc_create (void)
{
    lstdlc_t *lst;
    lst = (lstdlc_t *)xmalloc (CAF_LSTDLC_SZ);
    if (lst != NULL) {
        lst->frst = (lstdlcn_t *)NULL;
        lst->last = (lstdlcn_t *)NULL;
        lst->size = 0;
    }
    return lst;
}


int
lstdlc_delete (lstdlc_t *lst, CAF_LSTDLCNODE_CBDEL(del))
{
    lstdlcn_t *cur, *destroy;
    int cnt = 0;
    if (lst != (lstdlc_t *)NULL) {
        cur = lst->frst;
        while (cur != (lstdlcn_t *)NULL) {
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
        if (cur != (lstdlcn_t *)NULL) {
            if ((del (cur->data)) == 0) {
                xfree (cur);
                cnt++;
            } else {
                lst->frst = cur;
                return cnt;
            }
        }
        xfree(lst);
        lst = (lstdlc_t *)NULL;
        return CAF_OK;
    }
    return CAF_ERROR_SUB;
}


int
lstdlc_delete_nocb (lstdlc_t *lst)
{
    lstdlcn_t *cur, *destroy;
    int cnt = 0;
    if (lst != (lstdlc_t *)NULL) {
        cur = lst->frst;
        while (cur != (lstdlcn_t *)NULL) {
            cnt++;
            destroy = cur;
            cur = cur->next;
            xfree(destroy);
        }
        if (cur != (lstdlcn_t *)NULL) {
            cnt++;
            xfree (cur);
        }
        xfree(lst);
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
lstdlc_node_delete (lstdlc_t *lst, lstdlcn_t *n, CAF_LSTDLCNODE_CBDEL(del))
{
    lstdlcn_t *nr;
    lstdlcn_t *prev;
    lstdlcn_t *next;
    if (lst != (lstdlc_t *)NULL && n != (void *)NULL && del != NULL) {
        nr = lst->frst;
        if (nr != (lstdlcn_t *)NULL) {
            while (nr != (lstdlcn_t *)NULL) {
                if (nr != (lstdlcn_t *)NULL) {
                    if (nr == n) {
                        if ((del (nr->data)) == 0) {
                            prev = nr->prev;
                            next = nr->next;
                            if (prev != (lstdlcn_t *)NULL &&
                                next != (lstdlcn_t *)NULL) {
                                prev->next = next;
                                next->prev = prev;
                            } else if (prev != (lstdlcn_t *)NULL &&
                                       next == (lstdlcn_t *)NULL) {
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
lstdlc_node_delete_by_data (lstdlc_t *lst, void *n, CAF_LSTDLCNODE_CBDEL(del))
{
    lstdlcn_t *nr;
    lstdlcn_t *prev;
    lstdlcn_t *next;
    if (lst != (lstdlc_t *)NULL && n != (void *)NULL && del != NULL) {
        nr = lst->frst;
        if (nr != (lstdlcn_t *)NULL) {
            while (nr != (lstdlcn_t *)NULL) {
                if (nr != (lstdlcn_t *)NULL) {
                    if (nr->data == n) {
                        if ((del (nr->data)) == 0) {
                            prev = nr->prev;
                            next = nr->next;
                            if (prev != (lstdlcn_t *)NULL &&
                                next != (lstdlcn_t *)NULL) {
                                prev->next = next;
                                next->prev = prev;
                            } else if (prev != (lstdlcn_t *)NULL &&
                                       next == (lstdlcn_t *)NULL) {
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
lstdlc_length (lstdlc_t *lst)
{
    int c;
    lstdlcn_t *cur;
    if (lst != (lstdlc_t *)NULL) {
        cur = lst->frst;
        c = 0;
        while (cur != (lstdlcn_t *)NULL) {
            cur = cur->next;
            c++;
        }
        return c;
    }
    return 0;
}


lstdlc_t *
lstdlc_push (lstdlc_t *lst, void *data)
{
    lstdlcn_t *last, *xnew;
    if (lst != (lstdlc_t *)NULL) {
        xnew = (lstdlcn_t *)xmalloc (CAF_LSTDLCNODE_SZ);
        if (xnew != (lstdlcn_t *)NULL) {
            if (lst->last != (lstdlcn_t *)NULL &&
                lst->frst != (lstdlcn_t *)NULL) {
                last = lst->last;
                last->next = xnew;
                xnew->prev = last;
                xnew->next = (lstdlcn_t *)NULL;
                xnew->data = data;
                lst->last = xnew;
                lst->size++;
            } else {
                xnew->prev = (lstdlcn_t *)NULL;
                xnew->next = (lstdlcn_t *)NULL;
                xnew->data = data;
                lst->frst = xnew;
                lst->last = xnew;
                lst->size++;
            }
            return lst;
        }
    }
    return (lstdlc_t *)NULL;
}


lstdlcn_t *
lstdlc_pop (lstdlc_t *lst)
{
    lstdlcn_t *ret = (lstdlcn_t *)NULL;
    lstdlcn_t *ex;
    if (lst != (lstdlc_t *)NULL) {
        ret = lst->last;
        if (ret != (lstdlcn_t *)NULL) {
            ex = ret->prev;
            if (ex != (lstdlcn_t *)NULL) {
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


lstdlcn_t *
lstdlc_first (lstdlc_t *lst)
{
    lstdlcn_t *ret = (lstdlcn_t *)NULL;
    lstdlcn_t *ex;
    if (lst != (lstdlc_t *)NULL) {
        ret = lst->frst;
        if (ret != (lstdlcn_t *)NULL) {
            ex = ret->next;
            if (ex != (lstdlcn_t *)NULL) {
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
lstdlc_set (lstdlc_t *lst, int pos, void *data)
{
    lstdlcn_t *pn;
    int c;
    if (lst != (lstdlc_t *)NULL) {
        c = 0;
        pn = lst->frst;
        while (pn != (lstdlcn_t *)NULL) {
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
lstdlc_get (lstdlc_t *lst, int pos)
{
    lstdlcn_t *pn;
    int c;
    if (lst != (lstdlc_t *)NULL) {
        c = 0;
        pn = lst->frst;
        while (pn != (lstdlcn_t *)NULL) {
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
lstdlc_walk (lstdlc_t *lst, CAF_LSTDLCNODE_CBWALK(step))
{
    int c = 0;
    lstdlcn_t *n;
    if (lst != (lstdlc_t *)NULL) {
        n = lst->frst;
        while (n != (lstdlcn_t *)NULL) {
            step (n->data);
            n = n->next;
            c++;
        }
        return c;
    }
    return c;
}


int
lstdlc_walk_checked (lstdlc_t *lst, CAF_LSTDLCNODE_CBWALK(step))
{
    int c = 0;
    lstdlcn_t *n;
    if (lst != (lstdlc_t *)NULL) {
        n = lst->frst;
        while (n != (lstdlcn_t *)NULL) {
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
lstdlc_search (lstdlc_t *lst, void *data, CAF_LSTDLCNODE_CBSRCH(srch))
{
    int c = 0;
    lstdlcn_t *n;
    if (lst != (lstdlc_t *)NULL) {
        n = lst->frst;
        while (n != (lstdlcn_t *)NULL) {
            if ((srch (n->data, data)) == 0) {
                return n->data;
            }
            n = n->next;
            c++;
        }
    }
    return (void *)NULL;
}


lstdlcn_t *
lstdlc_search_node (lstdlc_t *lst, void *data)
{
    int c = 0;
    lstdlcn_t *n;
    if (lst != (lstdlc_t *)NULL) {
        n = lst->frst;
        while (n != (lstdlcn_t *)NULL) {
            if (n->data == data) {
                return n;
            }
            n = n->next;
            c++;
        }
        return (lstdlcn_t *)NULL;
    }
    return (lstdlcn_t *)NULL;
}


int
lstdlc_delete_cb (void *ptr)
{
    xfree(ptr);
    return CAF_OK;
}


int
lstdlc_str_delete_cb (void *ptr)
{
    xstrdestroy(ptr);
    return CAF_OK;
}


void
lstdlc_dump (FILE *out, lstdlc_t *lst, CAF_LSTDLCNODE_CBDUMP(dmp))
{
    lstdlcn_t *cur;
    if (lst != (lstdlc_t *)NULL) {
        cur = lst->frst;
        dmp(out, cur->data);
        while ((cur = cur->next) != (lstdlcn_t *)NULL) {
            dmp(out, cur->data);
        }
    }
}


int
lstdlc_dump_ptr (FILE *out, lstdlc_t *lst)
{
    int c = 0, a = 0;
    lstdlcn_t *cur;
    if (lst != (lstdlc_t *)NULL && out != (FILE *)NULL) {
        cur = lst->frst;
        a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
                      (void *)cur, (void *)cur->next, cur->data);
        while ((cur = cur->next) != (lstdlcn_t *)NULL) {
            c++;
            a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
                          (void *)cur, (void *)cur->next, cur->data);
        }
    }
    return a;
}


int
lstdlc_dump_str_cb (FILE *o, void *data)
{
    return fprintf (o, "%p = [len: %d] \"%s\"\n", data,
                    (int)strlen((char *)data), (char *)data);
}

/* caf_data_lstdl.c ends here */
