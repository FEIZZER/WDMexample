
	.code

EXTERN PrintTest: PROC
; EXTERN add: PROC
_Start PROC

	sub rsp, 28h
	mov	qword ptr [rsp+4*8], rcx
	mov	qword ptr [rsp+5*8], rdx
	mov	qword ptr [rsp+6*8], r8
	mov	qword ptr [rsp+7*8], r9

	call PrintTest

	mov	rcx, qword ptr [rsp+4*8]
	mov	rdx, qword ptr [rsp+5*8]
	mov	r8, qword ptr [rsp+6*8]
	mov	r9, qword ptr [rsp+7*8]
	add rsp, 28h

	ret

_Start endp

	.code zzzz
	dq _Start

end