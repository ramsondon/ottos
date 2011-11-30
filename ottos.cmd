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
   ext_ddr:  ORIGIN = 0x82000000  LENGTH = 0x10000000


}


SECTIONS
{

///////////////////////////// int_ram //////////////////////////////

   .intudef    > 0x4020FFC8
   .intswi     > 0x4020FFCC
   .intirq     > 0x4020FFDC
   .intpabt	   > 0x4020FFEC
   .intdabt    > 0x4020FFF0
   

   .bss        > int_ram {
       _kernelMasterTable = . ;
       . = . + (16 * 1024);
       *(.bss)
   }
   .far        > int_ram
   
   .stack      > int_ram
   .data       > int_ram
   .cinit      > int_ram
   .cio        > int_ram


   .switch     > int_ram
   .pinit      > int_ram {
   		*(.pinit)
   		_intRamStart = .;
   	}


///////////////////////////// ext_ddr //////////////////////////////
   .const      > ext_ddr
   .text	   > ext_ddr
   .sysmem     > ext_ddr
   .stackArea  > ext_ddr {
       . = . + (4 * 1024);
       kernelStack = .;
       . = . + (4 * 1024);
       irqStack = .;
       . = . + (4 * 1024);
       systemStack = .;
       . = . + (4 * 1024);
       _extDDRStart = .;
   }

}










 
