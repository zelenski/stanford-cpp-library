/*
 * memory.h
 * --------
 * This file declares utility functions dealing with memory regions and
 * allocation.
 *
 * @author Marty Stepp
 * @version 2019/04/16
 * - bug fixes for WIN64
 * @version 2018/12/04
 * - initial version
 */

#ifndef _memory_h
#define _memory_h

#include <cstdint>

namespace stanfordcpplib {
namespace memory {

/**
 * Fills in the given long int parameters with the distance from the given
 * pointer p to the stack, the heap, and the static storage area.
 * We work under the assumption that the smallest of these distances
 * represents the area in which p was allocated.
 */
void computeMemoryDistances(
        void* const p,
        uintptr_t& stackDist,
        uintptr_t& heapDist,
        uintptr_t& staticDist);

/**
 * Returns true if the given pointer points to memory on the heap.
 * (Note that this is not really possible to do in a cross-platform way for
 * all general cases; we provide a best effort that should work for most
 * common allocation cases.)
 */
bool isOnHeap(void* const p);

/**
 * Returns true if the given pointer points to memory on the stack.
 * (Note that this is not really possible to do in a cross-platform way for
 * all general cases; we provide a best effort that should work for most
 * common allocation cases.)
 */
bool isOnStack(void* const p);

/**
 * Returns true if the given pointer points to memory in static storage.
 * (Note that this is not really possible to do in a cross-platform way for
 * all general cases; we provide a best effort that should work for most
 * common allocation cases.)
 */
bool isOnStatic(void* const p);

} // namespace memory
} // namespace stanfordcpplib

#endif // _memory_h
