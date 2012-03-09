/**
 * puppybits code
 */

#include <ottos/types.h>
#include <ottos/io.h>
#include <ottos/kernel.h>
#include "../hal/omap353x/i2c.h"
#include "../drivers/i2c/i2c.h"

#include "i2c_test.h"

// TODO (thomas.bargetz@gmail.com) refactor
#define SCDA_LED 0x4a
#define SCD_LEDEN 0xee
#define SCD_LEDBON (1<<1)
#define SCD_LEDBPWM (1<<5)
#define SCD_PWMBOFF 0xf2


// Sine approximated as a parabola, only valid for [0, pi]
// see: http://www.devmaster.net/forums/showthread.php?t=5784
#define pi ((float)3.14159265)
#define fabs(f) ((f) < 0 ? -(f) : (f))
static float sine(float x)
{
        const float B = 4/pi;
        const float C = -4/(pi*pi);

        float y = B * x + C * x * fabs(x);

        //  const float Q = 0.775;
        const float P = 0.225;

        y = P * (y * fabs(y) - y) + y;   // Q * y + P * y * abs(y)

        return y;
}

// pulses the led connected to the TPS65950

void pulse_leds(void) {
        uint32_t base = I2C1_BASE;
        char buffer[5];
        int i;
        float a = 0.0;

        kernel_print("reading LED/PWM registers\n");

        i2c_read(base, SCDA_LED, SCD_LEDEN, buffer, 5);
        for (i=0;i<5;i++) {
          sprintf(&buffer[i], "%02x");
        }
        kernel_print(buffer);
        kernel_print("\n");

        // make sure it's on
        i2c_write8(base, SCDA_LED, SCD_LEDEN, SCD_LEDBON | SCD_LEDBPWM);


        while (1) {
                int v;

                if (a > pi)
                        v = 64 - (int)(sine( a-pi ) * 64);
                else
                        v = (int)(sine( a ) * 64 + 64);

                a += 0.1;
                if (a > 2 * pi)
                        a -= 2* pi;

                i2c_write8(base, SCDA_LED, SCD_PWMBOFF, v);
                kernel_sleep(100);
        }
}
