/*
    Date and time manipulation routines

    Public Domain code, source:
    http://stackoverflow.com/questions/1274964/how-to-decompose-unix-time-in-c
*/

#ifndef __DATETIME_H__
#define __DATETIME_H__

#include <stdint.h>

typedef struct
{
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t wday;
} datetime_t;

/**
 * Convert Unix time to y/m/d/h/m/s human-readable representation.
 * @arg unixtime
 *      Number of seconds since 00:00 01/01/1970
 * @arg dt
 *      Human-readable date/time representation
 */
extern void ut2dt (uint32_t unixtime, datetime_t *dt);

/**
 * Convert a y/m/d/h/m/s human-readable date/time representation
 * to Unix time
 * @arg dt
 *      Human-readable date/time representation
 * @return
 *      Number of seconds since 00:00 01/01/1970
 */
extern uint32_t dt2ut (datetime_t *dt);

#endif // __DATETIME_H__
