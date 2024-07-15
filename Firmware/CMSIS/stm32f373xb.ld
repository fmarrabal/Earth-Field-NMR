MEMORY {
  flash (rx)     : ORIGIN = 0x08000000, LENGTH = 64K
  ram (xrw)      : ORIGIN = 0x20000000, LENGTH = 24K
}

STACK_END = ORIGIN(ram) + LENGTH(ram);

SECTIONS {
  .text ORIGIN(flash): ALIGN(4) {
	  *(.vectors);
	  *(.text);
    *(.rodata .rodata.*)
  } > flash

  .data ORIGIN(ram): {
	  DATA_VALUES = LOADADDR(.data);
	  DATA_START = .;
	  *(.data);
	  DATA_END = .;
  } > ram AT > flash

	.bss (NOLOAD) : {
    BSS_START = .;
	  *(.bss);
    BSS_END = .;
	} > ram
}