.section .text
.global gdt_load
.type gdt_load,@function
gdt_load:
 movl 4(%esp),%eax
 lgdt (%eax)
 ljmp $0x08,$1f
1:
 movw $0x10,%ax
 movw %ax,%ds
 movw %ax,%es
 movw %ax,%fs
 movw %ax,%gs
 movw %ax,%ss
 ret
