.section .text
.extern interrupt_dispatch
.global isr_default
isr_default:
 cli
1: hlt
 jmp 1b
.macro ISR_NOERR n
.global isr\n
isr\n:
 pushl $0
 pushl $\n
 jmp isr_common
.endm
.macro ISR_ERR n
.global isr\n
isr\n:
 pushl $\n
 jmp isr_common
.endm
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_ERR 29
ISR_ERR 30
ISR_NOERR 31
ISR_NOERR 32
ISR_NOERR 33
isr_common:
 pusha
 pushl 32(%esp)
 call interrupt_dispatch
 addl $4,%esp
 popa
 addl $8,%esp
 iret
