.type	car, @function
car:
	movq	8(%rdi), %rax
	ret

.type	cdr, @function
cdr:
	movq	16(%rdi), %rax
	ret
