/**
 * @file    custom_alloc.cpp
 * @brief   Custom malloc() and free() implementations which wrap FreeRTOS allocation functions.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <cstddef>

extern "C" void* pvPortMalloc(size_t xSize);
extern "C" void vPortFree(void* pv);

extern "C" void* malloc(size_t size)
{
    return pvPortMalloc(size);
}

extern "C" void free(void* ptr)
{
    vPortFree(ptr);
}