OUTPUT_FORMAT(elf32-i386)
ENTRY(DEVICE_START)
SECTIONS {
   . = 0x100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
}
