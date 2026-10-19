#include <stdint.h>
#include "serial.h"
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static inline uint8_t inb(uint16_t p){uint8_t v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}
void serial_init(void){outb(0x3F9,0);outb(0x3FB,0x80);outb(0x3F8,3);outb(0x3F9,0);outb(0x3FB,3);outb(0x3FA,0xC7);outb(0x3FC,0x0B);}
void serial_write(const char*s){while(*s){while((inb(0x3FD)&0x20u)==0u){}outb(0x3F8,(uint8_t)*s++);}}
