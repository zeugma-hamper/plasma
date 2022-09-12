#ifdef __ELF__
#define _ob_jenkins_hash ob_jenkins_hash
#define _ob_jenkins_hash2 ob_jenkins_hash2
#define _ob_city_hash64 ob_city_hash64
#define _ob_city_hash64_with_seeds ob_city_hash64_with_seeds
	.text
#else
	.section	__TEXT,__text,regular,pure_instructions
#endif
	.globl	_ob_jenkins_hash
#ifdef __ELF__
	.type	_ob_jenkins_hash, @function
#endif
	.align	4, 0x90
_ob_jenkins_hash:                       ## @ob_jenkins_hash
## BB#0:                                ## %entry
	pushl	%ebp
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	subl	$36, %esp
	calll	L0$pb
L0$pb:
	popl	%eax
	movl	%eax, 4(%esp)           ## 4-byte Spill
	movl	$4097, 12(%esp)         ## imm = 0x1001
	movl	$0, 16(%esp)
	movl	$0, 20(%esp)
	movl	$0, 24(%esp)
	movl	$0, 28(%esp)
	movl	$0, 32(%esp)
	leal	12(%esp), %eax
	xorl	%edx, %edx
	## InlineAsm Start
	roll $3,  %edi ; roll $13, %edi
	roll $29, %edi ; roll $19, %edi
	xchgl %ebx,%ebx
	## InlineAsm End
	movl	%edx, 8(%esp)
	movl	64(%esp), %eax
	movl	60(%esp), %edi
	leal	-559038737(%edi,%eax), %edx
	movl	56(%esp), %ebx
	movl	8(%esp), %eax
	movl	%eax, (%esp)            ## 4-byte Spill
	testb	$3, %bl
	jne	LBB0_29
## BB#1:                                ## %if.then
	movl	%edx, %ecx
	movl	%edx, %eax
	cmpl	$13, %edi
	jb	LBB0_3
	.align	4, 0x90
LBB0_2:                                 ## %while.body
                                        ## =>This Inner Loop Header: Depth=1
	addl	8(%ebx), %eax
	addl	(%ebx), %edx
	subl	%eax, %edx
	addl	4(%ebx), %ecx
	movl	%eax, %esi
	roll	$4, %esi
	leal	(%eax,%ecx), %ebp
	xorl	%edx, %esi
	subl	%esi, %ecx
	movl	%esi, %eax
	roll	$6, %eax
	leal	(%esi,%ebp), %edx
	xorl	%ecx, %eax
	subl	%eax, %ebp
	movl	%eax, %ecx
	roll	$8, %ecx
	leal	(%eax,%edx), %eax
	xorl	%ebp, %ecx
	subl	%ecx, %edx
	movl	%ecx, %esi
	roll	$16, %esi
	leal	(%ecx,%eax), %ebp
	xorl	%edx, %esi
	subl	%esi, %eax
	movl	%esi, %ecx
	roll	$19, %ecx
	leal	(%esi,%ebp), %edx
	xorl	%eax, %ecx
	subl	%ecx, %ebp
	movl	%ecx, %eax
	roll	$4, %eax
	addl	%edx, %ecx
	xorl	%ebp, %eax
	addl	$12, %ebx
	addl	$-12, %edi
	cmpl	$12, %edi
	ja	LBB0_2
LBB0_3:                                 ## %while.end
	cmpl	$0, (%esp)              ## 4-byte Folded Reload
	jne	LBB0_15
## BB#4:                                ## %if.then110
	cmpl	$12, %edi
	ja	LBB0_67
## BB#5:                                ## %if.then110
	movl	4(%esp), %esi           ## 4-byte Reload
Ltmp0 = LJTI0_3-L0$pb
	addl	Ltmp0(%esi,%edi,4), %esi
	jmpl	*%esi
LBB0_6:                                 ## %sw.bb127
	addl	(%ebx), %edx
	addl	4(%ebx), %ecx
	movl	$16777215, %esi         ## imm = 0xFFFFFF
	andl	8(%ebx), %esi
	addl	%esi, %eax
	jmp	LBB0_67
LBB0_7:                                 ## %sw.bb144
	addl	(%ebx), %edx
	addl	4(%ebx), %ecx
	movzwl	8(%ebx), %esi
	addl	%esi, %eax
	jmp	LBB0_67
LBB0_8:                                 ## %sw.bb161
	addl	(%ebx), %edx
	addl	4(%ebx), %ecx
	movzbl	8(%ebx), %esi
	addl	%esi, %eax
	jmp	LBB0_67
LBB0_9:                                 ## %sw.bb189
	addl	(%ebx), %edx
	movl	$16777215, %esi         ## imm = 0xFFFFFF
	andl	4(%ebx), %esi
	addl	%esi, %ecx
	jmp	LBB0_67
LBB0_10:                                ## %sw.bb201
	addl	(%ebx), %edx
	movzwl	4(%ebx), %esi
	addl	%esi, %ecx
	jmp	LBB0_67
LBB0_11:                                ## %sw.bb213
	addl	(%ebx), %edx
	movzbl	4(%ebx), %esi
	addl	%esi, %ecx
	jmp	LBB0_67
LBB0_12:                                ## %sw.bb225
	addl	(%ebx), %edx
	jmp	LBB0_67
LBB0_13:                                ## %sw.bb231
	movl	$16777215, %esi         ## imm = 0xFFFFFF
	andl	(%ebx), %esi
	addl	%esi, %edx
	jmp	LBB0_67
LBB0_14:                                ## %sw.bb238
	movzwl	(%ebx), %esi
	addl	%esi, %edx
	jmp	LBB0_67
LBB0_15:                                ## %if.else
	cmpl	$12, %edi
	ja	LBB0_67
## BB#16:                               ## %if.else
	movl	4(%esp), %esi           ## 4-byte Reload
Ltmp1 = LJTI0_2-L0$pb
	addl	Ltmp1(%esi,%edi,4), %esi
	jmpl	*%esi
LBB0_17:                                ## %sw.bb
	addl	(%ebx), %edx
	addl	4(%ebx), %ecx
	addl	8(%ebx), %eax
	jmp	LBB0_67
LBB0_18:                                ## %sw.bb273
	movzbl	10(%ebx), %esi
	shll	$16, %esi
	addl	%esi, %eax
LBB0_19:                                ## %sw.bb280
	movzbl	9(%ebx), %esi
	shll	$8, %esi
	addl	%esi, %eax
LBB0_20:                                ## %sw.bb288
	movzbl	8(%ebx), %esi
	addl	%esi, %eax
LBB0_21:                                ## %sw.bb178
	addl	(%ebx), %edx
	addl	4(%ebx), %ecx
	jmp	LBB0_67
LBB0_22:                                ## %sw.bb306
	movzbl	6(%ebx), %esi
	shll	$16, %esi
	addl	%esi, %ecx
LBB0_23:                                ## %sw.bb314
	movzbl	5(%ebx), %esi
	shll	$8, %esi
	addl	%esi, %ecx
LBB0_24:                                ## %sw.bb322
	movzbl	4(%ebx), %esi
	addl	%esi, %ecx
LBB0_25:                                ## %sw.bb329
	addl	(%ebx), %edx
	jmp	LBB0_67
LBB0_26:                                ## %sw.bb335
	movzbl	2(%ebx), %esi
	shll	$16, %esi
	addl	%esi, %edx
LBB0_27:                                ## %sw.bb343
	movzbl	1(%ebx), %esi
	shll	$8, %esi
	addl	%esi, %edx
LBB0_28:                                ## %sw.bb245
	movzbl	(%ebx), %esi
	addl	%esi, %edx
	jmp	LBB0_67
LBB0_29:                                ## %if.else361
	testb	$1, %bl
	je	LBB0_35
## BB#30:                               ## %while.cond683.preheader
	movl	%edx, %ebp
	movl	%edx, %ecx
	movl	%edx, %eax
	jmp	LBB0_32
	.align	4, 0x90
LBB0_31:                                ## %while.body687
                                        ##   in Loop: Header=BB0_32 Depth=1
	movzbl	8(%ebx), %edx
	addl	%eax, %edx
	movzbl	9(%ebx), %eax
	shll	$8, %eax
	addl	%edx, %eax
	movzbl	10(%ebx), %edx
	shll	$16, %edx
	addl	%eax, %edx
	movzbl	11(%ebx), %eax
	shll	$24, %eax
	addl	%edx, %eax
	movzbl	(%ebx), %esi
	addl	%ebp, %esi
	movzbl	1(%ebx), %edx
	shll	$8, %edx
	addl	%esi, %edx
	movzbl	2(%ebx), %esi
	shll	$16, %esi
	addl	%edx, %esi
	movzbl	3(%ebx), %edx
	shll	$24, %edx
	addl	%esi, %edx
	subl	%eax, %edx
	movzbl	4(%ebx), %ebp
	addl	%ecx, %ebp
	movzbl	5(%ebx), %esi
	shll	$8, %esi
	addl	%ebp, %esi
	movzbl	6(%ebx), %ecx
	shll	$16, %ecx
	addl	%esi, %ecx
	movzbl	7(%ebx), %ebp
	shll	$24, %ebp
	addl	%ecx, %ebp
	movl	%eax, %ecx
	roll	$4, %ecx
	leal	(%eax,%ebp), %esi
	xorl	%edx, %ecx
	subl	%ecx, %ebp
	movl	%ecx, %eax
	roll	$6, %eax
	leal	(%ecx,%esi), %ecx
	xorl	%ebp, %eax
	subl	%eax, %esi
	movl	%eax, %edx
	roll	$8, %edx
	leal	(%eax,%ecx), %eax
	xorl	%esi, %edx
	subl	%edx, %ecx
	movl	%edx, %esi
	roll	$16, %esi
	leal	(%edx,%eax), %edx
	xorl	%ecx, %esi
	subl	%esi, %eax
	movl	%esi, %ecx
	roll	$19, %ecx
	leal	(%esi,%edx), %ebp
	xorl	%eax, %ecx
	subl	%ecx, %edx
	movl	%ecx, %eax
	roll	$4, %eax
	addl	%ebp, %ecx
	xorl	%edx, %eax
	addl	$12, %ebx
	addl	$-12, %edi
LBB0_32:                                ## %while.body687
                                        ## =>This Inner Loop Header: Depth=1
	cmpl	$12, %edi
	ja	LBB0_31
## BB#33:                               ## %while.end851
	cmpl	$12, %edi
	jbe	LBB0_54
## BB#34:
	movl	%ebp, %edx
	jmp	LBB0_67
LBB0_35:                                ## %if.then367
	movl	%edx, %esi
	movl	%edx, %ebp
	movl	%edx, %eax
	cmpl	$13, %edi
	jb	LBB0_37
	.align	4, 0x90
LBB0_36:                                ## %while.body377
                                        ## =>This Inner Loop Header: Depth=1
	movzwl	8(%ebx), %edx
	movzwl	10(%ebx), %ecx
	shll	$16, %ecx
	orl	%edx, %ecx
	addl	%eax, %ecx
	movzwl	(%ebx), %edx
	movzwl	2(%ebx), %eax
	shll	$16, %eax
	orl	%edx, %eax
	addl	%esi, %eax
	subl	%ecx, %eax
	movzwl	4(%ebx), %esi
	movzwl	6(%ebx), %edx
	shll	$16, %edx
	orl	%esi, %edx
	addl	%ebp, %edx
	movl	%ecx, %esi
	roll	$4, %esi
	leal	(%ecx,%edx), %ecx
	xorl	%eax, %esi
	subl	%esi, %edx
	movl	%esi, %eax
	roll	$6, %eax
	leal	(%esi,%ecx), %esi
	xorl	%edx, %eax
	subl	%eax, %ecx
	movl	%eax, %ebp
	roll	$8, %ebp
	leal	(%eax,%esi), %eax
	xorl	%ecx, %ebp
	subl	%ebp, %esi
	movl	%ebp, %edx
	roll	$16, %edx
	leal	(%ebp,%eax), %ecx
	xorl	%esi, %edx
	subl	%edx, %eax
	movl	%edx, %ebp
	roll	$19, %ebp
	leal	(%edx,%ecx), %esi
	xorl	%eax, %ebp
	subl	%ebp, %ecx
	movl	%ebp, %eax
	roll	$4, %eax
	addl	%esi, %ebp
	xorl	%ecx, %eax
	addl	$12, %ebx
	addl	$-12, %edi
	cmpl	$12, %edi
	ja	LBB0_36
LBB0_37:                                ## %while.end496
	cmpl	$12, %edi
	jbe	LBB0_39
## BB#38:
	movl	%ebp, %ecx
	movl	%esi, %edx
	jmp	LBB0_67
LBB0_39:                                ## %while.end496
	movl	4(%esp), %ecx           ## 4-byte Reload
Ltmp2 = LJTI0_1-L0$pb
	addl	Ltmp2(%ecx,%edi,4), %ecx
	jmpl	*%ecx
LBB0_40:                                ## %sw.bb499
	movzwl	(%ebx), %ecx
	movzwl	2(%ebx), %edx
	shll	$16, %edx
	orl	%ecx, %edx
	addl	%esi, %edx
	movzwl	4(%ebx), %esi
	movzwl	6(%ebx), %ecx
	shll	$16, %ecx
	orl	%esi, %ecx
	addl	%ebp, %ecx
	movzwl	8(%ebx), %edi
	movzwl	10(%ebx), %esi
	shll	$16, %esi
	orl	%edi, %esi
	addl	%esi, %eax
	jmp	LBB0_67
LBB0_41:                                ## %sw.bb536
	movzbl	10(%ebx), %ecx
	shll	$16, %ecx
	addl	%ecx, %eax
LBB0_42:                                ## %sw.bb544
	movzwl	8(%ebx), %ecx
	jmp	LBB0_44
LBB0_43:                                ## %sw.bb575
	movzbl	8(%ebx), %ecx
LBB0_44:                                ## %sw.bb575
	addl	%ecx, %eax
LBB0_45:                                ## %sw.bb582
	movzwl	(%ebx), %ecx
	movzwl	2(%ebx), %edx
	shll	$16, %edx
	orl	%ecx, %edx
	addl	%esi, %edx
	movzwl	4(%ebx), %esi
	movzwl	6(%ebx), %ecx
	shll	$16, %ecx
	orl	%esi, %ecx
	addl	%ebp, %ecx
	jmp	LBB0_67
LBB0_46:                                ## %sw.bb607
	movzbl	6(%ebx), %ecx
	shll	$16, %ecx
	addl	%ecx, %ebp
LBB0_47:                                ## %sw.bb615
	movzwl	4(%ebx), %ecx
	addl	%ebp, %ecx
	movzwl	(%ebx), %edi
	movzwl	2(%ebx), %edx
	shll	$16, %edx
	orl	%edi, %edx
	addl	%esi, %edx
	jmp	LBB0_67
LBB0_48:                                ## %sw.bb634
	movzbl	4(%ebx), %ecx
	addl	%ecx, %ebp
LBB0_49:                                ## %sw.bb641
	movzwl	(%ebx), %ecx
	movzwl	2(%ebx), %edx
	shll	$16, %edx
	orl	%ecx, %edx
	jmp	LBB0_52
LBB0_50:                                ## %sw.bb654
	movzbl	2(%ebx), %ecx
	shll	$16, %ecx
	addl	%ecx, %esi
LBB0_51:                                ## %sw.bb662
	movzwl	(%ebx), %edx
LBB0_52:                                ## %sw.bb662
	addl	%esi, %edx
	movl	%ebp, %ecx
	jmp	LBB0_67
LBB0_53:                                ## %sw.bb669
	movzbl	(%ebx), %edx
	jmp	LBB0_52
LBB0_54:                                ## %while.end851
	movl	4(%esp), %edx           ## 4-byte Reload
Ltmp3 = LJTI0_0-L0$pb
	addl	Ltmp3(%edx,%edi,4), %edx
	jmpl	*%edx
LBB0_55:                                ## %sw.bb853
	movzbl	11(%ebx), %edx
	shll	$24, %edx
	addl	%edx, %eax
LBB0_56:                                ## %sw.bb861
	movzbl	10(%ebx), %edx
	shll	$16, %edx
	addl	%edx, %eax
LBB0_57:                                ## %sw.bb869
	movzbl	9(%ebx), %edx
	shll	$8, %edx
	addl	%edx, %eax
LBB0_58:                                ## %sw.bb877
	movzbl	8(%ebx), %edx
	addl	%edx, %eax
LBB0_59:                                ## %sw.bb884
	movzbl	7(%ebx), %edx
	shll	$24, %edx
	addl	%edx, %ecx
LBB0_60:                                ## %sw.bb892
	movzbl	6(%ebx), %edx
	shll	$16, %edx
	addl	%edx, %ecx
LBB0_61:                                ## %sw.bb900
	movzbl	5(%ebx), %edx
	shll	$8, %edx
	addl	%edx, %ecx
LBB0_62:                                ## %sw.bb908
	movzbl	4(%ebx), %edx
	addl	%edx, %ecx
LBB0_63:                                ## %sw.bb915
	movzbl	3(%ebx), %edx
	shll	$24, %edx
	addl	%edx, %ebp
LBB0_64:                                ## %sw.bb923
	movzbl	2(%ebx), %edx
	shll	$16, %edx
	addl	%edx, %ebp
LBB0_65:                                ## %sw.bb931
	movzbl	1(%ebx), %edx
	shll	$8, %edx
	addl	%edx, %ebp
LBB0_66:                                ## %sw.bb939
	movzbl	(%ebx), %edx
	addl	%ebp, %edx
LBB0_67:                                ## %if.end950
	xorl	%ecx, %eax
	movl	%ecx, %esi
	roll	$14, %esi
	subl	%esi, %eax
	xorl	%eax, %edx
	movl	%eax, %esi
	roll	$11, %esi
	subl	%esi, %edx
	xorl	%edx, %ecx
	movl	%edx, %esi
	roll	$25, %esi
	subl	%esi, %ecx
	xorl	%ecx, %eax
	movl	%ecx, %esi
	roll	$16, %esi
	subl	%esi, %eax
	xorl	%eax, %edx
	movl	%eax, %esi
	roll	$4, %esi
	subl	%esi, %edx
	xorl	%edx, %ecx
	roll	$14, %edx
	subl	%edx, %ecx
	xorl	%ecx, %eax
	roll	$24, %ecx
	subl	%ecx, %eax
LBB0_68:                                ## %return
	addl	$36, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret
	.align	2, 0x90
L0_0_set_68 = LBB0_68-L0$pb
L0_0_set_66 = LBB0_66-L0$pb
L0_0_set_65 = LBB0_65-L0$pb
L0_0_set_64 = LBB0_64-L0$pb
L0_0_set_63 = LBB0_63-L0$pb
L0_0_set_62 = LBB0_62-L0$pb
L0_0_set_61 = LBB0_61-L0$pb
L0_0_set_60 = LBB0_60-L0$pb
L0_0_set_59 = LBB0_59-L0$pb
L0_0_set_58 = LBB0_58-L0$pb
L0_0_set_57 = LBB0_57-L0$pb
L0_0_set_56 = LBB0_56-L0$pb
L0_0_set_55 = LBB0_55-L0$pb
LJTI0_0:
	.long	L0_0_set_68
	.long	L0_0_set_66
	.long	L0_0_set_65
	.long	L0_0_set_64
	.long	L0_0_set_63
	.long	L0_0_set_62
	.long	L0_0_set_61
	.long	L0_0_set_60
	.long	L0_0_set_59
	.long	L0_0_set_58
	.long	L0_0_set_57
	.long	L0_0_set_56
	.long	L0_0_set_55
L0_1_set_68 = LBB0_68-L0$pb
L0_1_set_53 = LBB0_53-L0$pb
L0_1_set_51 = LBB0_51-L0$pb
L0_1_set_50 = LBB0_50-L0$pb
L0_1_set_49 = LBB0_49-L0$pb
L0_1_set_48 = LBB0_48-L0$pb
L0_1_set_47 = LBB0_47-L0$pb
L0_1_set_46 = LBB0_46-L0$pb
L0_1_set_45 = LBB0_45-L0$pb
L0_1_set_43 = LBB0_43-L0$pb
L0_1_set_42 = LBB0_42-L0$pb
L0_1_set_41 = LBB0_41-L0$pb
L0_1_set_40 = LBB0_40-L0$pb
LJTI0_1:
	.long	L0_1_set_68
	.long	L0_1_set_53
	.long	L0_1_set_51
	.long	L0_1_set_50
	.long	L0_1_set_49
	.long	L0_1_set_48
	.long	L0_1_set_47
	.long	L0_1_set_46
	.long	L0_1_set_45
	.long	L0_1_set_43
	.long	L0_1_set_42
	.long	L0_1_set_41
	.long	L0_1_set_40
L0_2_set_68 = LBB0_68-L0$pb
L0_2_set_28 = LBB0_28-L0$pb
L0_2_set_27 = LBB0_27-L0$pb
L0_2_set_26 = LBB0_26-L0$pb
L0_2_set_25 = LBB0_25-L0$pb
L0_2_set_24 = LBB0_24-L0$pb
L0_2_set_23 = LBB0_23-L0$pb
L0_2_set_22 = LBB0_22-L0$pb
L0_2_set_21 = LBB0_21-L0$pb
L0_2_set_20 = LBB0_20-L0$pb
L0_2_set_19 = LBB0_19-L0$pb
L0_2_set_18 = LBB0_18-L0$pb
L0_2_set_17 = LBB0_17-L0$pb
LJTI0_2:
	.long	L0_2_set_68
	.long	L0_2_set_28
	.long	L0_2_set_27
	.long	L0_2_set_26
	.long	L0_2_set_25
	.long	L0_2_set_24
	.long	L0_2_set_23
	.long	L0_2_set_22
	.long	L0_2_set_21
	.long	L0_2_set_20
	.long	L0_2_set_19
	.long	L0_2_set_18
	.long	L0_2_set_17
L0_3_set_68 = LBB0_68-L0$pb
L0_3_set_28 = LBB0_28-L0$pb
L0_3_set_14 = LBB0_14-L0$pb
L0_3_set_13 = LBB0_13-L0$pb
L0_3_set_12 = LBB0_12-L0$pb
L0_3_set_11 = LBB0_11-L0$pb
L0_3_set_10 = LBB0_10-L0$pb
L0_3_set_9 = LBB0_9-L0$pb
L0_3_set_21 = LBB0_21-L0$pb
L0_3_set_8 = LBB0_8-L0$pb
L0_3_set_7 = LBB0_7-L0$pb
L0_3_set_6 = LBB0_6-L0$pb
L0_3_set_17 = LBB0_17-L0$pb
LJTI0_3:
	.long	L0_3_set_68
	.long	L0_3_set_28
	.long	L0_3_set_14
	.long	L0_3_set_13
	.long	L0_3_set_12
	.long	L0_3_set_11
	.long	L0_3_set_10
	.long	L0_3_set_9
	.long	L0_3_set_21
	.long	L0_3_set_8
	.long	L0_3_set_7
	.long	L0_3_set_6
	.long	L0_3_set_17
#ifdef __ELF__
	.size	_ob_jenkins_hash, .-_ob_jenkins_hash
#endif

	.globl	_ob_jenkins_hash2
#ifdef __ELF__
	.type	_ob_jenkins_hash2, @function
#endif
	.align	4, 0x90
_ob_jenkins_hash2:                      ## @ob_jenkins_hash2
## BB#0:                                ## %entry
	pushl	%ebp
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	subl	$40, %esp
	calll	L1$pb
L1$pb:
	popl	%eax
	movl	%eax, 4(%esp)           ## 4-byte Spill
	movl	$4097, 16(%esp)         ## imm = 0x1001
	movl	$0, 20(%esp)
	movl	$0, 24(%esp)
	movl	$0, 28(%esp)
	movl	$0, 32(%esp)
	movl	$0, 36(%esp)
	leal	16(%esp), %eax
	xorl	%edx, %edx
	## InlineAsm Start
	roll $3,  %edi ; roll $13, %edi
	roll $29, %edi ; roll $19, %edi
	xchgl %ebx,%ebx
	## InlineAsm End
	movl	%edx, 12(%esp)
	movl	12(%esp), %eax
	movl	%eax, (%esp)            ## 4-byte Spill
	movl	68(%esp), %eax
	movl	(%eax), %eax
	movl	64(%esp), %esi
	leal	-559038737(%esi,%eax), %edi
	movl	72(%esp), %eax
	movl	(%eax), %eax
	addl	%edi, %eax
	movl	60(%esp), %ebx
	testb	$3, %bl
	jne	LBB1_28
## BB#1:                                ## %if.then
	cmpl	$13, %esi
	movl	%esi, 8(%esp)           ## 4-byte Spill
	movl	%edi, %edx
	jb	LBB1_3
	.align	4, 0x90
LBB1_2:                                 ## %while.body
                                        ## =>This Inner Loop Header: Depth=1
	addl	8(%ebx), %eax
	addl	(%ebx), %edi
	subl	%eax, %edi
	addl	4(%ebx), %edx
	movl	%eax, %esi
	roll	$4, %esi
	leal	(%eax,%edx), %ebp
	xorl	%edi, %esi
	subl	%esi, %edx
	movl	%esi, %ecx
	roll	$6, %ecx
	leal	(%esi,%ebp), %edi
	xorl	%edx, %ecx
	subl	%ecx, %ebp
	movl	%ecx, %edx
	roll	$8, %edx
	leal	(%ecx,%edi), %esi
	xorl	%ebp, %edx
	subl	%edx, %edi
	movl	%edx, %ecx
	roll	$16, %ecx
	leal	(%edx,%esi), %ebp
	xorl	%edi, %ecx
	subl	%ecx, %esi
	movl	%ecx, %edx
	roll	$19, %edx
	leal	(%ecx,%ebp), %edi
	xorl	%esi, %edx
	subl	%edx, %ebp
	movl	%edx, %eax
	roll	$4, %eax
	addl	%edi, %edx
	xorl	%ebp, %eax
	addl	$12, %ebx
	movl	8(%esp), %ecx           ## 4-byte Reload
	addl	$-12, %ecx
	movl	%ecx, 8(%esp)           ## 4-byte Spill
	cmpl	$12, %ecx
	ja	LBB1_2
LBB1_3:                                 ## %while.end
	cmpl	$0, (%esp)              ## 4-byte Folded Reload
	movl	4(%esp), %esi           ## 4-byte Reload
	jne	LBB1_14
## BB#4:                                ## %if.then115
	movl	8(%esp), %ecx           ## 4-byte Reload
	cmpl	$12, %ecx
	ja	LBB1_68
## BB#5:                                ## %if.then115
Ltmp4 = LJTI1_3-L1$pb
	addl	Ltmp4(%esi,%ecx,4), %esi
	jmpl	*%esi
LBB1_6:                                 ## %sw.bb149
	addl	(%ebx), %edi
	addl	4(%ebx), %edx
	movzwl	8(%ebx), %ecx
	jmp	LBB1_67
LBB1_7:                                 ## %sw.bb166
	addl	(%ebx), %edi
	addl	4(%ebx), %edx
	movzbl	8(%ebx), %ecx
	jmp	LBB1_67
LBB1_8:                                 ## %sw.bb194
	addl	(%ebx), %edi
	movl	$16777215, %ecx         ## imm = 0xFFFFFF
	andl	4(%ebx), %ecx
	addl	%ecx, %edx
	jmp	LBB1_68
LBB1_9:                                 ## %sw.bb206
	addl	(%ebx), %edi
	movzwl	4(%ebx), %ecx
	addl	%ecx, %edx
	jmp	LBB1_68
LBB1_10:                                ## %sw.bb218
	addl	(%ebx), %edi
	movzbl	4(%ebx), %ecx
	addl	%ecx, %edx
	jmp	LBB1_68
LBB1_11:                                ## %sw.bb230
	addl	(%ebx), %edi
	jmp	LBB1_68
LBB1_12:                                ## %sw.bb236
	movl	$16777215, %ecx         ## imm = 0xFFFFFF
	andl	(%ebx), %ecx
	addl	%ecx, %edi
	jmp	LBB1_68
LBB1_13:                                ## %sw.bb243
	movzwl	(%ebx), %ecx
	addl	%ecx, %edi
	jmp	LBB1_68
LBB1_14:                                ## %if.else
	movl	8(%esp), %ecx           ## 4-byte Reload
	cmpl	$12, %ecx
	ja	LBB1_68
## BB#15:                               ## %if.else
Ltmp5 = LJTI1_2-L1$pb
	addl	Ltmp5(%esi,%ecx,4), %esi
	jmpl	*%esi
LBB1_16:                                ## %sw.bb
	addl	(%ebx), %edi
	addl	4(%ebx), %edx
	addl	8(%ebx), %eax
	jmp	LBB1_68
LBB1_17:                                ## %sw.bb281
	movzbl	10(%ebx), %ecx
	shll	$16, %ecx
	addl	%ecx, %eax
LBB1_18:                                ## %sw.bb288
	movzbl	9(%ebx), %ecx
	shll	$8, %ecx
	addl	%ecx, %eax
LBB1_19:                                ## %sw.bb296
	movzbl	8(%ebx), %ecx
	addl	%ecx, %eax
LBB1_20:                                ## %sw.bb183
	addl	(%ebx), %edi
	addl	4(%ebx), %edx
	jmp	LBB1_68
LBB1_21:                                ## %sw.bb314
	movzbl	6(%ebx), %ecx
	shll	$16, %ecx
	addl	%ecx, %edx
LBB1_22:                                ## %sw.bb322
	movzbl	5(%ebx), %ecx
	shll	$8, %ecx
	addl	%ecx, %edx
LBB1_23:                                ## %sw.bb330
	movzbl	4(%ebx), %ecx
	addl	%ecx, %edx
LBB1_24:                                ## %sw.bb337
	addl	(%ebx), %edi
	jmp	LBB1_68
LBB1_25:                                ## %sw.bb343
	movzbl	2(%ebx), %ecx
	shll	$16, %ecx
	addl	%ecx, %edi
LBB1_26:                                ## %sw.bb351
	movzbl	1(%ebx), %ecx
	shll	$8, %ecx
	addl	%ecx, %edi
LBB1_27:                                ## %sw.bb250
	movzbl	(%ebx), %ecx
	addl	%ecx, %edi
	jmp	LBB1_68
LBB1_28:                                ## %if.else372
	testb	$1, %bl
	je	LBB1_34
## BB#29:                               ## %while.cond697.preheader
	movl	%edi, %ecx
	movl	%edi, %edx
	jmp	LBB1_31
	.align	4, 0x90
LBB1_30:                                ## %while.body701
                                        ##   in Loop: Header=BB1_31 Depth=1
	movl	%esi, 8(%esp)           ## 4-byte Spill
	movzbl	8(%ebx), %edi
	addl	%eax, %edi
	movzbl	9(%ebx), %esi
	shll	$8, %esi
	addl	%edi, %esi
	movzbl	10(%ebx), %eax
	shll	$16, %eax
	addl	%esi, %eax
	movzbl	11(%ebx), %esi
	shll	$24, %esi
	addl	%eax, %esi
	movzbl	(%ebx), %eax
	addl	%ecx, %eax
	movzbl	1(%ebx), %ecx
	shll	$8, %ecx
	addl	%eax, %ecx
	movzbl	2(%ebx), %eax
	shll	$16, %eax
	addl	%ecx, %eax
	movzbl	3(%ebx), %ecx
	shll	$24, %ecx
	addl	%eax, %ecx
	subl	%esi, %ecx
	movzbl	4(%ebx), %eax
	addl	%edx, %eax
	movzbl	5(%ebx), %edx
	shll	$8, %edx
	addl	%eax, %edx
	movzbl	6(%ebx), %eax
	shll	$16, %eax
	addl	%edx, %eax
	movzbl	7(%ebx), %edi
	shll	$24, %edi
	addl	%eax, %edi
	movl	%esi, %ebp
	roll	$4, %ebp
	leal	(%esi,%edi), %edx
	xorl	%ecx, %ebp
	subl	%ebp, %edi
	movl	%ebp, %eax
	roll	$6, %eax
	leal	(%ebp,%edx), %ecx
	xorl	%edi, %eax
	subl	%eax, %edx
	movl	%eax, %edi
	roll	$8, %edi
	leal	(%eax,%ecx), %esi
	xorl	%edx, %edi
	subl	%edi, %ecx
	movl	%edi, %eax
	roll	$16, %eax
	leal	(%edi,%esi), %edi
	xorl	%ecx, %eax
	subl	%eax, %esi
	movl	%eax, %edx
	roll	$19, %edx
	leal	(%eax,%edi), %ecx
	xorl	%esi, %edx
	movl	8(%esp), %esi           ## 4-byte Reload
	subl	%edx, %edi
	movl	%edx, %eax
	roll	$4, %eax
	addl	%ecx, %edx
	xorl	%edi, %eax
	addl	$12, %ebx
	addl	$-12, %esi
LBB1_31:                                ## %while.body701
                                        ## =>This Inner Loop Header: Depth=1
	cmpl	$12, %esi
	ja	LBB1_30
## BB#32:                               ## %while.end865
	cmpl	$12, %esi
	jbe	LBB1_53
## BB#33:
	movl	%ecx, %edi
	jmp	LBB1_68
LBB1_34:                                ## %if.then378
	movl	%edi, %edx
	movl	%edi, %ecx
	movl	%edx, %edi
	movl	%eax, %ebp
	cmpl	$13, %esi
	jb	LBB1_36
	.align	4, 0x90
LBB1_35:                                ## %while.body388
                                        ## =>This Inner Loop Header: Depth=1
	movzwl	8(%ebx), %eax
	movzwl	10(%ebx), %edx
	shll	$16, %edx
	orl	%eax, %edx
	addl	%ebp, %edx
	movzwl	(%ebx), %eax
	movl	%esi, %ebp
	movzwl	2(%ebx), %esi
	shll	$16, %esi
	orl	%eax, %esi
	addl	%edi, %esi
	subl	%edx, %esi
	movzwl	4(%ebx), %eax
	movzwl	6(%ebx), %edi
	shll	$16, %edi
	orl	%eax, %edi
	addl	%ecx, %edi
	movl	%edx, %ecx
	roll	$4, %ecx
	leal	(%edx,%edi), %edx
	xorl	%esi, %ecx
	subl	%ecx, %edi
	movl	%ecx, %eax
	roll	$6, %eax
	leal	(%ecx,%edx), %ecx
	xorl	%edi, %eax
	subl	%eax, %edx
	movl	%eax, %edi
	roll	$8, %edi
	leal	(%eax,%ecx), %esi
	xorl	%edx, %edi
	subl	%edi, %ecx
	movl	%edi, %eax
	roll	$16, %eax
	leal	(%edi,%esi), %edx
	xorl	%ecx, %eax
	subl	%eax, %esi
	movl	%eax, %ecx
	roll	$19, %ecx
	leal	(%eax,%edx), %edi
	xorl	%esi, %ecx
	movl	%ebp, %esi
	subl	%ecx, %edx
	movl	%ecx, %ebp
	roll	$4, %ebp
	addl	%edi, %ecx
	xorl	%edx, %ebp
	addl	$12, %ebx
	addl	$-12, %esi
	cmpl	$12, %esi
	ja	LBB1_35
LBB1_36:                                ## %while.end507
	cmpl	$12, %esi
	jbe	LBB1_38
LBB1_37:
	movl	%ebp, %eax
	movl	%ecx, %edx
	jmp	LBB1_68
LBB1_38:                                ## %while.end507
	movl	%esi, %eax
	movl	%edi, %esi
	movl	4(%esp), %edx           ## 4-byte Reload
Ltmp6 = LJTI1_1-L1$pb
	addl	Ltmp6(%edx,%eax,4), %edx
	jmpl	*%edx
LBB1_39:                                ## %sw.bb510
	movzwl	(%ebx), %eax
	movzwl	2(%ebx), %edi
	shll	$16, %edi
	orl	%eax, %edi
	addl	%esi, %edi
	movzwl	4(%ebx), %eax
	movzwl	6(%ebx), %edx
	shll	$16, %edx
	orl	%eax, %edx
	addl	%ecx, %edx
	movzwl	8(%ebx), %ecx
	movzwl	10(%ebx), %eax
	shll	$16, %eax
	orl	%ecx, %eax
	addl	%ebp, %eax
	jmp	LBB1_68
LBB1_40:                                ## %sw.bb547
	movzbl	10(%ebx), %eax
	shll	$16, %eax
	addl	%eax, %ebp
LBB1_41:                                ## %sw.bb555
	movzwl	8(%ebx), %eax
	addl	%ebp, %eax
	movzwl	(%ebx), %edx
	movzwl	2(%ebx), %edi
	shll	$16, %edi
	orl	%edx, %edi
	addl	%esi, %edi
	movzwl	4(%ebx), %esi
	movzwl	6(%ebx), %edx
	shll	$16, %edx
	orl	%esi, %edx
	addl	%ecx, %edx
	jmp	LBB1_68
LBB1_42:                                ## %sw.bb586
	movzbl	8(%ebx), %eax
	addl	%eax, %ebp
LBB1_43:                                ## %sw.bb593
	movzwl	(%ebx), %eax
	movzwl	2(%ebx), %edi
	shll	$16, %edi
	orl	%eax, %edi
	addl	%esi, %edi
	movzwl	4(%ebx), %eax
	movzwl	6(%ebx), %edx
	shll	$16, %edx
	orl	%eax, %edx
	addl	%ecx, %edx
	movl	%ebp, %eax
	jmp	LBB1_68
LBB1_44:                                ## %sw.bb618
	movzbl	6(%ebx), %eax
	shll	$16, %eax
	addl	%eax, %ecx
LBB1_45:                                ## %sw.bb626
	movzwl	4(%ebx), %edx
	addl	%ecx, %edx
	movzwl	(%ebx), %eax
	movzwl	2(%ebx), %edi
	shll	$16, %edi
	orl	%eax, %edi
	addl	%esi, %edi
	movl	%ebp, %eax
	jmp	LBB1_68
LBB1_46:                                ## %sw.bb645
	movzbl	4(%ebx), %eax
	addl	%eax, %ecx
LBB1_47:                                ## %sw.bb652
	movzwl	(%ebx), %eax
	movzwl	2(%ebx), %edi
	shll	$16, %edi
	orl	%eax, %edi
	addl	%esi, %edi
	jmp	LBB1_37
LBB1_48:                                ## %sw.bb665
	movzbl	2(%ebx), %eax
	shll	$16, %eax
	addl	%eax, %esi
LBB1_49:                                ## %sw.bb673
	movzwl	(%ebx), %edi
	addl	%esi, %edi
	jmp	LBB1_37
LBB1_51:                                ## %sw.bb680
	movzbl	(%ebx), %edi
	addl	%esi, %edi
	jmp	LBB1_37
LBB1_52:                                ## %sw.bb687
	movl	68(%esp), %eax
	movl	%ebp, (%eax)
	movl	72(%esp), %eax
	movl	%ecx, (%eax)
	jmp	LBB1_70
LBB1_53:                                ## %while.end865
	movl	4(%esp), %edi           ## 4-byte Reload
Ltmp7 = LJTI1_0-L1$pb
	addl	Ltmp7(%edi,%esi,4), %edi
	jmpl	*%edi
LBB1_54:                                ## %sw.bb867
	movzbl	11(%ebx), %esi
	shll	$24, %esi
	addl	%esi, %eax
LBB1_55:                                ## %sw.bb875
	movzbl	10(%ebx), %esi
	shll	$16, %esi
	addl	%esi, %eax
LBB1_56:                                ## %sw.bb883
	movzbl	9(%ebx), %esi
	shll	$8, %esi
	addl	%esi, %eax
LBB1_57:                                ## %sw.bb891
	movzbl	8(%ebx), %esi
	addl	%esi, %eax
LBB1_58:                                ## %sw.bb898
	movzbl	7(%ebx), %esi
	shll	$24, %esi
	addl	%esi, %edx
LBB1_59:                                ## %sw.bb906
	movzbl	6(%ebx), %esi
	shll	$16, %esi
	addl	%esi, %edx
LBB1_60:                                ## %sw.bb914
	movzbl	5(%ebx), %esi
	shll	$8, %esi
	addl	%esi, %edx
LBB1_61:                                ## %sw.bb922
	movzbl	4(%ebx), %esi
	addl	%esi, %edx
LBB1_62:                                ## %sw.bb929
	movzbl	3(%ebx), %esi
	shll	$24, %esi
	addl	%esi, %ecx
LBB1_63:                                ## %sw.bb937
	movzbl	2(%ebx), %esi
	shll	$16, %esi
	addl	%esi, %ecx
LBB1_64:                                ## %sw.bb945
	movzbl	1(%ebx), %esi
	shll	$8, %esi
	addl	%esi, %ecx
LBB1_65:                                ## %sw.bb953
	movzbl	(%ebx), %edi
	addl	%ecx, %edi
	jmp	LBB1_68
LBB1_66:                                ## %sw.bb132
	addl	(%ebx), %edi
	addl	4(%ebx), %edx
	movl	$16777215, %ecx         ## imm = 0xFFFFFF
	andl	8(%ebx), %ecx
LBB1_67:                                ## %sw.bb132
	addl	%ecx, %eax
LBB1_68:                                ## %if.end967
	xorl	%edx, %eax
	movl	%edx, %ecx
	roll	$14, %ecx
	subl	%ecx, %eax
	xorl	%eax, %edi
	movl	%eax, %ecx
	roll	$11, %ecx
	subl	%ecx, %edi
	xorl	%edi, %edx
	movl	%edi, %ecx
	roll	$25, %ecx
	subl	%ecx, %edx
	xorl	%edx, %eax
	movl	%edx, %ecx
	roll	$16, %ecx
	subl	%ecx, %eax
	xorl	%eax, %edi
	movl	%eax, %ecx
	roll	$4, %ecx
	subl	%ecx, %edi
	xorl	%edi, %edx
	roll	$14, %edi
	subl	%edi, %edx
	xorl	%edx, %eax
	movl	%edx, %ecx
	roll	$24, %ecx
	subl	%ecx, %eax
LBB1_69:                                ## %if.end967
	movl	68(%esp), %ecx
	movl	%eax, (%ecx)
	movl	72(%esp), %eax
	movl	%edx, (%eax)
LBB1_70:                                ## %return
	addl	$40, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret
	.align	2, 0x90
L1_0_set_69 = LBB1_69-L1$pb
L1_0_set_65 = LBB1_65-L1$pb
L1_0_set_64 = LBB1_64-L1$pb
L1_0_set_63 = LBB1_63-L1$pb
L1_0_set_62 = LBB1_62-L1$pb
L1_0_set_61 = LBB1_61-L1$pb
L1_0_set_60 = LBB1_60-L1$pb
L1_0_set_59 = LBB1_59-L1$pb
L1_0_set_58 = LBB1_58-L1$pb
L1_0_set_57 = LBB1_57-L1$pb
L1_0_set_56 = LBB1_56-L1$pb
L1_0_set_55 = LBB1_55-L1$pb
L1_0_set_54 = LBB1_54-L1$pb
LJTI1_0:
	.long	L1_0_set_69
	.long	L1_0_set_65
	.long	L1_0_set_64
	.long	L1_0_set_63
	.long	L1_0_set_62
	.long	L1_0_set_61
	.long	L1_0_set_60
	.long	L1_0_set_59
	.long	L1_0_set_58
	.long	L1_0_set_57
	.long	L1_0_set_56
	.long	L1_0_set_55
	.long	L1_0_set_54
L1_1_set_52 = LBB1_52-L1$pb
L1_1_set_51 = LBB1_51-L1$pb
L1_1_set_49 = LBB1_49-L1$pb
L1_1_set_48 = LBB1_48-L1$pb
L1_1_set_47 = LBB1_47-L1$pb
L1_1_set_46 = LBB1_46-L1$pb
L1_1_set_45 = LBB1_45-L1$pb
L1_1_set_44 = LBB1_44-L1$pb
L1_1_set_43 = LBB1_43-L1$pb
L1_1_set_42 = LBB1_42-L1$pb
L1_1_set_41 = LBB1_41-L1$pb
L1_1_set_40 = LBB1_40-L1$pb
L1_1_set_39 = LBB1_39-L1$pb
LJTI1_1:
	.long	L1_1_set_52
	.long	L1_1_set_51
	.long	L1_1_set_49
	.long	L1_1_set_48
	.long	L1_1_set_47
	.long	L1_1_set_46
	.long	L1_1_set_45
	.long	L1_1_set_44
	.long	L1_1_set_43
	.long	L1_1_set_42
	.long	L1_1_set_41
	.long	L1_1_set_40
	.long	L1_1_set_39
L1_2_set_69 = LBB1_69-L1$pb
L1_2_set_27 = LBB1_27-L1$pb
L1_2_set_26 = LBB1_26-L1$pb
L1_2_set_25 = LBB1_25-L1$pb
L1_2_set_24 = LBB1_24-L1$pb
L1_2_set_23 = LBB1_23-L1$pb
L1_2_set_22 = LBB1_22-L1$pb
L1_2_set_21 = LBB1_21-L1$pb
L1_2_set_20 = LBB1_20-L1$pb
L1_2_set_19 = LBB1_19-L1$pb
L1_2_set_18 = LBB1_18-L1$pb
L1_2_set_17 = LBB1_17-L1$pb
L1_2_set_16 = LBB1_16-L1$pb
LJTI1_2:
	.long	L1_2_set_69
	.long	L1_2_set_27
	.long	L1_2_set_26
	.long	L1_2_set_25
	.long	L1_2_set_24
	.long	L1_2_set_23
	.long	L1_2_set_22
	.long	L1_2_set_21
	.long	L1_2_set_20
	.long	L1_2_set_19
	.long	L1_2_set_18
	.long	L1_2_set_17
	.long	L1_2_set_16
L1_3_set_69 = LBB1_69-L1$pb
L1_3_set_27 = LBB1_27-L1$pb
L1_3_set_13 = LBB1_13-L1$pb
L1_3_set_12 = LBB1_12-L1$pb
L1_3_set_11 = LBB1_11-L1$pb
L1_3_set_10 = LBB1_10-L1$pb
L1_3_set_9 = LBB1_9-L1$pb
L1_3_set_8 = LBB1_8-L1$pb
L1_3_set_20 = LBB1_20-L1$pb
L1_3_set_7 = LBB1_7-L1$pb
L1_3_set_6 = LBB1_6-L1$pb
L1_3_set_66 = LBB1_66-L1$pb
L1_3_set_16 = LBB1_16-L1$pb
LJTI1_3:
	.long	L1_3_set_69
	.long	L1_3_set_27
	.long	L1_3_set_13
	.long	L1_3_set_12
	.long	L1_3_set_11
	.long	L1_3_set_10
	.long	L1_3_set_9
	.long	L1_3_set_8
	.long	L1_3_set_20
	.long	L1_3_set_7
	.long	L1_3_set_6
	.long	L1_3_set_66
	.long	L1_3_set_16
#ifdef __ELF__
	.size	_ob_jenkins_hash2, .-_ob_jenkins_hash2
#endif

	.globl	_ob_city_hash64
#ifdef __ELF__
	.type	_ob_city_hash64, @function
#endif
	.align	4, 0x90
_ob_city_hash64:                        ## @ob_city_hash64
## BB#0:                                ## %entry
	pushl	%ebp
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	subl	$104, %esp
	movl	128(%esp), %ebx
	movl	124(%esp), %edi
	cmpl	$32, %ebx
	ja	LBB2_10
## BB#1:                                ## %if.then
	cmpl	$16, %ebx
	ja	LBB2_9
## BB#2:                                ## %if.then4
	cmpl	$9, %ebx
	jb	LBB2_4
## BB#3:                                ## %if.then.i
	movl	-8(%ebx,%edi), %esi
	movl	%esi, 92(%esp)          ## 4-byte Spill
	movl	-4(%ebx,%edi), %ebp
	movl	%ebp, 96(%esp)          ## 4-byte Spill
	addl	%ebx, %esi
	adcl	$0, %ebp
	movb	%bl, %cl
	movl	%esi, %edx
	shrdl	%cl, %ebp, %edx
	movb	%bl, %cl
	movl	%ebp, %edi
	shrl	%cl, %edi
	xorl	%eax, %eax
	testb	$32, %bl
	cmovnel	%edi, %edx
	cmovnel	%eax, %edi
	movl	$64, %eax
	subl	%ebx, %eax
	movb	%al, %cl
	shldl	%cl, %esi, %ebp
	movb	%al, %cl
	shll	%cl, %esi
	testb	$32, %al
	cmovnel	%esi, %ebp
	movl	$0, %eax
	cmovnel	%eax, %esi
	orl	%edx, %esi
	movl	124(%esp), %ebx
	movl	(%ebx), %ecx
	xorl	%esi, %ecx
	movl	$-348639895, %edx       ## imm = 0xFFFFFFFFEB382D69
	movl	%ecx, %eax
	mull	%edx
	xorl	%esi, %eax
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	orl	%edi, %ebp
	movl	4(%ebx), %edx
	xorl	%ebp, %edx
	imull	$-348639895, %edx, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %esi
	movl	%esi, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	movl	$-348639895, %edi       ## imm = 0xFFFFFFFFEB382D69
	mull	%edi
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	xorl	%ebp, %esi
	imull	$-348639895, %esi, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %esi
	movl	%esi, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%edi
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	imull	$-348639895, %esi, %edx ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %edx
	xorl	96(%esp), %edx          ## 4-byte Folded Reload
	xorl	92(%esp), %eax          ## 4-byte Folded Reload
	jmp	LBB2_18
LBB2_4:                                 ## %if.end.i
	cmpl	$4, %ebx
	jb	LBB2_6
## BB#5:                                ## %if.then18.i
	movl	(%edi), %esi
	movl	%esi, %ebp
	shrl	$29, %ebp
	shll	$3, %esi
	addl	%ebx, %esi
	adcl	$0, %ebp
	movl	-4(%ebx,%edi), %ebx
	xorl	%ebx, %esi
	movl	$-348639895, %edi       ## imm = 0xFFFFFFFFEB382D69
	movl	%esi, %eax
	mull	%edi
	movl	%eax, %ecx
	imull	$-1646269944, %esi, %eax ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %eax
	imull	$-348639895, %ebp, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%eax, %esi
	movl	%esi, %eax
	shrl	$15, %eax
	xorl	%ebx, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%edi
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	imull	$-348639895, %esi, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %esi
	movl	%esi, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%edi
	jmp	LBB2_16
LBB2_6:                                 ## %if.end35.i
	testl	%ebx, %ebx
	jne	LBB2_8
## BB#7:
	movl	$797982799, %eax        ## imm = 0x2F90404F
	movl	$-1696503237, %edx      ## imm = 0xFFFFFFFF9AE16A3B
	jmp	LBB2_18
LBB2_8:                                 ## %if.then39.i
	movzbl	-1(%ebx,%edi), %eax
	leal	(%ebx,%eax,4), %esi
	movl	$1352557911, %ecx       ## imm = 0x509E6557
	movl	%esi, %eax
	mull	%ecx
	movl	%eax, %ecx
	imull	$-917907513, %esi, %ebp ## imm = 0xFFFFFFFFC949D7C7
	addl	%edx, %ebp
	shrl	%ebx
	movzbl	(%edi,%ebx), %eax
	shll	$8, %eax
	movzbl	(%edi), %edi
	orl	%eax, %edi
	movl	$797982799, %ebx        ## imm = 0x2F90404F
	movl	%edi, %eax
	mull	%ebx
	movl	%eax, %esi
	imull	$-1696503237, %edi, %edi ## imm = 0xFFFFFFFF9AE16A3B
	addl	%edx, %edi
	xorl	%ebp, %edi
	movl	%edi, %eax
	shrl	$15, %eax
	xorl	%ecx, %esi
	xorl	%eax, %esi
	movl	%esi, %eax
	mull	%ebx
	imull	$-1696503237, %esi, %ecx ## imm = 0xFFFFFFFF9AE16A3B
	addl	%edx, %ecx
	imull	$797982799, %edi, %edx  ## imm = 0x2F90404F
	jmp	LBB2_17
LBB2_9:                                 ## %if.else
	movl	-16(%ebx,%edi), %eax
	movl	%eax, 84(%esp)          ## 4-byte Spill
	movl	-8(%ebx,%edi), %esi
	movl	%esi, 72(%esp)          ## 4-byte Spill
	movl	$797982799, %ecx        ## imm = 0x2F90404F
	movl	%esi, %eax
	mull	%ecx
	movl	%eax, 88(%esp)          ## 4-byte Spill
	imull	$-1696503237, %esi, %eax ## imm = 0xFFFFFFFF9AE16A3B
	addl	%edx, %eax
	imull	$797982799, -4(%ebx,%edi), %ecx ## imm = 0x2F90404F
	addl	%eax, %ecx
	movl	(%edi), %esi
	movl	%edi, %eax
	movl	8(%eax), %edx
	movl	%edx, 96(%esp)          ## 4-byte Spill
	movl	%eax, %edi
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	movl	%esi, %eax
	mull	%edx
	movl	%eax, 92(%esp)          ## 4-byte Spill
	imull	$-1265453457, %esi, %eax ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %eax
	imull	$-1097272717, 4(%edi), %ebp ## imm = 0xFFFFFFFFBE98F273
	addl	%eax, %ebp
	movl	92(%esp), %edx          ## 4-byte Reload
	addl	%ebx, %edx
	movl	%ebp, %eax
	adcl	$0, %eax
	subl	88(%esp), %edx          ## 4-byte Folded Reload
	sbbl	%ecx, %eax
	movl	%eax, 80(%esp)          ## 4-byte Spill
	movl	12(%edi), %esi
	movl	96(%esp), %eax          ## 4-byte Reload
	subl	%eax, 92(%esp)          ## 4-byte Folded Spill
	sbbl	%esi, %ebp
	xorl	$1352557911, %eax       ## imm = 0x509E6557
	movl	%eax, 96(%esp)          ## 4-byte Spill
	xorl	$-917907513, %esi       ## imm = 0xFFFFFFFFC949D7C7
	movl	96(%esp), %edi          ## 4-byte Reload
	shldl	$12, %esi, %edi
	movl	%esi, %eax
	movl	96(%esp), %esi          ## 4-byte Reload
	shldl	$12, %esi, %eax
	addl	%edx, %eax
	movl	%eax, 76(%esp)          ## 4-byte Spill
	adcl	80(%esp), %edi          ## 4-byte Folded Reload
	movl	%edi, 96(%esp)          ## 4-byte Spill
	movl	$-1748291289, %edx      ## imm = 0xFFFFFFFF97CB3127
	movl	84(%esp), %edi          ## 4-byte Reload
	movl	%edi, %eax
	mull	%edx
	imull	$-1012545444, %edi, %edi ## imm = 0xFFFFFFFFC3A5C85C
	addl	%edx, %edi
	movl	124(%esp), %edx
	imull	$-1748291289, -12(%ebx,%edx), %edx ## imm = 0xFFFFFFFF97CB3127
	addl	%edi, %edx
	movl	%ebp, %ebx
	movl	92(%esp), %edi          ## 4-byte Reload
	shldl	$21, %edi, %ebx
	shrdl	$11, %edi, %ebp
	addl	%eax, %ebp
	adcl	%edx, %ebx
	shrdl	$30, %ecx, 88(%esp)     ## 4-byte Folded Spill
	xorl	%edx, %edx
	movl	72(%esp), %esi          ## 4-byte Reload
	movl	%esi, %eax
	mull	%edx
	movl	%eax, %edi
	orl	88(%esp), %edi          ## 4-byte Folded Reload
	imull	$-1103036100, %esi, %eax ## imm = 0xFFFFFFFFBE41013C
	addl	%edx, %eax
	shrl	$30, %ecx
	orl	%eax, %ecx
	addl	%ebp, %edi
	adcl	%ebx, %ecx
	movl	96(%esp), %ebx          ## 4-byte Reload
	xorl	%ebx, %ecx
	imull	$-348639895, %ecx, %ebp ## imm = 0xFFFFFFFFEB382D69
	movl	76(%esp), %eax          ## 4-byte Reload
	xorl	%eax, %edi
	movl	%eax, %esi
	movl	$-348639895, %ecx       ## imm = 0xFFFFFFFFEB382D69
	movl	%edi, %eax
	mull	%ecx
	movl	%eax, %ecx
	imull	$-1646269944, %edi, %edi ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %edi
	addl	%ebp, %edi
	movl	%edi, %eax
	shrl	$15, %eax
	xorl	%esi, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	movl	$-348639895, %edx       ## imm = 0xFFFFFFFFEB382D69
	mull	%edx
	movl	$-348639895, %ebp       ## imm = 0xFFFFFFFFEB382D69
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	xorl	%ebx, %edi
	imull	$-348639895, %edi, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %esi
	jmp	LBB2_15
LBB2_10:                                ## %if.else10
	cmpl	$64, %ebx
	ja	LBB2_12
## BB#11:                               ## %if.then13
	movl	-12(%ebx,%edi), %esi
	movl	%esi, 68(%esp)          ## 4-byte Spill
	movl	-16(%ebx,%edi), %ecx
	movl	%ecx, 76(%esp)          ## 4-byte Spill
	addl	%ebx, %ecx
	adcl	$0, %esi
	movl	$-1748291289, %edx      ## imm = 0xFFFFFFFF97CB3127
	movl	%ecx, %eax
	mull	%edx
	movl	%edi, %ebp
	imull	$-1012545444, %ecx, %edi ## imm = 0xFFFFFFFFC3A5C85C
	addl	%edx, %edi
	imull	$-1748291289, %esi, %ecx ## imm = 0xFFFFFFFF97CB3127
	addl	%edi, %ecx
	addl	(%ebp), %eax
	adcl	4(%ebp), %ecx
	movl	%ecx, %edx
	shldl	$27, %eax, %edx
	movl	%edx, 88(%esp)          ## 4-byte Spill
	movl	8(%ebp), %esi
	addl	%eax, %esi
	movl	%esi, 92(%esp)          ## 4-byte Spill
	movl	12(%ebp), %edi
	adcl	%ecx, %edi
	movl	%edi, 96(%esp)          ## 4-byte Spill
	movl	%esi, %edx
	shldl	$25, %edi, %edx
	movl	%eax, %ebp
	shldl	$27, %ecx, %ebp
	shldl	$25, %esi, %edi
	addl	%ebp, %edi
	adcl	88(%esp), %edx          ## 4-byte Folded Reload
	movl	124(%esp), %ebp
	movl	28(%ebp), %esi
	movl	%esi, 88(%esp)          ## 4-byte Spill
	movl	24(%ebp), %esi
	movl	%esi, 48(%esp)          ## 4-byte Spill
	addl	%esi, %eax
	adcl	88(%esp), %ecx          ## 4-byte Folded Reload
	movl	%ecx, %esi
	shldl	$12, %eax, %esi
	shrdl	$20, %eax, %ecx
	addl	%edi, %ecx
	adcl	%edx, %esi
	movl	%ebp, %eax
	movl	16(%eax), %ebp
	movl	20(%eax), %eax
	movl	%eax, 72(%esp)          ## 4-byte Spill
	movl	92(%esp), %edi          ## 4-byte Reload
	addl	%ebp, %edi
	movl	%edi, 92(%esp)          ## 4-byte Spill
	movl	96(%esp), %edx          ## 4-byte Reload
	adcl	%eax, %edx
	movl	%edx, 96(%esp)          ## 4-byte Spill
	movl	%edi, %eax
	shldl	$1, %edx, %eax
	shldl	$1, %edi, %edx
	addl	%ecx, %edx
	movl	%edx, 84(%esp)          ## 4-byte Spill
	adcl	%esi, %eax
	movl	%eax, 80(%esp)          ## 4-byte Spill
	movl	124(%esp), %eax
	movl	-28(%ebx,%eax), %ecx
	movl	-20(%ebx,%eax), %esi
	addl	-32(%ebx,%eax), %ebp
	movl	%eax, %edx
	adcl	72(%esp), %ecx          ## 4-byte Folded Reload
	movl	%ecx, %eax
	shldl	$27, %ebp, %eax
	movl	%eax, 64(%esp)          ## 4-byte Spill
	movl	%ebp, %eax
	shldl	$27, %ecx, %eax
	movl	%eax, 72(%esp)          ## 4-byte Spill
	movl	-24(%ebx,%edx), %edi
	addl	%ebp, %edi
	adcl	%ecx, %esi
	movl	%edi, %eax
	shldl	$25, %esi, %eax
	movl	%eax, 52(%esp)          ## 4-byte Spill
	movl	%esi, %eax
	shldl	$25, %edi, %eax
	movl	%eax, 56(%esp)          ## 4-byte Spill
	addl	76(%esp), %edi          ## 4-byte Folded Reload
	adcl	68(%esp), %esi          ## 4-byte Folded Reload
	movl	%edi, %eax
	shldl	$1, %esi, %eax
	movl	%eax, 60(%esp)          ## 4-byte Spill
	movl	%esi, %eax
	shldl	$1, %edi, %eax
	movl	%eax, 68(%esp)          ## 4-byte Spill
	movl	-4(%ebx,%edx), %eax
	movl	%eax, 76(%esp)          ## 4-byte Spill
	movl	-8(%ebx,%edx), %ebx
	addl	%ebx, %edi
	adcl	%eax, %esi
	addl	84(%esp), %edi          ## 4-byte Folded Reload
	adcl	80(%esp), %esi          ## 4-byte Folded Reload
	movl	%edi, %eax
	movl	$-1748291289, %edx      ## imm = 0xFFFFFFFF97CB3127
	mull	%edx
	movl	%eax, 100(%esp)         ## 4-byte Spill
	imull	$-1012545444, %edi, %eax ## imm = 0xFFFFFFFFC3A5C85C
	addl	%edx, %eax
	imull	$-1748291289, %esi, %esi ## imm = 0xFFFFFFFF97CB3127
	addl	%eax, %esi
	movl	92(%esp), %edx          ## 4-byte Reload
	addl	48(%esp), %edx          ## 4-byte Folded Reload
	movl	96(%esp), %eax          ## 4-byte Reload
	adcl	88(%esp), %eax          ## 4-byte Folded Reload
	addl	72(%esp), %edx          ## 4-byte Folded Reload
	adcl	64(%esp), %eax          ## 4-byte Folded Reload
	addl	%ebx, %ebp
	adcl	76(%esp), %ecx          ## 4-byte Folded Reload
	movl	%ecx, %ebx
	shldl	$12, %ebp, %ebx
	shrdl	$20, %ebp, %ecx
	addl	%edx, %ecx
	adcl	%eax, %ebx
	addl	56(%esp), %ecx          ## 4-byte Folded Reload
	adcl	52(%esp), %ebx          ## 4-byte Folded Reload
	addl	68(%esp), %ecx          ## 4-byte Folded Reload
	adcl	60(%esp), %ebx          ## 4-byte Folded Reload
	movl	$797982799, %edi        ## imm = 0x2F90404F
	movl	%ecx, %eax
	mull	%edi
	imull	$-1696503237, %ecx, %ecx ## imm = 0xFFFFFFFF9AE16A3B
	addl	%edx, %ecx
	imull	$797982799, %ebx, %ebx  ## imm = 0x2F90404F
	addl	%ecx, %ebx
	addl	100(%esp), %eax         ## 4-byte Folded Reload
	adcl	%esi, %ebx
	movl	%ebx, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	movl	$-1748291289, %edx      ## imm = 0xFFFFFFFF97CB3127
	mull	%edx
	imull	$-1012545444, %ecx, %ecx ## imm = 0xFFFFFFFFC3A5C85C
	addl	%edx, %ecx
	imull	$-1748291289, %ebx, %esi ## imm = 0xFFFFFFFF97CB3127
	addl	%ecx, %esi
	addl	84(%esp), %eax          ## 4-byte Folded Reload
	adcl	80(%esp), %esi          ## 4-byte Folded Reload
	movl	%esi, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%edi
	imull	$-1696503237, %ecx, %ecx ## imm = 0xFFFFFFFF9AE16A3B
	addl	%edx, %ecx
	imull	$797982799, %esi, %edx  ## imm = 0x2F90404F
	jmp	LBB2_17
LBB2_12:                                ## %Rotate.exit207
	movl	-16(%ebx,%edi), %eax
	movl	%eax, 92(%esp)          ## 4-byte Spill
	movl	-12(%ebx,%edi), %ecx
	movl	%ecx, 84(%esp)          ## 4-byte Spill
	movl	-52(%ebx,%edi), %ebp
	movl	%ebp, 44(%esp)          ## 4-byte Spill
	movl	-60(%ebx,%edi), %edx
	movl	-56(%ebx,%edi), %esi
	movl	%esi, 96(%esp)          ## 4-byte Spill
	movl	-64(%ebx,%edi), %eax
	addl	%ebx, %eax
	adcl	$0, %edx
	addl	%eax, %esi
	adcl	%edx, %ebp
	addl	-48(%ebx,%edi), %esi
	movl	%esi, 76(%esp)          ## 4-byte Spill
	adcl	-44(%ebx,%edi), %ebp
	movl	%ebp, 80(%esp)          ## 4-byte Spill
	movl	%edi, %ebx
	movl	%ebx, 100(%esp)         ## 4-byte Spill
	movl	%ebp, %edi
	shldl	$20, %esi, %edi
	shldl	$20, %ebp, %esi
	addl	%eax, %esi
	adcl	%edx, %edi
	movl	%edi, 68(%esp)          ## 4-byte Spill
	xorl	$-1265453457, %ecx      ## imm = 0xFFFFFFFFB492B66F
	movl	%ecx, 40(%esp)          ## 4-byte Spill
	movl	92(%esp), %edi          ## 4-byte Reload
	xorl	$-1097272717, %edi      ## imm = 0xFFFFFFFFBE98F273
	movl	%edi, 36(%esp)          ## 4-byte Spill
	addl	%edi, %eax
	adcl	%ecx, %edx
	movl	128(%esp), %edi
	movl	-36(%edi,%ebx), %ebp
	movl	%ebp, 72(%esp)          ## 4-byte Spill
	movl	-40(%edi,%ebx), %ecx
	movl	%ecx, 88(%esp)          ## 4-byte Spill
	addl	%ecx, %eax
	adcl	%ebp, %edx
	movl	%eax, %ecx
	shldl	$11, %edx, %ecx
	shldl	$11, %eax, %edx
	addl	%esi, %edx
	movl	%edx, 64(%esp)          ## 4-byte Spill
	adcl	68(%esp), %ecx          ## 4-byte Folded Reload
	movl	%ecx, 68(%esp)          ## 4-byte Spill
	movl	%ecx, %esi
	shrl	$15, %esi
	xorl	%edx, %esi
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	movl	%esi, %eax
	mull	%edx
	imull	$-1265453457, %esi, %esi ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %esi
	imull	$-1097272717, %ecx, %ecx ## imm = 0xFFFFFFFFBE98F273
	addl	%esi, %ecx
	movl	44(%esp), %edx          ## 4-byte Reload
	xorl	$-1012545444, %edx      ## imm = 0xFFFFFFFFC3A5C85C
	movl	96(%esp), %esi          ## 4-byte Reload
	xorl	$-1748291289, %esi      ## imm = 0xFFFFFFFF97CB3127
	addl	%eax, %esi
	movl	%esi, 96(%esp)          ## 4-byte Spill
	adcl	%ecx, %edx
	movl	%edx, 44(%esp)          ## 4-byte Spill
	movl	(%ebx), %ecx
	addl	%esi, %ecx
	movl	4(%ebx), %ebp
	adcl	%edx, %ebp
	movl	%ecx, %esi
	shldl	$25, %ebp, %esi
	movl	%esi, %eax
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	mull	%edx
	movl	%eax, 20(%esp)          ## 4-byte Spill
	imull	$-1265453457, %esi, %eax ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %eax
	shldl	$25, %ecx, %ebp
	imull	$-1097272717, %ebp, %ecx ## imm = 0xFFFFFFFFBE98F273
	addl	%eax, %ecx
	movl	%ecx, 28(%esp)          ## 4-byte Spill
	movl	%edi, %ebp
	movl	-28(%ebp,%ebx), %esi
	movl	%ebp, %eax
	movl	$-1097272717, %ecx      ## imm = 0xFFFFFFFFBE98F273
	mull	%ecx
	movl	%eax, %edi
	imull	$-1265453457, %ebp, %ecx ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %ecx
	movl	%ebx, %eax
	addl	-32(%ebp,%eax), %edi
	adcl	%esi, %ecx
	movl	-20(%ebp,%eax), %edx
	movl	-24(%ebp,%eax), %ebx
	movl	%eax, %esi
	addl	%edi, %ebx
	adcl	%ecx, %edx
	addl	92(%esp), %ebx          ## 4-byte Folded Reload
	adcl	84(%esp), %edx          ## 4-byte Folded Reload
	movl	%edx, 84(%esp)          ## 4-byte Spill
	movl	%edx, %eax
	shldl	$20, %ebx, %eax
	movl	%ebx, %ebp
	shldl	$20, %edx, %ebp
	addl	%edi, %ebp
	adcl	%ecx, %eax
	movl	%eax, 56(%esp)          ## 4-byte Spill
	movl	128(%esp), %edx
	movl	-4(%edx,%esi), %eax
	movl	%eax, 92(%esp)          ## 4-byte Spill
	movl	-8(%edx,%esi), %eax
	movl	%eax, 60(%esp)          ## 4-byte Spill
	addl	%eax, %edi
	adcl	92(%esp), %ecx          ## 4-byte Folded Reload
	addl	$-1748291289, %edi      ## imm = 0xFFFFFFFF97CB3127
	adcl	$-1012545444, %ecx      ## imm = 0xFFFFFFFFC3A5C85C
	movl	%edi, %eax
	shldl	$11, %ecx, %eax
	shldl	$11, %edi, %ecx
	addl	%ebp, %ecx
	movl	%ecx, 48(%esp)          ## 4-byte Spill
	adcl	56(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 56(%esp)          ## 4-byte Spill
	movl	76(%esp), %eax          ## 4-byte Reload
	addl	88(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 76(%esp)          ## 4-byte Spill
	movl	80(%esp), %eax          ## 4-byte Reload
	adcl	72(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 80(%esp)          ## 4-byte Spill
	addl	60(%esp), %ebx          ## 4-byte Folded Reload
	movl	%ebx, 52(%esp)          ## 4-byte Spill
	movl	84(%esp), %eax          ## 4-byte Reload
	adcl	92(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 84(%esp)          ## 4-byte Spill
	movl	36(%esp), %edi          ## 4-byte Reload
	movl	%edi, %esi
	movl	40(%esp), %ecx          ## 4-byte Reload
	shldl	$31, %ecx, %esi
	movl	%esi, %eax
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	mull	%edx
	movl	%eax, 60(%esp)          ## 4-byte Spill
	imull	$-1265453457, %esi, %eax ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %eax
	movl	%edi, %edx
	shrdl	$1, %ecx, %edx
	imull	$-1097272717, %edx, %edi ## imm = 0xFFFFFFFFBE98F273
	addl	%eax, %edi
	movl	128(%esp), %eax
	addl	$-65, %eax
	andl	$-64, %eax
	addl	$64, %eax
	addl	$56, 100(%esp)          ## 4-byte Folded Spill
	movl	%eax, %edx
	.align	4, 0x90
LBB2_13:                                ## %Rotate.exit
                                        ## =>This Inner Loop Header: Depth=1
	movl	%edx, 4(%esp)           ## 4-byte Spill
	movl	96(%esp), %edx          ## 4-byte Reload
	xorl	52(%esp), %edx          ## 4-byte Folded Reload
	movl	44(%esp), %eax          ## 4-byte Reload
	xorl	84(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, %ecx
	shldl	$31, %edx, %ecx
	movl	%ecx, 32(%esp)          ## 4-byte Spill
	movl	%eax, %ebx
	shrdl	$1, %edx, %ebx
	movl	%ebx, 36(%esp)          ## 4-byte Spill
	addl	48(%esp), %ebx          ## 4-byte Folded Reload
	adcl	56(%esp), %ecx          ## 4-byte Folded Reload
	movl	100(%esp), %edx         ## 4-byte Reload
	addl	-24(%edx), %ebx
	adcl	-20(%edx), %ecx
	movl	-12(%edx), %ebp
	movl	-16(%edx), %eax
	addl	%ebx, %eax
	adcl	%ecx, %ebp
	movl	-4(%edx), %esi
	movl	%esi, 96(%esp)          ## 4-byte Spill
	movl	-8(%edx), %esi
	movl	%esi, 92(%esp)          ## 4-byte Spill
	movl	92(%esp), %edx          ## 4-byte Reload
	addl	%edx, %eax
	movl	%eax, 88(%esp)          ## 4-byte Spill
	adcl	96(%esp), %ebp          ## 4-byte Folded Reload
	movl	%ebp, 72(%esp)          ## 4-byte Spill
	movl	%ebp, %esi
	shldl	$20, %eax, %esi
	shldl	$20, %ebp, %eax
	addl	%ebx, %eax
	movl	%eax, 44(%esp)          ## 4-byte Spill
	adcl	%ecx, %esi
	movl	%esi, 40(%esp)          ## 4-byte Spill
	movl	60(%esp), %eax          ## 4-byte Reload
	addl	64(%esp), %eax          ## 4-byte Folded Reload
	movl	%edi, %esi
	adcl	68(%esp), %esi          ## 4-byte Folded Reload
	addl	%edx, %eax
	adcl	96(%esp), %esi          ## 4-byte Folded Reload
	movl	%esi, %ebp
	shldl	$22, %eax, %ebp
	shrdl	$10, %eax, %esi
	movl	%esi, %eax
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	mull	%edx
	movl	%eax, 92(%esp)          ## 4-byte Spill
	imull	$-1265453457, %esi, %eax ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %eax
	imull	$-1097272717, %ebp, %edx ## imm = 0xFFFFFFFFBE98F273
	addl	%eax, %edx
	xorl	80(%esp), %edx          ## 4-byte Folded Reload
	movl	%edx, 24(%esp)          ## 4-byte Spill
	movl	92(%esp), %eax          ## 4-byte Reload
	xorl	76(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 92(%esp)          ## 4-byte Spill
	addl	%eax, %ebx
	adcl	%edx, %ecx
	movl	100(%esp), %ebp         ## 4-byte Reload
	movl	4(%ebp), %edx
	movl	%edx, 8(%esp)           ## 4-byte Spill
	movl	(%ebp), %eax
	movl	%eax, 12(%esp)          ## 4-byte Spill
	addl	%eax, %ebx
	adcl	%edx, %ecx
	movl	%ebx, %eax
	shldl	$11, %ecx, %eax
	shldl	$11, %ebx, %ecx
	addl	44(%esp), %ecx          ## 4-byte Folded Reload
	adcl	40(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 40(%esp)          ## 4-byte Spill
	movl	64(%esp), %esi          ## 4-byte Reload
	movl	%esi, %eax
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	mull	%edx
	imull	$-1265453457, %esi, %esi ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %esi
	imull	$-1097272717, 68(%esp), %edx ## 4-byte Folded Reload
                                        ## imm = 0xFFFFFFFFBE98F273
	addl	%esi, %edx
	movl	-52(%ebp), %esi
	movl	-40(%ebp), %ebx
	movl	%ebx, 96(%esp)          ## 4-byte Spill
	addl	-56(%ebp), %eax
	movl	%eax, 68(%esp)          ## 4-byte Spill
	adcl	%edx, %esi
	movl	-36(%ebp), %edx
	movl	%edx, 44(%esp)          ## 4-byte Spill
	addl	%ebx, %eax
	movl	%esi, %ebp
	adcl	%edx, %ebp
	movl	100(%esp), %edx         ## 4-byte Reload
	addl	-48(%edx), %eax
	movl	%eax, (%esp)            ## 4-byte Spill
	adcl	-44(%edx), %ebp
	movl	%ebp, %ebx
	shldl	$20, %eax, %ebx
	movl	%eax, %edx
	movl	%ebx, %eax
	movl	%edx, %ebx
	shldl	$20, %ebp, %ebx
	addl	68(%esp), %ebx          ## 4-byte Folded Reload
	movl	%ebx, 64(%esp)          ## 4-byte Spill
	adcl	%esi, %eax
	movl	%eax, 16(%esp)          ## 4-byte Spill
	movl	60(%esp), %eax          ## 4-byte Reload
	addl	76(%esp), %eax          ## 4-byte Folded Reload
	adcl	80(%esp), %edi          ## 4-byte Folded Reload
	addl	20(%esp), %eax          ## 4-byte Folded Reload
	adcl	28(%esp), %edi          ## 4-byte Folded Reload
	addl	96(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, %ebx
	adcl	44(%esp), %edi          ## 4-byte Folded Reload
	movl	%edi, %eax
	shldl	$27, %ebx, %eax
	movl	%eax, 80(%esp)          ## 4-byte Spill
	shrdl	$5, %ebx, %edi
	movl	%edi, %eax
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	mull	%edx
	movl	%eax, 96(%esp)          ## 4-byte Spill
	imull	$-1265453457, %edi, %eax ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %eax
	imull	$-1097272717, 80(%esp), %ebx ## 4-byte Folded Reload
                                        ## imm = 0xFFFFFFFFBE98F273
	addl	%eax, %ebx
	xorl	56(%esp), %ebx          ## 4-byte Folded Reload
	movl	%ebx, 44(%esp)          ## 4-byte Spill
	movl	68(%esp), %edi          ## 4-byte Reload
	addl	52(%esp), %edi          ## 4-byte Folded Reload
	adcl	84(%esp), %esi          ## 4-byte Folded Reload
	movl	100(%esp), %edx         ## 4-byte Reload
	movl	-28(%edx), %eax
	movl	%eax, 84(%esp)          ## 4-byte Spill
	movl	-32(%edx), %eax
	movl	%eax, 80(%esp)          ## 4-byte Spill
	movl	80(%esp), %edx          ## 4-byte Reload
	addl	%edx, %edi
	adcl	84(%esp), %esi          ## 4-byte Folded Reload
	movl	96(%esp), %eax          ## 4-byte Reload
	xorl	48(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 96(%esp)          ## 4-byte Spill
	addl	%eax, %edi
	adcl	%ebx, %esi
	movl	%edi, %ebx
	shldl	$11, %esi, %ebx
	shldl	$11, %edi, %esi
	addl	64(%esp), %esi          ## 4-byte Folded Reload
	movl	%esi, 64(%esp)          ## 4-byte Spill
	movl	(%esp), %esi            ## 4-byte Reload
	adcl	16(%esp), %ebx          ## 4-byte Folded Reload
	movl	%ebx, 68(%esp)          ## 4-byte Spill
	movl	88(%esp), %edi          ## 4-byte Reload
	addl	12(%esp), %edi          ## 4-byte Folded Reload
	movl	%edi, 88(%esp)          ## 4-byte Spill
	movl	72(%esp), %edi          ## 4-byte Reload
	adcl	8(%esp), %edi           ## 4-byte Folded Reload
	movl	%edi, 72(%esp)          ## 4-byte Spill
	addl	%edx, %esi
	movl	4(%esp), %edx           ## 4-byte Reload
	adcl	84(%esp), %ebp          ## 4-byte Folded Reload
	addl	$64, 100(%esp)          ## 4-byte Folded Spill
	addl	$-64, %edx
	movl	%ecx, 48(%esp)          ## 4-byte Spill
	movl	40(%esp), %eax          ## 4-byte Reload
	movl	%eax, 56(%esp)          ## 4-byte Spill
	movl	%eax, %ebx
	movl	88(%esp), %eax          ## 4-byte Reload
	movl	%eax, 52(%esp)          ## 4-byte Spill
	movl	%edi, 84(%esp)          ## 4-byte Spill
	movl	%esi, 76(%esp)          ## 4-byte Spill
	movl	%ebp, 80(%esp)          ## 4-byte Spill
	movl	92(%esp), %eax          ## 4-byte Reload
	movl	%eax, 60(%esp)          ## 4-byte Spill
	movl	24(%esp), %edi          ## 4-byte Reload
	movl	36(%esp), %eax          ## 4-byte Reload
	movl	%eax, 20(%esp)          ## 4-byte Spill
	movl	32(%esp), %eax          ## 4-byte Reload
	movl	%eax, 28(%esp)          ## 4-byte Spill
	jne	LBB2_13
## BB#14:                               ## %do.end
	movl	64(%esp), %edi          ## 4-byte Reload
	xorl	%ecx, %edi
	movl	$-348639895, %edx       ## imm = 0xFFFFFFFFEB382D69
	movl	%edi, %eax
	mull	%edx
	movl	%eax, 100(%esp)         ## 4-byte Spill
	imull	$-1646269944, %edi, %eax ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %eax
	movl	68(%esp), %edx          ## 4-byte Reload
	xorl	%ebx, %edx
	imull	$-348639895, %edx, %edi ## imm = 0xFFFFFFFFEB382D69
	addl	%eax, %edi
	movl	%edi, %eax
	shrl	$15, %eax
	movl	100(%esp), %edx         ## 4-byte Reload
	xorl	%ecx, %edx
	xorl	%eax, %edx
	movl	%edx, %eax
	movl	%edx, %ecx
	movl	$-348639895, %edx       ## imm = 0xFFFFFFFFEB382D69
	mull	%edx
	imull	$-1646269944, %ecx, %ebx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ebx
	xorl	40(%esp), %edi          ## 4-byte Folded Reload
	imull	$-348639895, %edi, %ecx ## imm = 0xFFFFFFFFEB382D69
	addl	%ebx, %ecx
	movl	%ecx, %edi
	shrl	$15, %edi
	xorl	%eax, %edi
	movl	%edi, %eax
	movl	$-348639895, %ebx       ## imm = 0xFFFFFFFFEB382D69
	mull	%ebx
	imull	$-1646269944, %edi, %edi ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %edi
	imull	$-348639895, %ecx, %ecx ## imm = 0xFFFFFFFFEB382D69
	addl	%edi, %ecx
	addl	36(%esp), %eax          ## 4-byte Folded Reload
	movl	%eax, 100(%esp)         ## 4-byte Spill
	adcl	32(%esp), %ecx          ## 4-byte Folded Reload
	movl	%ecx, 84(%esp)          ## 4-byte Spill
	movl	88(%esp), %ecx          ## 4-byte Reload
	xorl	%ecx, %esi
	movl	%esi, %eax
	mull	%ebx
	movl	%eax, %ebx
	imull	$-1646269944, %esi, %eax ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %eax
	xorl	72(%esp), %ebp          ## 4-byte Folded Reload
	imull	$-348639895, %ebp, %edi ## imm = 0xFFFFFFFFEB382D69
	addl	%eax, %edi
	movl	%edi, %eax
	shrl	$15, %eax
	xorl	%ecx, %ebx
	xorl	%eax, %ebx
	movl	%ebx, %eax
	movl	$-348639895, %ebp       ## imm = 0xFFFFFFFFEB382D69
	mull	%ebp
	imull	$-1646269944, %ebx, %esi ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %esi
	xorl	72(%esp), %edi          ## 4-byte Folded Reload
	imull	$-348639895, %edi, %ebx ## imm = 0xFFFFFFFFEB382D69
	addl	%esi, %ebx
	movl	%ebx, %edi
	shrl	$15, %edi
	xorl	%eax, %edi
	movl	%edi, %eax
	mull	%ebp
	movl	%eax, %esi
	imull	$-1646269944, %edi, %eax ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %eax
	imull	$-348639895, %ebx, %ebp ## imm = 0xFFFFFFFFEB382D69
	addl	%eax, %ebp
	movl	24(%esp), %ecx          ## 4-byte Reload
	movl	%ecx, %ebx
	shrl	$15, %ebx
	xorl	92(%esp), %ebx          ## 4-byte Folded Reload
	movl	$-1097272717, %edx      ## imm = 0xFFFFFFFFBE98F273
	movl	%ebx, %eax
	mull	%edx
	movl	%eax, %edi
	imull	$-1265453457, %ebx, %eax ## imm = 0xFFFFFFFFB492B66F
	addl	%edx, %eax
	imull	$-1097272717, %ecx, %ebx ## imm = 0xFFFFFFFFBE98F273
	addl	%eax, %ebx
	addl	96(%esp), %edi          ## 4-byte Folded Reload
	adcl	44(%esp), %ebx          ## 4-byte Folded Reload
	addl	%esi, %edi
	adcl	%ebp, %ebx
	movl	100(%esp), %ebp         ## 4-byte Reload
	xorl	%ebp, %edi
	movl	%edi, %eax
	movl	$-348639895, %edx       ## imm = 0xFFFFFFFFEB382D69
	mull	%edx
	movl	%eax, %esi
	imull	$-1646269944, %edi, %eax ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %eax
	movl	84(%esp), %ecx          ## 4-byte Reload
	xorl	%ecx, %ebx
	imull	$-348639895, %ebx, %edi ## imm = 0xFFFFFFFFEB382D69
	addl	%eax, %edi
	movl	%edi, %eax
	shrl	$15, %eax
	xorl	%ebp, %esi
	xorl	%eax, %esi
	movl	%esi, %eax
	movl	$-348639895, %ebp       ## imm = 0xFFFFFFFFEB382D69
	mull	%ebp
	imull	$-1646269944, %esi, %ebx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ebx
	xorl	%ecx, %edi
	imull	$-348639895, %edi, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%ebx, %esi
LBB2_15:                                ## %do.end
	movl	%esi, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%ebp
LBB2_16:                                ## %do.end
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	imull	$-348639895, %esi, %edx ## imm = 0xFFFFFFFFEB382D69
LBB2_17:                                ## %do.end
	addl	%ecx, %edx
LBB2_18:                                ## %return
	addl	$104, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret
#ifdef __ELF__
	.size	_ob_city_hash64, .-_ob_city_hash64
#endif

	.globl	_ob_city_hash64_with_seeds
#ifdef __ELF__
	.type	_ob_city_hash64_with_seeds, @function
#endif
	.align	4, 0x90
_ob_city_hash64_with_seeds:             ## @ob_city_hash64_with_seeds
## BB#0:                                ## %entry
	pushl	%ebp
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	subl	$12, %esp
	movl	36(%esp), %eax
	movl	%eax, 4(%esp)
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	calll	_ob_city_hash64
	movl	%eax, %edi
	movl	%edx, %esi
	subl	40(%esp), %edi
	sbbl	44(%esp), %esi
	movl	48(%esp), %ebp
	xorl	%ebp, %edi
	movl	$-348639895, %ebx       ## imm = 0xFFFFFFFFEB382D69
	movl	%edi, %eax
	mull	%ebx
	movl	%eax, %ecx
	imull	$-1646269944, %edi, %eax ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %eax
	movl	52(%esp), %edi
	xorl	%edi, %esi
	imull	$-348639895, %esi, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%eax, %esi
	movl	%esi, %eax
	shrl	$15, %eax
	xorl	%ebp, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%ebx
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	xorl	%edi, %esi
	imull	$-348639895, %esi, %esi ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %esi
	movl	%esi, %ecx
	shrl	$15, %ecx
	xorl	%eax, %ecx
	movl	%ecx, %eax
	mull	%ebx
	imull	$-1646269944, %ecx, %ecx ## imm = 0xFFFFFFFF9DDFEA08
	addl	%edx, %ecx
	imull	$-348639895, %esi, %edx ## imm = 0xFFFFFFFFEB382D69
	addl	%ecx, %edx
	addl	$12, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret
#ifdef __ELF__
	.size	_ob_city_hash64_with_seeds, .-_ob_city_hash64_with_seeds
#else
.subsections_via_symbols
#endif
/* For security, make sure the stack is not executable:
 * http://www.gentoo.org/proj/en/hardened/gnu-stack.xml#doc_chap6 */
#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
