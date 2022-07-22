if BYTECAP=2
	strings.CntH	equ	strings.CntW
	strings.CpyH	equ	strings.CpyW
else if BYTECAP=4
	strings.CntH	equ	strings.CntD
	strings.CpyH	equ	strings.CpyD
else if BYTECAP=8
	strings.CntH	equ	strings.CntQ
	strings.CpyH	equ	strings.CpyQ
end if

NewSCnt	strings.CntB,	scasb,	1
NewSCnt	strings.CntW,	scasw,	2
NewSCnt	strings.CntD,	scasd,	4
NewSCnt	strings.CntQ,	scasq,	8

NewSCpy	strings.CpyB,	scasb,	cl,	byte
NewSCpy	strings.CpyW,	scasw,	cx,	word
NewSCpy	strings.CpyD,	scasd,	ecx,	dword
NewSCpy	strings.CpyQ,	scasq,	rcx,	qword

;strings.CntStrB(HDI,al)(HAX)
;HDI    source string array
;al     terminator
;HAX    number of strings
cinc    strings.CntStrB
        call    rsto.Stoal
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
        call    rsto.Clodal
        ret
endcinc

;strings.CmpB(HSI,HDI,cl)(HAX)
;HSI    first string
;HDI    second string
;cl     terminator
;HAX    0/exact match,1/no exact match
cinc strings.CmpB
        call    rsto.Stoal
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
        call    rsto.Clodal
        ret
endcinc

;strings.PrfxB(HSI,HDI,cl)(HAX)
;HSI    first string
;HDI    second string
;cl     source string terminator
;HAX    0/prefix match,1/no prefix match
cinc    strings.PrfxB
        call    rsto.Stoal
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
        call    rsto.Clodal
        ret
endcinc

;strings.NxtB(HDI,al)(HAX)
;HDI    source string array
;al     byte terminator
;HAX    copy of resultant edi pointer
cinc    strings.NxtB
        call    rsto.Stoal
        .element:
        scasb
        jne     .element
        mov     HAX,HDI
        call    rsto.Clodal
        ret
endcinc

;strings.NB(HDI,al,HCX)(HAX)
;HDI    source string array
;al     byte terminator
;HCX    nth string, starting at 0
;HAX    copy of resultant edi pointer
cinc    strings.NB
        call    rsto.Stoal
        .element:
        scasb
        jne     .element
        dec     HCX
        cmp     HCX,0
        jne     .element
        mov     HAX,HDI
        call    rsto.Clodal
        ret
endcinc

;strings.CpynB(HSI,HDI,HCX,dl)(HAX)
;HDI    source string
;HDI    destination string
;HCX    number of bytes in output, including terminator
;dl     destination terminator
;HAX    copy of resultant edi pointer
cinc    strings.CpynB
        call    rsto.Stoal
        dec     HCX
        rep     movsb
        mov     byte[HDI],dl
        call    rsto.Clodal
        mov     HAX,HDI
        ret
endcinc