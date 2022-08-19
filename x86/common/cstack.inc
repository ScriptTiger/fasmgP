;cstack.Pusha()()
;Store current register set in the call stack
cinc	cstack.Pusha
	mov	[swap],HAX
	pop	HAX
	push	HCX
	push	HDX
	push	HBX
	push	HSI
	push	HDI
	push	HAX
	mov	HAX,[swap]
	ret
endcinc

;cstack.Popa()()
;Load register set from call stack
cinc	cstack.Popa
	mov	[swap],HAX
	pop	HAX
	pop	HDI
	pop	HSI
	pop	HBX
	pop	HDX
	pop	HCX
	push	HAX
	mov	HAX,[swap]
	ret
endcinc