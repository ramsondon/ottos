/*
* puppybits code
*/

#ifndef I2C_H_
#define I2C_H_

#include <ottos/types.h>

/* i2c bus */
void i2c_init(void);
void i2c_read(uint32_t base, uint8_t sa, uint8_t addr, uint8_t *buffer, int count);
// address stored as first byte(s) of buffer
void i2c_write(uint32_t base, uint8_t sa, uint8_t *buffer, int count);
// write 8 bites
void i2c_write8(uint32_t base, uint8_t sa, uint8_t addr, uint8_t v);


#endif /* I2C_H_ */
