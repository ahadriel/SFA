#include <stdint.h>
#include "interrupts.h"
#include "serial.h"
#define NIDT 256u
#define KBUF 128u
typedef struct __attribute__((packed)){uint16_t lo,sel;uint8_t zero,flags;uint16_t hi;} idt_entry;
typedef struct __attribute__((packed)){uint16_t limit;uint32_t base;} idtr;
static idt_entry idt[NIDT];static volatile uint32_t head,tail;static volatile uint8_t keys[KBUF];
extern void isr_default(void);
#define D(n) extern void isr##n(void)
D(0);D(1);D(2);D(3);D(4);D(5);D(6);D(7);D(8);D(9);D(10);D(11);D(12);D(13);D(14);D(15);D(16);D(17);D(18);D(19);D(20);D(21);D(22);D(23);D(24);D(25);D(26);D(27);D(28);D(29);D(30);D(31);D(32);D(33);
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static void set_gate(uint8_t n,void(*fn)(void)){uint32_t a=(uint32_t)(uintptr_t)fn;idt[n].lo=(uint16_t)a;idt[n].sel=8;idt[n].zero=0;idt[n].flags=0x8E;idt[n].hi=(uint16_t)(a>>16);}
static void pic_init(void){outb(0x20,0x11);outb(0xA0,0x11);outb(0x21,0x20);outb(0xA1,0x28);outb(0x21,4);outb(0xA1,2);outb(0x21,1);outb(0xA1,1);outb(0x21,0xFC);outb(0xA1,0xFF);}
void interrupts_init(void){void(*h[34])(void)={isr0,isr1,isr2,isr3,isr4,isr5,isr6,isr7,isr8,isr9,isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19,isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29,isr30,isr31,isr32,isr33};for(uint32_t i=0;i<NIDT;i++)set_gate((uint8_t)i,isr_default);for(uint32_t i=0;i<34;i++)set_gate((uint8_t)i,h[i]);idtr d={(uint16_t)(sizeof(idt)-1u),(uint32_t)(uintptr_t)idt};__asm__ volatile("lidt %0"::"m"(d));pic_init();uint32_t div=1193182u/100u;outb(0x43,0x36);outb(0x40,(uint8_t)div);outb(0x40,(uint8_t)(div>>8));__asm__ volatile("sti");}
uint8_t keyboard_read_scancode(void){if(tail==head)return 0;uint8_t c=keys[tail%KBUF];tail++;return c;}
void interrupt_dispatch(uint32_t v){if(v==32){outb(0x20,0x20);return;}if(v==33){uint8_t c;__asm__ volatile("inb $0x60,%0":"=a"(c));uint32_t n=head+1u;if(n-tail<=KBUF){keys[head%KBUF]=c;head=n;}outb(0x20,0x20);return;}if(v>=40&&v<=47)outb(0xA0,0x20);if(v>=32&&v<=47){outb(0x20,0x20);return;}serial_write("HYBRID PANIC: CPU exception\r\nSystem halted.\r\n");__asm__ volatile("cli");for(;;)__asm__ volatile("hlt");}
