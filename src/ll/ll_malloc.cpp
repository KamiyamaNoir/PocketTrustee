#include "main.h"
#include "FreeRTOS.h"
#include "portable.h"

void* operator new(size_t size) {
    DEBUG_INFO("malloc %d", size);
    return pvPortMalloc(size);
}

void operator delete(void* p) noexcept {
    DEBUG_INFO("free %p", p);
    vPortFree(p);
}