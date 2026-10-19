OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 1M;

    /* Must be the first allocatable bytes in the ELF file so GRUB finds
       the Multiboot v1 header within its first 8 KiB search window. */
    .multiboot : ALIGN(4)
    {
        KEEP(*(.multiboot))
    } :text

    .text ALIGN(4K) :
    {
        *(.text*)
    } :text

    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    } :text

    .data ALIGN(4K) :
    {
        *(.data*)
    } :data

    .bss ALIGN(4K) (NOLOAD) :
    {
        *(COMMON)
        *(.bss*)
    } :data

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame*)
        *(.note*)
    }
}
