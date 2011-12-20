
////////////////////////////////////////////////////////
// Linker command file for BeagleBoard)
// 
// Basic configuration using only external memory
//

-stack           0x00002000
-heap            0x00002000

MEMORY
{
   kernel_master_table: ORIGIN = 0x40200000 LENGTH = 0x00004000
   int_ram:  ORIGIN = 0x40204000  LENGTH = 0x0000BFC4
   int_vecs: ORIGIN = 0x4020FFC4  LENGTH = 0x0000003B
   
   ext_ddr:  ORIGIN = 0x82000000  LENGTH = 0x10000000 // 256 MBit
   										  
}


SECTIONS
{
///////////////////////////// int_ram //////////////////////////////
   .intvecs    > int_vecs {
   		_intvecsStart = .;
   		*(.intvecs)
   	}

   .kernelMasterTable > kernel_master_table {
       _kernelMasterTable = . ;
       . = . + (16 * 1024);
   }
   ORDER
	.cinit      > int_ram
	.data       > int_ram
	.far        > int_ram
   
	.stack      > int_ram
	.cio        > int_ram

	.switch     > int_ram 
	.text2      > int_ram {
		irq.obj
	}
	.pinit      > int_ram {
		*(.pinit)
			_intRamStart = .;
   	}


///////////////////////////// ext_ddr //////////////////////////////

	ORDER
	.text	   > ext_ddr
	.bss        > ext_ddr
	.const      > ext_ddr
	.sysmem     > ext_ddr
	.stackArea  > ext_ddr {
       . = . + (4* 1024);
       kernelStack = .;
	   . = . + (4 * 1024);
	   irqStack = .;
	   . = . + (4* 1024);
	   systemStack = .;
	   . = . + (4 * 1024);
	   abortStack = .;
	   . = . + (4 * 1024);
	   _extDDRStart = .;
   }
}
