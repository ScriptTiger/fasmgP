struc	store	cap
	.Length	RH	1
	.Store	rb	cap

end struc

macro	cpusha
	if BYTECAP=2
		call	cstack.Pusha
	else
		call	rsto.Stoal
	end if
end macro

macro	cpopa
	if BYTECAP=2
		call	cstack.Popa
	else
		call	rsto.Clodal
	end if
end macro

macro	NewStore	symbol,flushaction

	symbol:

	;.Sto(HAX)()
	cinc	.Sto
		cpusha
		mov	HBX,HAX
		mov	HCX,[.Length]
		mov	HAX,BYTECAP
		mul	HCX
		mov	HCX,HAX
		mov	HAX,HBX
		add	HCX,.Store
		inc	[.Length]
		mov	[HCX],HAX
		mov	HWORD[HCX+BYTECAP],0
		cpopa
		ret
	endcinc

	;.Validate(HAX)()
	cinc	.Validate
		cpusha
		cmp     [.Length],0
		je      .invalid
		cmp     HAX,0
		je      .invalid
		mov     HDI,.Store
		mov	HCX,[.Length]
		SCASH
		je	.valid
		repne	SCASH
		je      .valid
		.invalid:
		xor	HAX,HAX
		jmp	.vdone
		.valid:
		mov     HAX,1
		.vdone:
		cpopa
		ret
	endcinc

	;.Flush()()
	cinc    .Flush
		cpusha
		cmp     [.Length],0
		je      .fdone
		mov     HCX,.Store
		.felement:
		call    flushaction
		mov	HWORD[HCX],0
		add     HCX,BYTECAP
		cmp     HWORD[HCX],0
		jne     .felement
		mov     [.Length],0
		.fdone:
		cpopa
		ret
	endcinc

	;.Prune(HAX)()
	cinc    .Prune
		cpusha
		cmp     [.Length],0
		je      .pdone
		cmp     HAX,0
		je      .pdone
		mov     HDI,.Store
		mov	HCX,[.Length]
		SCASH
		je	.match
		repne	SCASH
		je	.match
		jmp	.pdone
		.match:
		cmp	HCX,0
		jne	.prune
		mov	HWORD[HDI-BYTECAP],0
		jmp	.adjustlen
		.prune:
		mov	HSI,HDI
		sub	HDI,BYTECAP
		xor	HDX,HDX
		call	strings.CpynH
		.adjustlen:
		dec	[.Length]
		.pdone:
		cpopa
		ret
	endcinc
end macro

;strings.Cnt(HSI,al-rax)(HAX)
;HDI	source string
;al-rax	terminator
;HAX	number of elements before terminator
macro	NewSCnt	symbol,instruction,length
	cinc	symbol
		cpusha
		mov	HDI,HSI
		.element:
		instruction
		jne	.element
		sub	HDI,HSI
		mov	HAX,HDI
		if length > 1
			mov	HCX,length
			xor	HDX,HDX
			div	HCX
		end if
		dec	HAX
		cpopa
		ret
	endcinc
end macro

;strings.Cpy(HSI,HDI,cl-rcx,dl-rdx)(HAX)
;HSI	source string
;HDI	destination string
;cl-rcx	source terminator
;dl-rdx	destination terminator
;HAX	Number of elements in output before terminator
macro	NewSCpy	symbol,instruction,counter,wsize,data
	cinc	symbol
		cpusha
		xor	HAX,HAX
		.element:
		instruction
		inc	HAX
		cmp	[HSI],counter
		jne	.element
		mov	wsize[HDI],data
		cpopa
		ret
	endcinc
end macro

;strings.Cpyn(HSI,HDI,HCX,dl-rdx)(HAX)
;HDI	source string
;HDI	destination string
;HCX	number of elements in output, including terminator
;dl-rdx	destination terminator
;HAX	copy of resultant destination string pointer
macro	NewSCpn	symbol,instruction,data
	cinc	symbol
		cpusha
		dec     HCX
		rep     instruction
		mov     [HDI],data
		cpopa
		mov     HAX,HDI
		ret
	endcinc
end macro

macro	common_lib
	include "x86/common/strings.inc"
	include "x86/common/rsto.inc"
	include "x86/common/asto.inc"
	include "x86/common/hsto.inc"
end macro

macro	mbr.Std	entry,etc&
	org	7c00h
	jmp	entry
	mbr.BPB	etc
end macro

macro	mbr.BPB	oem,id,volume

	;8 bytes for OEM Label
	db	oem

	dw	512
	db	1
	dw	1
	db	2
	dw	224
	dw	2880
	db	0f0h
	dw	9
	dw	18
	dw	2
	rd	1
	rd	1
	rw	1
	db	41

	;Volume ID
	dd	id

	;11 bytes for volume label
	db	volume

	db	"FAT12   "
end macro

macro	bios.Entry
	mov	bp,cstack.Seg
	mov	ss,bp
	mov	HBP,cstack.Base
	mov	HSP,HBP
end macro

macro	mbr.Entry
	bios.Entry
	mov     [btdrv],dl
end macro

macro	bios.Exit
	bios.Exit:
	include	"x86/common/cstack.inc"
	include "x86/common/strings.inc"
	include	"x86/common/bios.inc"

	datasegment:
	common_definitions
	common_reservations

	swap	RH	1

	cstack.Seg:
	if defined STACKCAPACITY
		rb	STACKCAPACITY
	else
		rb	4096
	end if
	cstack.Base:
end macro

macro	mbr.Exit
	mbr.Exit:
	include	"x86/common/cstack.inc"
	include "x86/common/strings.inc"
	include	"x86/common/bios.inc"

	btdrv   rb      1

	;Assembly-time MBR info
	macro dspaddr msg,iaddr
		display "MBR ",msg," "
		repeat  4
			addr = "0" + (iaddr) shr (16 - % * 4) and 0fh
			if addr > "9"
				addr = addr + "A" - "9" - 1
			end if
			display addr
		end repeat
		display "h",13,10
	end macro
	dspaddr "code starts at",7c00h
	dspaddr "code ends at",$
	dspaddr "limit is",7e00h

	;Padding
	mbrpad  rb      510 - ($ - 7c00h)

	;Magic Number
	dw     0aa55h

	common_reservations

	swap	RH	1

	cstack.Seg:
	if defined STACKCAPACITY
		rb	STACKCAPACITY
	else
		rb	4096
	end if
	cstack.Base:
end macro

if BYTECAP=2
	macro	stack	cap
		STACKCAPACITY	=	cap
	end macro
end if

macro   cinc    symbol
	if used symbol
		symbol:
end macro

macro     endcinc!
	end if
end macro

macro   cincl   symbol,etc&
	if used symbol
		symbol  etc
	end if
end macro

macro   common_definitions
	cincl   eol,		db	EOL
	cincl	astack.Ptr,	DH	astack.Base
end macro

macro   common_reservations
	if used astack|\
	   used astack.Ptr|\
	   used	astack.Base
		astack	rb	ASCAP
		astack.Base:
	end if

	cincl   os.Stdin,       RH      1
	cincl   os.Stdout,      RH      1
	cincl   os.Stderr,      RH      1
	cincl   os.Prchp,       RH      1
	cincl   os.Cmdline,     RH      1
	cincl   os.Argc,	RH      1
	cincl   os.Argv,	RH      1
	cincl   os.Penv,	RH      1

	if used bufio.Buffer|\
	   used bufio.Length
		bufio:
		;Current buffer length
		.Length		RH	1

		;Buffer and guards
		.Bufgrdl	RH	1
		.Buffer		rb	BUFCAP
		.Bufgrdh	RH	1
	end if

	if used rsto.Length|\
	   used rsto.Store
		rsto	store	RSTOCAP
	end if

	if used asto.Length|\
	   used asto.Store
		asto	store	ASTOCAP
	end if

	if used hsto.Length|\
	   used hsto.Store
		hsto	store	HSTOCAP
	end if
end macro