#ifndef _FH_GRAPHIC_FLEX_H
#define _FH_GRAPHIC_FLEX_H

#include "core/inc/define.h"
#include "core/inc/import.h"



#define FH_FLEX_ABSOLUTE	0
#define FH_FLEX_RELATIVE	1
#define FH_FLEX_INHERIT		2
#define FH_FLEX_AUTO		3


struct fh_flex {
	u8 				mode;
	u32 				value;
};


/*
 * This will set a flexvalue.
 *
 * Please note:
 *
 * 	For absolute values, you can just pass them directly to value. But for
 * 	relative values, which are represented by a percentage you have to use
 * 	the range 0 - 10000. This value will then be divided by 100 to give the
 * 	float value.
 *
 * @mode: The mode of the flexvalue
 * @value: The value of the flexvalue
 *
 * Returns: The set flexvalue
 */
FH_API struct fh_flex fh_flex_set(u8 mode, u32 value);


/*
 * Compute a flexvalue using the reference.
 *
 * @flex: The flexvalue to compute
 * @ref: The reference value
 *
 * Returns: The absolute value
 */
FH_API u32 fh_flex_comp(struct fh_flex *flex, u32 ref);


/*
 * Compute the flexvalue using the reference, but also consider limits.
 *
 * @flex: The flexvalue to compute
 * @min: The min limit
 * @max: The max limit
 * @ref: The reference value
 *
 * Returns: The absolute value
 */
FH_API u32 fh_flex_comp_lim(struct fh_flex *flex, struct fh_flex *min,
		struct fh_flex *max, u32 ref);


/*
 * Display a flex-constriction in the terminal. 
 *
 * @flex: Pointer to the flex
 */
FH_API void fh_flex_show(struct fh_flex *flex);

#endif /* _FH_GRAPHIC_FLEX_H */
