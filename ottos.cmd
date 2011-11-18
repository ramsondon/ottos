////////////////////////////////////////////////////////
// Linker command file for BeagleBoard)
// 
// Basic configuration using only external memory
//

-stack           0x00002000
-heap            0x00002000

MEMORY
{
   int_ram:  ORIGIN = 0x40200000  LENGTH = 0x0000FFFF
   ext_ddr:  ORIGIN = 0x82000000  LENGTH = 0x00010000
}

SECTIONS
{
   .intswi     > 0x4020FFCC
   .intirq     > 0x4020FFDC

   .const      > ext_ddr
   .text       > ext_ddr
  
   	
   .bss        > int_ram
    {
   	_kernelMasterTable = . ;
   	. = . + (16*1024);
   	*(.bss)
   	}
   .far        > int_ram
   
   .stack      > int_ram
   .data       > int_ram
   .cinit      > int_ram
   .cio        > int_ram
   
   
   .sysmem     > int_ram
   .switch     > int_ram
}
