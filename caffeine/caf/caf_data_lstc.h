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

    $Id$
 */
#ifndef CAF_DATA_LSTC_H
#define CAF_DATA_LSTC_H 1
/**
 * @defgroup      caf_circular_list    Circular List
 * @ingroup       caf_data_struct
 * @addtogroup    caf_circular_list
 * @{
 *
 * @brief     Caffeine Circular List Functions.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * List management functions. These are function to manage Circular
 * Lists. This includes push, pop, delete, set and other common
 * operations on lists.
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

	/** Computes de list structure size */
#define CAF_LSTC_SZ                   (sizeof(lstcn_t))
	/** Declares a deletion callback to use with list nodes */
#define CAF_LSTCNODE_CBDEL(del)       int (*del)(void *ptr)
	/** Declares a dump callback to use with list nodes */
#define CAF_LSTCNODE_CBDUMP(dmp)      int (*dmp)(FILE *out, void *ptr)
	/** Declares a walk callback to use with list nodes */
#define CAF_LSTCNODE_CBWALK(walk)     int (*walk)(void *ptr)
	/** Declares a search callback to use with list nodes */
#define CAF_LSTCNODE_CBSRCH(srch)     int (*srch)(void *ndata, void *data)

/**
 *
 * @brief    Caffeine double linked list node type.
 * The type of a double linked list node.
 * @see      caf_lst_node
 */
typedef struct caf_lstc_node_s lstcn_t;

/**
 *
 * @brief    Caffeine double linked list node structure.
 * The double linked list node structure stores a pointer data,
 * a pointer to de previous node and a pointer to the next node.
 * @see      caflstn_t
 */
struct caf_lstc_node_s {
	/** Pointer to the previous node in the list */
	lstcn_t *prev;
	/** Pointer to the next node in the list */
	lstcn_t *next;
	/** Pointer to the data in the list node */
	void *data;
};

/**
 *
 * @brief    Creates a new Caffeine Circular List.
 *
 * Creates a new Caffeine Circular List and can give a data
 * parameter to store in the first node.
 *
 * @param[in]    data            first node data.
 * @return       lstcn_t *       the allocated list.
 *
 * @see      lstcn_t
 */
lstcn_t *lstc_new (void *data);

/**
 *
 * @brief    Creates a new empty Caffeine Circular List.
 *
 * Creates a new Caffeine Circular List completly empty.
 *
 * @return       lstcn_t *     the allocated list.
 *
 * @see      lstcn_t
 */
lstcn_t *lstc_create (void);

/**
 *
 * @brief    Deletes a Caffeine Circular List.
 *
 * Try to free the memory allocated for the circular list.
 * Gives as parameters the list to free and a callback function
 * to delete (free) the memory allocated by the list nodes.
 * The delete callback must return zero on success, otherwise
 * the operations halts, modifying the given list pointer to
 * node where the deleting operation wasn't successfull.
 *
 * @param[in]    lst    the Caffeine double linked list.
 * @param[in]    del    the callback function to free the list nodes.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_delete (lstcn_t *lst, CAF_LSTCNODE_CBDEL(del));

/**
 *
 * @brief    Deletes a node from the given Caffeine Circular List.
 *
 * Free the memory allocated by a Caffeine Circular List. Gives as
 * parameters the list node and a callback function to free the memory
 * allocated by the data pointer in the list node. The delete callback
 * must return zero on success, or the node will be not deleted.
 *
 * @param[in]    lst      the list.
 * @param[in]    n        the list node.
 * @param[in]    del      the callback function to free the node data.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_node_delete (lstcn_t *lst, lstcn_t *n, CAF_LSTCNODE_CBDEL(del));

/**
 *
 * @brief    Deletes a node from the given Caffeine CL using de data pointer.
 *
 * Free the memory allocated by a Caffeine Circular List. Gives as
 * parameters the list node and a callback function to free the memory
 * allocated by the data pointer in the list node. The delete callback
 * must return zero on success, or the node will be not deleted.
 *
 * @param[in]    lst      the list.
 * @param[in]    n        the list node.
 * @param[in]    del      the callback function to free the node data.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_node_delete_by_data (lstcn_t *lst, void *data, CAF_LSTCNODE_CBDEL(del));


/**
 *
 * @brief    Verifies if the list is empty or not
 *
 * Verifies if the list is empty or not.
 *
 * @param[in]    lst    The list to get length
 * @return       CAF_OK if is empty, CAF_ERROR if not.
 *
 * @see    lstc_t
 */
int lstc_empty_list (lstcn_t *lst);


/**
 *
 * @brief    Verifies if the list have only one item
 *
 * Verifies if the list have only one item.
 *
 * @param[in]    lst    The list to get length
 * @return       CAF_OK if does have one item, CAF_ERROR if not.
 *
 * @see    lstc_t
 */
int lstc_oneitem_list (lstcn_t *lst);


/**
 *
 * @brief    Returns the len of the given list.
 *
 * This function crosses the given list and returns his length.
 *
 * @param[in]    lst    The list to get length
 *
 * @see    lstc_t
 */
int lstc_length (lstcn_t *lst);

/**
 *
 * @brief    Push a data node in the given list.
 *
 * This function gets a Caffeine Double Linked List and search sets
 * the last node in the list with the data pointer data.
 *
 * @param[in]    lst            the list to push a node.
 * @param[in]    data           the data to link in the list.
 * @return       lstcn_t *      the pointer to the list.
 *
 * @see    lstcn_t
 * @see    lstc_pop
 * @see    lstc_replace
 */
lstcn_t *lstc_push (lstcn_t *lst, void *data);

/**
 *
 * @brief    Pops (gets and removes) the lats element in the list.
 *
 * This function pops the last element in the list, this means
 * gets and removes from the list the last element.
 *
 * @param[in]    lst            the list to pop an element.
 * @return       lstcn_t *      the node to get from the list.
 *
 * @see    lstcn_t
 * @see    lstcn_push
 * @see    lstcn_set
 */
lstcn_t *lstc_pop (lstcn_t *lst);

/**
 *
 * @brief    Sets a data node in the list.
 *
 * Sets a data node in the list through the position of the node
 * in the list. Positions starts in zero. A posible failure in
 * this function is that the data pointer is not NULL, then you
 * must use lstc_replace function.
 *
 * @param[in]    lst    list where to set the node.
 * @param[in]    pos    data node position.
 * @param[in]    data   data pointer in the list.
 * @return       int    the position if found, -1 on failure.
 *
 * @see    lstc_t
 * @see    lstcn_t
 * @see    lstc_push
 * @see    lstc_pop
 * @see    lstc_replace
 */
int lstc_set (lstcn_t *lst, int pos, void *data);

/**
 *
 * @brief    Sets a data node in the list.
 *
 * Sets a data node in the list through the position of the node
 * in the list. Positions starts in zero. A posible failure in
 * this function is that the data pointer is not NULL, then you
 * must use lstc_replace function.
 *
 * @param[in]    lst    list where to set the node.
 * @param[in]    pos    data node position.
 * @param[in]    data   data pointer in the list.
 * @return       int    the position if found, -1 on failure.
 *
 * @see    lstc_t
 * @see    lstcn_t
 * @see    lstc_push
 * @see    lstc_pop
 * @see    lstc_replace
 */
void *lstc_get (lstcn_t *lst, int pos);

/**
 *
 * @brief    Apply a function to the list data nodes.
 *
 * This function crosses the list and apply the parameter function step
 * as callback over every data node in the list. The function does not
 * verify if the data node pointer is NULL. The walk callback must return
 * zero on success, or the operation halts and the count of processed nodes
 * is returned.
 *
 * @param[in]    lst        the list to walk.
 * @param[in]    step       the function to apply.
 * @return       int        the number of afected nodes.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_walk (lstcn_t *lst, CAF_LSTCNODE_CBWALK(step));

/**
 *
 * @brief    Apply a function to the list data nodes.
 *
 * This function crosses the list and apply the parameter function step
 * as callback over every data node in the list. The function does not
 * verify if the data node pointer is NULL. The walk callback must return
 * zero on success, or the operation halts and the count of processed nodes
 * is returned. Also, the function checks every "step" in the process,
 * and on the first failure returns the number of nodes processed.
 *
 * @param[in]    lst        the list to walk.
 * @param[in]    step       the function to apply.
 * @return       int        the number of afected nodes.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_walk_checked (lstcn_t *lst, CAF_LSTCNODE_CBWALK(step));

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
 * @see      lstcn_t
 */
void *lstc_search (lstcn_t *lst, void *data, CAF_LSTCNODE_CBSRCH(srch));

/**
 *
 * @brief    Search a node using the given data pointer.
 *
 * Search a node in the list comparing the data pointers.
 *
 * @param[in]    lst        the list to walk.
 * @param[in]    data       data pointer in the node.
 * @return       int        the number of afected nodes.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
lstcn_t *lstc_search_node (lstcn_t *lst, void *data);

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
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_delete_cb (void *ptr);

/**
 *
 * @brief    Default callback to free memory allocated by strings.
 *
 * This the default function to free memory allocated for strings
 * in any Caffeine Circular List node. By default, the callback must
 * return 1 on success, a more complex callback should verify the
 * operations.
 *
 * @param[in]    str        the string pointer.
 * @return       int        1 on success, 0 on failure.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_str_delete_cb (void *ptr);

/**
 *
 * @brief    Caffeine Circular List Dumper.
 *
 * Dumps the Circular List to the given output FILE stream.
 * Gives as parameters the output FILE stream, the list to dump
 * and the lstcn_t dumper callback function.
 *
 * @param[in]    out        FILE output stream.
 * @param[in]    lst        lstcn_t pointer to dump.
 * @param[in]    dmp        lstcn_t callback dumper.
 *
 * @see      lstcn_t
 */
void lstc_dump (FILE *out, lstcn_t *lst, CAF_LSTCNODE_CBDUMP(dmp));

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
 * @see      lstc_t
 */
int lstc_dump_ptr (FILE *out, lstcn_t *lst);

/**
 *
 * @brief    Default string dumper.
 *
 * This is the default string dumper to use with CAF nodes with
 * strings a data pointers.
 *
 * @param[in]    data    a pointer to a string.
 *
 * @see      lstc_t
 * @see      lstcn_t
 */
int lstc_dump_str_cb (FILE *o, void *data);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_LSTC_H */
/* caf_data_lstc.h ends here */
