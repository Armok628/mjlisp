.type	car, @function
car:
	movq	8(%rdi), %rax
	ret

.type	cdr, @function
cdr:
	movq	16(%rdi), %rax
	ret

.type	cons, @function
cons:
	pushq	%rsi
	pushq	%rdi
	movq	$24, %rdi
	call	malloc@plt
	movq	$0, (%rax) # Datatype
	popq	8(%rax) # car
	popq	16(%rax) # cdr
	ret
