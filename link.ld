ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5); /* R-X */
    data PT_LOAD FLAGS(6); /* RW- */
}

SECTIONS
{
    . = 0x08048000;

    .text : {
        *(.text.entry)
        *(.text .text.*)
    } :text

    .rodata : { *(.rodata .rodata.*) } :text
    .data   : { *(.data .data.*) } :data
    .bss    : { *(.bss .bss.* COMMON) } :data

    /DISCARD/ : { *(.comment) *(.eh_frame*) }
}
