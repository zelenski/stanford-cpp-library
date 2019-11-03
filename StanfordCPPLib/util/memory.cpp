/*
 * memory.cpp
 * ----------
 * This file defines utility functions dealing with memory regions and
 * allocation.
 *
 * @author Marty Stepp
 * @version 2019/04/16
 * - bug fixes for WIN64
 * @version 2018/12/04
 * - initial version
 */

#include "memory.h"
#include <cstddef>

namespace stanfordcpplib {
namespace memory {

void computeMemoryDistances(
        void* const p,
        uintptr_t& stackDist,
        uintptr_t& heapDist,
        uintptr_t& staticDist) {
    static int si = 0;
    int i = 0;
    int* pi = new int(0);

    // don't use abs/subtraction here because it can over/underflow with very large integers
    stackDist = p > &i ?
            reinterpret_cast<uintptr_t>(p) - reinterpret_cast<uintptr_t>(&i) :
            reinterpret_cast<uintptr_t>(&i) - reinterpret_cast<uintptr_t>(p);
    heapDist = p > pi ?
            reinterpret_cast<uintptr_t>(p) - reinterpret_cast<uintptr_t>(pi) :
            reinterpret_cast<uintptr_t>(pi) - reinterpret_cast<uintptr_t>(p);
    staticDist = p > &si ?
            reinterpret_cast<uintptr_t>(p) - reinterpret_cast<uintptr_t>(&si) :
            reinterpret_cast<uintptr_t>(&si) - reinterpret_cast<uintptr_t>(p);
    delete pi;
}

bool isOnHeap(void* const p) {
    uintptr_t stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return heapDist < stackDist && heapDist < staticDist;
}

bool isOnStack(void* const p) {
    uintptr_t stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return stackDist < heapDist && stackDist < staticDist;
}

bool isOnStatic(void* const p) {
    uintptr_t stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return staticDist < stackDist && staticDist < heapDist;
}

} // namespace memory
} // namespace stanfordcpplib
