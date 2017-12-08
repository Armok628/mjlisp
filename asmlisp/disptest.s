.include "inclusions.s"

test:
	.string "sym"

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	movq	$10, %rdi
	movq	$2, %rsi
	call	new_var
	pushq	%rax

	leaq	test(%rip), %rdi
	movq	$1, %rsi
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
