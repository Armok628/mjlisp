.type	atom, @function
atom:
	movq	NIL(%rip), %rax
	cmpq	$0, (%rdi)
	jz	.atomq
	movq	T(%rip), %rax
	.atomq:
	ret
