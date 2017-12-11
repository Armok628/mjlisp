# Experimentation with simple machine code generation
.type	test1, @function
test:
	movq	%rdi, %rax
	incq	%rax
	ret

test2:
	movq	$test, %rax # Not a relocatable definition, but OK at runtime, AFAIK
	call	*%rax
	ret
/*
Dump of assembler code for function test2:
   0x0000000000400647 <+0>:	48 c7 c0 40 06 40 00	mov    $0x400640,%rax
   0x000000000040064e <+7>:	ff d0	callq  *%rax
   0x0000000000400650 <+9>:	c3	retq   
*/

test3:
	.word 0x8d48
	.byte 0x05
	.long test-4-.
	.word 0xd0ff
	.byte 0xc3

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	movq	$10, %rdi
	call	malloc@plt
	movq	%rax, %rbx
	movq	%rbx, %rcx
	#########################
	movl	$0xc0c748, (%rcx) # mov (literal), %rax
	addq	$3, %rcx
	leal	test(%rip), %eax
	movl	%eax, (%rcx) # (literal) : test
	addq	$4, %rcx
	movw	$0xd0ff, (%rcx) # call *%rax
	addq	$2, %rcx
	movb	$0xc3, (%rcx) # ret
	#########################
	movq	$1, %rdi
call:
	call	*%rbx

	popq	%rbp
	ret
