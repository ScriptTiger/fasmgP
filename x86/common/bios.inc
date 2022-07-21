;bios.ClearScreen()()
cinc	bios.ClearScreen
        mov     ah,2h
        xor     bh,bh
        xor     dh,dh
        xor     dl,dl
        int     10h
        mov     ah,6h
        xor     al,al
        mov     bh,1bh
        xor     ch,ch
        mov     dh,24
        mov     dl,79
        int     10h
        ret
endcinc

;bios.Cout(si)()
cinc	bios.Cout
        pusha
        mov     ah,0eh

        .getchar:
                lodsb
                cmp     al,0
                jne     .prntchar
                popa
                ret

        .prntchar:
                int     10h
                jmp     .getchar
endcinc

;bios.Reboot()()
cinc	bios.Reboot
	jmp	0ffffh:0
	ret
endcinc

;bios.PowerOff()()
cinc	bios.PowerOff
        mov     ax,5307h
        mov     cx,3
        mov     bx,1
        int     15h
endcinc