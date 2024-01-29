
	.code

EXTERN PrintTest: PROC
; EXTERN add: PROC
_Start PROC
	sub rsp, 28h
	mov rax, 444h
	call PrintTest
	add rsp, 28h
	ret

_Start endp

	.code zzzz
	dq _Start

end