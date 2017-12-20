.include "inclusions.s"
.include "utils/lambda_size.s"

teststr:
	.string "sym"

iform:
	.string "%li\n"

.type	printint, @function
printint:
	leaq	iform(%rip), %rdi
	movq	8(%rsp), %rsi
	xorq	%rax, %rax
	call	printf@plt
	call	swap
	call	drop
	ret

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

	leaq	teststr(%rip), %rdi
	movq	$1, %rsi
	call	new_var
	pushq	%rax

	leaq	NIL(%rip), %rax
	pushq	%rax
	call	cons
	call	cons

	movq	$30, %rdi
	movq	$2, %rsi
	call	new_var
	pushq	%rax

	leaq	NIL(%rip), %rax
	pushq	%rax

	# (10 (20 sym) 30)
	call	cons
	call	cons
	call	cons
	call	dup
	call	disp
	call	terpri
	call	lambda_size
	call	printint

	leave
	ret
