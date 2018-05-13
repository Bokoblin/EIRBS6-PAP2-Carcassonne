#ifndef SET_H
#define SET_H

#include <stddef.h>

/**
 * Implementation of an ordered LIST Abstract Data Type
 *
 * YOU HAVE TO GIVE TO THE SET (at the initialisation):
 *
 * A comparison function respecting the following rule:
 * int function( void* x,  void* y);
 * - x > y : 1
 * - x == y : 0
 * - x < y : -1
 *
 * A copy function for the type you want to use, using a malloc :
 * simple example (just query replace the <type>)
 * void* <type>_copy( void* x){
 *   <type> *y = x;
 *   <type>* y_copy = malloc(sizeof(type));
 *   *y_copy = *y;
 *   return y_copy;
 * }
 *
 * A free function for all the allocated memory used by the type
 * * void delete(void*);
 */

/**
 * @brief A sorted ADT to stock elements of the same type.
 * 
 */
struct set;


/**
 * @brief Set the empty object
 * 
 * @param copy_op is a copy function for the used type.
 * The copy function must have the following prototype and use a malloc:
 * void* copy_op(void*);
 * 
 * @param delete_op is a free function for all the allocated memory used by the type
 * The delete function must have the following prototype:
 * void delete_op(void*);
 * 
 * @param compare_op is a comparison function for the used type.
 * The comparison function must have the following prototype:
 * int cmp_op(void* x, void* y);
 * It have to respect the following rule:
 * - x > y : 1
 * - x == y : 0
 * - x < y : -1
 *
 * @param debug_op is a function displaying set debug information and its content
 * 
 * @return An empty struct set* 
 */
struct set *set__empty(void *copy_op, void *delete_op, void *compare_op, void *debug_op);


/**
 * @brief Test to know if a set is empty
 * 
 * @param s is the given set
 * @return 1 if the set is empty, 0 else
 */
int set__is_empty(const struct set *s);


/**
 * @brief Searches an object into the given set
 * 
 * @param s is the given set
 * @param e is the object you're searching
 * @return 1 if the object is in the set, 0 else
 */
int set__find(const struct set *s, void* e);


/**
 * @brief Give the current amount of objects in the set
 * 
 * @param s is the set
 * @return The amount of objects in the set
 */
size_t set__size(const struct set *s);


/**
 * @brief Adds a copy of an object into the given set
 * The object won't be added if it already is in the set.
 * 
 * @param s is the set
 * @param e is the object you want to add
 * @return 0 for a success, 1 for a failure
 */
int set__add(struct set *s, void* e);

/**
 * @brief Adds an object into the given set, without copy.
 * The object won't be added if it already is in the set.
 * 
 * WARNINGS : Every object added in the set must be dynamically allocated.
 * There will be an issue if it's allocated in the stack, because it will be deleted at the end of the function.
 * There will also be an issue in the "set__free" if it's not dynamically allocated.
 * DO NOT free those objects, or there will be a double free in "set__free".
 * If set__add == 1, the object have to be freed.
 * It is recommended to use only set__add or only set__add_no_copy for a given set.
 * 
 * @param s is the set
 * @param e is the object you want to add
 * @return 0 for a success, 1 for a failure
 */
int set__add_no_copy(struct set *s, void* e);


/**
 * @brief Remove an object from the given set
 * 
 * @param s is the set
 * @param e is the object you want to remove
 * @return 0 for a success, 1 for a failure
 */
int set__remove(struct set *s, void* e);


/**
 * @brief Gets the pointer stocked into the set to the researched element
 * @attention DO NOT FREE THAT POINTER !! Or the set__free will have a double-free issue.
 * @warning As the real pointer is retrieved, if you need to edit its content, you shouldn't use this function only
 * if it affects element comparison, as the set is ordered at add step, this will break search feature otherwise.
 * In this case you should use set__retrieve, then make a copy and edit it, use set__remove, and set__add at last
 * 
 * @param s is the given set
 * @param e is the element you're looking for
 * @return A pointer to the data stocked by the set without a copy.
 */
void* set__retrieve(struct set *s, void* e);


/**
 * @brief Get the element in position i into the set.
 * 
 * @param s in the given set
 * @param i is the i th element into the set (0 <= i < set__size(set))
 * @return a copy of the umpteenth element stocked into the set if i is valid, NULL else.
 */
void* set__get_umpteenth(struct set const *s, size_t i);


/**
 * @brief Get the element in position i into the set.
 * @attention DO NOT FREE THAT POINTER !! Or the set__free will have a double-free issue.
 * @warning As the real pointer is retrieved, if you need to edit its content, you shouldn't use this function only
 * if it affects element comparison, as the set is ordered at add step, this will break search feature otherwise.
 * In this case you should use set__retrieve, then make a copy and edit it, use set__remove, and set__add at last
 *
 * @param s in the given set
 * @param i is the i th element into the set (0 <= i < set__size(set))
 * @return the umpteenth element stocked into the set if i is valid, NULL else.
 */
void* set__get_umpteenth_no_copy(struct set const *s, size_t i);


/**
 * @brief Realise the fusion of the two given set
 * WARNING:The functions given in the initialisation MUST BE the same for the two sets.
 * 
 * @return a struct set which is the fusion of the two given set
 */
struct set *set__fusion(struct set const *set1, struct set const *set2);


/**
 * @brief Free all the allocated memory in the set.
 * 
 * @param s is the set you want to free
 */
void set__free(struct set *s);


/**
 * @brief Prints the set
 * 
 * @param s is the set you want to print
 * @param is_compact to display a compact debug (only values)
 */
void set__debug(const struct set *s, int is_compact);


/**
 * function pointer for lambda applying
 */
typedef void (*applying_func_t)(void *object);

/**
 * @brief Applies a function with only a set object as parameters
 * @param s the set
 * @param f the applying function
 */
void set__apply_to_all(struct set *s, applying_func_t f);

#endif
