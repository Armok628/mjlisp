.type	car, @function
car:
	cmpq	$0, (%rdi)
	jne	.nil_ret
	movq	8(%rdi), %rax
	ret

.type	cdr, @function
cdr:
	cmpq	$0, (%rdi)
	jne	.nil_ret
	movq	16(%rdi), %rax
	ret

.nil_ret:
	leaq	NIL(%rip), %rax
	ret
