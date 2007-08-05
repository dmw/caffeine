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
    Lesser General Public License for more denexts.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA

    $Id$
 */
#ifndef CAF_DATA_LSTDL_H
#define CAF_DATA_LSTDL_H 1

#include <stdio.h>

/**
 * @defgroup      caf_dlnk_list    Double Linked List
 * @ingroup       caf_data_struct
 * @addtogroup    caf_dlnk_list
 * @{
 *
 * @brief     Caffeine Double Linked List Functions.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * List management functions. These are function to manage Double
 * Linked Lists. This includes push, pop, delete, set and other common
 * operations on lists.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Computes de list structure size */
#define CAF_LSTDL_SZ                   (sizeof(lstdl_t))
/** Computes de list node structure size */
#define CAF_LSTDLNODE_SZ               (sizeof(lstdln_t))
/** Declares a deletion callback to use with list nodes */
#define CAF_LSTDLNODE_CBDEL(del)       int (*del)(void *ptr)
/** Declares a dump callback to use with list nodes */
#define CAF_LSTDLNODE_CBDUMP(dmp)      int (*dmp)(FILE *out, void *ptr)
/** Declares a walk callback to use with list nodes */
#define CAF_LSTDLNODE_CBWALK(walk)     int (*walk)(void *ptr)
/** Declares a search callback to use with list nodes */
#define CAF_LSTDLNODE_CBSRCH(srch)     int (*srch)(void *ndata, void *data)

/**
 *
 * @brief    Caffeine double linked list node type.
 * The type of a double linked list node.
 * @see      caf_lst_node
 */
typedef struct caf_lstdl_node_s lstdln_t;

/**
 *
 * @brief    Caffeine double linked list node structure.
 * The double linked list node structure stores a pointer data,
 * a pointer to de previous node and a pointer to the next node.
 * @see      lstdln_t
 */
struct caf_lstdl_node_s {
	/** Pointer to the previous node in the list */
	lstdln_t *prev;
	/** Pointer to the next node in the list */
	lstdln_t *next;
	/** Pointer to the data in the list node */
	void *data;
};

/**
 *
 * @brief    Caffeine double linked list type.
 *
 * The type of a double linked list.
 *
 * @see      caf_lst
 */
typedef struct caf_lstdl_s lstdl_t;
/**
 *
 * @brief    Caffeine double linked list structure.
 *
 * The double linked list node structure stores a pointer to the
 * first list node, a pointer to de second list node and an integer
 * with the list size.
 *
 * @see      lstdln_t
 * @see      lstdl_t
 */
struct caf_lstdl_s {
	lstdln_t *head;
	lstdln_t *tail;
	int size;
};

/**
 *
 * @brief    Creates a new Caffeine Double Linked List.
 *
 * Creates a new Caffeine Double Linked List and can give a data
 * parameter to store in the first node.
 *
 * @param[in]    data            first node data.
 * @return       lstdl_t *     the allocated list.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
lstdl_t *lstdl_new (void *data);

/**
 *
 * @brief    Creates a new Caffeine Double Linked List.
 *
 * Creates a new Caffeine Double Linked List and can give a data
 * parameter to store in the first node.
 *
 * @param[in]    data            first node data.
 * @return       lstdl_t *     the allocated list.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
lstdl_t *lstdl_create (void);

/**
 *
 * @brief    Deletes a Caffeine Double Linked List.
 *
 * Try to free the memory allocated for the Double Linked List.
 * Gives as parameters the list to free and a callback function
 * to delete (free) the memory allocated by the list nodes.
 * The callback must return zero on success, other result on error,
 * this will stop the operation and modify the lst pointer to
 * set the first element to the non deleted node.
 *
 * @param[in]    lst    the Caffeine Double Linked List.
 * @param[in]    del    the callback function to free the list nodes.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_delete (lstdl_t *lst, CAF_LSTDLNODE_CBDEL(del));

/**
 *
 * @brief    Deletes a Caffeine Double Linked List.
 *
 * Try to free the memory allocated for the Double Linked List.
 *
 * @param[in]    lst    the Caffeine Double Linked List.
 * @param[in]    del    the callback function to free the list nodes.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_delete_nocb (lstdl_t *lst);

/**
 *
 * @brief    Deletes a node from the given Caffeine dobule linked list.
 *
 * Free the memory allocated by a Caffeine Double Linked List. Gives as
 * parameters the list node and a callback function to free the memory
 * allocated by the data pointer in the list node.
 * The callback must return zero on success.
 *
 * @param[in]    n        the list node.
 * @param[in]    del      the callback function to free the node data.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_node_delete (lstdl_t *lst, lstdln_t *n, CAF_LSTDLNODE_CBDEL(del));

/**
 *
 * @brief    Deletes a node from the given Caffeine DLL comparing data.
 *
 * Free the memory allocated by a Caffeine Double Linked List. Gives as
 * parameters the list node data pointer and a callback function to free
 * the memory allocated by the data pointer in the list node. The callback
 * must return zero on success.
 *
 * @param[in]    n        the list node.
 * @param[in]    del      the callback function to free the node data.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_node_delete_by_data (lstdl_t *lst, void *n,
							   CAF_LSTDLNODE_CBDEL(del));

/**
 *
 * @brief    Verifies if the list is empty or not
 *
 * Verifies if the list is empty or not.
 *
 * @param[in]    lst    The list to get length
 * @return       CAF_OK if is empty, CAF_ERROR if not.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 */
int lstdl_empty_list (lstdl_t *lst);

/**
 *
 * @brief    Verifies if the list have only one item
 *
 * Verifies if the list have only one item.
 *
 * @param[in]    lst    The list to get length
 * @return       CAF_OK if does have one item, CAF_ERROR if not.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 */
int lstdl_oneitem_list (lstdl_t *lst);

/**
 *
 * @brief    Returns the len of the given list.
 *
 * This function crosses the given list and returns his length.
 *
 * @param[in]    lst    The list to get length
 *
 * @see    lstdl_t
 */
int lstdl_length (lstdl_t *lst);

/**
 *
 * @brief    Push a data node in the given list.
 *
 * This function gets a Caffeine Double Linked List and search sets
 * the tail node in the list with the data pointer data.
 *
 * @param[in]    lst            the list to push a node.
 * @param[in]    data           the data to link in the list.
 * @return       lstdl_t *     the pointer to the list.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 * @see    lstdl_pop
 * @see    lstdl_first
 * @see    lstdl_replace
 */
lstdl_t *lstdl_push (lstdl_t *lst, void *data);

/**
 *
 * @brief    Pops (gets and removes) the lats element in the list.
 *
 * This function pops the tail element in the list, this means
 * gets and removes from the list the tail element.
 *
 * @param[in]    lst            the list to pop an element.
 * @return       lstdln_t *    the node to get from the list.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 * @see    lstdl_first
 * @see    lstdl_push
 * @see    lstdl_replace
 * @see    lstdl_set
 */
lstdln_t *lstdl_pop (lstdl_t *lst);

/**
 *
 * @brief    Gets and removes from the list the first node.
 *
 * This functions returns and removes from the given list the
 * first node.
 *
 * @param[in]    lst            list to get the first element.
 * @return       lstdln_t *    first node pointer.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 * @see    lstdl_pop
 * @see    lstdl_push
 * @see    lstdl_replace
 * @see    lstdl_set
 */
lstdln_t *lstdl_first (lstdl_t *lst);

/**
 *
 * @brief    Sets a data node in the list.
 *
 * Sets a data node in the list through the position of the node
 * in the list. Positions starts in zero. A posible failure in
 * this function is that the data pointer is not NULL, then you
 * must use lstdl_replace function.
 *
 * @param[in]    lst    list where to set the node.
 * @param[in]    pos    data node position.
 * @param[in]    data   data pointer in the list.
 * @return       int    the position if found, -1 on failure.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 * @see    lstdl_first
 * @see    lstdl_push
 * @see    lstdl_pop
 * @see    lstdl_replace
 */
void *lstdl_get (lstdl_t *lst, int pos);

/**
 *
 * @brief    Sets a data node in the list.
 *
 * Sets a data node in the list through the position of the node
 * in the list. Positions starts in zero. A posible failure in
 * this function is that the data pointer is not NULL, then you
 * must use lstdl_replace function.
 *
 * @param[in]    lst    list where to set the node.
 * @param[in]    pos    data node position.
 * @return       int    the position if found, -1 on failure.
 *
 * @see    lstdl_t
 * @see    lstdln_t
 * @see    lstdl_first
 * @see    lstdl_push
 * @see    lstdl_pop
 * @see    lstdl_replace
 */
int lstdl_set (lstdl_t *lst, int pos, void *data);

/**
 *
 * @brief    Apply a function to the list data nodes.
 *
 * This function crosses the list and apply the parameter function step
 * as callback over every data node in the list. The function does not
 * verify if the data node pointer is NULL.
 * nodes processed.
 *
 * @param[in]    lst        the list to walk.
 * @param[in]    step       the function to apply.
 * @return       int        the number of afected nodes.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_walk (lstdl_t *lst, CAF_LSTDLNODE_CBWALK(step));

/**
 *
 * @brief    Apply a function to the list data nodes.
 *
 * This function crosses the list and apply the parameter function step
 * as callback over every data node in the list. The function does not
 * verify if the data node pointer is NULL. Also, the function checks every
 * "step" in the process, and on the first failure returns the number of
 * nodes processed.
 *
 * @param[in]    lst        the list to walk.
 * @param[in]    step       the function to apply.
 * @return       int        the number of afected nodes.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_walk_checked (lstdl_t *lst, CAF_LSTDLNODE_CBWALK(step));

/**
 *
 * @brief    Searches for a node using a callback function.
 *
 * Runs over the list comparing the input field *data against
 * the data nodes in the list using the callback function srch.
 * The comparision callback must return zero on success.
 *
 * @param[in]    lst        the list to search.
 * @param[in]    data       the data to search.
 * @param[in]    srch       the comparision callback.
 * @return       void *     the data pointed by the result and NULL on error.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
void *lstdl_search (lstdl_t *lst, void *data, CAF_LSTDLNODE_CBSRCH(srch));

/**
 *
 * @brief    Apply a function to the list data nodes.
 *
 * This function crosses the list and apply the parameter function step
 * as callback over every data node in the list. The function does not
 * verify if the data node pointer is NULL.
 *
 * @param[in]    lst        the list to walk.
 * @param[in]    step       the function to apply.
 * @return       int        the number of afected nodes.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
lstdln_t *lstdl_search_node (lstdl_t *lst, void *data);

/**
 *
 * @brief    Default callback function to free list nodes.
 *
 * This the default function to free the memory allocated by the Caffeine
 * Double Linked List nodes. By default, the callback must return 1
 * on success, a more complex callback should verify the operations.
 *
 * @param[in]    ptr        the data pointer.
 * @return       int        1 on success, 0 on failure.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_delete_cb (void *ptr);

/**
 *
 * @brief    Default callback to free memory allocated by strings.
 *
 * This the default function to free memory allocated for strings
 * in any Caffeine Double Linked List node. By default, the callback must
 * return 1 on success, a more complex callback should verify the
 * operations.
 *
 * @param[in]    str        the string pointer.
 * @return       int        1 on success, 0 on failure.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_str_delete_cb (void *ptr);

/**
 *
 * @brief    Caffeine Double Linked List Dumper.
 *
 * Dumps the Double Linked List to the given output FILE stream.
 * Gives as parameters the output FILE stream, the list to dump
 * and the lstdln_t dumper callback function.
 *
 * @param[in]    out        FILE output stream.
 * @param[in]    lst        lstdl_t pointer to dump.
 * @param[in]    dmp        lstdln_t callback dumper.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
void lstdl_dump (FILE *out, lstdl_t *lst, CAF_LSTDLNODE_CBDUMP(dmp));

/**
 *
 * @brief    Dumps the lists pointer nodes.
 *
 * This function dumps the pointers list pointers to the given FILE
 * ouput stream.
 *
 * @param[in]    out    FILE output stream to dump the list.
 * @param[in]    lst    list to dump.
 *
 * @see      lstdl_t
 */
int lstdl_dump_ptr (FILE *out, lstdl_t *lst);

/**
 *
 * @brief    Default string dumper.
 *
 * This is the default string dumper to use with CAF nodes with
 * strings a data pointers.
 *
 * @param[in]    data    a pointer to a string.
 *
 * @see      lstdl_t
 * @see      lstdln_t
 */
int lstdl_dump_str_cb (FILE *o, void *data);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_LSTDL_H */
/* caf_data_lstdl.h ends here */
