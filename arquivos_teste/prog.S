.data

.text
.globl f1
	f1:
		pushq %rbp
		movq %rsp, %rbp
		subq $16, %rsp



		movl $0, %eax
		leave
		ret
.globl f2
	f2:
		pushq %rbp
		movq %rsp, %rbp
		subq $128, %rsp

		#vi1: -4(%rbp)
		#va2: -124(%rbp)
		#vi3: -128(%rbp)


		movl $-1, %eax
		leave
		ret
.globl f3
	f3:
		pushq %rbp
		movq %rsp, %rbp
		subq $128, %rsp

		#va1: -40(%rbp)
		#va2: -120(%rbp)
		#vi3: -124(%rbp)


		movl $5, %eax
		leave
		ret
.globl f4
	f4:
		pushq %rbp
		movq %rsp, %rbp
		subq $16, %rsp

		#vi1: -4(%rbp)
		#vi2: -8(%rbp)

		movl $1, -4(%rbp)

		leaq -4(%rbp), %r9
		movl (%r9), %r8d
		movl %r8d, -8(%rbp)

		movl %edi, %r9d
		leaq -8(%rbp), %r8
		movl (%r8), %r10d
		addl %r9d, %r10d
		movl %r10d, -4(%rbp)

		leaq -4(%rbp), %r8
		movl (%r8), %r9d
		movl $-5, %r10d
		imull %r9d, %r10d
		movl %r10d, -8(%rbp)


		movl -4(%rbp), %eax
		leave
		ret
.globl f5
	f5:
		pushq %rbp
		movq %rsp, %rbp
		subq $16, %rsp



		movl %edi, %eax
		leave
		ret
.globl f6
	f6:
		pushq %rbp
		movq %rsp, %rbp
		subq $128, %rsp

		#vi1: -4(%rbp)
		#vi2: -8(%rbp)
		#va3: -128(%rbp)

		movl $1, -4(%rbp)


		movl -8(%rbp), %eax
		leave
		ret
.globl f7
	f7:
		pushq %rbp
		movq %rsp, %rbp
		subq $48, %rsp

		#vi1: -4(%rbp)
		#va2: -44(%rbp)

		movl %edi, %r9d
		movl $1, %r10d
		addl %r9d, %r10d
		movl %r10d, -4(%rbp)

		leaq -44(%rbp), %r9
		movabs $5, %r11
		imulq $4, %r11
		addq %r9, %r11
		movl $2, (%r11)

		movq %rsi, %r9
		movabs $0, %r11
		imulq $4, %r11
		addq %r9, %r11
		movl -4(%rbp), %r15d
		movl %r15d, (%r11)

		#vi1 = va2[8]
		leaq -44(%rbp), %r10
		movabs $8, %r9
		imulq $4, %r9
		addq %r10, %r9
		movl (%r9), %r8d
		movl %r8d, -4(%rbp)


		movl %edi, %eax
		leave
		ret
.globl f8
	f8:
		pushq %rbp
		movq %rsp, %rbp
		subq $48, %rsp

		#vi1: -4(%rbp)
		#va2: -44(%rbp)

		movl $3, -4(%rbp)

		movl -4(%rbp), %r10d
		cmpl $0, %r10d
		je end_if0
		movl $0, -4(%rbp)

		end_if0:
		movl -4(%rbp), %r10d
		cmpl $0, %r10d
		je end_if1
		#vi1 = va2[8]
		leaq -44(%rbp), %r10
		movabs $8, %r9
		imulq $4, %r9
		addq %r10, %r9
		movl (%r9), %r8d
		movl %r8d, -4(%rbp)

		movl -4(%rbp), %r10d
		cmpl $0, %r10d
		je end_if1

		movl -4(%rbp), %eax
		leave
		ret
		end_if1:

		movl $-1, %eax
		leave
		ret
