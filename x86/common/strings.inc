;strings.CntB(HSI,al)(HAX)
;HDI    source byte string
;al     byte terminator
;HAX    number of bytes before terminator
cinc    strings.CntB
        call    rsto.Stoal
        mov     HDI,HSI
        .character:
        scasb
        jne     .character
        sub     HDI,HSI
        mov     HAX,HDI
        dec     HAX
        call    rsto.Clodal
        ret
endcinc

;strings.CntH(HSI,HAX)(HAX)
;HSI    source double word string
;HAX    Highest word terminator
;HAX    Number of highest words before terminator
cinc     strings.CntH
        call    rsto.Stoal
        mov     HDI,HSI
        .character:
        SCASH
        jne     .character
        sub     HDI,HSI
        mov     HAX,HDI
        mov     HCX,BYTECAP
        xor     HDX,HDX
        div     HCX
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
        .character:
        scasb
        jne     .character
        inc     HCX
        scasb
        je      .done
        jmp     .character
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
        .character:
        cmpsb
        jne     .mismatch
        cmp     byte[HSI-1],cl
        je      .match
        jmp     .character
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
        .character:
        cmpsb
        jne     .mismatch
        cmp     byte[HSI],cl
        je      .match
        jmp     .character
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
        .character:
        scasb
        jne     .character
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
        .character:
        scasb
        jne     .character
        dec     HCX
        cmp     HCX,0
        jne     .character
        mov     HAX,HDI
        call    rsto.Clodal
        ret
endcinc

;strings.CpyB(HSI,HDI,cl)(HAX)
;HSI    source string
;HDI    destination string
;cl     source terminator
;HAX    Number of bytes in output before null terminator
cinc    strings.CpyB
        call    rsto.Stoal
        xor     HAX,HAX
        .character:
        movsb
        inc     HAX
        cmp     byte[HSI],cl
        jne    .character
        mov     byte[HDI],0
        call    rsto.Clodal
        ret
endcinc

;strings.CpyH(HSI,HDI,HCX)(HAX)
;HSI    source string
;HDI    destination string
;HCX    source terminator
;HAX    Number of highest words in output before null terminator
cinc    strings.CpyH
        call    rsto.Stoal
        xor     HAX,HAX
        .character:
        MOVSH
        inc     HAX
        cmp     HWORD[HSI],HCX
        jne    .character
        mov     HWORD[HDI],0
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