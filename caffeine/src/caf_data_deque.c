/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Caffeine - C Application Framework
  Copyright (C) 2006 Daniel Molina Wegener <dmw@coder.cl>

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
#include "caf/caf_data_deque.h"


deque_t *
deque_new (void *data) {
	deque_t *lst;
	caf_dequen_t *n;
	lst = (deque_t *)xmalloc (CAF_DEQUE_SZ);
	if (lst != NULL) {
		n = (caf_dequen_t *)xmalloc (CAF_CAF_DEQUENODE_SZ);
		if (n != (caf_dequen_t *)NULL) {
			if (data != (void *)NULL) {
				n->data = data;
			}
			n->prev = (void *)NULL;
			n->next = (void *)NULL;
			lst->head = n;
			lst->tail = n;
			lst->size = 1;
		} else {
			free (lst);
			lst = (deque_t *)NULL;
			n = (caf_dequen_t *)NULL;
		}
	}
	return lst;
}


deque_t *
deque_create (void) {
	deque_t *lst;
	lst = (deque_t *)xmalloc (CAF_DEQUE_SZ);
	if (lst != NULL) {
		lst->head = (caf_dequen_t *)NULL;
		lst->tail = (caf_dequen_t *)NULL;
		lst->size = 0;
	}
	return lst;
}


int
deque_delete (deque_t *lst, CAF_CAF_DEQUENODE_CBDEL(del)) {
	caf_dequen_t *cur, *destroy;
	int cnt = 0;
	if (lst != (deque_t *)NULL) {
		cur = lst->head;
		while (cur != (caf_dequen_t *)NULL) {
			destroy = cur;
			cur = cur->next;
			if ((del (destroy->data)) == CAF_OK) {
				xfree(destroy);
				cnt++;
			} else {
				lst->head = destroy;
				return cnt;
			}
		}
		if (cur != (caf_dequen_t *)NULL) {
			if ((del (cur->data)) == CAF_OK) {
				xfree (cur);
				cnt++;
			} else {
				lst->head = cur;
				return cnt;
			}
		}
		xfree(lst);
		lst = (deque_t *)NULL;
		return CAF_OK;
	}
	return CAF_ERROR_SUB;
}


int
deque_delete_nocb (deque_t *lst) {
	caf_dequen_t *cur, *destroy;
	int cnt = 0;
	if (lst != (deque_t *)NULL) {
		cur = lst->head;
		while (cur != (caf_dequen_t *)NULL) {
			cnt++;
			destroy = cur;
			cur = cur->next;
			xfree(destroy);
		}
		if (cur != (caf_dequen_t *)NULL) {
			cnt++;
			xfree (cur);
		}
		xfree(lst);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
deque_node_delete (deque_t *lst, caf_dequen_t *n, CAF_CAF_DEQUENODE_CBDEL(del)) {
	caf_dequen_t *nr;
	caf_dequen_t *prev;
	caf_dequen_t *next;
	if (lst != (deque_t *)NULL && n != (void *)NULL && del != NULL) {
		nr = lst->head;
		if (nr != (caf_dequen_t *)NULL) {
			while (nr != (caf_dequen_t *)NULL) {
				if (nr != (caf_dequen_t *)NULL) {
					if (nr == n) {
						if ((del (nr->data)) == CAF_OK) {
							prev = nr->prev;
							next = nr->next;
							if (prev != (caf_dequen_t *)NULL &&
								next != (caf_dequen_t *)NULL) {
								prev->next = next;
								next->prev = prev;
							} else if (prev != (caf_dequen_t *)NULL &&
							           next == (caf_dequen_t *)NULL) {
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
deque_node_delete_by_data (deque_t *lst, void *n, CAF_CAF_DEQUENODE_CBDEL(del)) {
	caf_dequen_t *nr;
	caf_dequen_t *prev;
	caf_dequen_t *next;
	if (lst != (deque_t *)NULL && n != (void *)NULL && del != NULL) {
		nr = lst->head;
		if (nr != (caf_dequen_t *)NULL) {
			while (nr != (caf_dequen_t *)NULL) {
				if (nr != (caf_dequen_t *)NULL) {
					if (nr->data == n) {
						if ((del (nr->data)) == CAF_OK) {
							prev = nr->prev;
							next = nr->next;
							if (prev != (caf_dequen_t *)NULL &&
								next != (caf_dequen_t *)NULL) {
								prev->next = next;
								next->prev = prev;
							} else if (prev != (caf_dequen_t *)NULL &&
							           next == (caf_dequen_t *)NULL) {
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
deque_empty_list (deque_t *lst) {
	if (lst != (deque_t *)NULL) {
		return ((lst->tail == lst->head && lst->tail == (caf_dequen_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
deque_oneitem_list (deque_t *lst) {
	if (lst != (deque_t *)NULL) {
		return ((lst->tail == lst->head && lst->tail != (caf_dequen_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
deque_length (deque_t *lst) {
	int c;
	caf_dequen_t *cur;
	if (lst != (deque_t *)NULL) {
		cur = lst->head;
		c = 0;
		while (cur != (caf_dequen_t *)NULL) {
			cur = cur->next;
			c++;
		}
		return c;
	}
	return 0;
}


deque_t *
deque_push (deque_t *lst, void *data) {
	caf_dequen_t *tail, *xnew;
	if (lst != (deque_t *)NULL) {
		xnew = (caf_dequen_t *)xmalloc (CAF_CAF_DEQUENODE_SZ);
		if (xnew != (caf_dequen_t *)NULL) {
			if (lst->tail != (caf_dequen_t *)NULL &&
				lst->head != (caf_dequen_t *)NULL) {
				tail = lst->tail;
				tail->next = xnew;
				xnew->prev = tail;
				xnew->next = (caf_dequen_t *)NULL;
				xnew->data = data;
				lst->tail = xnew;
				lst->size++;
			} else {
				xnew->prev = (caf_dequen_t *)NULL;
				xnew->next = (caf_dequen_t *)NULL;
				xnew->data = data;
				lst->head = xnew;
				lst->tail = xnew;
				lst->size++;
			}
			return lst;
		}
	}
	return (deque_t *)NULL;
}


caf_dequen_t *
deque_pop (deque_t *lst) {
	caf_dequen_t *ret = (caf_dequen_t *)NULL;
	caf_dequen_t *ex;
	if (lst != (deque_t *)NULL) {
		ret = lst->tail;
		if (ret != (caf_dequen_t *)NULL) {
			ex = ret->prev;
			if (ex != (caf_dequen_t *)NULL) {
				ret->next = (void *)NULL;
				ret->prev = (void *)NULL;
				ex->next = (void *)NULL;
				lst->tail = ex;
				lst->size--;
			}
		}
	}
	return ret;
}


caf_dequen_t *
deque_first (deque_t *lst) {
	caf_dequen_t *ret = (caf_dequen_t *)NULL;
	caf_dequen_t *ex;
	if (lst != (deque_t *)NULL) {
		ret = lst->head;
		if (ret != (caf_dequen_t *)NULL) {
			ex = ret->next;
			if (ex != (caf_dequen_t *)NULL) {
				ret->next = (void *)NULL;
				ret->prev = (void *)NULL;
				ex->prev = (void *)NULL;
				lst->head = ex;
				lst->size--;
			}
		}
	}
	return ret;
}


int
deque_set (deque_t *lst, int pos, void *data) {
	caf_dequen_t *pn;
	int c;
	if (lst != (deque_t *)NULL) {
		c = 0;
		pn = lst->head;
		while (pn != (caf_dequen_t *)NULL) {
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


int
deque_insert (deque_t *lst, int pos, void *data) {
	caf_dequen_t *pn, *xnew;
	int c;
	if (lst != (deque_t *)NULL) {
		c = 0;
		pn = lst->head;
		while (pn != (caf_dequen_t *)NULL) {
			if (pos == c) {
				xnew = (caf_dequen_t *)xmalloc (CAF_CAF_DEQUENODE_SZ);
				xnew->data = data;
				xnew->prev = pn->prev;
				xnew->next = pn;
				pn->prev = xnew;
				return pos;
			}
			pn = pn->next;
			c++;
		}
	}
	return CAF_ERROR_SUB;
}


void *
deque_get (deque_t *lst, int pos) {
	caf_dequen_t *pn;
	int c;
	if (lst != (deque_t *)NULL) {
		c = 0;
		pn = lst->head;
		while (pn != (caf_dequen_t *)NULL) {
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
deque_map (deque_t *lst, CAF_CAF_DEQUENODE_CBMAP(step)) {
	int c = 0;
	caf_dequen_t *n;
	if (lst != (deque_t *)NULL) {
		n = lst->head;
		while (n != (caf_dequen_t *)NULL) {
			step (n->data);
			n = n->next;
			c++;
		}
		return c;
	}
	return c;
}


int
deque_map_checked (deque_t *lst, CAF_CAF_DEQUENODE_CBMAP(step)) {
	int c = 0;
	caf_dequen_t *n;
	if (lst != (deque_t *)NULL) {
		n = lst->head;
		while (n != (caf_dequen_t *)NULL) {
			if ((step (n->data)) == CAF_OK) {
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
deque_search (deque_t *lst, void *data, CAF_CAF_DEQUENODE_CBSRCH(srch)) {
	int c = 0;
	caf_dequen_t *n;
	if (lst != (deque_t *)NULL) {
		n = lst->head;
		while (n != (caf_dequen_t *)NULL) {
			if ((srch (n->data, data)) == CAF_OK) {
				return n->data;
			}
			n = n->next;
			c++;
		}
	}
	return (void *)NULL;
}


caf_dequen_t *
deque_search_node (deque_t *lst, void *data) {
	int c = 0;
	caf_dequen_t *n;
	if (lst != (deque_t *)NULL) {
		n = lst->head;
		while (n != (caf_dequen_t *)NULL) {
			if (n->data == data) {
				return n;
			}
			n = n->next;
			c++;
		}
		return (caf_dequen_t *)NULL;
	}
	return (caf_dequen_t *)NULL;
}


int
deque_delete_cb (void *ptr) {
	xfree(ptr);
	return CAF_OK;
}


int
deque_str_delete_cb (void *ptr) {
	xstrdestroy(ptr);
	return CAF_OK;
}


void
deque_dump (FILE *out, deque_t *lst, CAF_CAF_DEQUENODE_CBDUMP(dmp)) {
	caf_dequen_t *cur;
	if (lst != (deque_t *)NULL) {
		cur = lst->head;
		dmp(out, cur->data);
		while (cur != (caf_dequen_t *)NULL) {
			dmp(out, cur->data);
			cur = cur->next;
		}
	}
}


int
deque_dump_ptr (FILE *out, deque_t *lst) {
	int c = 0, a = 0;
	caf_dequen_t *cur;
	if (lst != (deque_t *)NULL && out != (FILE *)NULL) {
		cur = lst->head;
		a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
		              (void *)cur, (void *)cur->next, cur->data);
		while ((cur = cur->next) != (caf_dequen_t *)NULL) {
			c++;
			a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
			              (void *)cur, (void *)cur->next, cur->data);
		}
	}
	return a;
}


int
deque_dump_str_cb (FILE *o, void *data) {
	return fprintf (o, "%p = [len: %d] \"%s\"\n", data,
	                (int)strlen((char *)data), (char *)data);
}

/* caf_data_deque.c ends here */

