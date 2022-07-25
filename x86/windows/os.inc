;os.FmtArgs()()
;Format argument string into argv string array and update argc with number of arguments
cinc    os.FmtArgs
	call    rsto.Stoal
	mov     HSI,[os.Cmdline]
	xor     al,al
	call    strings.CntB
	add     HAX,2
	mov     HCX,HAX
	call    os.New
	mov     [os.Argv],HAX
	mov     HSI,[os.Cmdline]
	mov     HDI,[os.Argv]
	xor     HCX,HCX
	.character:
	lodsb
	cmp     word[HSI-1],2020h
	je      .doublewhite
	cmp     word[HSI-1],909h
	je      .doublewhite
	cmp     word[HSI-1],2009h
	je      .doublewhite
	cmp     word[HSI-1],920h
	je      .doublewhite
	cmp     al,20h
	je      .split
	cmp     al,9h
	je      .split
	cmp     al,22h
	je      .quote
	cmp     al,0
	je      .countargs
	.copycharacter:
	mov     [HDI],al
	.nextcharacter:
	inc     HDI
	jmp     .character
	.doublewhite:
	cmp     HCX,0
	jne     .copycharacter
	jmp     .character
	.split:
	cmp     HCX,0
	jne     .copycharacter
	mov     byte[HDI],0
	jmp     .nextcharacter
	.quote:
	cmp     HCX,0
	jne     .unquote
	inc     HCX
	jmp     .copycharacter
	.unquote:
	dec     HCX
	jmp     .copycharacter
	.countargs:
	mov     HDI,[os.Argv]
	xor     al,al
	call    strings.CntStrB
	mov     [os.Argc],HAX
	call    rsto.Clodal
	ret
endcinc

;os.GetEnv(HSI)(HAX)
;HSI	environmental variable to get
;HAX	pointer to value,0 if none
cinc	os.GetEnv
	call	rsto.Stoal
	mov     HDI,[os.Penv]
	.env:
	xor     cl,cl
	call    strings.PrfxB
	cmp     HAX,0
	jne     .nextenv
	xor     al,al
	call    strings.CntB
	inc	HAX
	add     HAX,HDI
	cmp	byte[HAX-1],"="
	je	.done
	.nextenv:
	xor     al,al
	call    strings.NxtB
	cmp	byte[HAX],0
	je	.nomatch
	mov     HDI,HAX
	jmp     .env
	.nomatch:
	xor	HAX,HAX
	.done:
	call	rsto.Clodal
	ret
endcinc

;os.New(HCX)(HAX)
;HCX    Number of bytes to allocate from the heap
;HAX    Heap allocation
cinc    os.New
	call    rsto.Stoal
	invoke  HeapAlloc,\
		[os.Prchp],\
		HEAP_ZERO_MEMORY,\
		HCX
	mov     HCX,asto.Store
	add     HCX,[asto.Length]
	add     [asto.Length],BYTECAP
	mov     [HCX],HAX
	mov     dword[HCX+4],0
	call    rsto.Clodal
	ret
endcinc

;os.Delete(HCX)()
;HCX    Heap allocation to free
cinc	os.Delete
	NewInvalidate	asto.Validate,\
			unsafe.Delete,\
			asto.Prune
endcinc

;os.Close(HCX)()
;HCX    Handle to close
cinc	os.Close
	NewInvalidate	hsto.Validate,\
			unsafe.Close,\
			hsto.Prune
endcinc

;os.Cin(HDI,HCX,HDX)(HAX)
;HDI    Pointer to string input
;HCX    Minimum character count
;HDX    Maximum character count
;HAX    Character count in input
cinc    os.Cin
	call    rsto.Stoal
	add     HCX,2
	add     HDX,2
	call    rsto.Stoal
	.readline:
	invoke  ReadFile,\
		[os.Stdin],\
		addr bufio.Buffer,\
		BUFCAP,\
		addr bufio.Length,\
		0
	call    rsto.Lodal
	cmp     [bufio.Length],HCX
	jl      .short
	cmp     [bufio.Length],HDX
	jle     .valid
	mov     HSI,.AskInLess
	call    os.Cout
	jmp     .readline
	.short:
	mov     esi,.AskInMore
	call    os.Cout
	jmp     .readline
	.valid:
	sub     [rsto.Length],RSTOINC
	dec     [bufio.Length]
	mov     HCX,[bufio.Length]
	call    os.New
	mov     [HDI],eax
	mov     HSI,bufio.Buffer
	mov     HDI,[HDI]
	mov     HCX,[bufio.Length]
	xor     dl,dl
	call    strings.CpynB
	mov     HAX,[bufio.Length]
	call    rsto.Clodal
	ret

	.AskInLess   db      "Your input is too long!",13,10,0
	.AskInMore   db      "Your input is too short!",13,10,0
endcinc

;os.Cout(HSI)()
cinc    os.Cout
	call    rsto.Stoal
	mov     HBX,HAX
	xor     al,al
	call    strings.CntB
	invoke  WriteFile,\
		[os.Stdout],\
		HSI,\
		HAX,\
		0,\
		0
	mov     HAX,HBX
	call    rsto.Clodal
	ret
endcinc

;os.Couteol()()
cinc    os.Couteol
	call    rsto.Stoal
	mov     HSI,eol
	call    os.Cout
	call    rsto.Clodal
	ret
endcinc