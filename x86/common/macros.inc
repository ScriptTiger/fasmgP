struc	store	cap
	.Length	RH	1
	.Store	rb	cap

end struc

macro	NewStore	symbol,flushaction

	symbol:

	;.Validate(HAX)()
	cinc	.Validate
		call    rsto.Stoal
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
		call    rsto.Clodal
		ret
	endcinc

	;.Flush()()
	cinc    .Flush
		call    rsto.Stoal
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
		call    rsto.Clodal
		ret
	endcinc

	;.Prune(HAX)()
	cinc    .Prune
		call    rsto.Stoal
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
		call    rsto.Clodal
		ret
	endcinc
end macro

macro	bios.Exit
	include	"x86/common/bios.inc"
end macro

macro	common_lib
	include "x86/common/rsto.inc"
	include "x86/common/strings.inc"
	include "x86/common/asto.inc"
	include "x86/common/hsto.inc"
end macro

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

	if used astack|\
	   used pastack
		pastack DH      astack+ASCAP
	end if

end macro

macro   common_reservations
	if used astack|\
	   used pastack
		astack  rb      ASCAP
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