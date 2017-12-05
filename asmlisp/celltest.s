.include "macros.s"
.include "asmfuncs.s"

.iform:
	.string "%i\n"

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	# Pretend these are addresses
	movq	$1, %rdi
	movq	$2, %rsi
	chain	cons

	pushq	%rax
	call	car
	movq	%rax, %rsi
	leaq	.iform(%rip), %rdi
	zero	%rax
	call	printf@plt

	popq	%rdi
	call	cdr
	movq	%rax, %rsi
	leaq	.iform(%rip), %rdi
	zero	%rax
	call	printf@plt

	popq	%rbp
	zero	%rax
	ret
