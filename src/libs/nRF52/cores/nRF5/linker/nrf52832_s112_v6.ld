/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x19000, LENGTH = 0x6D000 - 0x19000

  /* SRAM required by S112 depend on
   * - Attribute Table Size
   * - Vendor UUID count
   * - Max ATT MTU
   * - Concurrent connection peripheral + secure links
   * - Event Len, HVN queue, Write CMD queue
   */ 
  RAM (rwx) :  ORIGIN = 0x20002300, LENGTH = (0x20010000 - 0x08) - 0x20002300
  NOINIT (rwx) :  ORIGIN = 0x20010000 - 0x08, LENGTH = 0x8
}

SECTIONS
{
  . = ALIGN(4);
  .noinit(NOLOAD):
  {
    PROVIDE(__start_noinit_data = .);
    *(.noinit)
    . = ALIGN(4);
    PROVIDE(__stop_noinit_data = .);
  } > NOINIT
} INSERT AFTER .bss

SECTIONS
{
  . = ALIGN(4);
  .svc_data :
  {
    PROVIDE(__start_svc_data = .);
    KEEP(*(.svc_data))
    PROVIDE(__stop_svc_data = .);
  } > RAM
  
  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM 

} INSERT AFTER .data;

INCLUDE "nrf52_common.ld"
