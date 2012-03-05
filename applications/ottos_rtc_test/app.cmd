////////////////////////////////////////////////////////
// 
// Linker command file for a Task for the Ottos-Kernel
//

-stack           0x00002000
-heap            0x00004000

MEMORY
{														
   virtual_memory:     ORIGIN 0x00020000 LENGTH = 0x01000000
   stack_memory:       ORIGIN 0x10000000 LENGTH = 0x00002000
   sysmem_memory:      ORIGIN 0x10002000 LENGTH = 0x00004000

}

SECTIONS
{
   ORDER
   .text       > virtual_memory  {
      boot.obj
      *(.text)
   }
   .bss        > virtual_memory
   .const      > virtual_memory
   .cinit      > virtual_memory
   .pinit      > virtual_memory
   .cio        > virtual_memory
   .switch     > virtual_memory
   .far        > virtual_memory
   .data       > virtual_memory
   .switch     > virtual_memory
   .init_array > virtual_memory

   .stack      > stack_memory  {
      systemStack = .;
   }

   .sysmem     > sysmem_memory {
      sysmem = .;
   }


}
