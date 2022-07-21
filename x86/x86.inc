HAX	equ	eax
HCX	equ	ecx
HDX	equ	edx
HBX	equ	ebx
HSP	equ	esp
HBP	equ	ebp
HSI	equ	esi
HDI	equ	edi

struc	(label)	RH	etc&
	label	rd	etc
end struc

struc	(label)	DH	etc&
	label	dd	etc
end struc

macro	RH	etc&
	rd	etc
end macro

macro	DH	etc&
	dd	etc
end macro

macro	MOVSH
	movsd
end macro

macro	SCASH
	scasd
end macro

BYTECAP	=	4
HWORD	equ	dword
RSTOINC	=	5*BYTECAP