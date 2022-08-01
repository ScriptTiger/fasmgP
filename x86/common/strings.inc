if BYTECAP=2
	strings.CntH	equ	strings.CntW
	strings.CpyH	equ	strings.CpyW
	strings.CpynH	equ	strings.CpynW
else if BYTECAP=4
	strings.CntH	equ	strings.CntD
	strings.CpyH	equ	strings.CpyD
	strings.CpynH	equ	strings.CpynD
else if BYTECAP=8
	strings.CntH	equ	strings.CntQ
	strings.CpyH	equ	strings.CpyQ
	strings.CpynH	equ	strings.CpynQ
end if

;strings.CntB(HSI,al)(HAX)
;HDI	source byte string
;al	byte terminator
;HAX	number of bytes before byte terminator
NewSCnt	strings.CntB,\
	scasb,\
	1

;strings.CntW(HSI,ax)(HAX)
;HDI	source word string
;ax	word terminator
;HAX	number of words before word terminator
NewSCnt	strings.CntW,\
	scasw,\
	2

;strings.CntD(HSI,eax)(HAX)
;HDI	source dword string
;eax	dword terminator
;HAX	number of dwords before dword terminator
NewSCnt	strings.CntD,\
	scasd,\
	4

;strings.CntQ(HSI,rax)(HAX)
;HDI	source qword string
;rax	qword terminator
;HAX	number of qwords before qword terminator
NewSCnt	strings.CntQ,\
	scasq,\
	8

;strings.CpyB(HSI,HDI,cl,dl)(HAX)
;HSI	source byte string
;HDI	destination byte string
;cl	source byte terminator
;dl	destination byte terminator
;HAX	Number of bytes in output before byte terminator
NewSCpy	strings.CpyB,\
	movsb,\
	cl,\
	byte,\
	dl

;strings.CpyBW(HSI,HDI,cx,dl)(HAX)
;HSI	source byte string
;HDI	destination byte string
;cx	source word terminator
;dl	destination byte terminator
;HAX	Number of bytes in output before word terminator
NewSCpy	strings.CpyBW,\
	movsb,\
	cx,\
	byte,\
	dl

;strings.CpyBD(HSI,HDI,ecx,dl)(HAX)
;HSI	source byte string
;HDI	destination byte string
;ecx	source dword terminator
;dl	destination byte terminator
;HAX	Number of bytes in output before dword terminator
NewSCpy	strings.CpyBD,\
	movsb,\
	ecx,\
	byte,\
	dl

;strings.CpyBQ(HSI,HDI,rcx,dl)(HAX)
;HSI	source byte string
;HDI	destination byte string
;rcx	source qword terminator
;dl	destination byte terminator
;HAX	Number of bytes in output before qword terminator
NewSCpy	strings.CpyBQ,\
	movsb,\
	rcx,\
	byte,\
	dl

;strings.CpyW(HSI,HDI,cx,dx)(HAX)
;HSI	source word string
;HDI	destination word string
;cx	source word terminator
;dx	destination word terminator
;HAX	Number of words in output before word terminator
NewSCpy	strings.CpyW,\
	movsw,\
	cx,\
	word,\
	dx

;strings.CpyWD(HSI,HDI,ecx,dx)(HAX)
;HSI	source word string
;HDI	destination word string
;ecx	source dword terminator
;dx	destination word terminator
;HAX	Number of words in output before dword terminator
NewSCpy	strings.CpyWD,\
	movsw,\
	ecx,\
	word,\
	dx

;strings.CpyWQ(HSI,HDI,rcx,dx)(HAX)
;HSI	source word string
;HDI	destination word string
;rcx	source qword terminator
;dx	destination word terminator
;HAX	Number of words in output before qword terminator
NewSCpy	strings.CpyWQ,\
	movsw,\
	rcx,\
	word,\
	dx

;strings.CpyD(HSI,HDI,ecx,edx)(HAX)
;HSI	source dword string
;HDI	destination dword string
;ecx	source dword terminator
;edx	destination dword terminator
;HAX	Number of dwords in output before dword terminator
NewSCpy	strings.CpyD,\
	movsd,\
	ecx,\
	dword,\
	edx

;strings.CpyDQ(HSI,HDI,rcx,edx)(HAX)
;HSI	source dword string
;HDI	destination dword string
;rcx	source qword terminator
;edx	destination dword terminator
;HAX	Number of dwords in output before qword terminator
NewSCpy	strings.CpyDQ,\
	movsd,\
	rcx,\
	dword,\
	edx

;strings.CpyQ(HSI,HDI,rcx,rdx)(HAX)
;HSI	source qword string
;HDI	destination qword string
;rcx	source qword terminator
;rdx	destination qword terminator
;HAX	Number of qwords in output before qword terminator
NewSCpy	strings.CpyQ,\
	movsq,\
	rcx,\
	qword,\
	rdx

;strings.CpynB(HSI,HDI,HCX,dl)(HAX)
;HDI	source byte string
;HDI	destination byte string
;HCX	number of bytes in output, including byte terminator
;dl	destination byte terminator
;HAX	copy of resultant destination byte string pointer
NewSCpn	strings.CpynB,\
	movsb,\
	dl

;strings.CpynW(HSI,HDI,HCX,dx)(HAX)
;HDI	source word string
;HDI	destination word string
;HCX	number of words in output, including word terminator
;dx	destination word terminator
;HAX	copy of resultant destination word string pointer
NewSCpn	strings.CpynW,\
	movsw,\
	dx

;strings.CpynD(HSI,HDI,HCX,edx)(HAX)
;HDI	source dword string
;HDI	destination dword string
;HCX	number of dwords in output, including dword terminator
;edx	destination dword terminator
;HAX	copy of resultant destination dword string pointer
NewSCpn	strings.CpynD,\
	movsd,\
	edx

;strings.CpynQ(HSI,HDI,HCX,rdx)(HAX)
;HDI	source qword string
;HDI	destination qword string
;HCX	number of qwords in output, including qword terminator
;rdx	destination qword terminator
;HAX	copy of resultant destination qword string pointer
NewSCpn	strings.CpynQ,\
	movsq,\
	rdx

;strings.CntStrB(HDI,al)(HAX)
;HDI    source string array
;al     terminator
;HAX    number of strings
cinc    strings.CntStrB
	cpusha
        xor     HCX,HCX
        .element:
        scasb
        jne     .element
        inc     HCX
        scasb
        je      .done
        jmp     .element
        .done:
        mov     HAX,HCX
	cpopa
        ret
endcinc

;strings.CmpB(HSI,HDI,cl)(HAX)
;HSI    first string
;HDI    second string
;cl     terminator
;HAX    0/exact match,1/no exact match
cinc strings.CmpB
	cpusha
        xor     HAX,HAX
        .element:
        cmpsb
        jne     .mismatch
        cmp     byte[HSI-1],cl
        je      .match
        jmp     .element
        .mismatch:
        inc     HAX
        .match:
	cpopa
        ret
endcinc

;strings.PrfxB(HSI,HDI,cl)(HAX)
;HSI    first string
;HDI    second string
;cl     source string terminator
;HAX    0/prefix match,1/no prefix match
cinc    strings.PrfxB
	cpusha
        xor     HAX,HAX
        .element:
        cmpsb
        jne     .mismatch
        cmp     byte[HSI],cl
        je      .match
        jmp     .element
        .mismatch:
        inc     HAX
        .match:
	cpopa
        ret
endcinc

;strings.NxtB(HDI,al)(HAX)
;HDI    source string array
;al     byte terminator
;HAX    copy of resultant edi pointer
cinc    strings.NxtB
	cpusha
        .element:
        scasb
        jne     .element
        mov     HAX,HDI
	cpopa
        ret
endcinc

;strings.NB(HDI,al,HCX)(HAX)
;HDI    source string array
;al     byte terminator
;HCX    nth string, starting at 0
;HAX    copy of resultant edi pointer
cinc    strings.NB
	cpusha
        .element:
        scasb
        jne     .element
        dec     HCX
        cmp     HCX,0
        jne     .element
        mov     HAX,HDI
	cpopa
        ret
endcinc