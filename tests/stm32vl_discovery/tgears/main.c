#include "stm32vl_discovery.h"
#include "gears.h"

char test [100];

void check_area (int tidx, char *data, unsigned size, char val)
{
    for (unsigned i = 0; i < size; i++)
        if (data [i] != val)
            printf ("test %d: 0x%x + %d contains '%02x', expected '%02x'\r\n",
                tidx, data, i, data [i], val);
}

int main (void)
{
    usart1_init ();
    printf ("libgears test started\r\n");

    for (;;)
    {
        memset (test, 0xEA, sizeof (test));
        check_area (1, test, sizeof (test), 0xEA);

        memset (test + 5, 0xD5, sizeof (test) - 10);
        check_area (2, test, 5, 0xEA);
        check_area (2, test + 5, sizeof (test) - 10, 0xD5);
        check_area (2, test + sizeof (test) - 5, 5, 0xEA);

        memcpy (test + 1, test + sizeof (test) - 11, 11);
        check_area (3, test, 1, 0xEA);
        check_area (3, test + 1, 6, 0xD5);
        check_area (3, test + 7, 5, 0xEA);
        check_area (3, test + 12, sizeof (test) - 5 - 12, 0xD5);
    }
}
