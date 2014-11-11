/*
    Assembly implementation of memset and memclr for ARM/Thumb
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

	.syntax unified
	.cpu cortex-m3
	.thumb

// void memclr (void *dest, unsigned len)
	.section .text,"ax",%progbits
	.global	memclr
	.type	memclr, %function
memclr:
	mov	r2, r1
	mov	r1, #0

// void memset (void *dest, char c, unsigned len)
	.global	memset
	.type	memset, %function
memset:
	and	r1, r1, #0xff
	orr	r1, r1, r1, lsl #8
	orr	r1, r1, r1, lsl #16

// Align target address to word boundary first
1:	cbz	r2, 4f		// quit if len=0
	ands	r3, r0, #3
	itttt	ne
	strbne	r1, [r0]
	addne	r0, #1
	subne	r2, #1
	bne	1b

// Fill with words
2:	bics	r3, r2, #3	// r3 = number of words
	itttt	ne
	strne	r1, [r0]
	addne	r0, #4
	subne	r2, #4
	bne	2b

3:	ands	r2, #3		// r2 = remaining bytes
	itttt	ne
	strbne	r1, [r0]
	addne	r0, #1
	subne	r2, #1
	bne	3b

4:	bx	lr
