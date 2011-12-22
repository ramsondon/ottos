////////////////////////////////////////////////////////
// Linker command file for BeagleBoard)
// 
// Basic configuration using only external memory
//

-stack           0x00002000
-heap            0x00002000

MEMORY
{
   int_ram:  ORIGIN = 0x40200000  LENGTH = 0x0000BFC4
   int_vecs: ORIGIN = 0x4020BFC4  LENGTH = 0x0000003B
   ext_ddr:  ORIGIN = 0x82000000  LENGTH = 0x02000000 // 256 MBit
}

SECTIONS
{
   .intvecs    > int_vecs
   
   .irq        > int_ram {
   		irq.obj
   }
   
   .const      > ext_ddr
   .bss        > ext_ddr
   .far        > ext_ddr
   
   .stack      > ext_ddr
   .data       > ext_ddr
   .cinit      > ext_ddr
   .cio        > ext_ddr
   
   .text       > ext_ddr
   .sysmem     > ext_ddr
   .switch     > ext_ddr
}
