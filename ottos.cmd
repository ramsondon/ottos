////////////////////////////////////////////////////////
// Linker command file for BeagleBoard
//
// Configuration of the OttOS-Project
//

-stack 0x00002000
-heap 0x00002000

MEMORY
{
   
   int_ram: ORIGIN = 0x40200000 LENGTH = 0x0000FFC4
   int_vecs: ORIGIN = 0x4020FFC4 LENGTH = 0x00000020
   
   ext_ddr: ORIGIN = 0x82000000 LENGTH = 0x10000000 // 256 MB
   
}

SECTIONS
{
///////////////////////////// int_ram //////////////////////////////
   .intvecs > int_vecs

   .const > int_ram {
       _kernelMasterTable = . ;
       . = . + (16 * 1024);
       *(.const)
   }
   .bss > int_ram
   .far > int_ram
   
   .stack > int_ram
   .data > int_ram
   .cinit > int_ram
   .cio > int_ram
   
   .text > int_ram
   .sysmem > int_ram
   .switch > int_ram
   .pinit > int_ram
   
///////////////////////////// ext_ddr //////////////////////////////
   .stackArea > ext_ddr {
       . = . + (4 * 1024);
       kernelStack = .;
       . = . + (4 * 1024);
       irqStack = .;
       . = . + (4 * 1024);
       systemStack = .;
   }
}