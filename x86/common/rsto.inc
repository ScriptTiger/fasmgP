;rsto.Stoal()()
;Store current register set in the register store
cinc    rsto.Stoal
        sub     [pastack],BYTECAP
        mov     [pastack],HAX
        mov     HAX,rsto.Store
        add     HAX,[rsto.Length]
        add     [rsto.Length],RSTOINC
        mov     [HAX],HCX
        mov     [HAX+BYTECAP],HDX
        mov     [HAX+(BYTECAP*2)],HBX
        mov     [HAX+(BYTECAP*3)],HSI
        mov     [HAX+(BYTECAP*4)],HDI
        mov     HAX,[pastack]
        add     [pastack],BYTECAP
        ret
endcinc

;rsto.Clodal()()
;Load register set from store and clear them from the register store length
cinc    rsto.Clodal
        sub     [pastack],BYTECAP
        mov     [pastack],HAX
        mov     HAX,rsto.Store
        sub     [rsto.Length],RSTOINC
        add     HAX,[rsto.Length]
        mov     HCX,[HAX]
        mov     HDX,[HAX+BYTECAP]
        mov     HBX,[HAX+(BYTECAP*2)]
        mov     HSI,[HAX+(BYTECAP*3)]
        mov     HDI,[HAX+(BYTECAP*4)]
        mov     HAX,[pastack]
        add     [pastack],BYTECAP
        ret
endcinc

;rsto.Lodal()()
;Load register set from store
cinc    rsto.Lodal
        sub     [pastack],BYTECAP
        mov     [pastack],HAX
        mov     HAX,rsto.Store
        add     HAX,[rsto.Length]
        sub     HAX,RSTOINC
        mov     HCX,[HAX]
        mov     HDX,[HAX+BYTECAP]
        mov     HBX,[HAX+(BYTECAP*2)]
        mov     HSI,[HAX+(BYTECAP*3)]
        mov     HDI,[HAX+(BYTECAP*4)]
        mov     HAX,[pastack]
        add     [pastack],BYTECAP
        ret
endcinc