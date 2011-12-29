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
  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);
  //process_create(1, (int) serial_test_write_1);
  //process_create(1, (int) serial_test_write_2);
  //process_create(1, (int) serial_test_write_3);
  //process_create(1, (int) serial_test_write_4);
  //process_create(1, (int) serial_test_write_5);
  process_create(1, (int) serial_test_calculator);

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
  process_create(1, (int) toggle_led1);
  process_create(1, (int) serial_test_calculator);

  devices_init();

  irq_init();

  timer_init();

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
}

void process_exit_test() {

  process_table_init();
  process_create(1, (int) serial_test_write_exit_1);
  process_create(1, (int) serial_test_write_exit_2);
  process_create(1, (int) toggle_led1);

  devices_init();

  irq_init();
  timer_init();
  irq_register_context_switch();
  irq_enable();

  kernel_to_user_mode();
}

void console_test() {

  process_table_init();
  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);

  //process_create(1, (int) dummy_process);
  process_create(1, (int) console_start);

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
  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);

  irq_init();
  timer_init();
  irq_register_context_switch();


  kernel_to_user_mode();
}

int _main(int argc, char **argv) {

  //  process_test();
  //  timer_test();
  //  serial_test();
  //  serial_test_calc();
  //  process_exit_test();
    console_test();
  //  fs_test();
  //i2c_test();
  //mmu_test();

  for (;;)
    ;

  return 0;
}
