#include <assert.h>

#define ELT_T int
#include "09-list.c"

#define Stack			List
#define newStack()		newList()
// add more #defines here
# define Stack_isEmpty(A)	List_isEmpty(A)
# define Stack_push(A, B)	List_addFirst(A, B)
# define Stack_pop(A)		List_removeFirst(A)
# define Stack_top(A)		List_first(A)

int main(int argc, char **argv)
{
    struct Stack *stack = newStack();

    assert(Stack_isEmpty(stack));

    for (int i = 1;  i < 10;  ++i) {
        Stack_push(stack, i*i);
        assert(!Stack_isEmpty(stack));
        assert(Stack_top(stack) == i*i);
    }

    for (int i = 9;  i > 0;  --i) {
        assert(!Stack_isEmpty(stack));
        int j = Stack_pop(stack);
        assert(j == i*i);
    }

    assert(Stack_isEmpty(stack));

    printf("all tests passed\n");

    return 0;
}
