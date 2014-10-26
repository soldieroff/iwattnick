/*
    stdout redirected via RS232
    Copyright (C) 2010 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "serio.h"

void serio_putc (USART_TypeDef *usart, uint8_t c)
{
    // Wait till transmission register empty
    while (!(usart->SR & USART_SR_TXE))
        ;
    usart->DR = c;
}
