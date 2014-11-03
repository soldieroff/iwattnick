/*
    stdout redirected via RS232
    Copyright (C) 2010 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "usart.h"
#include "printf.h"

void stdio_init (USART_TypeDef *usart)
{
    init_printf (usart, (void (*) (void*,char)) usart_putc);
}
