/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
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
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstc.h"


lstcn_t *
lstc_new (void *data) {
	lstcn_t *node = (lstcn_t *)xmalloc (CAF_LSTC_SZ);
	if (node != (lstcn_t *)NULL) {
		node->data = data;
		node->next = node;
		node->prev = node;
	}
	return node;
}


lstcn_t *
lstc_create () {
	lstcn_t *node = (lstcn_t *)xmalloc (CAF_LSTC_SZ);
	if (node != (lstcn_t *)NULL) {
		node->data = (void *)NULL;
		node->next = (lstcn_t *)NULL;
		node->prev = (lstcn_t *)NULL;
	}
	return node;
}


int
lstc_delete (lstcn_t *lst, CAF_LSTCNODE_CBDEL(del)) {
	lstcn_t *current = lst;
	lstcn_t *xtodel = (lstcn_t *)NULL;
	if (lst != (lstcn_t *)NULL) {
		do {
			if ((del (current->data)) == CAF_OK) {
				xtodel = current;
				current = current->next;
				xfree(xtodel);
			} else {
				lst = current;
				return CAF_ERROR;
			}
		} while (current != lst);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
lstc_node_delete (lstcn_t *lst, lstcn_t *n, CAF_LSTCNODE_CBDEL(del)) {
	lstcn_t *nr;
	lstcn_t *next;
	lstcn_t *prev;
	nr = lst;
	do {
		if (nr == n) {
			next = nr->next;
			prev = nr->prev;
			next->prev = prev;
			prev->next = next;
			if ((del (nr->data)) == CAF_OK) {
				xfree (nr);
				return CAF_OK;
			} else {
				nr->next = next;
				nr->prev = prev;
				next->prev = nr;
				prev->next = nr;
				return CAF_ERROR;
			}
		}
		nr = nr->next;
	} while (nr != lst);
	return CAF_ERROR;
}


int
lstc_node_delete_by_data (lstcn_t *lst, void *data, CAF_LSTCNODE_CBDEL(del)) {
	lstcn_t *nr;
	lstcn_t *next;
	lstcn_t *prev;
	nr = lst;
	do {
		if (nr != (lstcn_t *)NULL) {
			if (nr->data == data) {
				next = nr->next;
				prev = nr->prev;
				next->prev = prev;
				prev->next = next;
				if ((del (nr->data) == CAF_OK)) {
					xfree (nr);
					return CAF_OK;
				} else {
					nr->next = next;
					nr->prev = prev;
					next->prev = nr;
					prev->next = nr;
					return CAF_ERROR;
				}
			}
			nr = nr->next;
		}
	} while (nr != lst);
	return CAF_ERROR;
}


int
lstc_empty_list (lstcn_t *lst) {
	if (lst != (lstcn_t *)NULL) {
		return ((lst->next == lst->prev && lst->prev == (lstcn_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
lstc_oneitem_list (lstcn_t *lst) {
	if (lst != (lstcn_t *)NULL) {
		return ((lst->next == lst->prev && lst->prev != (lstcn_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
lstc_length (lstcn_t *lst) {
	int c;
	lstcn_t *nr;
	if (lst != (lstcn_t *)NULL) {
		c = 0;
		nr = lst;
		do {
			nr = nr->next;
			c++;
		} while (nr != lst);
		return c;
	}
	return 0;
}


lstcn_t *
lstc_push (lstcn_t *lst, void *data) {
	lstcn_t *last, *xnew;
	if (lst != (lstcn_t *)NULL) {
		if (lstc_empty_list (lst) == CAF_OK) {
			lst->next = lst;
			lst->prev = lst;
			lst->data = data;
		} else {
			xnew = (lstcn_t *)xmalloc (CAF_LSTC_SZ);
			if (xnew != (lstcn_t *)NULL) {
				last = lst->prev;
				last->next = xnew;
				xnew->prev = last;
				xnew->next = lst;
				xnew->data = data;
				lst->prev = xnew;
				return lst;
			}
		}
	}
	return (lstcn_t *)NULL;
}


lstcn_t *
lstc_pop (lstcn_t *lst) {
	lstcn_t *ret = (lstcn_t *)NULL;
	lstcn_t *ex;
	if (lst != (lstcn_t *)NULL) {
		ret = lst->prev;
		if (ret != (lstcn_t *)NULL) {
			ex = ret->prev;
			if (ex != (lstcn_t *)NULL) {
				if (lst != ex->next) {
					ex->next = lst;
				} else {
					ex->next = (void *)NULL;
					ex->prev = (void *)NULL;
				}
				lst->prev = ex;
			}
		}
	}
	return ret;
}


int
lstc_set (lstcn_t *lst, int pos, void *data) {
	lstcn_t *pn = lst;
	int c = 0;
	if (lst != (lstcn_t *)NULL) {
		do {
			if (pos == c) {
				pn->data = data;
				return pos;
			}
			pn = pn->next;
			c++;
		} while (pn != lst);
	}
	return CAF_ERROR_SUB;
}


void *
lstc_get (lstcn_t *lst, int pos) {
	lstcn_t *pn = lst;
	int c = 0;
	if (lst != (lstcn_t *)NULL) {
		do {
			if (pos == c) {
				return pn->data;
			}
			pn = pn->next;
			c++;
		} while (pn != lst);
	}
	return (void *)NULL;
}


int
lstc_walk (lstcn_t *lst, CAF_LSTCNODE_CBWALK(step)) {
	int c = 0;
	lstcn_t *n;
	if (lst != (lstcn_t *)NULL) {
		n = lst;
		do {
			step(n->data);
			n = n->next;
			c++;
		} while (n != lst);
		return c;
	}
	return c;
}


int
lstc_walk_checked (lstcn_t *lst, CAF_LSTCNODE_CBWALK(step)) {
	int c = 0;
	lstcn_t *n;
	if (lst != (lstcn_t *)NULL) {
		n = lst;
		do {
			if ((step(n->data)) == CAF_OK) {
				n = n->next;
				c++;
			} else {
				return c;
			}
		} while (n != lst);
		return c;
	}
	return c;
}


void *
lstc_search (lstcn_t *lst, void *data, CAF_LSTCNODE_CBSRCH(srch)) {
	lstcn_t *n;
	if (lst != (lstcn_t *)NULL) {
		n = lst;
		do {
			if ((srch(n->data, data)) == CAF_OK) {
				return n->data;
			}
		} while (n != lst);
	}
	return (void *)NULL;
}


lstcn_t *
lstc_search_node (lstcn_t *lst, void *data) {
	int c = 0;
	lstcn_t *n;
	if (lst != (lstcn_t *)NULL) {
		n = lst;
		do {
			if (n->data == data) {
				return n;
			}
			n = n->next;
			c++;
		} while (n != lst);
		return (lstcn_t *)NULL;
	}
	return (lstcn_t *)NULL;
}


int
lstc_delete_cb (void *ptr) {
	xfree(ptr);
	return CAF_OK;
}


int
lstc_str_delete_cb (void *ptr) {
	xstrdestroy(ptr);
	return CAF_OK;
}


void
lstc_dump (FILE *out, lstcn_t *lst, CAF_LSTCNODE_CBDUMP(dmp)) {
	lstcn_t *cur = lst;
	if (lst != (lstcn_t *)NULL) {
		if ((lstc_empty_list (lst)) != CAF_OK) {
			do {
				dmp(out, cur->data);
				cur = cur->next;
			} while (cur != lst);
		}
	}
}


int
lstc_dump_ptr (FILE *out, lstcn_t *lst) {
	int c = 0, a = 0;
	lstcn_t *cur;
	if (lst != (lstcn_t *)NULL && out != (FILE *)NULL) {
		if ((lstc_empty_list (lst)) != CAF_OK) {
			c = 1;
			cur = lst;
			a += fprintf (out, "%d: %p < %p > %p\n", c, (void *)cur->prev,
						  (void *)cur, (void *)cur->next);
			while ((cur = cur->next) != lst) {
				c++;
				a += fprintf (out, "%d: %p < %p > %p\n", c, (void *)cur->prev,
							  (void *)cur, (void *)cur->next);
			}
		}
	}
	return a;
}


int
lstc_dump_str_cb (FILE *o, void *data) {
	char *d = (char *)data;
	return fprintf(o, "%p[len:%d] = \"%s\"\n", data, (int)strlen(d),
	               (char *)data);
}

/* caf_data_lstc.c ends here */
