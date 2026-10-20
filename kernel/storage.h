#ifndef HYBRID_STORAGE_H
#define HYBRID_STORAGE_H
#include <stdint.h>
/* Read-only legacy ATA/IDE inventory. No write-sector API is exposed. */
uint8_t storage_scan(uint8_t *partition_count);
#endif
