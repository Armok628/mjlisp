.include "inclusions.s"

dbug:
	.string "debug"
dbug2:
	.string "debug2"

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	leaq	dbug(%rip), %rdi
	.breakpoint:
	movq	$1, %rsi
	chain	new_var
	push	%rax

	leaq	dbug(%rip), %rdi
	movq	$1, %rsi
	chain	new_var
	popq	%rsi

	chain	eq
	call	disp
	call	terpri

	popq	%rbp
	zero	%rax
	ret
