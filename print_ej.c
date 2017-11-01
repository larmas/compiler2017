#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("%i\n",1 );
	return 1;
}

/*Assembly*/

	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"%i\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:

	pushq	%rbp			//Enter
	movq	%rsp, %rbp

	movl	$1, %eax		//return 1

	movl	%eax, %esi		
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf

	popq	%rbp			//Leave
	ret

.LFE2:
	.size	main, .-main
