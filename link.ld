OUTPUT_FORMAT(elf32-i386)
ENTRY(DEVICE_START)
SECTIONS {
   . = 0x0;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
}
