#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef TEST_LIST // defined if we are compiling the unit tests
# define ELT_T int
#endif

#ifndef ELT_T // the client program forgot to say what the element type is
# error "List: ELT_T is not defined"
#endif

/* struct Link represents each link in a linked list.
 * aLink->data holds one element in the list.
 * aLink->next holds a pointer to the next link, or 0 if aLink is the last link in the list.
 */

struct Link {
    ELT_T    data;  // the element stored in this link
    struct Link *next;  // pointer to next link, or 0 if this is the last link
};

/* Create a new link with the given element data and pointer to the next link in the list.
 */
struct Link *newLink(ELT_T data, struct Link *next)
{
    struct Link *link = malloc(sizeof(*link));
    link->data = data;
    link->next = next;
    return link;
}

/* Delete a link.
 */
void Link_delete(struct Link *link)
{
    free(link);
}

/* struct List represents an entire linked list.
 * If aList is empty then both aList->first and aList->last should be 0.
 * If aList contains one link then aList->first and aList->last both point to it.
 * If aList has more than one link then...
 *   aList->first points to the first link in the list, and
 *   aList->last points to the last link in the list.
 * The last link in the list should always have its 'next' pointer set to 0.
 */

struct List {
    struct Link *first; // first Link in list, or 0 if list is empty
    struct Link *last;  // last Link in list, or 0 if list is empty
};

/* This expression can be used to initialise a non-malloc()ed list, like this:
 *    struct List myList = LIST_INITIALISER;
 */
#define List_INITIALISER { 0, 0 }

/* Initialise the given list to empty.
 * Assume there was nothing stored there.
 */
struct List *List_init(struct List *list)
{
    list->first = 0; // first and last are both 0 when the list is empty
    list->last  = 0;
    return list;
}

/* Allocate, initialise, and return a new list object.
 */
struct List *newList(void)
{
    return List_init(malloc(sizeof(struct List)));
}

/* Delete a List and all the links in it.
 */
void List_delete(struct List *list)
{
    struct Link *link = list->first;
    while (link != 0) {                  // there are more links to delete
	struct Link *next = link->next;  // remember this because...
	Link_delete(link);               //   link no longer exists
	link = next;                     // move to the next link in the list
    }
    free(list);                          // delete the list object
}

/* Return true if the list is empty, otherwise 0.
 */
int List_isEmpty(struct List *list)
{
    return 0 == list->first;
}

/* Ensure that the list is not empty.
 */
void List_emptyCheck(struct List *list)
{
    if (List_isEmpty(list)) {
	fprintf(stderr, "list is empty\n");
	exit(1);
    }
}

/* Count and return the number of links in the list.
 */
size_t List_size(struct List *list)
{
    size_t size = 0;
    for (struct Link *link = list->first;  link != 0;  link = link->next)
	++size;
    return size;
}

/* Add an element to the start of the list.
 */
void List_addFirst(struct List *list, ELT_T element)
{
    struct Link *link = newLink(element, list->first);
    list->first = link;
    if (0 == list->last) list->last = link;
}

/* Remove and return an element from the front of the list.
 */
ELT_T List_removeFirst(struct List *list)
{
    List_emptyCheck(list);
    struct Link *link = list->first;
    list->first = link->next;
    if (0 == list->first) list->last = 0;
    ELT_T data = link->data;
    Link_delete(link);
    return data;
}

/* Add an element to the end of the list.
 */
void List_addLast(struct List *list, ELT_T element)
{
    struct Link *link = newLink(element, 0);
    if (0 != list->last) {
	assert(0 == list->last->next);
	list->last->next = link;
    }
    list->last = link;
    if (0 == list->first) list->first = link;
}

/* Return the element at the start of the list.
 * Requires that the list is not empty.
 */
ELT_T List_first(struct List *list)
{
    List_emptyCheck(list);
    return list->first->data;
}

/* Return the element at the end of the list.
 * Requires that the list is not empty.
 */
ELT_T List_last(struct List *list)
{
    List_emptyCheck(list);
    return list->last->data;
}

#ifdef TEST_LIST // this is defined if we should include the unit tests

int main(int argc, char**argv)
{
    struct List list = List_INITIALISER;

    assert(List_isEmpty(&list));
    assert(0 == List_size(&list));
    List_addFirst(&list, 42);
    assert(!List_isEmpty(&list));
    assert(1 == List_size(&list));
    assert(42 == List_first(&list));
    assert(42 == List_last(&list));
    assert(42 == List_removeFirst(&list));
    assert(List_isEmpty(&list));
    assert(0 == List_size(&list));

    List_addLast (&list, 2);
    List_addFirst(&list, 1);
    List_addLast (&list, 3);
    List_addFirst(&list, 0);
    assert(4 == List_size(&list));
    assert(0 == List_first(&list));
    assert(3 == List_last (&list));
    assert(0 == List_removeFirst(&list));
    assert(1 == List_removeFirst(&list));
    assert(2 == List_removeFirst(&list));
    assert(3 == List_removeFirst(&list));
    assert(List_isEmpty(&list));

    List_addFirst(&list, 2);
    List_addLast (&list, 1);
    List_addFirst(&list, 3);
    List_addLast (&list, 0);
    assert(4 == List_size(&list));
    assert(0 == List_last (&list));
    assert(3 == List_first(&list));
    assert(0 == List_removeLast(&list));
    assert(1 == List_removeLast(&list));
    assert(2 == List_removeLast(&list));
    assert(3 == List_removeLast(&list));
    assert(List_isEmpty(&list));

    printf("all tests passed\n");

    return 0;
}

#endif
