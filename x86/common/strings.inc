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


;strings.CntB(HSI,al)(HAX)
;HDI    source byte string
;al     byte terminator
;HAX    number of bytes before terminator
cinc    strings.CntB
        call    rsto.Stoal
        mov     HDI,HSI
        .element:
        scasb
        jne     .element
        sub     HDI,HSI
        mov     HAX,HDI
        dec     HAX
        call    rsto.Clodal
        ret
endcinc

;strings.CntW(HSI,ax)(HAX)
;HDI	source word string
;ax	word terminator
;HAX	number of words before terminator
cinc    strings.CntW
	call    rsto.Stoal
        mov     HDI,HSI
        .element:
        scasw
        jne     .element
        sub     HDI,HSI
        mov     HAX,HDI
        dec     HAX
        call    rsto.Clodal
        ret
endcinc

;strings.CntD(HSI,eax)(HAX)
;HDI	source dword string
;eax	dword terminator
;HAX	number of dwords before terminator
cinc    strings.CntD
	call    rsto.Stoal
        mov     HDI,HSI
        .element:
        scasd
        jne     .element
        sub     HDI,HSI
        mov     HAX,HDI
        dec     HAX
        call    rsto.Clodal
        ret
endcinc

;strings.CntQ(HSI,rax)(HAX)
;HDI	source qword string
;rax	qword terminator
;HAX	number of qwords before terminator
cinc    strings.CntQ
	call    rsto.Stoal
        mov     HDI,HSI
        .element:
        scasq
        jne     .element
        sub     HDI,HSI
        mov     HAX,HDI
        dec     HAX
        call    rsto.Clodal
        ret
endcinc

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

;strings.CpyB(HSI,HDI,cl)(HAX)
;HSI    source byte string
;HDI    destination byte string
;cl     source byte terminator
;HAX    Number of bytes in output before null terminator
cinc    strings.CpyB
        call    rsto.Stoal
        xor     HAX,HAX
        .element:
        movsb
        inc     HAX
        cmp     byte[HSI],cl
        jne    .element
        mov     byte[HDI],0
        call    rsto.Clodal
        ret
endcinc

;strings.CpyW(HSI,HDI,cx)(HAX)
;HSI    source word string
;HDI    destination word string
;cx     source word terminator
;HAX    Number of words in output before null terminator
cinc    strings.CpyW
        call    rsto.Stoal
        xor     HAX,HAX
        .element:
        movsw
        inc     HAX
        cmp     byte[HSI],cx
        jne    .element
        mov     byte[HDI],0
        call    rsto.Clodal
        ret
endcinc

;strings.CpyD(HSI,HDI,ecx)(HAX)
;HSI    source dword string
;HDI    destination dword string
;ecx    source dword terminator
;HAX    Number of dwords in output before null terminator
cinc    strings.CpyD
        call    rsto.Stoal
        xor     HAX,HAX
        .element:
        movsd
        inc     HAX
        cmp     byte[HSI],ecx
        jne    .element
        mov     byte[HDI],0
        call    rsto.Clodal
        ret
endcinc

;strings.CpyQ(HSI,HDI,rcx)(HAX)
;HSI    source string
;HDI    destination qword string
;rcx    source qword terminator
;HAX    Number of qwords in output before null terminator
cinc    strings.CpyQ
        call    rsto.Stoal
        xor     HAX,HAX
        .element:
        movsq
        inc     HAX
        cmp     byte[HSI],rcx
        jne    .element
        mov     byte[HDI],0
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