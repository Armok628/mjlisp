.type	atom, @function
atom:
	leaq	.NIL(%rip), %rax
	cmpq	$0, (%rdi)
	jz	.atomq
	leaq	.T(%rip), %rax
	.atomq:
	ret
