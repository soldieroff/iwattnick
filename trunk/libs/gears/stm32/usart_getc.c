/*
    stdout redirected via RS232
    Copyright (C) 2010 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "usart.h"

uint8_t usart_getc (USART_TypeDef *usart)
{
    while (!(usart->SR & USART_SR_RXNE))
        ;
    return usart->DR;
}
