#ifndef __ELF__
#define ob_jenkins_hash _ob_jenkins_hash
#define ob_jenkins_hash2 _ob_jenkins_hash2
#define ob_city_hash64 _ob_city_hash64
#define ob_city_hash64_with_seeds _ob_city_hash64_with_seeds
#endif
	.text
	.p2align 4,,15
	.globl	ob_jenkins_hash
#ifdef __ELF__
	.type	ob_jenkins_hash, @function
#endif
ob_jenkins_hash:
.LFB15:
	movq	$4097, -56(%rsp)
	movl	%edx, %ecx
	movq	$0, -48(%rsp)
	leaq	-56(%rsp), %rax
	movq	$0, -40(%rsp)
	xorl	%edx, %edx
	movq	$0, -32(%rsp)
	movq	$0, -24(%rsp)
	movq	$0, -16(%rsp)
	rolq $3,  %rdi ; rolq $13, %rdi
	rolq $61, %rdi ; rolq $51, %rdi
	xchgq %rbx,%rbx
	leal	-559038737(%rcx,%rsi), %eax
	testb	$3, %dil
	movq	%rdx, -64(%rsp)
	movq	-64(%rsp), %r10
	movl	%eax, %ecx
	movl	%eax, %edx
	je	.L79
	testb	$1, %dil
	jne	.L81
	cmpq	$12, %rsi
	jbe	.L84
	.p2align 4,,10
	.p2align 3
.L38:
	movzwl	(%rdi), %r8d
	subq	$12, %rsi
	addl	%r8d, %edx
	movzwl	2(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %edx
	movzwl	4(%rdi), %r8d
	addl	%r8d, %ecx
	movzwl	6(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %ecx
	movzwl	8(%rdi), %r8d
	addl	%r8d, %eax
	movzwl	10(%rdi), %r8d
	addq	$12, %rdi
	sall	$16, %r8d
	addl	%r8d, %eax
	movl	%eax, %r8d
	subl	%eax, %edx
	addl	%ecx, %eax
	roll	$4, %r8d
	xorl	%r8d, %edx
	movl	%edx, %r8d
	subl	%edx, %ecx
	addl	%eax, %edx
	roll	$6, %r8d
	xorl	%ecx, %r8d
	movl	%r8d, %ecx
	subl	%r8d, %eax
	addl	%edx, %r8d
	roll	$8, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	subl	%ecx, %edx
	leal	(%rcx,%r8), %r9d
	roll	$16, %eax
	xorl	%edx, %eax
	movl	%eax, %ecx
	subl	%eax, %r8d
	leal	(%rax,%r9), %edx
	roll	$19, %ecx
	xorl	%r8d, %ecx
	movl	%ecx, %eax
	subl	%ecx, %r9d
	addl	%edx, %ecx
	roll	$4, %eax
	xorl	%r9d, %eax
	cmpq	$12, %rsi
	ja	.L38
.L84:
	leaq	.L51(%rip), %r8
	movslq	(%r8,%rsi,4), %rsi
	addq	%rsi, %r8
	jmp	*%r8
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L51:
	.long	.L6-.L51
	.long	.L53-.L51
	.long	.L40-.L51
	.long	.L41-.L51
	.long	.L42-.L51
	.long	.L43-.L51
	.long	.L44-.L51
	.long	.L45-.L51
	.long	.L46-.L51
	.long	.L47-.L51
	.long	.L48-.L51
	.long	.L49-.L51
	.long	.L50-.L51
	.text
	.p2align 4,,10
	.p2align 3
.L4:
	addl	8(%rdi), %eax
	addl	(%rdi), %edx
	subq	$12, %rsi
	addl	4(%rdi), %ecx
	addq	$12, %rdi
	movl	%eax, %r8d
	subl	%eax, %edx
	roll	$4, %r8d
	addl	%ecx, %eax
	xorl	%r8d, %edx
	movl	%edx, %r8d
	subl	%edx, %ecx
	addl	%eax, %edx
	roll	$6, %r8d
	xorl	%ecx, %r8d
	movl	%r8d, %ecx
	subl	%r8d, %eax
	addl	%edx, %r8d
	roll	$8, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	subl	%ecx, %edx
	leal	(%rcx,%r8), %r9d
	roll	$16, %eax
	xorl	%edx, %eax
	movl	%eax, %ecx
	subl	%eax, %r8d
	leal	(%rax,%r9), %edx
	roll	$19, %ecx
	xorl	%r8d, %ecx
	movl	%ecx, %eax
	subl	%ecx, %r9d
	addl	%edx, %ecx
	roll	$4, %eax
	xorl	%r9d, %eax
.L79:
	cmpq	$12, %rsi
	ja	.L4
	testl	%r10d, %r10d
	jne	.L5
	cmpq	$12, %rsi
	jbe	.L85
.L6:
	.p2align 4,,4
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L52:
	movzbl	(%rdi), %r8d
	addl	%r8d, %edx
	movzbl	1(%rdi), %r8d
	sall	$8, %r8d
	addl	%r8d, %edx
	movzbl	2(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %edx
	movzbl	3(%rdi), %r8d
	sall	$24, %r8d
	addl	%r8d, %edx
	movzbl	4(%rdi), %r8d
	addl	%r8d, %ecx
	movzbl	5(%rdi), %r8d
	sall	$8, %r8d
	addl	%r8d, %ecx
	movzbl	6(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %ecx
	movzbl	7(%rdi), %r8d
	sall	$24, %r8d
	addl	%r8d, %ecx
	movzbl	8(%rdi), %r8d
	addl	%r8d, %eax
	movzbl	9(%rdi), %r8d
	sall	$8, %r8d
	addl	%r8d, %eax
	movzbl	10(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %eax
	movzbl	11(%rdi), %r8d
	sall	$24, %r8d
	addl	%r8d, %eax
	movl	%eax, %r8d
	subl	%eax, %edx
	leal	(%rax,%rcx), %r9d
	roll	$4, %r8d
	xorl	%r8d, %edx
	movl	%edx, %eax
	subl	%edx, %ecx
	addl	%r9d, %edx
	roll	$6, %eax
	xorl	%ecx, %eax
	movl	%eax, %ecx
	subl	%eax, %r9d
	roll	$8, %ecx
	xorl	%r9d, %ecx
	leal	(%rax,%rdx), %r9d
	movl	%ecx, %eax
	subl	%ecx, %edx
	roll	$16, %eax
	leal	(%rcx,%r9), %r8d
	xorl	%edx, %eax
	subq	$12, %rsi
	addq	$12, %rdi
	movl	%eax, %ecx
	subl	%eax, %r9d
	leal	(%rax,%r8), %edx
	roll	$19, %ecx
	xorl	%r9d, %ecx
	movl	%ecx, %eax
	subl	%ecx, %r8d
	addl	%edx, %ecx
	roll	$4, %eax
	xorl	%r8d, %eax
.L81:
	cmpq	$12, %rsi
	ja	.L52
	leaq	.L65(%rip), %r8
	movslq	(%r8,%rsi,4), %rsi
	addq	%rsi, %r8
	jmp	*%r8
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L65:
	.long	.L6-.L65
	.long	.L53-.L65
	.long	.L54-.L65
	.long	.L55-.L65
	.long	.L56-.L65
	.long	.L57-.L65
	.long	.L58-.L65
	.long	.L59-.L65
	.long	.L60-.L65
	.long	.L61-.L65
	.long	.L62-.L65
	.long	.L63-.L65
	.long	.L64-.L65
	.text
	.p2align 4,,10
	.p2align 3
.L5:
	cmpq	$12, %rsi
	ja	.L6
	leaq	.L33(%rip), %r8
	movslq	(%r8,%rsi,4), %rsi
	addq	%rsi, %r8
	jmp	*%r8
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L33:
	.long	.L6-.L33
	.long	.L53-.L33
	.long	.L54-.L33
	.long	.L55-.L33
	.long	.L76-.L33
	.long	.L25-.L33
	.long	.L26-.L33
	.long	.L27-.L33
	.long	.L75-.L33
	.long	.L29-.L33
	.long	.L30-.L33
	.long	.L31-.L33
	.long	.L32-.L33
	.text
	.p2align 4,,10
	.p2align 3
.L85:
	leaq	.L19(%rip), %r8
	movslq	(%r8,%rsi,4), %rsi
	addq	%rsi, %r8
	jmp	*%r8
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L19:
	.long	.L6-.L19
	.long	.L53-.L19
	.long	.L40-.L19
	.long	.L9-.L19
	.long	.L76-.L19
	.long	.L25-.L19
	.long	.L12-.L19
	.long	.L13-.L19
	.long	.L75-.L19
	.long	.L29-.L19
	.long	.L16-.L19
	.long	.L17-.L19
	.long	.L32-.L19
	.text
.L64:
	movzbl	11(%rdi), %esi
	sall	$24, %esi
	addl	%esi, %eax
.L63:
	movzbl	10(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %eax
.L62:
	movzbl	9(%rdi), %esi
	sall	$8, %esi
	addl	%esi, %eax
.L61:
	movzbl	8(%rdi), %esi
	addl	%esi, %eax
.L60:
	movzbl	7(%rdi), %esi
	sall	$24, %esi
	addl	%esi, %ecx
.L59:
	movzbl	6(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %ecx
.L58:
	movzbl	5(%rdi), %esi
	sall	$8, %esi
	addl	%esi, %ecx
.L57:
	movzbl	4(%rdi), %esi
	addl	%esi, %ecx
.L56:
	movzbl	3(%rdi), %esi
	sall	$24, %esi
	addl	%esi, %edx
.L55:
	movzbl	2(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %edx
.L54:
	movzbl	1(%rdi), %esi
	sall	$8, %esi
	addl	%esi, %edx
.L53:
	movzbl	(%rdi), %esi
	addl	%esi, %edx
.L20:
	movl	%ecx, %esi
	xorl	%ecx, %eax
	roll	$14, %esi
	subl	%esi, %eax
	movl	%eax, %esi
	xorl	%eax, %edx
	roll	$11, %esi
	subl	%esi, %edx
	movl	%edx, %esi
	xorl	%edx, %ecx
	roll	$25, %esi
	subl	%esi, %ecx
	movl	%ecx, %esi
	xorl	%ecx, %eax
	roll	$16, %esi
	subl	%esi, %eax
	movl	%eax, %esi
	xorl	%eax, %edx
	roll	$4, %esi
	subl	%esi, %edx
	xorl	%edx, %ecx
	roll	$14, %edx
	subl	%edx, %ecx
	xorl	%ecx, %eax
	roll	$24, %ecx
	subl	%ecx, %eax
	ret
.L32:
	addl	8(%rdi), %eax
.L75:
	addl	4(%rdi), %ecx
.L76:
	addl	(%rdi), %edx
	jmp	.L20
.L43:
	movzbl	4(%rdi), %esi
	addl	%esi, %ecx
	.p2align 4,,10
	.p2align 3
.L42:
	movzwl	(%rdi), %esi
	addl	%esi, %edx
	movzwl	2(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %edx
	jmp	.L20
.L47:
	movzbl	8(%rdi), %esi
	addl	%esi, %eax
.L46:
	movzwl	4(%rdi), %esi
	addl	%esi, %ecx
	movzwl	6(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %ecx
	jmp	.L42
.L27:
	movzbl	6(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %ecx
.L26:
	movzbl	5(%rdi), %esi
	sall	$8, %esi
	addl	%esi, %ecx
.L25:
	movzbl	4(%rdi), %esi
	addl	(%rdi), %edx
	addl	%esi, %ecx
	jmp	.L20
.L31:
	movzbl	10(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %eax
.L30:
	movzbl	9(%rdi), %esi
	sall	$8, %esi
	addl	%esi, %eax
.L29:
	movzbl	8(%rdi), %esi
	addl	%esi, %eax
	jmp	.L75
.L41:
	movzbl	2(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %edx
.L40:
	movzwl	(%rdi), %esi
	addl	%esi, %edx
	jmp	.L20
.L49:
	movzbl	10(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %eax
.L48:
	movzwl	8(%rdi), %esi
	addl	%esi, %eax
	jmp	.L46
.L45:
	movzbl	6(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %ecx
.L44:
	movzwl	4(%rdi), %esi
	addl	%esi, %ecx
	jmp	.L42
.L50:
	movzwl	8(%rdi), %esi
	addl	%esi, %eax
	movzwl	10(%rdi), %esi
	sall	$16, %esi
	addl	%esi, %eax
	jmp	.L46
.L17:
	movl	8(%rdi), %esi
	addl	4(%rdi), %ecx
	addl	(%rdi), %edx
	andl	$16777215, %esi
	addl	%esi, %eax
	jmp	.L20
.L16:
	movzwl	8(%rdi), %esi
	addl	4(%rdi), %ecx
	addl	(%rdi), %edx
	addl	%esi, %eax
	jmp	.L20
.L13:
	movl	4(%rdi), %esi
	addl	(%rdi), %edx
	andl	$16777215, %esi
	addl	%esi, %ecx
	jmp	.L20
.L12:
	movzwl	4(%rdi), %esi
	addl	(%rdi), %edx
	addl	%esi, %ecx
	jmp	.L20
.L9:
	movl	(%rdi), %esi
	andl	$16777215, %esi
	addl	%esi, %edx
	jmp	.L20
.LFE15:
#ifdef __ELF__
	.size	ob_jenkins_hash, .-ob_jenkins_hash
#endif
	.p2align 4,,15
	.globl	ob_jenkins_hash2
#ifdef __ELF__
	.type	ob_jenkins_hash2, @function
#endif
ob_jenkins_hash2:
.LFB16:
	pushq	%rbx
.LCFI0:
	movq	%rdx, %r11
	xorl	%edx, %edx
	movq	$4097, -48(%rsp)
	leaq	-48(%rsp), %rax
	movq	$0, -40(%rsp)
	movq	$0, -32(%rsp)
	movq	$0, -24(%rsp)
	movq	$0, -16(%rsp)
	movq	$0, -8(%rsp)
	rolq $3,  %rdi ; rolq $13, %rdi
	rolq $61, %rdi ; rolq $51, %rdi
	xchgq %rbx,%rbx
	leal	-559038737(%rsi), %r9d
	addl	(%r11), %r9d
	movl	(%rcx), %eax
	movq	%rdx, -56(%rsp)
	movq	-56(%rsp), %rbx
	addl	%r9d, %eax
	testb	$3, %dil
	movl	%r9d, %r8d
	je	.L168
	testb	$1, %dil
	jne	.L170
	cmpq	$12, %rsi
	jbe	.L172
	.p2align 4,,10
	.p2align 3
.L125:
	movzwl	(%rdi), %edx
	movzwl	2(%rdi), %r10d
	subq	$12, %rsi
	addl	%edx, %r8d
	sall	$16, %r10d
	leal	(%r8,%r10), %edx
	movzwl	4(%rdi), %r8d
	addl	%r8d, %r9d
	movzwl	6(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %r9d
	movzwl	8(%rdi), %r8d
	addl	%r8d, %eax
	movzwl	10(%rdi), %r8d
	addq	$12, %rdi
	sall	$16, %r8d
	addl	%r8d, %eax
	movl	%eax, %r8d
	subl	%eax, %edx
	addl	%r9d, %eax
	roll	$4, %r8d
	xorl	%r8d, %edx
	movl	%edx, %r8d
	subl	%edx, %r9d
	addl	%eax, %edx
	roll	$6, %r8d
	xorl	%r9d, %r8d
	movl	%r8d, %r9d
	subl	%r8d, %eax
	leal	(%r8,%rdx), %r10d
	roll	$8, %r9d
	xorl	%eax, %r9d
	movl	%r9d, %r8d
	subl	%r9d, %edx
	leal	(%r9,%r10), %eax
	roll	$16, %r8d
	xorl	%edx, %r8d
	movl	%eax, %edx
	movl	%r8d, %r9d
	subl	%r8d, %r10d
	addl	%eax, %r8d
	roll	$19, %r9d
	xorl	%r10d, %r9d
	movl	%r9d, %eax
	subl	%r9d, %edx
	addl	%r8d, %r9d
	roll	$4, %eax
	xorl	%edx, %eax
	cmpq	$12, %rsi
	ja	.L125
.L172:
	leaq	.L139(%rip), %rdx
	movslq	(%rdx,%rsi,4), %rsi
	addq	%rsi, %rdx
	jmp	*%rdx
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L139:
	.long	.L165-.L139
	.long	.L142-.L139
	.long	.L128-.L139
	.long	.L129-.L139
	.long	.L130-.L139
	.long	.L131-.L139
	.long	.L132-.L139
	.long	.L133-.L139
	.long	.L134-.L139
	.long	.L135-.L139
	.long	.L136-.L139
	.long	.L137-.L139
	.long	.L138-.L139
	.text
	.p2align 4,,10
	.p2align 3
.L89:
	addl	8(%rdi), %eax
	addl	(%rdi), %r8d
	subq	$12, %rsi
	addl	4(%rdi), %r9d
	addq	$12, %rdi
	movl	%eax, %edx
	subl	%eax, %r8d
	roll	$4, %edx
	addl	%r9d, %eax
	xorl	%edx, %r8d
	movl	%r8d, %r10d
	subl	%r8d, %r9d
	addl	%eax, %r8d
	roll	$6, %r10d
	xorl	%r9d, %r10d
	movl	%r10d, %r9d
	subl	%r10d, %eax
	addl	%r8d, %r10d
	roll	$8, %r9d
	xorl	%eax, %r9d
	movl	%r9d, %eax
	subl	%r9d, %r8d
	leal	(%r9,%r10), %edx
	roll	$16, %eax
	xorl	%r8d, %eax
	movl	%eax, %r9d
	subl	%eax, %r10d
	leal	(%rax,%rdx), %r8d
	roll	$19, %r9d
	xorl	%r10d, %r9d
	movl	%r9d, %eax
	subl	%r9d, %edx
	addl	%r8d, %r9d
	roll	$4, %eax
	xorl	%edx, %eax
.L168:
	cmpq	$12, %rsi
	ja	.L89
	testl	%ebx, %ebx
	jne	.L90
	cmpq	$12, %rsi
	jbe	.L173
.L165:
	movl	%eax, (%r11)
	movl	%r9d, (%rcx)
	popq	%rbx
.LCFI1:
	ret
	.p2align 4,,10
	.p2align 3
.L140:
.LCFI2:
	movzbl	(%rdi), %edx
	movzbl	2(%rdi), %r10d
	addl	%edx, %r8d
	movzbl	1(%rdi), %edx
	sall	$16, %r10d
	sall	$8, %edx
	addl	%r8d, %edx
	movzbl	4(%rdi), %r8d
	addl	%r10d, %edx
	movzbl	3(%rdi), %r10d
	addl	%r8d, %r9d
	movzbl	5(%rdi), %r8d
	sall	$24, %r10d
	addl	%r10d, %edx
	sall	$8, %r8d
	addl	%r8d, %r9d
	movzbl	6(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %r9d
	movzbl	7(%rdi), %r8d
	sall	$24, %r8d
	addl	%r8d, %r9d
	movzbl	8(%rdi), %r8d
	addl	%r8d, %eax
	movzbl	9(%rdi), %r8d
	sall	$8, %r8d
	addl	%r8d, %eax
	movzbl	10(%rdi), %r8d
	sall	$16, %r8d
	addl	%r8d, %eax
	movzbl	11(%rdi), %r8d
	sall	$24, %r8d
	addl	%r8d, %eax
	movl	%eax, %r8d
	subl	%eax, %edx
	addl	%r9d, %eax
	roll	$4, %r8d
	xorl	%r8d, %edx
	movl	%edx, %r8d
	subl	%edx, %r9d
	addl	%eax, %edx
	roll	$6, %r8d
	xorl	%r9d, %r8d
	movl	%eax, %r9d
	movl	%r8d, %eax
	subl	%r8d, %r9d
	roll	$8, %eax
	xorl	%r9d, %eax
	leal	(%r8,%rdx), %r9d
	movl	%eax, %r8d
	subl	%eax, %edx
	roll	$16, %r8d
	addl	%r9d, %eax
	subq	$12, %rsi
	xorl	%edx, %r8d
	movl	%r9d, %edx
	addq	$12, %rdi
	movl	%r8d, %r9d
	subl	%r8d, %edx
	addl	%eax, %r8d
	roll	$19, %r9d
	xorl	%edx, %r9d
	movl	%eax, %edx
	movl	%r9d, %eax
	subl	%r9d, %edx
	addl	%r8d, %r9d
	roll	$4, %eax
	xorl	%edx, %eax
.L170:
	cmpq	$12, %rsi
	ja	.L140
	leaq	.L154(%rip), %rdx
	movslq	(%rdx,%rsi,4), %rsi
	addq	%rsi, %rdx
	jmp	*%rdx
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L154:
	.long	.L165-.L154
	.long	.L142-.L154
	.long	.L143-.L154
	.long	.L144-.L154
	.long	.L145-.L154
	.long	.L146-.L154
	.long	.L147-.L154
	.long	.L148-.L154
	.long	.L149-.L154
	.long	.L150-.L154
	.long	.L151-.L154
	.long	.L152-.L154
	.long	.L153-.L154
	.text
	.p2align 4,,10
	.p2align 3
.L90:
	cmpq	$12, %rsi
	ja	.L165
	leaq	.L120(%rip), %rdx
	movslq	(%rdx,%rsi,4), %rsi
	addq	%rsi, %rdx
	jmp	*%rdx
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L120:
	.long	.L165-.L120
	.long	.L142-.L120
	.long	.L143-.L120
	.long	.L144-.L120
	.long	.L164-.L120
	.long	.L112-.L120
	.long	.L113-.L120
	.long	.L114-.L120
	.long	.L115-.L120
	.long	.L116-.L120
	.long	.L117-.L120
	.long	.L118-.L120
	.long	.L119-.L120
	.text
	.p2align 4,,10
	.p2align 3
.L173:
	leaq	.L104(%rip), %rdx
	movslq	(%rdx,%rsi,4), %rsi
	addq	%rsi, %rdx
	jmp	*%rdx
#ifdef __ELF__
	.section	.rodata
#endif
	.align 4
	.align 4
.L104:
	.long	.L165-.L104
	.long	.L142-.L104
	.long	.L128-.L104
	.long	.L94-.L104
	.long	.L164-.L104
	.long	.L96-.L104
	.long	.L97-.L104
	.long	.L98-.L104
	.long	.L115-.L104
	.long	.L116-.L104
	.long	.L101-.L104
	.long	.L102-.L104
	.long	.L119-.L104
	.text
.L153:
	movzbl	11(%rdi), %edx
	sall	$24, %edx
	addl	%edx, %eax
.L152:
	movzbl	10(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %eax
.L151:
	movzbl	9(%rdi), %edx
	sall	$8, %edx
	addl	%edx, %eax
.L150:
	movzbl	8(%rdi), %edx
	addl	%edx, %eax
.L149:
	movzbl	7(%rdi), %edx
	sall	$24, %edx
	addl	%edx, %r9d
.L148:
	movzbl	6(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %r9d
.L147:
	movzbl	5(%rdi), %edx
	sall	$8, %edx
	addl	%edx, %r9d
.L146:
	movzbl	4(%rdi), %edx
	addl	%edx, %r9d
.L145:
	movzbl	3(%rdi), %edx
	sall	$24, %edx
	addl	%edx, %r8d
.L144:
	movzbl	2(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %r8d
.L143:
	movzbl	1(%rdi), %edx
	sall	$8, %edx
	addl	%edx, %r8d
.L142:
	movzbl	(%rdi), %edx
	addl	%edx, %r8d
.L105:
	movl	%r9d, %edx
	xorl	%r9d, %eax
	roll	$14, %edx
	subl	%edx, %eax
	movl	%eax, %edx
	xorl	%eax, %r8d
	roll	$11, %edx
	subl	%edx, %r8d
	movl	%r8d, %edx
	xorl	%r8d, %r9d
	roll	$25, %edx
	subl	%edx, %r9d
	movl	%r9d, %edx
	xorl	%r9d, %eax
	roll	$16, %edx
	subl	%edx, %eax
	movl	%eax, %edx
	xorl	%eax, %r8d
	roll	$4, %edx
	subl	%edx, %r8d
	xorl	%r8d, %r9d
	roll	$14, %r8d
	subl	%r8d, %r9d
	movl	%r9d, %edx
	xorl	%r9d, %eax
	roll	$24, %edx
	subl	%edx, %eax
	movl	%eax, (%r11)
	movl	%r9d, (%rcx)
	popq	%rbx
.LCFI3:
	ret
.L118:
.LCFI4:
	movzbl	10(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %eax
.L117:
	movzbl	9(%rdi), %edx
	sall	$8, %edx
	addl	%edx, %eax
.L116:
	movzbl	8(%rdi), %edx
	addl	%edx, %eax
.L115:
	addl	4(%rdi), %r9d
.L164:
	addl	(%rdi), %r8d
	jmp	.L105
.L135:
	movzbl	8(%rdi), %edx
	addl	%edx, %eax
.L134:
	movzwl	4(%rdi), %edx
	addl	%edx, %r9d
	movzwl	6(%rdi), %edx
	sall	$16, %edx
.L163:
	addl	%edx, %r9d
.L130:
	movzwl	(%rdi), %edx
	addl	%edx, %r8d
	movzwl	2(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %r8d
	jmp	.L105
.L129:
	movzbl	2(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %r8d
.L128:
	movzwl	(%rdi), %edx
	addl	%edx, %r8d
	jmp	.L105
.L114:
	movzbl	6(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %r9d
.L113:
	movzbl	5(%rdi), %edx
	sall	$8, %edx
	addl	%edx, %r9d
.L112:
	movzbl	4(%rdi), %edx
	addl	%edx, %r9d
	jmp	.L164
.L119:
	addl	8(%rdi), %eax
	addl	4(%rdi), %r9d
	addl	(%rdi), %r8d
	jmp	.L105
.L137:
	movzbl	10(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %eax
.L136:
	movzwl	8(%rdi), %edx
	addl	%edx, %eax
	jmp	.L134
.L133:
	movzbl	6(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %r9d
.L132:
	movzwl	4(%rdi), %edx
	jmp	.L163
.L131:
	movzbl	4(%rdi), %edx
	jmp	.L163
.L138:
	movzwl	8(%rdi), %edx
	addl	%edx, %eax
	movzwl	10(%rdi), %edx
	sall	$16, %edx
	addl	%edx, %eax
	jmp	.L134
.L102:
	movl	8(%rdi), %edx
	addl	4(%rdi), %r9d
	addl	(%rdi), %r8d
	andl	$16777215, %edx
	addl	%edx, %eax
	jmp	.L105
.L101:
	movzwl	8(%rdi), %edx
	addl	4(%rdi), %r9d
	addl	(%rdi), %r8d
	addl	%edx, %eax
	jmp	.L105
.L98:
	movl	4(%rdi), %edx
	addl	(%rdi), %r8d
	andl	$16777215, %edx
	addl	%edx, %r9d
	jmp	.L105
.L97:
	movzwl	4(%rdi), %edx
	addl	(%rdi), %r8d
	addl	%edx, %r9d
	jmp	.L105
.L96:
	movzbl	4(%rdi), %edx
	addl	(%rdi), %r8d
	addl	%edx, %r9d
	jmp	.L105
.L94:
	movl	(%rdi), %edx
	andl	$16777215, %edx
	addl	%edx, %r8d
	jmp	.L105
.LFE16:
#ifdef __ELF__
	.size	ob_jenkins_hash2, .-ob_jenkins_hash2
#endif
	.p2align 4,,15
	.globl	ob_city_hash64
#ifdef __ELF__
	.type	ob_city_hash64, @function
#endif
ob_city_hash64:
.LFB30:
	pushq	%r13
.LCFI5:
	cmpq	$32, %rsi
	pushq	%r12
.LCFI6:
	pushq	%rbp
.LCFI7:
	pushq	%rbx
.LCFI8:
	ja	.L175
	cmpq	$16, %rsi
	jbe	.L187
	movabsq	$-5435081209227447693, %rcx
	movq	8(%rdi), %r8
	movabsq	$-3942382747735136937, %rdx
	imulq	(%rdi), %rcx
	movabsq	$-7286425919675154353, %rax
	xorq	%r8, %rdx
	imulq	-8(%rdi,%rsi), %rax
	rorq	$20, %rdx
	leaq	(%rcx,%rsi), %r9
	movq	-16(%rdi,%rsi), %rsi
	subq	%r8, %rcx
	rorq	$43, %rcx
	addq	%r9, %rdx
	movabsq	$-4348849565147123417, %r9
	imulq	%r9, %rsi
	subq	%rax, %rdx
	rorq	$30, %rax
	addq	%rsi, %rcx
	addq	%rcx, %rax
.L186:
	movabsq	$-7070675565921424023, %rcx
	xorq	%rdx, %rax
	imulq	%rcx, %rax
	popq	%rbx
.LCFI9:
	popq	%rbp
.LCFI10:
	popq	%r12
.LCFI11:
	xorq	%rax, %rdx
	shrq	$47, %rax
	xorq	%rax, %rdx
	imulq	%rcx, %rdx
	popq	%r13
.LCFI12:
	movq	%rdx, %rax
	shrq	$47, %rax
	xorq	%rdx, %rax
	imulq	%rcx, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L175:
.LCFI13:
	cmpq	$64, %rsi
	jbe	.L188
	leaq	-64(%rdi,%rsi), %rdx
	movq	%rsi, %r8
	movabsq	$-5435081209227447693, %rbp
	movq	%rbp, %rax
	xorq	-16(%rdi,%rsi), %rax
	movabsq	$-4348849565147123417, %r12
	addq	(%rdx), %r8
	movq	24(%rdx), %r9
	movq	%r12, %r10
	xorq	-56(%rdi,%rsi), %r10
	movq	%r8, %rcx
	addq	8(%rdx), %rcx
	addq	16(%rdx), %rcx
	leaq	(%rax,%r8), %rdx
	rorq	$33, %rax
	imulq	%rbp, %rax
	addq	%r9, %rdx
	rorq	$21, %rdx
	leaq	(%rcx,%r9), %rbx
	rorq	$44, %rcx
	addq	%r8, %rcx
	leaq	-32(%rdi,%rsi), %r8
	addq	%rdx, %rcx
	movq	%rsi, %rdx
	subq	$1, %rsi
	imulq	%rbp, %rdx
	movq	24(%r8), %r11
	andq	$-64, %rsi
	addq	(%r8), %rdx
	addq	%r11, %r12
	movq	%rdx, %r9
	addq	8(%r8), %r9
	addq	%rdx, %r12
	addq	16(%r8), %r9
	rorq	$21, %r12
	addq	%r9, %r11
	rorq	$44, %r9
	addq	%r12, %r9
	addq	%rdx, %r9
	movq	%rcx, %rdx
	shrq	$47, %rdx
	xorq	%rcx, %rdx
	imulq	%rbp, %rdx
	addq	%rdx, %r10
	movq	%r10, %r8
	addq	(%rdi), %r8
	rorq	$39, %r8
	imulq	%rbp, %r8
	jmp	.L182
	.p2align 4,,10
	.p2align 3
.L184:
	movq	%rdx, %r10
.L182:
	addq	%rax, %r8
	movq	16(%rdi), %r12
	movq	48(%rdi), %r13
	leaq	(%r8,%rbx), %rdx
	addq	%rcx, %rax
	movq	%r11, %r8
	imulq	%rbp, %rcx
	xorq	%r10, %r8
	movq	24(%rdi), %r10
	addq	%r12, %rdx
	addq	%r13, %rax
	addq	(%rdi), %rcx
	rorq	$37, %rdx
	addq	8(%rdi), %r12
	rorq	$42, %rax
	addq	%r10, %r11
	rorq	$33, %r8
	imulq	%rbp, %rdx
	imulq	%rbp, %rax
	addq	%rcx, %r12
	xorq	%r9, %rdx
	addq	32(%rdi), %r9
	xorq	%rbx, %rax
	leaq	(%r12,%r10), %rbx
	rorq	$44, %r12
	addq	%rcx, %r12
	addq	%r11, %rcx
	addq	%rdx, %rcx
	leaq	(%r9,%r8), %r10
	movq	%r13, %r9
	addq	40(%rdi), %r9
	rorq	$21, %rcx
	addq	%r12, %rcx
	movq	56(%rdi), %r12
	addq	$64, %rdi
	addq	%r10, %r9
	leaq	(%r9,%r12), %r11
	rorq	$44, %r9
	leaq	(%r9,%r10), %r13
	leaq	(%r10,%r12), %r9
	addq	%rax, %r9
	rorq	$21, %r9
	addq	%r13, %r9
	subq	$64, %rsi
	jne	.L184
	movabsq	$-7070675565921424023, %rsi
	xorq	%r9, %rcx
	xorq	%r11, %rbx
	imulq	%rsi, %rcx
	movq	%rax, %rdi
	imulq	%rsi, %rbx
	shrq	$47, %rdi
	xorq	%rax, %rdi
	movabsq	$-5435081209227447693, %rax
	xorq	%rcx, %r9
	shrq	$47, %rcx
	xorq	%rcx, %r9
	xorq	%rbx, %r11
	shrq	$47, %rbx
	imulq	%rsi, %r9
	xorq	%rbx, %r11
	imulq	%rsi, %r11
	imulq	%rax, %rdi
	movq	%r9, %rcx
	shrq	$47, %rcx
	xorq	%r9, %rcx
	addq	%rdi, %rdx
	imulq	%rsi, %rcx
	addq	%rcx, %r8
	movq	%r11, %rcx
	shrq	$47, %rcx
	xorq	%r11, %rcx
	imulq	%rsi, %rcx
	addq	%rcx, %rdx
	xorq	%r8, %rdx
	imulq	%rsi, %rdx
	xorq	%rdx, %r8
	shrq	$47, %rdx
	xorq	%rdx, %r8
	imulq	%rsi, %r8
	movq	%r8, %rax
	shrq	$47, %rax
	xorq	%r8, %rax
	imulq	%rsi, %rax
.L180:
	popq	%rbx
.LCFI14:
	popq	%rbp
.LCFI15:
	popq	%r12
.LCFI16:
	popq	%r13
.LCFI17:
	ret
	.p2align 4,,10
	.p2align 3
.L187:
.LCFI18:
	cmpq	$8, %rsi
	ja	.L189
	cmpq	$3, %rsi
	ja	.L190
	testq	%rsi, %rsi
	movabsq	$-7286425919675154353, %rax
	je	.L180
	movzbl	-1(%rdi,%rsi), %edx
	movabsq	$-3942382747735136937, %rcx
	leal	(%rsi,%rdx,4), %edx
	shrq	%rsi
	imulq	%rcx, %rdx
	movzbl	(%rdi,%rsi), %ecx
	movzbl	(%rdi), %esi
	sall	$8, %ecx
	addl	%esi, %ecx
	imulq	%rax, %rcx
	xorq	%rcx, %rdx
	movq	%rdx, %rcx
	shrq	$47, %rcx
	xorq	%rdx, %rcx
	imulq	%rcx, %rax
	jmp	.L180
	.p2align 4,,10
	.p2align 3
.L188:
	movq	-16(%rdi,%rsi), %r11
	movabsq	$-4348849565147123417, %rcx
	movq	24(%rdi), %r10
	movq	16(%rdi), %r8
	leaq	(%r11,%rsi), %rax
	imulq	%rcx, %rax
	addq	(%rdi), %rax
	movq	%rax, %rbx
	addq	8(%rdi), %rbx
	leaq	(%rax,%r10), %rdx
	rorq	$37, %rax
	rorq	$52, %rdx
	leaq	(%rbx,%r8), %r9
	rorq	$7, %rbx
	addq	-32(%rdi,%rsi), %r8
	addq	%rbx, %rdx
	addq	%rdx, %rax
	movq	-8(%rdi,%rsi), %rdx
	movq	-24(%rdi,%rsi), %rdi
	movq	%r9, %rbp
	rorq	$31, %rbp
	addq	%r8, %rdi
	addq	%rbp, %rax
	leaq	(%rdi,%r11), %rsi
	movq	%r8, %r11
	addq	%rdx, %r8
	rorq	$37, %r11
	rorq	$52, %r8
	rorq	$7, %rdi
	addq	%r11, %r10
	addq	%r10, %r8
	addq	%rdi, %r8
	leaq	(%rsi,%rdx), %rdi
	rorq	$31, %rsi
	addq	%r8, %rsi
	movabsq	$-7286425919675154353, %r8
	addq	%rax, %rdi
	addq	%r9, %rsi
	imulq	%rcx, %rdi
	imulq	%r8, %rsi
	popq	%rbx
.LCFI19:
	popq	%rbp
.LCFI20:
	popq	%r12
.LCFI21:
	addq	%rdi, %rsi
	movq	%rsi, %rdx
	shrq	$47, %rdx
	xorq	%rsi, %rdx
	imulq	%rcx, %rdx
	popq	%r13
.LCFI22:
	addq	%rax, %rdx
	movq	%rdx, %rax
	shrq	$47, %rax
	xorq	%rdx, %rax
	imulq	%r8, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L189:
.LCFI23:
	movq	-8(%rdi,%rsi), %r8
	movl	%esi, %ecx
	leaq	(%r8,%rsi), %rdx
	rorq	%cl, %rdx
	movabsq	$-7070675565921424023, %rcx
	movq	%rdx, %rax
	xorq	(%rdi), %rax
	imulq	%rcx, %rax
	xorq	%rax, %rdx
	shrq	$47, %rax
	xorq	%rax, %rdx
	imulq	%rcx, %rdx
	movq	%rdx, %rax
	shrq	$47, %rax
	xorq	%rdx, %rax
	imulq	%rcx, %rax
	xorq	%r8, %rax
	jmp	.L180
	.p2align 4,,10
	.p2align 3
.L190:
	mov	(%rdi), %eax
	mov	-4(%rdi,%rsi), %edx
	leaq	(%rsi,%rax,8), %rax
	jmp	.L186
.LFE30:
#ifdef __ELF__
	.size	ob_city_hash64, .-ob_city_hash64
#endif
	.p2align 4,,15
	.globl	ob_city_hash64_with_seeds
#ifdef __ELF__
	.type	ob_city_hash64_with_seeds, @function
#endif
ob_city_hash64_with_seeds:
.LFB31:
	pushq	%rbp
.LCFI24:
	movq	%rdx, %rbp
	pushq	%rbx
.LCFI25:
	movq	%rcx, %rbx
	subq	$8, %rsp
.LCFI26:
#ifdef __ELF__
	call	ob_city_hash64@PLT
#else
	call	ob_city_hash64
#endif
	subq	%rbp, %rax
	movabsq	$-7070675565921424023, %rdx
	addq	$8, %rsp
.LCFI27:
	xorq	%rbx, %rax
	imulq	%rdx, %rax
	movq	%rax, %rcx
	shrq	$47, %rax
	xorq	%rbx, %rcx
	xorq	%rax, %rcx
	imulq	%rdx, %rcx
	popq	%rbx
.LCFI28:
	popq	%rbp
.LCFI29:
	movq	%rcx, %rax
	shrq	$47, %rax
	xorq	%rcx, %rax
	imulq	%rdx, %rax
	ret
.LFE31:
#ifdef __ELF__
	.size	ob_city_hash64_with_seeds, .-ob_city_hash64_with_seeds
#endif
/* For security, make sure the stack is not executable:
 * http://www.gentoo.org/proj/en/hardened/gnu-stack.xml#doc_chap6 */
#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
