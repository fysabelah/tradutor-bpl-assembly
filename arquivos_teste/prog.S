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
