.fnum:
	.string "%li"
.fstr:
	.string "%s"

.type	disp, @function
disp:
	cmp	$0, %rdi
	jz	.disp_nil
	cmp	$0, (%rdi)
	jz	.disp_cell
	cmp	$1, (%rdi)
	je	.disp_sym
	cmp	$2, (%rdi)
	je	.disp_num
	jmp	.disp_exit # Unknown datatype
	.disp_nil:
	leaq	NILstr(%rip), %rdi
	zero	%rax
	call	printf@plt
	jmp	.disp_exit
	.disp_sym:
	movq	8(%rdi), %rsi
	leaq	.fstr(%rip), %rdi
	zero	%rax
	call	printf@plt
	jmp	.disp_exit
	.disp_num:
	call	car
	leaq	.fnum(%rip), %rdi
	movq	%rax, %rsi
	zero	%rax
	call	printf@plt
	jmp	.disp_exit
	.disp_cell:
	pushq	%rdi # Temporarily store the cell
	movq	$40, %rdi
	call	putchar@plt # (
	popq	%rdi # Recall the cell
	.disp_cell_l:
	pushq	%rdi # Put the cell on the stack e.g. (10 20)
	chain	car # e.g. 10
	call	disp # Print the head
	movq	$32, %rdi
	call	putchar@plt # space
	popq	%rdi # Get the cell e.g. (10 20)
	chain	cdr # e.g. (20)
	cmpq	$0, %rdi # Is tail null?
	jz	.disp_cell_lx 
	cmpq	$0, (%rdi) # Is tail another list?
	jz	.disp_cell_l
	pushq	%rdi # Tail must be a non-nil atom
	movq	$46, %rdi # .
	call	putchar@plt
	movq	$32, %rdi # space
	call	putchar@plt
	popq	%rdi
	call	disp
	movq	$32, %rdi # space
	call	putchar@plt
	.disp_cell_lx: # Done printing list
	movq	$8, %rdi
	call	putchar@plt # backspace (LAZY HACK)
	movq	$41, %rdi
	call	putchar@plt # )
	.disp_exit:
	zero	%rax
	ret
