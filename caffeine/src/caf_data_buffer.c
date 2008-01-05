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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"



cbuffer_t *
cbuf_new (void) {
	cbuffer_t *buf;
	buf = (cbuffer_t *)xmalloc (CAF_BUFF_SZ);
	if (buf != (cbuffer_t *)NULL) {
		buf->sz = 0;
		buf->iosz = 0;
		buf->data = (void *)NULL;
	}
	return buf;
}


cbuffer_t *
cbuf_create (size_t sz) {
	cbuffer_t *buf;
	buf = (cbuffer_t *)xmalloc (CAF_BUFF_SZ);
	if (buf != (cbuffer_t *)NULL) {
		if (sz > 0) {
			buf->sz = sz;
			buf->iosz = 0;
			buf->data = (void *)xmalloc (sz);
			if (buf->data != (void *)NULL) {
				return buf;
			} else {
				xfree (buf);
				return (void *)NULL;
			}
		} else {
			buf->sz = 0;
			buf->iosz = 0;
			buf->data = (void *)NULL;
		}
	}
	return buf;
}


void
cbuf_delete (cbuffer_t *buf) {
	if (buf != (cbuffer_t *)NULL) {
		if (buf->data != (void *)NULL) {
			xfree (buf->data);
			buf->data = (void *)NULL;
		}
		buf->sz = 0;
		xfree (buf);
		buf = (cbuffer_t *)NULL;
	}
}


int
cbuf_delete_interactive (cbuffer_t *buf, CAF_BUFF_DELETE_CB(cb)) {
	if (buf != (cbuffer_t *)NULL) {
		if ((cb (buf->data, buf->sz)) == 0) {
			xfree (buf);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
cbuf_delete_callback (void *cbuf) {
	cbuf_delete ((cbuffer_t *)cbuf);
	return CAF_OK;
}


void
cbuf_clean (cbuffer_t *buf) {
	if (buf != (cbuffer_t *)NULL) {
		xempty(buf->data, buf->sz);
		buf->iosz = 0;
	}
}


size_t
cbuf_copy (cbuffer_t *dst, const cbuffer_t *src) {
	void *new_ptr = (void *)NULL;
	if (src != (cbuffer_t *)NULL && dst != (cbuffer_t *)NULL) {
		if (src->sz > 0 && src->data != (void *)NULL) {
			if (dst->sz > 0 && dst->data != (void *)NULL) {
				cbuf_clean(dst);
				dst->data = xrealloc (dst->data, src->sz);
				if (dst->data == (void *)NULL) {
					return 0;
				}
			} else if (dst->data != (void *)NULL) {
				new_ptr = xrealloc(dst->data, src->sz);
				if (new_ptr != (void *)NULL) {
					xempty(new_ptr, src->sz);
					dst->data = new_ptr;
					dst->sz = src->sz;
				} else {
					return 0;
				}
			} else {
				dst->data = xmalloc(src->sz);
				if (dst->data == (void *)NULL) {
					return 0;
				}
			}
			if ((xmemcpy(dst->data, src->data, src->sz)) != (void *)NULL) {
				dst->sz = src->sz;
				return dst->sz;
			}
		}
	}
	return 0;
}


size_t
cbuf_import (cbuffer_t *dst, const void *data, const size_t sz) {
	if (data != (void *)NULL && dst != (cbuffer_t *)NULL && sz > 0) {
		cbuf_clean(dst);
		if (dst->data != (void *)NULL) {
			dst->data = xrealloc (dst->data, sz);
			if (dst->data == (void *)NULL) {
				return 0;
			}
		} else {
			dst->data = xmalloc(sz);
			if (dst->data == (void *)NULL) {
				return 0;
			}
		}
		if ((xmemcpy((void *)dst->data, (void *)data, sz)) != (void *)NULL) {
			dst->sz = sz;
			return sz;
		}
	}
	return 0;
}


cbuffer_t *
cbuf_extract (const cbuffer_t *src, const size_t from, const size_t to) {
	cbuffer_t *ret = (cbuffer_t *)NULL;
	size_t imp = (size_t)NULL, diff = (size_t)NULL;
	void *b_ptr = (void *)NULL;
	if (src != (cbuffer_t *)NULL) {
		if (from > to) {
			return (cbuffer_t *)NULL;
		} else {
			ret = cbuf_new();
			cbuf_clean(ret);
			b_ptr = (void *)((size_t)src->data + from);
			diff = to - from;
			imp = cbuf_import (ret, b_ptr, diff);
			if (imp > 0) {
				return ret;
			}
			cbuf_delete(ret);
			return (cbuffer_t *)NULL;
		}
	}
	return (cbuffer_t *)NULL;
}


cbuffer_t *
cbuf_cut (cbuffer_t *src, const size_t from, const size_t to) {
	cbuffer_t *newb = (cbuffer_t *)NULL;
	void *final = (void *)NULL;
	size_t diff = 0, trailing = 0, final_sz = 0;
	void *sptr = (void *)NULL, *eptr = (void *)NULL;
	if (src != (cbuffer_t *)NULL) {
		if (from < to) {
			newb = cbuf_extract(src, from, to);
			if (newb != (cbuffer_t *)NULL) {
				diff = to - from;
				final_sz = src->sz - diff;
				if (diff > 0 && final_sz > 0) {
					sptr = (void *)((size_t)src->data + from);
					eptr = (void *)((size_t)sptr + diff);
					trailing = src->sz - to;
					final = xmemcpy(eptr, sptr, trailing);
					if (final != (void *)NULL) {
						final = xrealloc(src->data, final_sz);
						if (final != (void *)NULL) {
							src->data = final;
							src->sz = final_sz;
							return newb;
						}
					}
					cbuf_delete(newb);
					return (cbuffer_t *)NULL;
				} else {
					cbuf_delete(newb);
					return (cbuffer_t *)NULL;
				}
			}
		}
	}
	return (cbuffer_t *)NULL;
}


cbuffer_t *
cbuf_paste (cbuffer_t *dst, const cbuffer_t *src, const size_t from_dst,
            const size_t from_src, const size_t to_src) {
	size_t diff_src = 0, diff_dst = 0;
	void *from_ptr = (void *)NULL, *to_ptr = (void *)NULL;
	if (dst != (cbuffer_t *)NULL && src != (cbuffer_t *)NULL) {
		if ((int)from_dst < 0 || ((int)to_src < 0 || (int)from_src < 0)) {
			return (cbuffer_t *)NULL;
		} else {
			diff_dst = dst->sz - from_dst;
			diff_src = to_src - from_src;
			if (diff_dst < diff_src) {
				return (cbuffer_t *)NULL;
			} else {
				from_ptr = src->data;
				from_ptr = (void *)((size_t)from_ptr + from_src);
				to_ptr = dst->data;
				to_ptr = (void *)((size_t)to_ptr + from_dst);
				if (memcpy(to_ptr, from_ptr, diff_src) == to_ptr) {
					return dst;
				}
			}
		}
	}
	return (cbuffer_t *)NULL;
}


lstdl_t *
cbuf_split (cbuffer_t *src, const void *pattern, size_t patsz) {
	cbuffer_t *current = (cbuffer_t *)NULL;
	lstdl_t *lst = (lstdl_t *)NULL;
	size_t idx = 0, idx_current = 0, idx_over = 0, idx_tail = 0;
	size_t btop = 0;
	if (src != (cbuffer_t *)NULL && pattern != (void *)NULL) {
		lst = lstdl_create ();
		btop = ((size_t)src->data) + src->sz;
		idx_current = ((size_t)src->data);
		idx_over = ((size_t)src->data) + (idx + patsz);
		idx_tail = 0;
		while (idx_current < btop) {
			if (idx_over < btop) {
				if (memcmp ((void *)idx_current, pattern, patsz) == 0) {
					current = cbuf_extract (src, idx_tail, idx);
					if (current != (cbuffer_t *)NULL) {
						lstdl_push (lst, current);
					}
					idx += patsz;
					idx_tail = idx;
				}
			} else {
				current = cbuf_extract (src, idx_tail, src->sz);
				if (current != (cbuffer_t *)NULL) {
					lstdl_push (lst, current);
				}
				idx = btop;
			}
			idx++;
			idx_current = ((size_t)src->data) + (idx);
			idx_over = ((size_t)src->data) + (idx + patsz);
		}
	}
	return lst;
}


cbuffer_t *
cbuf_join (lstdl_t *lst) {
	cbuffer_t *buffer = (cbuffer_t *)NULL;
	cbuffer_t *current;
	lstdln_t *c;
	size_t btotal = 0, idx = 0;
	if (lst != (lstdl_t *)NULL) {
		c = lst->head;
		while (c != lst->tail && c != (lstdln_t *)NULL) {
			current = (cbuffer_t *)c->data;
			if (current != (cbuffer_t *)NULL) {
				btotal += current->sz;
			}
			c = c->next;
		}
		current = (cbuffer_t *)lst->tail->data;
		if (current != (cbuffer_t *)NULL) {
			btotal += current->sz;
		}
	}
	buffer = cbuf_create (btotal);
	if (buffer != (cbuffer_t *)NULL) {
		if (lst != (lstdl_t *)NULL) {
			c = lst->head;
			while (c != lst->tail && c != (lstdln_t *)NULL) {
				current = (cbuffer_t *)c->data;
				if (current != (cbuffer_t *)NULL) {
					buffer = cbuf_paste (buffer, current, idx, 0,
										 current->sz);
					idx += current->sz;
				}
				c = c->next;
			}
			current = (cbuffer_t *)lst->tail->data;
			if (current != (cbuffer_t *)NULL) {
				buffer = cbuf_paste (buffer, current, idx, 0,
									 current->sz);
				idx += current->sz;
			}
		}
	}
	return buffer;
}


cbuffer_t *
cbuf_replace (cbuffer_t *src, void *srch, void *repl, size_t srchsz,
              size_t replsz) {
	cbuffer_t *buffer, *current;
	lstdl_t *lst;
	lstdln_t *n;
	size_t idx = 0, idx_current = 0, idx_over = 0, idx_tail = 0;
	size_t btop = 0, btotal = 0, count = 0;
	if (src != (cbuffer_t *)NULL && srch != (void *)NULL) {
		btotal = src->sz;
		btop = (size_t)src->data + (size_t)src->sz;
		idx_current = ((size_t)src->data);
		idx_over = ((size_t)src->data) + srchsz;
		lst = lstdl_create ();
		while (idx_current < btop) {
			if (idx_over <= btop) {
				if (memcmp ((void *)idx_current, srch, srchsz) == 0) {
					current = cbuf_extract (src, idx_tail,
											idx_tail + srchsz);
					if (current != (cbuffer_t *)NULL) {
						lstdl_push (lst, (void *)idx_current);
						cbuf_delete (current);
						count++;
					}
					idx += srchsz;
					idx_tail = idx;
				}
			} else {
				idx = btop;
			}
			idx++;
			idx_current = ((size_t)src->data) + (idx);
			idx_over = ((size_t)src->data) + (idx + srchsz);
		}
	} else {
		return (cbuffer_t *)NULL;
	}
	btotal = (btotal - count * srchsz) + count * replsz;
	buffer = cbuf_create (btotal);
	if (buffer != (cbuffer_t *)NULL && lst != (lstdl_t *)NULL) {
		btop = (size_t)src->data + (size_t)src->sz;
		idx = (size_t)src->data;
		idx_current = (size_t)buffer->data;
		n = lst->head;
		while (n != (lstdln_t *)NULL) {
			btotal = ((size_t)n->data - idx); /* idx is src */
			btop = idx_current + btotal; /* btop is dst */
			memcpy ((void *)idx_current, (void *)idx, btotal);
			memcpy ((void *)btop, (void *)repl, replsz);
			idx += (btotal + srchsz); /* btop is src */
			idx_current += (btotal + replsz); /* idx_current is dst */
			n = n->next;
		}
		count = (size_t)src->data + src->sz;
		if (idx < count) {
			memcpy ((void *)idx_current, (void *)idx, (count - idx));
		}
	}
	lstdl_delete_nocb (lst);
	return buffer;
}


cbuffer_t *
cbuf_append (cbuffer_t *head, cbuffer_t *tail)
{
	size_t tsz = 0;
	cbuffer_t *r = (cbuffer_t *)NULL;
	if (head != (cbuffer_t *)NULL && tail != (cbuffer_t *)NULL) {
		if (head->iosz > 0 && tail->iosz > 0) {
			tsz = (size_t)head->iosz + (size_t)tail->iosz;
			r = cbuf_create (tsz);
			if (r != (cbuffer_t *)NULL) {
				cbuf_clean (r);
				xmemcpy (r->data, head->data, head->iosz);
				xmemcpy ((void *)((size_t)(r->data) + head->iosz), tail->data, tail->iosz);
			}
		} else {
			tsz = head->sz + tail->sz;
			r = cbuf_create (tsz);
			if (r != (cbuffer_t *)NULL) {
				cbuf_clean (r);
				xmemcpy (r->data, head->data, head->sz);
				xmemcpy ((void *)((size_t)(r->data) + head->sz), tail->data, tail->sz);
			}
		}
	}
	return r;
}


lstdl_t *
cbuf_search (cbuffer_t *src, void *srch, size_t srchsz) {
	lstdl_t *lst;
	size_t idx = 0, idx_current = 0, idx_over = 0, idx_tail = 0;
	size_t btop = 0;
	if (src != (cbuffer_t *)NULL && srch != (void *)NULL) {
		btop = (size_t)src->data + (size_t)src->sz;
		idx_current = ((size_t)src->data);
		idx_over = ((size_t)src->data) + srchsz;
		lst = lstdl_create ();
		while (idx_current < btop) {
			if (idx_over <= btop) {
				if (memcmp ((void *)idx_current, srch, srchsz) == 0) {
					lstdl_push (lst, (void *)idx_current);
					idx_tail = idx_current;
					idx_current += srchsz;
				} else {
					idx_current++;
				}
			} else {
				idx = btop;
			}
			idx_over = idx_current + srchsz;
		}
		return lst;
	}
	return (lstdl_t *)NULL;
}


cbuffer_t *
cbuf_head (cbuffer_t *src, size_t sz) {
	cbuffer_t *ret = (cbuffer_t *)NULL;
	if (src != (cbuffer_t *)NULL && sz > 0) {
		if (sz <= src->sz) {
			ret = cbuf_extract (src, 0, sz);
		}
	}
	return ret;
}


cbuffer_t *
cbuf_tail (cbuffer_t *src, size_t sz) {
	cbuffer_t *ret = (cbuffer_t *)NULL;
	if (src != (cbuffer_t *)NULL && sz > 0) {
		if (sz <= src->sz) {
			ret = cbuf_extract (src, src->sz - sz, src->sz);
		}
	}
	return ret;
}


cbuffer_t *
cbuf_head_cut (cbuffer_t *src, size_t sz) {
	cbuffer_t *ret = (cbuffer_t *)NULL;
	if (src != (cbuffer_t *)NULL && sz > 0) {
		if (sz <= src->sz) {
			ret = cbuf_cut (src, 0, sz);
		}
	}
	return ret;
}


cbuffer_t *
cbuf_tail_cut (cbuffer_t *src, size_t sz) {
	cbuffer_t *ret = (cbuffer_t *)NULL;
	if (src != (cbuffer_t *)NULL && sz > 0) {
		if (sz <= src->sz) {
			ret = cbuf_cut (src, src->sz - sz, src->sz);
		}
	}
	return ret;
}

/* caf_data_buffer.c ends here */

