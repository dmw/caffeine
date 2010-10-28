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
static char Id[] = "$Id: caf_data_lstdl.c 8 2007-05-03 00:50:25Z damowe $";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_cdeque.h"


cdeque_t *
cdeque_new (void *data) {
	cdeque_t *lst;
	caf_cdequen_t *n;
	lst = (cdeque_t *)xmalloc (CAF_CDEQUE_SZ);
	if (lst != NULL) {
		n = (caf_cdequen_t *)xmalloc (CAF_LSTDLCNODE_SZ);
		if (n != (caf_cdequen_t *)NULL) {
			if (data != (void *)NULL) {
				n->data = data;
			}
			n->prev = n;
			n->next = n;
			lst->head = n;
			lst->tail = n;
			lst->size = 1;
		} else {
			free (lst);
			lst = (cdeque_t *)NULL;
			n = (caf_cdequen_t *)NULL;
		}
	}
	return lst;
}


cdeque_t *
cdeque_create (void) {
	cdeque_t *lst;
	lst = (cdeque_t *)xmalloc (CAF_CDEQUE_SZ);
	if (lst != NULL) {
		lst->head = (caf_cdequen_t *)NULL;
		lst->tail = (caf_cdequen_t *)NULL;
		lst->size = 0;
	}
	return lst;
}


int
cdeque_delete (cdeque_t *lst, CAF_LSTDLCNODE_CBDEL(del)) {
	caf_cdequen_t *cur, *destroy;
	if (lst != (cdeque_t *)NULL) {
		if (lst->head != (caf_cdequen_t *)NULL
			&& lst->tail != (caf_cdequen_t *)NULL) {
			if (cdeque_empty_list (lst) == CAF_OK) {
				xfree (lst);
				return CAF_OK;
			} else if (cdeque_oneitem_list (lst) == CAF_OK) {
				if (del (lst->head->data) == CAF_OK) {
					xfree (lst->head);
					xfree (lst);
					return CAF_OK;
				}
			} else {
				cur = lst->head;
				do {
					if ((del (cur->data)) == CAF_OK) {
						destroy = cur;
						cur = cur->next;
						xfree(destroy);
					} else {
						lst->head = cur;
						return CAF_ERROR;
					}
				} while (cur != lst->tail);
				if (del (lst->tail->data) == CAF_OK) {
					xfree (lst->tail);
				}
				xfree(lst);
				lst = (cdeque_t *)NULL;
				return CAF_OK;
			}
		} else {
			return CAF_OK;
		}
	}
	return CAF_ERROR_SUB;
}


int
cdeque_delete_nocb (cdeque_t *lst) {
	caf_cdequen_t *cur, *destroy;
	if (lst != (cdeque_t *)NULL) {
		if (lst->head != (caf_cdequen_t *)NULL
			&& lst->tail != (caf_cdequen_t *)NULL) {
			if (cdeque_empty_list (lst) == CAF_OK) {
				xfree (lst);
				return CAF_OK;
			} else if (cdeque_oneitem_list (lst) == CAF_OK) {
				xfree (lst->head);
				xfree (lst);
				return CAF_OK;
			} else {
				cur = lst->head;
				do {
					destroy = cur;
					cur = cur->next;
					xfree(destroy);
				} while (cur != lst->tail);
				xfree (lst->tail);
				xfree (lst);
				return CAF_OK;
			}
		} else {
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
cdeque_node_delete (cdeque_t *lst, caf_cdequen_t *n, CAF_LSTDLCNODE_CBDEL(del)) {
	caf_cdequen_t *nr;
	caf_cdequen_t *prev;
	caf_cdequen_t *next;
	if (lst != (cdeque_t *)NULL && n != (void *)NULL && del != NULL) {
		nr = lst->head;
		if (nr != (caf_cdequen_t *)NULL) {
			if (cdeque_empty_list (lst) == CAF_OK) {
				return CAF_ERROR;
			} else if (cdeque_oneitem_list (lst) == CAF_OK) {
				if ((del (lst->head->data)) == CAF_OK) {
					xfree (lst->head);
					lst->head = (caf_cdequen_t *)NULL;
					lst->tail = (caf_cdequen_t *)NULL;
					return CAF_OK;
				}
			} else {
				if (nr == n) {
					if ((del (nr->data)) == CAF_OK) {
						prev = nr->prev;
						next = nr->next;
						prev->next = next;
						next->prev = prev;
						xfree (nr);
						lst->size--;
						lst->head = next;
						return CAF_OK;
					}
				} else if (lst->tail == n) {
					if ((del (nr->data)) == CAF_OK) {
						prev = nr->prev;
						next = nr->next;
						prev->next = next;
						next->prev = prev;
						xfree (nr);
						lst->size--;
						lst->tail = prev;
						return CAF_OK;
					}
				} else {
					do {
						if (nr == n) {
							if ((del (nr->data)) == CAF_OK) {
								prev = nr->prev;
								next = nr->next;
								prev->next = next;
								next->prev = prev;
								xfree (nr);
								lst->size--;
								return CAF_OK;
							}
						}
						nr = nr->next;
					} while (nr != lst->tail);
				}
			}
		}
	}
	return CAF_ERROR;
}


int
cdeque_node_delete_by_data (cdeque_t *lst, void *n,
							CAF_LSTDLCNODE_CBDEL(del)) {
	caf_cdequen_t *nr;
	caf_cdequen_t *prev;
	caf_cdequen_t *next;
	if (lst != (cdeque_t *)NULL && n != (void *)NULL && del != NULL) {
		nr = lst->head;
		if (nr != (caf_cdequen_t *)NULL) {
			if (cdeque_empty_list (lst) == CAF_OK) {
				return CAF_ERROR;
			} else if (cdeque_oneitem_list (lst) == CAF_OK) {
				if ((del (lst->head->data)) == CAF_OK) {
					xfree (lst->head);
					lst->head = (caf_cdequen_t *)NULL;
					lst->tail = (caf_cdequen_t *)NULL;
					return CAF_OK;
				}
			} else {
				if (nr->data == n) {
					if ((del (nr->data)) == CAF_OK) {
						prev = nr->prev;
						next = nr->next;
						prev->next = next;
						next->prev = prev;
						xfree (nr);
						lst->size--;
						lst->head = next;
						return CAF_OK;
					}
				} else if (lst->tail->data == n) {
					if ((del (nr->data)) == CAF_OK) {
						prev = nr->prev;
						next = nr->next;
						prev->next = next;
						next->prev = prev;
						xfree (nr);
						lst->size--;
						lst->tail = prev;
						return CAF_OK;
					}
				} else {
					do {
						if (nr->data == n) {
							if ((del (nr->data)) == CAF_OK) {
								prev = nr->prev;
								next = nr->next;
								prev->next = next;
								next->prev = prev;
								xfree (nr);
								lst->size--;
								return CAF_OK;
							}
						}
						nr = nr->next;
					} while (nr != lst->tail);
				}
			}
		}
	}
	return CAF_ERROR;
}


int
cdeque_empty_list (cdeque_t *lst) {
	if (lst != (cdeque_t *)NULL) {
		return ((lst->tail == lst->head && lst->head == (caf_cdequen_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
cdeque_oneitem_list (cdeque_t *lst) {
	if (lst != (cdeque_t *)NULL) {
		return ((lst->tail == lst->head && lst->head != (caf_cdequen_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
cdeque_length (cdeque_t *lst) {
	int c;
	caf_cdequen_t *cur;
	if (lst != (cdeque_t *)NULL) {
		if (lst->head != (caf_cdequen_t *)NULL
			&& lst->tail != (caf_cdequen_t *)NULL) {
			cur = lst->head;
			c = 0;
			do {
				cur = cur->next;
				c++;
			} while (cur != lst->tail);
			return c;
		}
	}
	return 0;
}


cdeque_t *
cdeque_push (cdeque_t *lst, void *data) {
	caf_cdequen_t *tail = (caf_cdequen_t *)NULL, *head = (caf_cdequen_t *)NULL;
	caf_cdequen_t *xnew = (caf_cdequen_t *)NULL;
	if (lst != (cdeque_t *)NULL) {
		xnew = (caf_cdequen_t *)xmalloc (CAF_LSTDLCNODE_SZ);
		if (xnew != (caf_cdequen_t *)NULL) {
			xnew->data = data;
			if (cdeque_empty_list (lst) == CAF_OK) {
				xnew->prev = xnew;
				xnew->next = xnew;
				lst->tail = xnew;
				lst->head = xnew;
				lst->size++;
			} else {
				tail = lst->tail;
				head = lst->head;
				tail->next = xnew;
				xnew->prev = tail;
				xnew->next = head;
				xnew->data = data;
				head->prev = xnew;
				lst->tail = xnew;
				lst->size++;
				return lst;
			}
		}
	}
	return (cdeque_t *)NULL;
}


caf_cdequen_t *
cdeque_pop (cdeque_t *lst) {
	caf_cdequen_t *ret = (caf_cdequen_t *)NULL;
	caf_cdequen_t *ex = (caf_cdequen_t *)NULL;
	;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return ret;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			ret = lst->tail;
			lst->head = (caf_cdequen_t *)NULL;
			lst->tail = (caf_cdequen_t *)NULL;
			ret->next = (void *)NULL;
			ret->prev = (void *)NULL;
			lst->size--;
		} else {
			ex = lst->tail->prev;
			ret = lst->tail;
			ret->next = (void *)NULL;
			ret->prev = (void *)NULL;
			ex->next = lst->head;
			lst->tail = ex;
			lst->size--;
		}
	}
	return ret;
}


caf_cdequen_t *
cdeque_first (cdeque_t *lst) {
	caf_cdequen_t *ret = (caf_cdequen_t *)NULL;
	caf_cdequen_t *ex;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return ret;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			ret = lst->head;
			lst->head = (caf_cdequen_t *)NULL;
			lst->tail = (caf_cdequen_t *)NULL;
			ret->next = (void *)NULL;
			ret->prev = (void *)NULL;
			lst->size--;
		} else {
			ret = lst->head;
			ex = ret->next;
			ret->next = (void *)NULL;
			ret->prev = (void *)NULL;
			ex->prev = (void *)NULL;
			lst->head = ex;
			lst->size--;
		}
	}
	return ret;
}


int
cdeque_set (cdeque_t *lst, int pos, void *data) {
	caf_cdequen_t *pn;
	int c;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return CAF_ERROR_SUB;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			if (pos == 0) {
				lst->head->data = data;
				return 0;
			}
			return CAF_ERROR_SUB;
		} else {
			c = 0;
			pn = lst->head;
			do {
				if (pos == c) {
					pn->data = data;
					return pos;
				}
				pn = pn->next;
				c++;
			} while (pn != lst->tail);
			if (pos == c) {
				lst->tail->data = data;
			}
		}
	}
	return CAF_ERROR_SUB;
}


int
cdeque_insert (cdeque_t *lst, int pos, void *data) {
	caf_cdequen_t *pn; caf_cdequen_t *nn;
	int c;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return CAF_ERROR_SUB;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			if (pos == 0) {
				nn = (caf_cdequen_t *)xmalloc (CAF_LSTDLCNODE_SZ);
				nn->data = data;
				nn->next = lst->head;
				lst->head->prev = nn;
				lst->head = nn;
				return 0;
			}
			return CAF_ERROR_SUB;
		} else {
			c = 0;
			pn = lst->head;
			do {
				if (pos == c) {
					nn = (caf_cdequen_t *)xmalloc (CAF_LSTDLCNODE_SZ);
					nn->data = data;
					nn->next = pn;
					nn = pn->prev;
					return pos;
				}
				pn = pn->next;
				c++;
			} while (pn != lst->tail);
			if (pos == c) {
				nn = (caf_cdequen_t *)xmalloc (CAF_LSTDLCNODE_SZ);
				nn->data = data;
				nn->next = lst->head;
				nn->prev = lst->tail;
				lst->tail->next = nn;
				lst->head->prev = nn;
				lst->tail = nn;
			}
		}
	}
	return CAF_ERROR_SUB;
}


void *
cdeque_get (cdeque_t *lst, int pos) {
	caf_cdequen_t *pn;
	int c = 0;
	if (lst != (cdeque_t *)NULL) {
		if ((cdeque_empty_list (lst) == CAF_OK)) {
			return (void *)NULL;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			return ((c == 0) ? lst->head->data : (void *)NULL);
		} else {
			c = 0;
			pn = lst->head;
			do {
				if (pos == c) {
					return pn->data;
				}
				pn = pn->next;
				c++;
			} while (pn != lst->tail);
			return ((pos == c) ? lst->tail->data : (void *)NULL);
		}
	}
	return (void *)NULL;
}


int
cdeque_map (cdeque_t *lst, CAF_LSTDLCNODE_CBMAP(step)) {
	int c = 0;
	caf_cdequen_t *n;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return 0;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			step (lst->head->data);
			return 0;
		} else {
			n = lst->head;
			do {
				step (n->data);
				n = n->next;
				c++;
			} while (n != lst->tail);
			step (lst->tail->data);
		}
	}
	return c;
}


int
cdeque_map_checked (cdeque_t *lst, CAF_LSTDLCNODE_CBMAP(step)) {
	int c = 0;
	caf_cdequen_t *n;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return 0;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			if (step (lst->head->data) == CAF_OK) {
				c++;
				return c;
			}
		} else {
			n = lst->head;
			do {
				if ((step (n->data)) == CAF_OK) {
					n = n->next;
					c++;
				} else {
					return c;
				}
			} while (n != lst->tail);
			if (step (lst->tail->data) == CAF_OK) {
				c++;
			}
		}
	}
	return c;
}


void *
cdeque_search (cdeque_t *lst, void *data, CAF_LSTDLCNODE_CBSRCH(srch)) {
	int c = 0;
	caf_cdequen_t *n;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return (void *)NULL;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			if (srch (lst->head->data, data) == CAF_OK) {
				return lst->head->data;
			}
		} else {
			n = lst->head;
			do {
				if (n != (caf_cdequen_t *)NULL) {
					if ((srch (n->data, data)) == CAF_OK) {
						return n->data;
					}
					n = n->next;
					c++;
				}
			} while (n != lst->tail);
			if (srch (lst->tail->data, data) == CAF_OK) {
				return lst->tail->data;
			}
		}
	}
	return (void *)NULL;
}


caf_cdequen_t *
cdeque_search_node (cdeque_t *lst, void *data) {
	int c = 0;
	caf_cdequen_t *n;
	if (lst != (cdeque_t *)NULL) {
		if (cdeque_empty_list (lst) == CAF_OK) {
			return (void *)NULL;
		} else if (cdeque_oneitem_list (lst) == CAF_OK) {
			if (lst->head->data == data) {
				return lst->head->data;
			}
		} else {
			n = lst->head;
			do {
				if (n->data == data) {
					return n;
				}
				n = n->next;
				c++;
			} while (n != lst->tail);
			if (data == lst->tail->data) {
				return lst->tail->data;
			}
		}
	}
	return (caf_cdequen_t *)NULL;
}


int
cdeque_delete_cb (void *ptr) {
	xfree(ptr);
	return CAF_OK;
}


int
cdeque_str_delete_cb (void *ptr) {
	xstrdestroy(ptr);
	return CAF_OK;
}


void
cdeque_dump (FILE *out, cdeque_t *lst, CAF_LSTDLCNODE_CBDUMP(dmp)) {
	caf_cdequen_t *cur;
	if (lst != (cdeque_t *)NULL) {
		if ((cdeque_empty_list (lst) != CAF_OK)
			&& (cdeque_oneitem_list (lst) == CAF_OK)) {
			dmp(out, lst->head->data);
		} else {
			cur = lst->head;
			do {
				dmp(out, cur->data);
			} while ((cur = cur->next) != lst->tail);
			dmp(out, lst->tail->data);
		}
	}
}


int
cdeque_dump_ptr (FILE *out, cdeque_t *lst) {
	int c = 0, a = 0;
	caf_cdequen_t *cur;
	if (lst != (cdeque_t *)NULL && out != (FILE *)NULL) {
		if ((cdeque_empty_list (lst)) != CAF_OK) {
			cur = lst->head;
			do {
				c++;
				a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
							  (void *)cur, (void *)cur->next, cur->data);
			} while ((cur = cur->next) != lst->tail);
			cur = lst->tail;
			a += fprintf (out, "%d: %p < %p > %p : %p\n", c, (void *)cur->prev,
						  (void *)cur, (void *)cur->next, cur->data);
		}
	}
	return a;
}


int
cdeque_dump_str_cb (FILE *o, void *data) {
	if (data != (void *)NULL) {
		return fprintf (o, "%p = [len: %d] \"%s\"\n", data,
		                (int)strlen((char *)data), (char *)data);
	} else {
		return fprintf (o, "%p = NULL\n", data);
	}
}

/* caf_data_cdeque.c ends here */

