/*
    stdout redirected via RS232
    Copyright (C) 2010 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "serio.h"

uint8_t serio_getc (USART_TypeDef *usart)
{
    while (!(usart->SR & USART_SR_RXNE))
        ;
    return usart->DR;
}

uint8_t serio_iready (USART_TypeDef *usart)
{
    return (usart->SR & USART_SR_RXNE) ? 1 : 0;
}
