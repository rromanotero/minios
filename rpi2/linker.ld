
MEMORY
{
	ram : ORIGIN = 0x8000, LENGTH = 0x10000000
}

__stack_size__ = DEFINED(__stack_size__) ? __stack_size__ : 0x3000;

SECTIONS
{
   . = 0x8000;
    __start = .;


    .text : {
        __text_start = .;
        KEEP(*(.boot))
        *(.text)
        __text_end = .;
    } > ram



    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)
        __data_end__ = .;
    } > ram



    .bss (NOLOAD):
    {
       . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        __bss_end__ = .;
    } > ram



    .rodata : {
        . = ALIGN(4);
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
    } > ram



      .stack (NOLOAD):
      {
          . = ALIGN(8);
          __stack_start = .;
          . = . + __stack_size__;
          __stack_end = .;
      } > ram



	__end = .;
}
