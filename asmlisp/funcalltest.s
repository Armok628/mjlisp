.include "inclusions.s"
.include "lispfuncs/funcall.s"

.type	add, @function
add:
	popq	%rbx
	popq	%rcx
	popq	%rax
	movq	8(%rax), %rdi
	addq	8(%rcx), %rdi
	movq	$2, %rsi
	call	new_var
	pushq	%rax
	pushq	%rbx
	ret

x:
	.quad	2,10
y:
	.quad	2,20
f:
	.quad	-2,1,1,1,2,add,0,0

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	leaq	x(%rip), %rax
	pushq	%rax
	leaq	y(%rip), %rax
	pushq	%rax
	leaq	f(%rip), %rax
	pushq	%rax
	call	funcall
	call	disp
	call	terpri

	leave
	ret
