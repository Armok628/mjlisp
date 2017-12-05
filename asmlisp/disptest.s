.include "macros.s"
.include "asmfuncs.s"

.dbug:
	.string "debug"

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	movq	$10, %rdi
	movq	$2, %rsi
	call	new_var
	pushq	%rax

	movq	$20, %rdi
	movq	$2, %rsi
	call	new_var
	pushq	%rax

	movq	$30, %rdi
	movq	$2, %rsi
	call	new_var

	movq	%rax, %rsi
	popq	%rdi
	call	cons # (20 . 30)
	movq	%rax, %rsi	
	popq	%rdi
	chain	cons # (10 20 . 30)

	call	disp
	movq	$10, %rdi
	call	putchar@plt

	popq	%rbp
	zero	%rax
	ret
