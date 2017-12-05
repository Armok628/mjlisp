.type	eq, @function
eq:
	leaq	.NIL(%rip), %rax
	# Check datatype
	cmpq	$0, (%rdi) # Check if cell
	je	.eqret
	cmpq	$0, (%rsi) # Check if cell
	je	.eqret
	movq	(%rdi), %rcx
	cmpq	%rcx, (%rsi) # Check if datatypes are equal
	jne	.eqret
	# Check contents
	cmpq	$1, (%rdi) # Check if dataypes are symbols
	je	.eqsym
	movq	8(%rdi), %rcx
	cmpq	%rcx, 8(%rsi) # Compare number
	jne	.eqret
	leaq	.T(%rip), %rax
	jmp	.eqret
	.eqsym:
	cdr	%rdi
	cdr	%rsi
	call	strcmp@plt
	cmp	$0, %rax
	leaq	.T(%rip), %rax
	je	.eqret
	leaq	.NIL(%rip), %rax
	.eqret:
	ret
