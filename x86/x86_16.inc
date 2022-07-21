HAX	equ	ax
HCX	equ	cx
HDX	equ	dx
HBX	equ	bx
HSP	equ	sp
HBP	equ	bp
HSI	equ	si
HDI	equ	di

struc	(label)	RH	etc&
	label	rw	etc
end struc

struc	(label)	DH	etc&
	label	dw	etc
end struc

macro	RH	etc&
	rw	etc
end macro

macro	DH	etc&
	dw	etc
end macro

macro	MOVSH
	movsw
end macro

macro	SCASH
	scasw
end macro

BYTECAP	=	2
HWORD	equ	word
RSTOINC	=	5*BYTECAP