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
   ext_ddr:  ORIGIN = 0x82000000  LENGTH = 0x00100000
}

SECTIONS
{
   .intudef    > 0x4020FFC8
   .intswi     > 0x4020FFCC
   .intpabt	   > 0x4020FFEC
   .intdabt    > 0x4020FFF0
   .intirq     > 0x4020FFDC   

   .const      > ext_ddr
   .bss        > ext_ddr
   .far        > ext_ddr
   
   .stack      > ext_ddr
   .data       > ext_ddr
   .cinit      > ext_ddr
   .cio        > ext_ddr
   
   .text       > int_ram
   .sysmem     > ext_ddr
   .switch     > ext_ddr
}
