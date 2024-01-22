#ifndef _FH_UTILITY_LIST_H
#define _FH_UTILITY_LIST_H

#include "core/inc/define.h"
#include "core/inc/import.h"


struct fh_list {
	s16 size;	/* The size of a single entry */

	s16 count;	/* The number of entries */
	s16 alloc;	/* The allocated number of entries */

	u8 *data;
};


/*
 * Create a new list and preallocate the number of entries.
 *
 * @size: The size of a single entry in bytes
 * @alloc: The number of slots to preallocate
 *
 * Returns: Either a pointer to the newly created list or NULL if an error
 * 	    occurred
 */
FH_API struct fh_list *fh_list_create(s16 size, s16 alloc);


/*
 * Destroy a list and free the allocated memory.
 *
 * @lst: Pointer to the list
 */
FH_API void fh_list_destroy(struct fh_list *lst);


/*
 * Push a new entry onto the list.
 *
 * @lst: Pointer to the list
 * @inp: Pointer to the new entry
 *
 * Returns: On success the number of elements in the list and if an error
 *          occurred -1
 */
FH_API s16 fh_list_push(struct fh_list *lst, void *inp);


/*
 * Pop the newest entry from the list and write it to the given pointer. Note,
 * this function returns 0 if the list is empty on call.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the popped entry to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
FH_API s16 fh_list_pop(struct fh_list *lst, void *out);


/*
 * Get the oldest element in the list.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the shifted element to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
FH_API s16 fh_list_shift(struct fh_list *lst, void *out);


/*
 * Get the oldest element in the list without removing it.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the element to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
FH_API s16 fh_list_test_head(struct fh_list *lst, void *out);


/*
 * Get the newest element in the list without removing it.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the element to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
FH_API s16 fh_list_test_tail(struct fh_list *lst, void *out);

#endif /* _FH_UTILITY_LIST_H */
