
	.code

EXTERN PrintTest: PROC
EXTERN add: PROC
main PROC

	mov rbp, rsp
	call PrintTest
	mov rsp, rbp
	ret

main endp


end