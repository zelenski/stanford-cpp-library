/**
 * MemoryUtils.h
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 */
#pragma once

/**
 * Macro: DISALLOW_COPYING_OF(Type)
 *
 * Disables copying / assignment of the specified type.
 */
#define DISALLOW_COPYING_OF(Type)                                           \
    Type(const Type &) = delete;                                            \
    Type(Type &&) = delete;                                                 \
    void operator= (Type) = delete



