#ifndef HYBRID_INTERRUPTS_H
#define HYBRID_INTERRUPTS_H
#include <stdint.h>
void interrupts_init(void);
uint8_t keyboard_read_scancode(void);
void interrupt_dispatch(uint32_t vector);
#endif
