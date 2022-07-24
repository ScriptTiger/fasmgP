struc	store	cap
	.Length	RH	1
	.Store	rb	cap

end struc

macro	cpusha
	if BYTECAP=2
		pusha
	else
		call	rsto.Stoal
	end if
end macro

macro	cpopa
	if BYTECAP=2
		popa
	else
		call	rsto.Clodal
	end if
end macro

macro	NewStore	symbol,flushaction

	symbol:

	;.Validate(HAX)()
	cinc	.Validate
		cpusha
		cmp     [.Length],0
		je      .invalid
		cmp     HAX,0
		je      .invalid
		mov     HDI,.Store
		.pointer:
		scasd
		je      .valid
		cmp     HWORD[HDI-BYTECAP],0
		je      .invalid
		jmp     .pointer
		.valid:
		mov     HAX,1
		jmp     .done
		.invalid:
		mov     HAX,0
		.done:
		cpopa
		ret
	endcinc

	;.Flush()()
	cinc    .Flush
		cpusha
		cmp     [.Length],0
		je      .done
		mov     HCX,.Store
		.pointer:
		call    flushaction
		add     HCX,BYTECAP
		cmp     HWORD[HCX],0
		jne     .pointer
		mov     [.Length],0
		.done:
		cpopa
		ret
	endcinc

	;.Prune(HAX)()
	cinc    .Prune
		cpusha
		cmp     [.Length],0
		je      .done
		cmp     HAX,0
		je      .done
		mov     HDI,.Store
		.pointer:
		scasd
		je      .match
		cmp     HWORD[HDI-BYTECAP],0
		je      .done
		jmp     .pointer
		.match:
		cmp     HWORD[HDI],0
		jne     .prune
		mov     HWORD[HDI-BYTECAP],0
		jmp     .adjustlen
		.prune:
		mov     HSI,HDI
		sub     HDI,BYTECAP
		xor     HCX,HCX
		call    strings.CpyH
		.adjustlen:
		sub     [.Length],BYTECAP
		.done:
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

;strings.Cpy(HSI,HDI,cl-rcx)(HAX)
;HSI	source string
;HDI	destination string
;cl-rcx	source terminator
;HAX	Number of elements in output before null terminator
macro	NewSCpy	symbol,instruction,accumulator,wsize
	cinc	symbol
		cpusha
		xor	HAX,HAX
		.element:
		instruction
		inc	HAX
		cmp	wsize[HSI],accumulator
		jne	.element
		mov	wsize[HDI],0
		cpopa
		ret
	endcinc
end macro

macro	common_lib
	include "x86/common/strings.inc"
	include "x86/common/rsto.inc"
	include "x86/common/asto.inc"
	include "x86/common/hsto.inc"
end macro

macro	bios.Entry
	mov	bp,stacksegment
	mov	ss,bp
	mov	HBP,stackbase
	mov	HSP,HBP
end macro

macro	mbr.Entry
	bios.Entry
        mov     [btdrv],dl
end macro

macro	bios.Exit
	bios.Exit:
	include "x86/common/strings.inc"
	include	"x86/common/bios.inc"

	datasegment:
	common_definitions
	common_reservations

	stacksegment:
	if defined STACKCAPACITY
		rb	STACKCAPACITY
	else
		rb	4096
	end if
	stackbase:
end macro

macro	mbr.Exit
	mbr.Exit:
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

	stacksegment:
	if defined STACKCAPACITY
		rb	STACKCAPACITY
	else
		rb	4096
	end if
	stackbase:
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
	cincl   eol,    db      EOL
	cincl	astackp,DH	astackb
end macro

macro   common_reservations
	if used astack|\
	   used astackp|\
	   used	astackb
		astack  rb      ASCAP
		astackb:
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