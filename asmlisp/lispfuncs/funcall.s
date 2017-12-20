.type	funcall, @function
funcall:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	16(%rsp), %rax # Load the pointer to the function
	movq	$1, %rcx # Set the counter to 1 (0 is # of args)
	.funcall_loop:
	movq	(%rax,%rcx,8), %rdx # Load the next address
	cmpq	$0, %rdx
	jz	.funcall_pushq
	cmpq	$1, %rdx
	je	.funcall_pusharg
	movq	%rax, %r14
	movq	%rcx, %r15
	call	*%rdx
	movq	%r15, %rcx
	movq	%r14, %rax
	incq	%rcx
	jmp	.funcall_loop
	.funcall_pushq:
	incq	%rcx
	movq	(%rax,%rcx,8), %rdx # Load the literal to be pushed
	cmpq	$0, %rdx
	jz	.funcall_exit # If null, exit (so 0x0 0x0 denotes the end of the function)
	pushq	%rdx
	incq	%rcx
	jmp	.funcall_loop
	.funcall_pusharg:
	incq	%rcx
	movq	(%rax,%rcx,8), %rdx
	addq	$2, %rdx
	pushq	(%rbp,%rdx,8)
	incq	%rcx
	jmp	.funcall_loop
	.funcall_exit:
	# At this point there must only be one thing on the stack
	popq	%rdx # Take that thing off the stack
	movq	%rbp, %rsp # Reset the stack
	popq	%rbp # (see above)
	popq	%rdi # Preserve the return address
	# Now the arguments are on top of the stack
test:
	movq	(%rax), %rcx # Store the number of variables
	negq	%rcx
	incq	%rcx # +1 for function address left on stack
	pushq	%rdx # Put return value back on stack
	.funcall_dropargs:
	popq	8(%rsp) # nip (drop second stack item)
	decq	%rcx
	cmpq	$0, %rcx
	jnz	.funcall_dropargs
	pushq	%rdx
	pushq	%rdi # Replace the return address
	ret
