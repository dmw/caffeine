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
lstdlc_new (void *data) {
	lstdlc_t *lst;
	lstdlcn_t *n;
	lst = (lstdlc_t *)xmalloc (CAF_LSTDLC_SZ);
	if (lst != NULL) {
		n = (lstdlcn_t *)xmalloc (CAF_LSTDLCNODE_SZ);
		if (n != (lstdlcn_t *)NULL) {
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
			lst = (lstdlc_t *)NULL;
			n = (lstdlcn_t *)NULL;
		}
	}
	return lst;
}


lstdlc_t *
lstdlc_create (void) {
	lstdlc_t *lst;
	lst = (lstdlc_t *)xmalloc (CAF_LSTDLC_SZ);
	if (lst != NULL) {
		lst->head = (lstdlcn_t *)NULL;
		lst->tail = (lstdlcn_t *)NULL;
		lst->size = 0;
	}
	return lst;
}


int
lstdlc_delete (lstdlc_t *lst, CAF_LSTDLCNODE_CBDEL(del)) {
	lstdlcn_t *cur, *destroy;
	if (lst != (lstdlc_t *)NULL) {
		if (lst->head != (lstdlcn_t *)NULL && lst->tail != (lstdlcn_t *)NULL) {
			if (lstdlc_empty_list (lst) == CAF_OK) {
				xfree (lst);
				return CAF_OK;
			} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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
				lst = (lstdlc_t *)NULL;
				return CAF_OK;
			}
		} else {
			return CAF_OK;
		}
	}
	return CAF_ERROR_SUB;
}


int
lstdlc_delete_nocb (lstdlc_t *lst) {
	lstdlcn_t *cur, *destroy;
	if (lst != (lstdlc_t *)NULL) {
		if (lst->head != (lstdlcn_t *)NULL && lst->tail != (lstdlcn_t *)NULL) {
			if (lstdlc_empty_list (lst) == CAF_OK) {
				xfree (lst);
				return CAF_OK;
			} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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
lstdlc_node_delete (lstdlc_t *lst, lstdlcn_t *n, CAF_LSTDLCNODE_CBDEL(del)) {
	lstdlcn_t *nr;
	lstdlcn_t *prev;
	lstdlcn_t *next;
	if (lst != (lstdlc_t *)NULL && n != (void *)NULL && del != NULL) {
		nr = lst->head;
		if (nr != (lstdlcn_t *)NULL) {
			if (lstdlc_empty_list (lst) == CAF_OK) {
				return CAF_ERROR;
			} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
				if ((del (lst->head->data)) == CAF_OK) {
					xfree (lst->head);
					lst->head = (lstdlcn_t *)NULL;
					lst->tail = (lstdlcn_t *)NULL;
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
lstdlc_node_delete_by_data (lstdlc_t *lst, void *n, CAF_LSTDLCNODE_CBDEL(del)) {
	lstdlcn_t *nr;
	lstdlcn_t *prev;
	lstdlcn_t *next;
	if (lst != (lstdlc_t *)NULL && n != (void *)NULL && del != NULL) {
		nr = lst->head;
		if (nr != (lstdlcn_t *)NULL) {
			if (lstdlc_empty_list (lst) == CAF_OK) {
				return CAF_ERROR;
			} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
				if ((del (lst->head->data)) == CAF_OK) {
					xfree (lst->head);
					lst->head = (lstdlcn_t *)NULL;
					lst->tail = (lstdlcn_t *)NULL;
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
lstdlc_empty_list (lstdlc_t *lst) {
	if (lst != (lstdlc_t *)NULL) {
		return ((lst->tail == lst->head && lst->head == (lstdlcn_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
lstdlc_oneitem_list (lstdlc_t *lst) {
	if (lst != (lstdlc_t *)NULL) {
		return ((lst->tail == lst->head && lst->head != (lstdlcn_t *)NULL)
		        ? CAF_OK : CAF_ERROR);
	}
	return CAF_ERROR;
}


int
lstdlc_length (lstdlc_t *lst) {
	int c;
	lstdlcn_t *cur;
	if (lst != (lstdlc_t *)NULL) {
		if (lst->head != (lstdlcn_t *)NULL && lst->tail != (lstdlcn_t *)NULL) {
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


lstdlc_t *
lstdlc_push (lstdlc_t *lst, void *data) {
	lstdlcn_t *tail = (lstdlcn_t *)NULL, *head = (lstdlcn_t *)NULL;
	lstdlcn_t *xnew = (lstdlcn_t *)NULL;
	if (lst != (lstdlc_t *)NULL) {
		xnew = (lstdlcn_t *)xmalloc (CAF_LSTDLCNODE_SZ);
		if (xnew != (lstdlcn_t *)NULL) {
			xnew->data = data;
			if (lstdlc_empty_list (lst) == CAF_OK) {
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
	return (lstdlc_t *)NULL;
}


lstdlcn_t *
lstdlc_pop (lstdlc_t *lst) {
	lstdlcn_t *ret = (lstdlcn_t *)NULL;
	lstdlcn_t *ex = (lstdlcn_t *)NULL;
	;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return ret;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
			ret = lst->tail;
			lst->head = (lstdlcn_t *)NULL;
			lst->tail = (lstdlcn_t *)NULL;
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


lstdlcn_t *
lstdlc_first (lstdlc_t *lst) {
	lstdlcn_t *ret = (lstdlcn_t *)NULL;
	lstdlcn_t *ex;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return ret;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
			ret = lst->head;
			lst->head = (lstdlcn_t *)NULL;
			lst->tail = (lstdlcn_t *)NULL;
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
lstdlc_set (lstdlc_t *lst, int pos, void *data) {
	lstdlcn_t *pn;
	int c;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return CAF_ERROR_SUB;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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


void *
lstdlc_get (lstdlc_t *lst, int pos) {
	lstdlcn_t *pn;
	int c = 0;
	if (lst != (lstdlc_t *)NULL) {
		if ((lstdlc_empty_list (lst) == CAF_OK)) {
			return (void *)NULL;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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
lstdlc_walk (lstdlc_t *lst, CAF_LSTDLCNODE_CBWALK(step)) {
	int c = 0;
	lstdlcn_t *n;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return 0;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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
lstdlc_walk_checked (lstdlc_t *lst, CAF_LSTDLCNODE_CBWALK(step)) {
	int c = 0;
	lstdlcn_t *n;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return 0;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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
lstdlc_search (lstdlc_t *lst, void *data, CAF_LSTDLCNODE_CBSRCH(srch)) {
	int c = 0;
	lstdlcn_t *n;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return (void *)NULL;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
			if (srch (lst->head->data, data) == CAF_OK) {
				return lst->head->data;
			}
		} else {
			n = lst->head;
			do {
				if (n != (lstdlcn_t *)NULL) {
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


lstdlcn_t *
lstdlc_search_node (lstdlc_t *lst, void *data) {
	int c = 0;
	lstdlcn_t *n;
	if (lst != (lstdlc_t *)NULL) {
		if (lstdlc_empty_list (lst) == CAF_OK) {
			return (void *)NULL;
		} else if (lstdlc_oneitem_list (lst) == CAF_OK) {
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
	return (lstdlcn_t *)NULL;
}


int
lstdlc_delete_cb (void *ptr) {
	xfree(ptr);
	return CAF_OK;
}


int
lstdlc_str_delete_cb (void *ptr) {
	xstrdestroy(ptr);
	return CAF_OK;
}


void
lstdlc_dump (FILE *out, lstdlc_t *lst, CAF_LSTDLCNODE_CBDUMP(dmp)) {
	lstdlcn_t *cur;
	if (lst != (lstdlc_t *)NULL) {
		if ((lstdlc_empty_list (lst) == CAF_OK)
		        || (lstdlc_oneitem_list (lst) == CAF_OK)) {
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
lstdlc_dump_ptr (FILE *out, lstdlc_t *lst) {
	int c = 0, a = 0;
	lstdlcn_t *cur;
	if (lst != (lstdlc_t *)NULL && out != (FILE *)NULL) {
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
	return a;
}


int
lstdlc_dump_str_cb (FILE *o, void *data) {
	if (data != (void *)NULL) {
		return fprintf (o, "%p = [len: %d] \"%s\"\n", data,
		                (int)strlen((char *)data), (char *)data);
	} else {
		return fprintf (o, "%p = NULL\n", data);
	}
}

/* caf_data_lstdlc.c ends here */
