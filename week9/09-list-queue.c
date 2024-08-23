# include <assert.h>

#define ELT_T int
#include "09-list.c"

#define Queue			List
#define newQueue()		newList()
// add more #defines here
#define Queue_isEmpty(A)    List_isEmpty(A)
#define Queue_add(A, B)     List_addLast(A, B)
#define Queue_remove(A)     List_removeFirst(A)
#define Queue_first(A)      List_last(A)

int main(int argc, char **argv)
{
    struct Queue *queue = newQueue();

    assert(Queue_isEmpty(queue));

    for (int i = 1;  i < 10;  ++i) {
        Queue_add(queue, i*i);
        assert(!Queue_isEmpty(queue));
        assert(Queue_first(queue) == i*i);
    }

    for (int i = 1;  i < 10;  ++i) {
        assert(!Queue_isEmpty(queue));
        int j = Queue_remove(queue);
        assert(j == i*i);
    }

    assert(Queue_isEmpty(queue));

    printf("all tests passed\n");

    return 0;
}
