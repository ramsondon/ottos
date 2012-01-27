////////////////////////////////////////////////////////
// Linker command file for BeagleBoard)
// 
// Basic configuration using only external memory
//

-stack           0x00001000
-heap            0x00020000

MEMORY
{
   kernel_master_table: ORIGIN = 0x40200000 LENGTH = 0x00004000
   int_ram:  ORIGIN = 0x40204000  LENGTH = 0x0000BFC4
   int_vecs: ORIGIN = 0x4020FFC4  LENGTH = 0x0000003B
   
   ext_ddr:  ORIGIN = 0x80000000  LENGTH = 0x03FFFFFF // we are to stupid to allocate 256 MB, so we use just 64MB
   										  
}


SECTIONS
{
///////////////////////////// int_ram //////////////////////////////
   .intvecs    > int_vecs {
   		_intvecs_start = .;
   		*(.intvecs)
   	}

   ._kernel_master_table > kernel_master_table {
       _kernel_master_table = . ;
       . = . + (0x4000); 	//Größe der mastertable 16 Kbyte
   }
   ORDER
	.cinit      > int_ram
	.data       > int_ram
	.far        > int_ram
   
	.stack      > int_ram
	.cio        > int_ram

	.switch     > int_ram 
	.text2      > int_ram {
		irq.obj                      //IRQ Object für Data Abort der MMU muss im intRam sein
	}
	.pinit      > int_ram {
			_int_RAM_start = .;
				
   	}


///////////////////////////// ext_ddr //////////////////////////////

	ORDER
	 .text	    > ext_ddr

	.bss        > ext_ddr
	.const      > ext_ddr
	.sysmem     > ext_ddr
	.stackArea  > ext_ddr {
	. = ALIGN(0x4);                    
       . = . + (0x1000);  // Größe des Kernel Stacks 4 Kbyte
       kernelStack = .;
	   . = . + (0x1000);// Größe IRQ Stack  4 Kbyte
	   irqStack = .;
	   . = . + (0x1000);// Größe System Stack 4 Kbyte
	   systemStack = .;
	   . = . + (0x1000);// Größedes Abort Stack 4 Kbyte
	   abortStack = .;
	   _ext_DDR_start = .;
   }
}
