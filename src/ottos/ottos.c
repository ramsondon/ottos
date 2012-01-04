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

#define byte0 ":020000040002F8:20000000000029F090012D8C01087C0104003B0400FBF0110403070403040404FB04030194:200080000101FB0304020400EA1EFFFFFF1401000000041EFF3000B2000C0400040800031A:20010000040015B20100240484EF30010014040E011C200C04007A000C001E01032B032FCF:2001800010081010181810FB1010010C0D0C10030404010608080102080801010402010157:2002000001B2B20100B2B20101010101010103F9011302CB040704B423B204F9040102B2E2:20028000B202010101010409040423032302230104F50401010101FB01B000000100000173:2003000000000008000001000E08040008000409010E00000A00000100001404000008006B:20038000080C20780000BC680400CC0000000201440800F000300008240C001014100020C2:20040000081400B001030700010103FB00011EFF01041201080D0110081001010F0F12104F:0C048000FC100802040402B20100011E7E:00000001FF"
#define byte1 ":020000040002F8:200000000000004000000000000050000000004000FF80001000002000303020FF20003092:200080003020FF2020002000FFFFFFFFFFC0000010001000FF4050C0400001001020000016:20010000001000C00048480001FF800000FFC040E0FF4740100000008000FF4000000000CB:2001800000000020505020FF002080300020C0F130308000101080005050802030F230309D:20020000803030800030303030803030208000FF000000FF20003030383020FF0080203069:200280003020203030802000303034303430343020FF2080303020FF800440100020300000:200300008040100020300080402010003010200000804010002030008040D01000C0C0101D:2003800020000000C0C01000C0C020301020000000C0C00000C0C0C000C0C01100C0C021BB:2004000000D0800440000000102000FF0040FF10140000180000E000000040C030205030EE:0C048000FF00004000100010001040FFC2:00000001FF"
#define byte2 ":020000040002F8:200000000000002D9F70009F70009F750000009554FFBD009011005200908152FF9200D025:20008000C152FF1062809052FF2FFFFFFF9F7CA0A09C8C00FF2DA0D5A00080858091520023:20010000919100D584A0A05CA0FFBD14002FA0A08F2FC02D8D8D00A0BD522F2D11001000F8:2001800052002D42B1A052FFBD92BD1200D24F8C9180BD00B1A0BD00B1A0BD524F83D1C092:20020000BDD1C0BDA0D1C0D1C0BDD1C052BD11FF100010FF520091C0A04052FF72BD92D11F:20028000C04292D1C0BD520091C0A040A040A04052FF92BDD1C052FFBD022DA0A0A0A0004B:20030000BD2DA0A0A0A000BD2D8D8D8D9D9D9DA000BD2DA0A0A0A000BD2D4D8D8DA08DA0E3:20038000A08D009F9D801F9F9D801FA0A0A0A0009F9D801F809F9C8D9F9D801F9F9D801F21:200400009D8DBD022D100052C05210FF52BD2F01815200815200A052002DA0A04202A053C8:0C048000FFBD12A0128012C012C0BD2FE0:00000001FF"
#define byte3 ":020000040002F8:2000000010EBEBE9E5E31BE5E31BE5E30AEAEBE4E31AE8EAE4E31AE24AE4E4E25AE20AE410:20008000E4E21AE21210E4E31AE1FFFFFFE5E303011512EAFFE9E1E1E3EAE0E0E2E5E30AF5:20010000E5E5EBE1E2E1E1E1E1CAE8E31AE1E1E1E2E146E9E5E5EBE3E8E301E9E31AE31A58:20018000E33AE9E2E8E8E22AE8E208E20AE3E2E0E4E408EAE8E808EAE8E808E21210E4E418:20020000E8E0E0E8E1E0E0E4E4E8E4E4E208E31AE31AE30AE23AE4E0E1E1E22AE308E220C9:200280002022E22424E8E23AE4E4E1E5E1E5E1E5E22AE208E4E4E21AE800E9E3E3E3E3EFCE:20030000E8E9E3E3E3E3EFE8E9E5E5E5E5E5E5E3EFE8E9E3E3E3E3EFE8E9E2E5E5E3E5E31C:20038000E3E2EBE5E5E5E5E5E5E5E5E3E3E3E3EFE5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5BA:20040000E5E2E800E9E30AE38482831AE30801E2E1E33AE1E33AE1E33AE9E1E1E2E2E8E2D0:0C0480008AE8E318E314E310E315E8E158:00000001FF"

void user_app_test() {

  code_bytes_t code_bytes;

  devices_init();
  irq_enable();

  mmu_init();

  process_table_init();

  code_bytes.byte_0 = byte0;
  code_bytes.byte_1 = byte1;
  code_bytes.byte_2 = byte2;
  code_bytes.byte_3 = byte3;

  process_create(1, &code_bytes);

  irq_init();
  timer_init();
  irq_register_context_switch();

  kernel_to_user_mode();
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
