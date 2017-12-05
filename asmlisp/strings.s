.type	toupper, @function
toupper:
	pushq	%rdi
	.toupperl:
	cmpb	$0, (%rdi)
	jz	.toupperq
	cmpb	$97, (%rdi)
	jnge	.toupperc
	cmpb	$122, (%rdi)
	jnle	.toupperc
	subq	$32, (%rdi)
	.toupperc:
	incq	%rdi
	jmp	.toupperl
	.toupperq:
	popq	%rax
	ret

.type	chomp, @function
chomp:
	pushq	%rdi
	.chomp_l:
	cmpb	$10, (%rdi)
	je	.chomp_x
	cmpb	$0, (%rdi)
	je	.chomp_q
	incq	%rdi
	jmp	.chomp_l
	.chomp_x:
	movq	$0, (%rdi)
	.chomp_q:
	popq	%rax
	ret

.type	mystrcmp, @function
mystrcmp:
	.mystrcmpl:
	cmpb	$0, (%rdi)
	jz	.mystrcmpq
	cmpb	$0, (%rsi)
	jz	.mystrcmpq
	movsbq	(%rdi), %rcx
	cmpq	%rcx, (%rsi)
	jne	.mystrcmpq
	incq	%rdi
	incq	%rsi
	jmp	.mystrcmpl
	.mystrcmpq:
	movsbq	(%rdi), %rax
	movsbq	(%rsi), %rcx
	subq	%rcx, %rax
	ret
