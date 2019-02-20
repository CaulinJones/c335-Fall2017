00008ba4 <main>:
    8ba4:	b580      	push	{r7, lr}
    8ba6:	b082      	sub	sp, #8
    8ba8:	af00      	add	r7, sp, #0
    8baa:	2300      	movs	r3, #0
    8bac:	607b      	str	r3, [r7, #4]
    8bae:	e007      	b.n	8bc0 <main+0x1c>
    8bb0:	f7ff ffa2 	bl	8af8 <stop_light>
    8bb4:	687b      	ldr	r3, [r7, #4]
    8bb6:	1c18      	adds	r0, r3, #0
    8bb8:	f000 f80a 	bl	8bd0 <inc>
    8bbc:	1c03      	adds	r3, r0, #0
    8bbe:	607b      	str	r3, [r7, #4]
    8bc0:	687b      	ldr	r3, [r7, #4]
    8bc2:	2b13      	cmp	r3, #19
    8bc4:	ddf4      	ble.n	8bb0 <main+0xc>
    8bc6:	1c18      	adds	r0, r3, #0
    8bc8:	46bd      	mov	sp, r7
    8bca:	b002      	add	sp, #8
    8bcc:	bd80      	pop	{r7, pc}
    8bce:	46c0      	nop			; (mov r8, r8)
