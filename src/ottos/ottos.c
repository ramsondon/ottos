#include <ottos/system.h>
#include <ottos/kernel.h>
#include <ottos/const.h>

#include "../../bin/led_test.h"
#include "../../bin/serial_test.h"
#include "../../bin/console.h"
#include "../../bin/i2c_test.h"
#include "../drivers/i2c/i2c.h"

#include "kernel/intc/irq.h"
#include "kernel/pm/process.h"
#include "kernel/timer/timer.h"
#include "dev/devices.h"

#include "../hal/uart.h"

#include "../fs/fs.h"
#include "../fs/vfat/fat_filelib.h"

#include "../../drivers/mmchs/mmchs.h"

#include "kernel/mmu/mmu.h"
#include "../lib/ottos/parser.h"
#include "kernel/loader/loader.h"

void timer_test() {

	process_table_init();
}

void devices_test() {
	devices_init();
}

void serial_test() {

	process_table_init();

	//process_create(1, (int) led1_on);
	//process_create(1, (int) led1_off);
	//  process_create(1, (int) toggle_led1);
	//  process_create(1, (int) toggle_led2);
	//process_create(1, (int) serial_test_write_1);
	//process_create(1, (int) serial_test_write_2);
	//process_create(1, (int) serial_test_write_3);
	//process_create(1, (int) serial_test_write_4);
	//process_create(1, (int) serial_test_write_5);
	//  process_create(1, (int) serial_test_calculator);

	devices_init();
	serial_test_create();

	irq_init();

	timer_init();

	irq_register_context_switch();

	irq_enable();
	kernel_to_user_mode();
	//    sys_yield();

	//    serial_test_test();
}

void serial_test_calc() {

	process_table_init();
	//  process_create(1, (int) toggle_led1);
	//  process_create(1, (int) serial_test_calculator);

	devices_init();

	irq_init();

	timer_init();

	irq_register_context_switch();

	irq_enable();
	kernel_to_user_mode();
}

void process_exit_test() {

	process_table_init();
	//  process_create(1, (int) serial_test_write_exit_1);
	//  process_create(1, (int) serial_test_write_exit_2);
	//  process_create(1, (int) toggle_led1);

	devices_init();

	irq_init();
	timer_init();
	irq_register_context_switch();
	irq_enable();

	kernel_to_user_mode();
}

void console_test() {

	process_table_init();
	//  process_create(1, (int) toggle_led1);
	//  process_create(1, (int) toggle_led2);

	//process_create(1, (int) dummy_process);
	//  process_create(1, (int) console_start);

	devices_init();

	irq_init();
	timer_init();
	irq_register_context_switch();
	irq_enable();

	kernel_to_user_mode();
}

void fs_test() {
	file_t* file;
	char buffer[512];
	//char text[] = { 't', 'e', 's', 't', '\n', '\r' };
	char text[] = "new file\n\r\0";

	devices_init();
	mmchs_init();
	fs_init();
	fl_listdirectory("/");

	file = (file_t*) fl_fopen("/test/thenewest.txt", "a");
	fl_fwrite(text, sizeof(text), sizeof(text), file);
	fl_fclose(file);

	fl_listdirectory("/test/");

	file = (file_t*) fl_fopen("/test/thenewest.txt", "r");
	fl_fread(buffer, 512, sizeof(text), file);
	kernel_print(buffer);
	fl_fclose(file);

}

void i2c_test() {
	devices_init();

	i2c_init();
	pulse_leds();
}

void mmu_test() {
	devices_init();
	irq_enable();

	mmu_init();

	process_table_init();
	//process_create(1, (int) toggle_led1);
	//process_create(1, (int) toggle_led2);

	irq_init();
	timer_init();
	irq_register_context_switch();

	kernel_to_user_mode();
}

#define byte0 ":020000040002F8:20000000000029F090012D8C01087C0104003B0400FBF0110403070403040404FB04030194:200080000101FB0304020400EA1EFFFFFF1401000000041EFF3000B2000C0400040800031A:20010000040023B20100240484EF30010014040E011C200800000001002C006B00F700003F:20018000000A000001EFA0001E01032B032F10081010181810FB1010010C0D0C1003040472:20020000010608080102080801010402010101B2B20100B2B20101010101010103F90113C9:2002800002CB040704B423B204F9040102B2B202010101010409040423032302230104F50E:200300000401010101FB01780C0038A1C00002000120022002A020201CA0172012A00D20C3:2003800008A0038203820203028203820203028203820203028203820203020222E4038EE1:0704000000000C1E000C1EA1:00000001FF"
#define byte1 ":020000040002F8:200000000000004000000000000050000000004000FF80001000002000303020FF20003092:200080003020FF2020002000FFFFFFFFFFC0000010001000FF4050C0400001001020000016:20010000001000C00048480001FF800000FFC040E0FF4740C0C0C0C0C010000000FFC0C04B:200180001000203000FF8600FF400000000000000020505020FF002080300020C0F130305B:200200008000101080005050802030F23030803030800030303030803030208000FF00005D:2002800000FF20003030383020FF008020303020203030802000303034303430343020FF6D:200300002080303020FF8047400000E0E00031102008280424000001000100020002000335:200380000003000330030330030230020230020130010130010030000030000024FF10E0AF:07040000100040FF0040FF67:00000001FF"
#define byte2 ":020000040002F8:200000000000002D9F70009F70009F750000009554FFBD009011005200908152FF9200D025:20008000C152FF1062809052FF2FFFFFFF9F7CA0A09C8C00FF2DA0D5A00080858091520023:20010000919100D584A0A05CA0FFBD14002FA0A08F2FC02DA08D9D8C8D9F9D0050FFA08DA3:20018000A0A0A0A000FF01522F2D1100100052002D42B1A052FFBD92BD1200D24F8C9180D1:20020000BD00B1A0BD00B1A0BD524F83D1C0BDD1C0BDA0D1C0D1C0BDD1C052BD11FF100006:2002800010FF520091C0A04052FF72BD92D1C04292D1C0BD520091C0A040A040A04052FF73:2003000092BDD1C052FFBDC02D5100A03E601161B052A052A0520052005200520052005281:2003800000520050A34050A34050A34050A34050A34050A34050A34050A34051A0FFA0B033:070400006360BD2FA0BD2FBA:00000001FF"
#define byte3 ":020000040002F8:2000000010EBEBE9E5E31BE5E31BE5E30AEAEBE4E31AE8EAE4E31AE24AE4E4E25AE20AE410:20008000E4E21AE21210E4E31AE1FFFFFFE5E303011512EAFFE9E1E1E3EAE0E0E2E5E30AF5:20010000E5E5EBE1E2E1E1E1E1CAE8E31AE1E1E1E2E146E9E3E5E5E2E5E5E5EBE31AE3E5B1:20018000E3E3E3E3EFEA00E301E9E31AE31AE33AE9E2E8E8E22AE8E208E20AE3E2E0E4E444:2002000008EAE8E808EAE8E808E21210E4E4E8E0E0E8E1E0E0E4E4E8E4E4E208E31AE31AF9:20028000E30AE23AE4E0E1E1E22AE308E2202022E22424E8E23AE4E4E1E5E1E5E1E5E22A40:20030000E208E4E4E21AE846E9E30AE1E042E212E1E191E191E18AE18AE18AE18AE18AE177:200380008AE18AE1E020E1E020E1E020E1E020E1E020E1E020E1E020E1E020E1313AE1E152:070400004222E8E1E3E8E11C:00000001FF"

void user_app_test() {
	code_bytes_t code_bytes;

	irq_disable();

	mmu_init();

	devices_init();
	process_table_init();
	irq_init();
	timer_init();

	code_bytes.byte_0 = byte0;
	code_bytes.byte_1 = byte1;
	code_bytes.byte_2 = byte2;
	code_bytes.byte_3 = byte3;

	process_create(1, &code_bytes);

	irq_register_context_switch();
	irq_enable();

	//mmu_init_memory_for_process(process_table[0]);
	//asm(" MOV R0, #131072");
	//asm(" MOV PC, #131072");

	kernel_to_user_mode();

	kernel_print("go\r\n");

}

int main(int argc, char **argv) {

	//  process_test();
	//  timer_test();
	//  serial_test();
	//  serial_test_calc();
	//  process_exit_test();
	//  console_test();
	//  fs_test();
	//i2c_test();
	//mmu_test();
	user_app_test();

	for (;;)
		;

	return 0;
}
