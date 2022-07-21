HAX	equ	rax
HCX	equ	rcx
HDX	equ	rdx
HBX	equ	rbx
HSP	equ	rsp
HBP	equ	rbp
HSI	equ	rsi
HDI	equ	rdi

struc	(label)	RH	etc&
	label	rq	etc
end struc

struc	(label)	DH	etc&
	label	dq	etc
end struc

macro	RH	etc&
	rq	etc
end macro

macro	DH	etc&
	dq	etc
end macro

macro	MOVSH
	movsq
end macro

macro	SCASH
	scasq
end macro

BYTECAP	=	8
HWORD	equ	qword
RSTOINC	=	5*BYTECAP