#include "bump.h"

#define HEAP_SIZE 1024

static _Alignas(16) unsigned char heap[HEAP_SIZE];  // куча
static size_t top = 0;                              // номер первого свободного байта

// Задача 2: напишите здесь bump_malloc, bump_free и bump_used из bump.h.
