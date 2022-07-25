macro	windows_lib
	include	"x86/windows/unsafe.inc"
	include	"x86/windows/os.inc"
	include	"x86/windows/libc.inc"
end macro

macro	all_lib
	common_lib
	windows_lib
end macro

macro	os.Entry
	if used rsto.Length|\
	   used bufio.Length|\
	   used asto.Length
		xor	HCX,HCX
	end if

	if used rsto.Length
		mov	[rsto.Length],HCX
	end if

	if used bufio.Length
		mov     [bufio.Length],HCX
	end if

	if used hsto.Length
		mov     [hsto.Length],HCX
	end if

	if used asto.Length
		mov     [asto.Length],HCX
	end if

	if used os.Stdin
		invoke  GetStdHandle,\
			STD_INPUT_HANDLE
		mov     [os.Stdin],HAX
		mov	HWORD[hsto.Store],HAX
		add	HWORD[hsto.Length],BYTECAP
	end if

	if used os.Stdout
		invoke  GetStdHandle,\
			STD_OUTPUT_HANDLE
		mov     [os.Stdout],HAX
		mov	HWORD[hsto.Store],HAX
		add	HWORD[hsto.Length],BYTECAP
	end if

	if used os.Stderr
		invoke  GetStdHandle,\
			STD_ERROR_HANDLE
		mov     [os.Stderr],HAX
		mov	HWORD[hsto.Store],HAX
		add	HWORD[hsto.Length],BYTECAP
	end if

	if used os.Prchp
		invoke  GetProcessHeap
		mov     [os.Prchp],HAX
	end if

	if used os.Cmdline|\
	   used os.Argc|\
	   used os.Argv
		invoke  GetCommandLine
		mov     [os.Cmdline],HAX
		call    os.FmtArgs
	end if

	if used os.Penv
		invoke  GetEnvironmentStrings
		mov     [os.Penv],HAX
	end if
end macro

;Places both the os.Exit procedure as well as a label so it can be easily jumped to
;os.Exit can also be called, but this is unnecessary since it won't be returning
macro	os.Exit
	os.Exit:
	mov	HBX,HCX

	if used hsto.Lengh|\
	   used hsto.Store
		call	hsto.Flush
	end if

	if used asto.Lengh|\
	   used asto.Store
		call	asto.Flush
	end if

	if used os.Penv
		invoke	FreeEnvironmentStrings,\
			[os.Penv]
	end if

	invoke	ExitProcess,\
		HBX

	all_lib
end macro

macro	NewInvalidate	validate,action,prune
;(HCX)()
	call	rsto.Stoal
	mov	HAX,[HCX]
	call	validate
	cmp	HAX,0
	je	.done
	call	action
	mov	HAX,[HCX]
	call	prune
	.done:
	call    rsto.Clodal
	ret
end macro

macro	implib	name,table
	dd	0,0,0,rva	name,rva	table
end macro

macro	ctble	symbol,intsymbol
	if used symbol
		symbol	DH	rva	intsymbol
	end if
end macro

macro	clibf	symbol,file
	if used symbol
		symbol	db	file,0
	end if
end macro

macro	cimp symbol,import
	if used symbol
		symbol  dw      0
			db      import,0
	end if
end macro

macro   imports
	if used ExitProcess|\
	   used HeapFree|\
	   used GetStdHandle|\
	   used GetProcessHeap|\
	   used GetCommandLine|\
	   used GetEnvironmentStrings|\
	   used CloseHandle|\
	   used FreeEnvironmentStrings|\
	   used HeapAlloc|\
	   used ReadFile|\
	   used WriteFile|\
	   used FlushConsoleInputBuffer|\
	   used Sleep
		implib	kernel_name,	kernel_table
	end if

	if used _kbhit
		implib	msvcrt_name,	msvcrt_table
	end if

	if used URLDownloadToFile
		implib	urlmon_name,	urlmon_table
	end if

	dd      0,0,0,0,0

	cinc    kernel_table
		ctble	ExitProcess,		_ExitProcess
		ctble	HeapFree,		_HeapFree
		ctble	GetStdHandle,		_GetStdHandle
		ctble	GetProcessHeap,		_GetProcessHeap
		ctble	GetCommandLine,		_GetCommandLine
		ctble	GetEnvironmentStrings,	_GetEnvironmentStrings
		ctble	CloseHandle,		_CloseHandle
		ctble	FreeEnvironmentStrings,	_FreeEnvironmentStrings
		ctble	HeapAlloc,		_HeapAlloc
		ctble	ReadFile,		_ReadFile
		ctble	WriteFile,		_WriteFile
		ctble	FlushConsoleInputBuffer,_FlushConsoleInputBuffer
		ctble	Sleep,			_Sleep
		DH      0

	endcinc

	cinc	msvcrt_table
		ctble	_kbhit,			__kbhit
		DH	0
	endcinc

	cinc	urlmon_table
		ctble	URLDownloadToFile,	_URLDownloadToFile
		DH      0
	endcinc

	clibf	kernel_name,			"kernel32.dll"
	clibf	msvcrt_name,			"msvcrt.dll"
	clibf	urlmon_name,			"urlmon.dll"

	cimp	_ExitProcess,			"ExitProcess"
	cimp	_HeapFree,			"HeapFree"
	cimp	_GetStdHandle,			"GetStdHandle"
	cimp	_GetProcessHeap,		"GetProcessHeap"
	cimp	_GetCommandLine,		"GetCommandLineA"
	cimp	_GetEnvironmentStrings,		"GetEnvironmentStringsA"
	cimp	_CloseHandle,			"CloseHandle"
	cimp	_FreeEnvironmentStrings,	"FreeEnvironmentStringsA"
	cimp	_HeapAlloc,			"HeapAlloc"
	cimp	_ReadFile,			"ReadFile"
	cimp	_WriteFile,			"WriteFile"
	cimp	_FlushConsoleInputBuffer,	"FlushConsoleInputBuffer"
	cimp	_Sleep,				"Sleep"
	cimp	__kbhit,			"_kbhit"
	cimp	_URLDownloadToFile,		"URLDownloadToFileA"
end macro