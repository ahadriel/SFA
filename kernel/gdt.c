#include <stdint.h>
#include "gdt.h"
typedef struct __attribute__((packed)){uint16_t limit;uint32_t base;} gdtr_t;
static uint64_t gdt[3];
extern void gdt_load(const gdtr_t *);
void gdt_init(void){
 gdt[0]=0;
 gdt[1]=0x00CF9A000000FFFFull;
 gdt[2]=0x00CF92000000FFFFull;
 gdtr_t p={(uint16_t)(sizeof(gdt)-1u),(uint32_t)(uintptr_t)gdt};
 gdt_load(&p);
}
