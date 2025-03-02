#include <stddef.h>
#include "../include/min_heap.h"

// MEMORY ALLOCATION CONTROL U EKLEMEDIM

// Creates a new heap with initial capacity
// Element_size specifies the size of stored elements in bytes
// Compare function must return negative if first argument is smaller
MinHeap *heap_create(size_t capacity, size_t element_size, int (*compare)(const void *, const void *))
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap)
        return NULL;
    heap->capacity = capacity;
    heap->element_size = element_size;
    heap->size = 0;
    heap->compare = compare;
    heap->data = malloc(heap->element_size * capacity);
    if (!heap->data)
    {
        free(heap);
        return NULL;
    }

    return heap;
}

// Deallocates all memory used by the heap
void heap_destroy(MinHeap *heap)
{
    if (!heap)
        return;
    if (heap->data)
        free(heap->data);
    free(heap);
}

// Adds new element to heap
// Element is added at the end and bubbled up to maintain heap property
// If heap is full, capacity is doubled automatically
// Returns 1 if successful, 0 if memory allocation fails
int heap_insert(MinHeap *heap, const void *element)
{
    if (!heap || !element) // if no heap or no element
        return 0;
    if (heap->size >= heap->capacity)
    {
        if (heap->capacity == 0)
            heap->capacity = 1;
        else
            // double the capacity
            heap->capacity *= 2;
        // realloc
        void *newData = realloc(heap->data, heap->capacity * heap->element_size);
        if (!newData)
            return 0; // allocation failed
        heap->data = newData;
    }
    // add new element to the end
    void *target = (char *)heap->data + (heap->size * heap->element_size);
    memcpy(target, element, heap->element_size);
    heap->size++;
    heapify_up(heap, heap->size - 1);
    return 1;
}

void swap(void *a, void *b, size_t element_size)
{
    void *temp = malloc(element_size);
    if (!temp)
        return;
    memcpy(temp, a, element_size); // temp  = current
    memcpy(a, b, element_size);    // current = parent
    memcpy(b, temp, element_size); // parent = temp
    free(temp);                    // free temp
}

// bunu kendim sıfırdan yazacağım
void heapify_up(MinHeap *heap, size_t index)
{
    if (!heap || !heap->data || index <= 0)
        return;
    size_t parentIndex = (index - 1) / 2;
    void *current = (char *)heap->data + (index * heap->element_size);
    void *parent = (char *)heap->data + (parentIndex * heap->element_size);
    if (heap->compare(current, parent) < 0) // if first smaller returns negative <0
    {                                       // parent equal or bigger
        // swap
        swap(current, parent, heap->element_size);
        index = parentIndex;
    }
    else // heapify done
        return;
    heapify_up(heap, index);
}
void heapify_down(MinHeap *heap, size_t i)
{
    if (!heap || !heap->data || i >= heap->size)
        return;

    size_t smallest = i;
    size_t leftIndex = 2 * i + 1;
    size_t rightIndex = 2 * i + 2;

    void *curr = (char *)heap->data + (i * heap->element_size);
    void *left = NULL;
    void *right = NULL;

    if (leftIndex < heap->size)
        left = (char *)heap->data + ((2 * i + 1) * heap->element_size);
    if (rightIndex < heap->size)
        right = (char *)heap->data + ((2 * i + 2) * heap->element_size);

    if (left && heap->compare(left, curr) < 0)                                     // if first smaller returns negative <0
        smallest = leftIndex;                                                      // so left choosen
    if (right && heap->compare(right, curr) < 0 && heap->compare(right, left) < 0) // right smaller than left So right chosen
        smallest = rightIndex;

    if (smallest == i) // no more need to continue // DONE
        return;
    else if (smallest == leftIndex)
        swap(curr, left, heap->element_size);
    else
        swap(curr, right, heap->element_size);

    heapify_down(heap, smallest);
}

// Removes and returns the minimum element (root)
// Last element is moved to root and bubbled down
// Returns 1 if successful, 0 if heap is empty
int heap_extract_min(MinHeap *heap, void *result)
{
    if (!heap || heap->size == 0)
        return 0;
    memcpy(result, heap->data, heap->element_size); // root is the smallest => will be result

    void *lastElement = (char *)heap->data + ((heap->size - 1) * heap->element_size);

    memcpy(heap->data, lastElement, heap->element_size); // place last element to the root

    heap->size--; // last Element removed // out of size

    heapify_down(heap, 0); // heapify from the root to the leaves

    return 1;
}

// Returns the minimum element without removing it
// Returns 1 if successful, 0 if heap is empty
int heap_peek(const MinHeap *heap, void *result)
{
    if (!heap || !heap->data || heap->size == 0)
        return 0;
    memcpy(result, heap->data, heap->element_size);
    return 1;
}

// Returns current number of elements in heap
size_t heap_size(const MinHeap *heap)
{
    if (!heap)
        return 0;
    return heap->size;
}

// Merges heap2 into heap1
// Grows capacity of heap1 if needed
// Returns 1 if successful, 0 if memory allocation fails or heaps are incompatible
int heap_merge(MinHeap *heap1, const MinHeap *heap2)
{
    // check whether they are using same data type
    if (heap1->element_size != heap2->element_size || heap1->compare != heap2->compare) // what else ?
        return 0;

    if (!heap2)
        return 1;

    if (!heap1)
        heap1 = heap_create(heap2->capacity, heap2->element_size, heap2->compare);

    for (int i = heap2->size - 1; i >= 0; i--)
    {
        void *lastElementOf2 = malloc(sizeof(heap2->element_size)); // heap1->element_size = heap2->element_size
        if (!lastElementOf2)
            return 0;
        lastElementOf2 = (char *)heap2->data + (i * heap2->element_size);

        heap_insert(heap1, lastElementOf2);
    }

    return 1;
}
