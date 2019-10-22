
kernel:     file format elf32-littlearm


Disassembly of section .init:

00008000 <_start>:
    8000:	e10f0000 	mrs	r0, CPSR
    8004:	e200001f 	and	r0, r0, #31
    8008:	e3a0101a 	mov	r1, #26
    800c:	e1500001 	cmp	r0, r1
    8010:	0a000006 	beq	8030 <_exitHyper>

00008014 <_loopCores>:
    8014:	ee100fb0 	mrc	15, 0, r0, cr0, cr0, {5}
    8018:	e3100003 	tst	r0, #3
    801c:	1a000001 	bne	8028 <_bsprak+0x8>

00008020 <_bsprak>:
    8020:	e3a0d802 	mov	sp, #131072	; 0x20000
    8024:	eb000009 	bl	8050 <start_kernel>
    8028:	e320f003 	wfi
    802c:	eafffffd 	b	8028 <_bsprak+0x8>

00008030 <_exitHyper>:
    8030:	e59fe014 	ldr	lr, [pc, #20]	; 804c <_exitHyper+0x1c>
    8034:	e12ef30e 	msr	ELR_hyp, lr
    8038:	e10f0000 	mrs	r0, CPSR
    803c:	e3c0001f 	bic	r0, r0, #31
    8040:	e3800013 	orr	r0, r0, #19
    8044:	e16ef300 	msr	SPSR_hyp, r0
    8048:	e160006e 	eret
    804c:	00008014 	andeq	r8, r0, r4, lsl r0

Disassembly of section .text:

00008050 <start_kernel>:
    8050:	eaffffff 	b	8054 <yellow_on>

00008054 <yellow_on>:
    8054:	e3a03000 	mov	r3, #0
    8058:	e3a01602 	mov	r1, #2097152	; 0x200000
    805c:	e3433f20 	movt	r3, #16160	; 0x3f20
    8060:	e3a02080 	mov	r2, #128	; 0x80
    8064:	e5831000 	str	r1, [r3]
    8068:	e583201c 	str	r2, [r3, #28]
    806c:	e12fff1e 	bx	lr

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00003041 	andeq	r3, r0, r1, asr #32
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	00000026 	andeq	r0, r0, r6, lsr #32
  10:	45563705 	ldrbmi	r3, [r6, #-1797]	; 0xfffff8fb
  14:	070a0600 	streq	r0, [sl, -r0, lsl #12]
  18:	09010841 	stmdbeq	r1, {r0, r6, fp}
  1c:	14041202 	strne	r1, [r4], #-514	; 0xfffffdfe
  20:	17011501 	strne	r1, [r1, -r1, lsl #10]
  24:	1a011803 	bne	46038 <yellow_on+0x3dfe4>
  28:	2a012201 	bcs	48834 <yellow_on+0x407e0>
  2c:	44022c01 	strmi	r2, [r2], #-3073	; 0xfffff3ff
  30:	Address 0x0000000000000030 is out of bounds.


Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <yellow_on+0x10c8cd0>
   4:	4e472820 	cdpmi	8, 4, cr2, cr7, cr0, {1}
   8:	38202955 	stmdacc	r0!, {r0, r2, r4, r6, r8, fp, sp}
   c:	302e332e 	eorcc	r3, lr, lr, lsr #6
	...
